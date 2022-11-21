#include "diffusion2d.h"

#include "../../../MeshIO/MeshHandler/meshhandler.h"
#include "../../../MeshIO/VolumeMesher/volumemesher.h"
#include "../../../MeshIO/Cell/cell_polygon.h"



extern std::vector<meshio::MeshHandler*> meshhandler_stack;


int physicsio::Diffusion2D::Initialize()
{
    if(not initialize_items)
    {
        InitializeItems();
    }

    meshio::Region* region_a = regions.back();
    meshio::MeshContinuum* volume_continuum = region_a->mesh_volume_continua.back();
    meshio::MeshHandler* mesh_handler = meshio::GetCurrentHandler();
    meshio::VolumeMesher* mesher = mesh_handler->volume_mesher;

    auto node_count = (int)volume_continuum->nodes.size();



    // ================================================== Setting up Vector x & Vector b
    PetscBarrier(nullptr);
    error_code = VecCreate(PETSC_COMM_WORLD, &vector_x); CHKERRQ(error_code);
    error_code = PetscObjectSetName((PetscObject) vector_x, "Solution"); CHKERRQ(error_code);
    error_code = VecSetSizes(vector_x, local_rows_to - local_rows_from + 1, node_count); CHKERRQ(error_code);
    error_code = VecSetType(vector_x, VECMPI); CHKERRQ(error_code);
    error_code = VecDuplicate(vector_x, &vector_b); CHKERRQ(error_code);
    error_code = VecDuplicate(vector_x, &vector_u); CHKERRQ(error_code);


    // ================================================== Creating Matrix
    error_code = MatCreate(PETSC_COMM_WORLD, &matrix_a); CHKERRQ(error_code);
    error_code = MatSetSizes(matrix_a,
                             local_rows_to - local_rows_from + 1,
                             local_rows_to - local_rows_from + 1,
                             node_count,
                             node_count); CHKERRQ(error_code);
    error_code = MatSetType(matrix_a, MATMPIAIJ); CHKERRQ(error_code);
    error_code = MatSetUp(matrix_a); CHKERRQ(error_code);

    int temp_local_from, temp_local_to;
    error_code = MatGetOwnershipRange(matrix_a, &temp_local_from, &temp_local_to); CHKERRQ(error_code);


    // ================================================== Initializing DOF
    for(int i = 0; i < volume_continuum->nodes.size(); ++i)
    {
        auto* new_node_links = new std::vector<int>;
        nodal_connections.push_back(new_node_links);
        new_node_links = new std::vector<int>;
        nodal_cell_connections.push_back(new_node_links);

        nodal_boundary_connections.push_back(0);
        nodal_in_diagonal.push_back(0);
        nodal_off_diagonal.push_back(0);
    }


    // ================================================== Determing DOF
    size_t number_local_cells = volume_continuum->local_cell_indices.size();

    for(int j = 0; j < number_local_cells; ++j)
    {
        int global_index = volume_continuum->local_cell_indices[j];
        auto cell = volume_continuum->cells[global_index];

        if(typeid(cell) == typeid(meshio::CellPolygon*))
        {
            auto* polygon_cell = (meshio::CellPolygon*)(cell);
            for(int i = 0; i < polygon_cell->vertex_indices.size(); ++i)
            {
                int ir = mesher->MapNode(polygon_cell->vertex_indices[i]);

                for(auto &edge : polygon_cell->edges)
                {
                    if(edge[2] < 0)
                    {
                        int index_v0 = mesher->MapNode(edge[0]);
                        int index_v1 = mesher->MapNode(edge[1]);

                        if((ir == index_v0) || (ir == index_v1))
                        {
                            int boundary_type = boundaries[abs(edge[2])]->boundary_type;

                            if(boundary_type == BOUNDARY_CONDITION_DIRICHLET)
                            {
                                nodal_boundary_connections[ir] = edge[2];
                            }
                            break;
                        }
                    }
                }

                // ================================================== Setting up Nodal Connections
                std::vector<int>* node_links = nodal_connections[ir];

                for(int vertex_indice : polygon_cell->vertex_indices)
                {
                    int jr = mesher->MapNode(vertex_indice);

                    bool duplicates = false;

                    for(int node_link : *node_links)
                    {
                        if(node_link == jr)
                        {
                            duplicates = true;
                            break;
                        }
                    }

                    if(!duplicates)
                    {
                        (*node_links).push_back(jr);

                        if((jr >= local_rows_from) && (jr <= local_rows_to))
                        {
                            nodal_in_diagonal[ir] += 1;
                        }
                        else
                        {
                            nodal_off_diagonal[ir] += 1;
                        }
                    }
                }
            }
        }
    }

    this->discretizations->AddViewOfLocalContinuum(volume_continuum,
                                                   (int)volume_continuum->local_cell_indices.size(),
                                                   volume_continuum->local_cell_indices.data());

    MatMPIAIJSetPreallocation(matrix_a, 0, &nodal_in_diagonal[local_rows_from], 0, &nodal_off_diagonal[local_rows_from]);
    MatSetOption(matrix_a, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE);


    return false;
}
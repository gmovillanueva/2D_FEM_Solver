#include "diffusion2d.h"
#include "../../../MathIO/Discretizations/PWL/pwl_polygon.h"
#include "../../../MeshIO/MeshHandler/meshhandler.h"
#include "../../../MeshIO/VolumeMesher/volumemesher.h"
#include "../Boundary/boundary.h"
#include "../Boundary/boundary_dirichlet.h"



int physicsio::Diffusion2D::LoopOverCells()
{
    meshio::Region* region_a = regions.back();
    meshio::MeshContinuum* volume_continum = region_a->mesh_volume_continua.back();
    meshio::MeshHandler* mesh_handler = meshio::GetCurrentHandler();
    meshio::VolumeMesher* mesher = mesh_handler->volume_mesher;

    mathio::PWL* fe_discretization = ((mathio::PWL*)(this->discretizations));

    size_t number_local_cells = volume_continum->local_cell_indices.size();
    for(int lc = 0; lc < number_local_cells; ++lc)
    {
        int global_cell_index = volume_continum->local_cell_indices[lc];
        meshio::Cell* cell = volume_continum->cells[global_cell_index];

        if(typeid(cell) == typeid(meshio::CellPolygon*))
        {
            auto* polygon_cell = (meshio::CellPolygon*)(cell);
            auto* fe_view = (PolygonFEView*)fe_discretization->MapFeView(global_cell_index);

            for(int i = 0; i < fe_view->degrees_of_freedom; ++i)
            {
                int ir = mesher->MapNode(polygon_cell->vertex_indices[i]);

                int ir_boundary_type = BOUNDARY_CONDITION_NONE;
                if(nodal_boundary_connections[ir] < 0)
                {
                    ir_boundary_type = boundaries[abs(nodal_boundary_connections[ir])]->boundary_type;
                }

                if(ir_boundary_type == BOUNDARY_CONDITION_DIRICHLET)
                {
                    double ir_matrix_entry = 1.0;
                    error_code = MatSetValue(matrix_a, ir, ir, ir_matrix_entry, ADD_VALUES); CHKERRQ(error_code);
                    auto dirichlet_boundary = (physicsio::BoundaryDirichlet*)boundaries[abs(nodal_boundary_connections[ir])];
                    double value_b = dirichlet_boundary->boundary_value;
                    VecSetValue(vector_b, ir, value_b, ADD_VALUES);
                    VecSetValue(vector_x, ir, value_b, INSERT_VALUES);
                }
                else if(ir_boundary_type == BOUNDARY_CONDITION_NONE)
                {
                    for(int j = 0; j < fe_view->degrees_of_freedom; ++j)
                    {
                        int jr = mesher->MapNode(polygon_cell->vertex_indices[j]);
                        double jr_matrix_entry = 0.0;

                        for(int s = 0; s < fe_view->number_of_subtriangles; ++s)
                        {
                            for(int qp = 0; qp < fe_view->quadrature_points.size(); ++qp)
                            {
                                jr_matrix_entry += fe_view->weight[qp]*
                                                                      (fe_view->GradVarphiX(s, i, qp)*
                                                                       fe_view->GradVarphiX(s, j , qp)+
                                                                       fe_view->GradVarphiY(s, i, qp)*
                                                                       fe_view->GradVarphiY(s, j, qp))*
                                                                       fe_view->JacobianDeterminant(s, qp);
                            }
                        }

                        int jr_boundary_type = BOUNDARY_CONDITION_NONE;
                        if(nodal_boundary_connections[jr] < 0)
                        {
                            jr_boundary_type = boundaries[abs(nodal_boundary_connections[jr])]->boundary_type;
                        }

                        if(jr_boundary_type == BOUNDARY_CONDITION_DIRICHLET)
                        {
                            auto dirichlet_boundary = (physicsio::BoundaryDirichlet*)boundaries[abs(nodal_boundary_connections[jr])];
                            double value_b = -1 * jr_matrix_entry * dirichlet_boundary->boundary_value;
                            VecSetValues(vector_b, 1 , &ir, &value_b, ADD_VALUES);
                        }
                        else if(jr_boundary_type == BOUNDARY_CONDITION_NONE)
                        {
                            error_code = MatSetValue(matrix_a, ir, jr, jr_matrix_entry, ADD_VALUES); CHKERRQ(error_code);
                        }
                    }

                    double value = 0.0;
                    for(int s = 0; s < fe_view->number_of_subtriangles; ++s)
                    {
                        for(int qp = 0; qp < fe_view->quadrature_points.size(); ++qp)
                        {
                            value += fe_view->weight[qp]*
                                     fe_view->Varphi(s, i , qp)*
                                     1.0*fe_view->JacobianDeterminant(s, qp);
                        }
                    }

                    VecSetValues(vector_b, 1, &ir, &value, ADD_VALUES);
                }
            }

        }
    }

    return false;

}
#ifndef _MATHIO_PWL_POLYGON_
#define _MATHIO_PWL_POLYGON_

#include "pwl.h"
#include "pwl_base.h"
#include "../../../MeshIO/Cell/cell_polygon.h"
#include "../../../MathIO/Quadrature/quadrature.h"
#include "../../../MeshIO/MeshContinuum/meshcontinuum.h"



class PolygonFEView : public CellFEView
{
public:
    int number_of_subtriangles{};
    double beta{};

    meshio::Vertex vertex_center;
    std::vector<meshio::Vector> vector01, vector02;
    std::vector<double> jacobian_determinant;
    std::vector<int*> node_side_map;

    std::vector<QuadPoint2D*> quadrature_points;
    std::vector<double> weight;

    // ================================================== Constructor
    PolygonFEView(meshio::CellPolygon* polygon_cell,
                  meshio::MeshContinuum* volume_continuum) :
                  CellFEView((int)polygon_cell->vertex_indices.size())
    {
        // ================================================== Creating Quad Point
        auto new_qpoint = new QuadPoint2D;
        new_qpoint->point_x = 1.0/6.0;
        new_qpoint->point_y = 1.0/6.0;
        quadrature_points.push_back(new_qpoint);
        weight.push_back(1.0/6.0);

        new_qpoint = new QuadPoint2D;
        new_qpoint->point_x = 4.0/6.0;
        new_qpoint->point_y = 1.0/6.0;
        quadrature_points.push_back(new_qpoint);
        weight.push_back(1.0/6.0);

        new_qpoint = new QuadPoint2D;
        new_qpoint->point_x = 1.0/6.0;
        new_qpoint->point_y = 4.0/6.0;
        quadrature_points.push_back(new_qpoint);
        weight.push_back(1.0/6.0);

        // ==================================================  Calculate Raw
        vertex_center = polygon_cell->centroid;

        // ================================================== Calculating the legs
        for(int side = 0; side < number_of_subtriangles; side++)
        {
            int* side_indices = polygon_cell->edges[side];

            meshio::Vertex vertex00 = *volume_continuum->nodes[side_indices[0]];
            meshio::Vertex vertex01 = *volume_continuum->nodes[side_indices[1]];
            meshio::Vertex vertex02 = vertex_center;

            meshio::Vector side_vertex01 = vertex01 - vertex00;
            meshio::Vector side_vertex02 = vertex02 - vertex00;

            vector01.push_back(side_vertex01);
            vector02.push_back(side_vertex02);

            double  side_jacobian_determinant = ((side_vertex01.xComp) *
                                                 (side_vertex02.yComp) -
                                                 (side_vertex02.xComp) *
                                                 (side_vertex01.yComp));
            jacobian_determinant.push_back(side_jacobian_determinant);
        }

        // ================================================== Compute node to side mapping
        for(int s = 0; s < polygon_cell->vertex_indices.size(); s++)
        {
            int  vertix_index = polygon_cell->vertex_indices[s];
            auto side_mapping = new int[number_of_subtriangles];

            for(int side = 0; side < number_of_subtriangles; side++)
            {
                side_mapping[side] = -1;

                int* side_indices = polygon_cell->edges[side];

                if(side_indices[0] == vertix_index)
                {
                    side_mapping[side] = 0;
                }
                if(side_indices[1] == vertix_index)
                {
                    side_mapping[side] = 1;
                }
            }
            node_side_map.push_back(side_mapping);
        }
    }

    // ================================================== Varphi
    double Varphi(int side, int indices, int quadrature_point_index)
    {
        QuadPoint2D* quadrature_point = quadrature_points.at((unsigned long)quadrature_point_index);

        int index = node_side_map[indices][side];
        double value = 0;

        if(index == 0)
        {
            value = 1.0 - quadrature_point->point_x - quadrature_point->point_y;
        }
        if(index == 1)
        {
            value = quadrature_point->point_x;
        }

        value += beta * quadrature_point->point_y;

        return value;
    }

    // ================================================== Grad Varphi X
    double GradVarphiX(int side, int indices, int quadrature_point_index)
    {
        int index = node_side_map[indices][side];
        double value = 0;

        if(index == 0)
        {
            value = (vector02[side].yComp*(-1) -
                     vector01[side].yComp*(-1)) /
                     jacobian_determinant[side];
        }
        if(index == 1)
        {
            value = (vector02[side].yComp*(1) -
                     vector01[side].yComp*(0)) /
                     jacobian_determinant[side];
        }

        value += beta *
                (vector02[side].yComp*(0) -
                 vector01[side].yComp*(1)) /
                 jacobian_determinant[side];

        return value;
    }

    // ================================================== Grad Varphi Y
    double GradVarphiY(int side, int indices, int quadrature_point_index)
    {
        int index = node_side_map[indices][side];
        double value = 0;

        if(index == 0)
        {
            value = (-vector02[side].xComp*(-1) +
                     vector01[side].yComp*(-1)) /
                     jacobian_determinant[side];
        }
        if(index == 1)
        {
            value = (-vector02[side].xComp*(1) +
                     vector01[side].xComp*(0)) /
                     jacobian_determinant[side];
        }

        value += beta *
                (-vector02[side].xComp*(0) -
                 vector01[side].xComp*(1)) /
                 jacobian_determinant[side];

        return value;
    }

    // ================================================== Jacobian
    double JacobianDeterminant(int side, int quadrature_point_index)
    {
        return jacobian_determinant[side];
    }

};

#endif // PWL_POLYGON_H
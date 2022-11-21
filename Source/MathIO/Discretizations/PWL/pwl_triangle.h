#ifndef _MATHIO_PWL_TRIANGLE_
#define _MATHIO_PWL_TRIANGLE_

#include "pwl.h"
#include "pwl_base.h"
#include "../../../MeshIO/Cell/cell_polygon.h"
#include "../../../MeshIO/Cell/cell_triangle.h"
#include "../../../MathIO/Quadrature/quadrature.h"


class TriangleFEView : public CellFEView
{
public:
    std::vector<QuadPoint2D*> quadrature_points;
    std::vector<double> weight;
    double jacobian_determinant;
    meshio::Vector vector_a;
    meshio::Vector vector_b;
    //std::vector<meshio::Vector> vector_a;
    //std::vector<meshio::Vector> vector_b;
    //std::vector<double> jacobian_determinant;

    TriangleFEView(meshio::CellTriangle* cell_tri, meshio::MeshContinuum* volume_continuum) : CellFEView(3)
    {
        // Creating Quad Point
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

        // Grab d'em vertices
        meshio::Vertex vert[3];
        for(int vi : cell_tri->vertix_index)
        {
            //vert[k] = 1;
        }

        // Calculating the legs
        vector_a = vert[1]-vert[0];
        vector_b = vert[2]-vert[0];

        jacobian_determinant = ((vector_a.xComp * vector_b.yComp) - (vector_b.xComp * vector_a.yComp));
    }

    double Varphi(int index, int qpoint_index)
    {
        if(index == 0)
        {
            auto qpoint = quadrature_points.at((unsigned long)qpoint_index);
            return 1.0 - qpoint->point_x - qpoint->point_y;
        }
        if(index == 1)
        {
            auto qpoint = quadrature_points.at((unsigned long)qpoint_index);
            return qpoint->point_x;
        }
        if(index == 2)
        {
            auto qpoint = quadrature_points.at((unsigned long)qpoint_index);
            return qpoint->point_y;
        }
        return 0;
    }

    double GradVarphiX(int index, int x)
    {
        if(index == 0)
        {
            return (vector_b.yComp*(-1) - vector_a.yComp*(-1))/jacobian_determinant;
        }
        if(index == 1)
        {
            return (vector_b.yComp*(1) - vector_a.yComp*(0))/jacobian_determinant;
        }
        if(index == 2)
        {
            return (vector_b.yComp*(0) - vector_a.yComp*(1))/jacobian_determinant;
        }
        return 0;
    }

    double GradVarphiY(int index, int y)
    {
        if(index == 0)
        {
            return (vector_b.xComp*(-1) - vector_a.xComp*(-1))/jacobian_determinant;
        }
        if(index == 1)
        {
            return (vector_b.xComp*(1) - vector_a.xComp*(0))/jacobian_determinant;
        }
        if(index == 2)
        {
            return (vector_b.xComp*(0) - vector_a.xComp*(1))/jacobian_determinant;
        }
        return 0;
    }

    double JacobianDeterminant(int index)
    {
        return jacobian_determinant;
    }

};

#endif // PWL_TRIANGLE_H
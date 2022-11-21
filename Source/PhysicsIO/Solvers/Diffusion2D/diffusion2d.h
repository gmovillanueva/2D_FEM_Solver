#ifndef _PHYSICSIO_DIFFUSION2D_
#define _PHYSICSIO_DIFFUSION2D_

#define SYSTEM_SOLVER_EIGEN 1
#define SYSTEM_SOLVER_PETSC 2

#include <vector>
#include "../solvers.h"
#include "../../../MathIO/Discretizations/discretizations.h"
#include "../../../../Libs/Eigen/Sparse"

#include <petscksp.h>


class physicsio::Diffusion2D : public physicsio::Solvers
{
public:

    int solver_system;
    double residual_tolerance;
    bool initialize_items;


    std::vector<physicsio::Boundary*> boundaries;
    mathio::Discretizations* discretizations;


    Vec vector_x, vector_x_cell, vector_b, vector_u;
    Mat matrix_a;
    KSP linear_solver;
    PC  preconditioner;
    PetscReal solution_normal;
    PetscErrorCode error_code;
    PetscInt column[3], iterators, local_rows_from, local_rows_to;
    PetscMPIInt  size;
    PetscScalar one = 1.0, value[3];
    PetscBool nonzero_guess = PETSC_FALSE, change_side = PETSC_FALSE;

    std::vector<std::vector<int>*> nodal_connections;
    std::vector<std::vector<int>*> nodal_cell_connections;
    std::vector<int> nodal_boundary_connections;
    std::vector<int> nodal_in_diagonal;
    std::vector<int> nodal_off_diagonal;



    //Eigen::SparseMatrix<double, Eigen::RowMajor> matrix_a;
    //Eigen::VectorXd vector_b;
    //Eigen::VectorXd vector_x;

    //Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver_bicgstab;
    //Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver_cg;

public:
        Diffusion2D();
        void InitializeItems();
        int Initialize();
        void Execute();
        int LoopOverCells();

};

#endif // DIFFUSION2D_H
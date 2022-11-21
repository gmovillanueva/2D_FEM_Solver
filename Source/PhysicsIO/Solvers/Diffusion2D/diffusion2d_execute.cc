#include "diffusion2d.h"



void physicsio::Diffusion2D::Execute()
{
    std::cout << "Diffusion Solver starting. \n";

    // ================================================== Looping Over Cells
    LoopOverCells();

    // ================================================== Assembling Matrix
    error_code = MatAssemblyBegin(matrix_a, MAT_FINAL_ASSEMBLY);
    error_code = MatAssemblyEnd(matrix_a, MAT_FINAL_ASSEMBLY);
    VecAssemblyBegin(vector_b);
    VecAssemblyEnd(vector_b);
    VecAssemblyBegin(vector_x);
    VecAssemblyEnd(vector_x);

    PetscBool is_symmetric;
    error_code = MatIsSymmetric(matrix_a, 1.0E-4, &is_symmetric);
    if(!is_symmetric)
    {
        std::cout << "Assembled Matrix is not symmetric. \n";
    }

    error_code = KSPCreate(PETSC_COMM_WORLD, &linear_solver);
    error_code = KSPSetOperators(linear_solver, matrix_a, matrix_a);

    error_code = KSPGetPC(linear_solver,&preconditioner);
    error_code = PCSetType(preconditioner, PCGAMG);
    error_code = KSPSetType(linear_solver, KSPCG);
    error_code = KSPSetTolerances(linear_solver, 1.0E-50, residual_tolerance, 1.0E50, 1000);

    //error_code = KSPMonitorSet(linear_solver,&Diffusion2D::)

    error_code = KSPSetInitialGuessNonzero(linear_solver, PETSC_TRUE);
    error_code = KSPSolve(linear_solver, vector_b, vector_x);

    KSPConvergedReason reason;
    KSPGetConvergedReason(linear_solver, &reason);

    std::cout << "Convergence reason: " << reason << "\n";

    // ================================================== Outputting Results


    std::cout << "2D Diffusion Solver execution has been completed. \n";
}

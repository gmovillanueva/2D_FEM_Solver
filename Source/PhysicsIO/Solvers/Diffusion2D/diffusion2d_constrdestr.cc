#include "diffusion2d.h"

physicsio::Diffusion2D::Diffusion2D()
{
    initialize_items = false;
    solver_system = SYSTEM_SOLVER_PETSC;
    residual_tolerance = 1.0E-8;
}
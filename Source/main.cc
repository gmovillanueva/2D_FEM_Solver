#include <iostream>

#include "MeshIO/SurfaceMesh/surfacemesh.h"
#include "MeshIO/MeshHandler/meshhandler.h"

#include <petscksp.h>


int current_meshhandler;

int main(int argc,char **args)
{
    PetscErrorCode error_code;

    error_code = PetscInitialize(&argc, &args,(char*) nullptr, nullptr);

    auto* new_meshhandler = new meshio::MeshHandler;
    meshhandler_stack.push_back(new_meshhandler);


    auto* new_surfacemesh = new meshio::SurfaceMesh;
    new_meshhandler->surfacemesh_stack.push_back(new_surfacemesh);

    bool verbose = false;

    new_surfacemesh->LoadObj("Resources/Meshes/128_Triangles.obj", verbose);
    new_surfacemesh->ExportObj("Resources/Meshes/128_Triangles_Exported.obj", verbose);

    PetscFinalize();
}

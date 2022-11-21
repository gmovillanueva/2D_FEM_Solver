#ifndef _MESH_SURFACEMESH_
#define _MESH_SURFACEMESH_

#include <cstdio>
#include <vector>
#include <string>

#include "../meshio.h"


class meshio::SurfaceMesh
{
public:
    std::string type;

    std::vector<meshio::Vertex> vertices;
    std::vector<meshio::Normal> normals;
    std::vector<meshio::Face> faces;

public:
    void LoadObj(std::string const& filename, bool verbose = false);
    void UpdateConnections();
    void ExportObj(std::string const& filename, bool verbose = false);

};

#endif // SURFACEMESH_H
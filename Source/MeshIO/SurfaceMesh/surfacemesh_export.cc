#include "surfacemesh.h"

#include <fstream>
#include <iomanip>



extern std::vector<meshio::MeshHandler*> meshhandler_stack;

void meshio::SurfaceMesh::ExportObj(std::string const& filename, bool verbose)
{
    std::ofstream file;
    file.open(filename);

    if(!file.is_open())
    {
        std::cout << "Error creating file " << filename << "\n";
    }
    else
    {
        if(verbose)
        {
            std::cout << "File " << filename << "created successfully." << "\n";
        }
    }

    if(verbose)
    {
        std::cout << "Outputting information below:" << "\n\n";
        std::cout << "================================================================================ \n" ;
    }

    file << "# \n";
    file << "# Exported Mesh File. \n";
    file << "# \n";
    file << "o Plane \n";

    std::vector<meshio::Vertex>::iterator ivertex;
    for (ivertex = this->vertices.begin(); ivertex != this->vertices.end(); ivertex++)
    {
        file << "v " <<
        std::fixed << std::setprecision(6) <<
        ivertex->xComp << " " <<
        ivertex->yComp << " " <<
        ivertex->zComp << "\n";

        if(verbose)
        {
            std::cout << "v " <<
            ivertex->xComp << " " <<
            ivertex->yComp << " " <<
            ivertex->zComp << "\n";
        }
    }

    if(!faces.empty())
    {
        meshio::Face first_face = this->faces.front();

        file << "vn " <<
        first_face.geometric_normal.xComp << " " <<
        first_face.geometric_normal.yComp << " " <<
        first_face.geometric_normal.zComp << "\n";

        file << "# \n";
        file << "s off \n";

        if(verbose)
        {
            std::cout << "vn " <<
            first_face.geometric_normal.xComp << " " <<
            first_face.geometric_normal.yComp << " " <<
            first_face.geometric_normal.zComp << "\n";
        }

        std::vector<meshio::Face>::iterator iface;
        for (iface = this->faces.begin(); iface != this->faces.end(); iface++)
        {
            file << "f " <<
                 iface->vertix_indices[0]+1 << "//1 " <<
                 iface->vertix_indices[1]+1 << "//1 " <<
                 iface->vertix_indices[2]+1 << "//1 \n";

            if(verbose)
            {
                std::cout << "f " <<
                iface->vertix_indices[0]+1 << "//1 " <<
                iface->vertix_indices[1]+1 << "//1 " <<
                iface->vertix_indices[2]+1 << "//1 \n";
            }
        }
    }

    file.close();

    if(verbose)
    {
        std::cout << "================================================================================ \n" ;
    }

    std::cout << "Finished exporting " << filename << ". \n";
}

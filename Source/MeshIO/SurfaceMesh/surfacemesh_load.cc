#include "surfacemesh.h"
#include "../../TextIO/textio.h"

#include <fstream>



extern std::vector<meshio::MeshHandler*> meshhandler_stack;

void meshio::SurfaceMesh::LoadObj(std::string const &filename, bool verbose)
{

    if(verbose)
    {
        //std::cout << "================================================================================ \n\n" ;
        std::cout << "\n" << "Loading OBJ file " << filename << "\n";
    }

    int counter = 0;
    std::ifstream file;
    file.open(filename);

    if(!file.is_open())
    {
        std::cout << "Error opening OBJ file " << filename << "\n";
    }
    else
    {
        if(verbose)
        {
            std::cout << "File " << filename << "loaded successfully." << "\n";
        }
    }


    std::string line;
    std::string word;
    std::string delimiter = " ";
    std::vector<std::string> sub_str;
    std::vector<std::string> sub_sub_str;

    //auto* sm = new meshio::SurfaceMesh;
    textio::Operations split;

    if(verbose)
    {
        std::cout << "Outputting file below:" << "\n\n";
        std::cout << "================================================================================ \n" ;
    }

    while(std::getline(file, line))
    {
        counter++;
        sub_str = split.string_split(line, ' ');
        type = sub_str.at(0);

        if(type == "v")
        {
            auto *new_vertex = new meshio::Vertex;
            new_vertex->xComp = std::stod(sub_str.at(1));
            new_vertex->yComp = std::stod(sub_str.at(2));
            new_vertex->zComp = std::stod(sub_str.at(3));
            this->vertices.push_back(*new_vertex);

            if(verbose)
            {
                std::cout << "Line " <<
                counter << ": v " <<
                new_vertex->xComp << " " <<
                new_vertex->yComp << " " <<
                new_vertex->zComp << "\n";
            }
        }
        else if(type == "vn")
        {
            auto *new_normal = new meshio::Vertex;
            new_normal->xComp = std::stod(sub_str.at(1));
            new_normal->yComp = std::stod(sub_str.at(2));
            new_normal->zComp = std::stod(sub_str.at(3));
            this->normals.push_back(*new_normal);

            if(verbose)
            {
                std::cout << "Line " <<
                counter << ": vn " <<
                new_normal->xComp << " " <<
                new_normal->yComp << " " <<
                new_normal->zComp << "\n";
            }
        }
        else if(type == "f")
        {
            int total_vertices = (int) sub_str.size() - 1;
            auto *new_face = new meshio::Face;

            for(int i = 1; i <= total_vertices; ++i)
            {
                sub_sub_str = split.string_split(sub_str.at((unsigned long)i), '/');
                new_face->vertix_indices.push_back(std::stoi(sub_sub_str.at(0))-1);
            }

            for(int v = 0; v < (new_face->vertix_indices.size()); v++)
            {
                auto side_indices = new int[4];

                side_indices[0] = new_face->vertix_indices[v];
                side_indices[1] = new_face->vertix_indices[v+1];
                side_indices[2] = -1;
                side_indices[3] = -1;

                if((v + 1) >= new_face->vertix_indices.size())
                {
                    side_indices[1] = new_face->vertix_indices[0];
                }

                new_face->edges.push_back(side_indices);
            }
            this->faces.push_back(*new_face);

            if(verbose)
            {
                std::cout << "Line " <<
                counter << ": " <<
                line << "\n";
            }
        }
        else
        {
            if(verbose)
            {
                std::cout << "Line " << counter << ": " << line << "\n";
            }
        }
    }
    file.close();

    if(verbose)
    {
        std::cout << "================================================================================ \n\n" ;
    }

    this->UpdateConnections();
}

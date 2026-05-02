// include readPLYfile function
#include "readPLYfile.h"

// readPLYFile constructore
void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces){
    
    // open file
    std::ifstream file(fname);
    if(!file.is_open()){
        std::cout << "No open file" << std::endl;
        return;
    }

    // initializing variables
    std::string line;
    std::string token;

    int numVertices = 0;
    int numFaces = 0;

    std::vector<std::string> propertyOrder;

    // reading file for vertices, faces, and properties
    while(getline(file, line)){
        std::stringstream ss(line);
        ss >> token;

        if(token == "element"){
            ss >> token;
            if(token == "vertex"){
                ss >> numVertices;
            }
            else if(token == "face"){
                ss >> numFaces;
            }

        }
        else if(token == "property"){
            std::string type, name;
            ss >> type >> name;

            if(name != "vertex_indices"){
                propertyOrder.push_back(name);
            }
        }
        else if(token == "end_header"){
            break;
        }
    }

    // getting UV, position, olour, and normals
    for (int i = 0; i<numVertices; i++){
        getline(file, line);
        std::stringstream ss(line);

        VertexData v{};
        for(int j = 0; j<propertyOrder.size(); j++){
            float value;
            ss >> value;

            if(propertyOrder[j] == "x") v.x = value;
            else if(propertyOrder[j] == "y") v.y = value;
            else if (propertyOrder[j] == "z") v.z = value;
            else if (propertyOrder[j] == "nx") v.nx = value;
            else if (propertyOrder[j] == "ny") v.ny = value;
            else if (propertyOrder[j] == "nz") v.nz = value;
            else if (propertyOrder[j] == "red") v.r = value / 255.0f;
            else if (propertyOrder[j] == "green") v.g = value / 255.0f;
            else if (propertyOrder[j] == "blue") v.b = value / 255.0f;
            else if (propertyOrder[j] == "u") v.u = value;
            else if (propertyOrder[j] == "v") v.v = value;
        }
        vertices.push_back(v);
    }

    // getting triangle meshes
    for(int i = 0; i < numFaces; i++){
        getline(file, line);
        std::stringstream ss(line);

        int count;
        ss >> count;

        if(count != 3) continue;

        int a, b, c;
        ss >> a >> b >> c;

        faces.push_back(TriData(a, b, c));
    }

    // close file
    file.close();
}
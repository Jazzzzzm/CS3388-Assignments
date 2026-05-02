
#include "processing.h"

#include <fstream>


std::vector<float> compute_normals(std::vector<float> vertices){
    std::vector<float> normals;

    for(size_t i = 0; i < vertices.size(); i += 9)
    {
        // triangle vertices
        float x1 = vertices[i];
        float y1 = vertices[i+1];
        float z1 = vertices[i+2];

        float x2 = vertices[i+3];
        float y2 = vertices[i+4];
        float z2 = vertices[i+5];

        float x3 = vertices[i+6];
        float y3 = vertices[i+7];
        float z3 = vertices[i+8];

        // edge vectors
        float ax = x2 - x1;
        float ay = y2 - y1;
        float az = z2 - z1;

        float bx = x3 - x1;
        float by = y3 - y1;
        float bz = z3 - z1;

        // cross product
        float nx = ay*bz - az*by;
        float ny = az*bx - ax*bz;
        float nz = ax*by - ay*bx;

        // normalize
        float length = sqrt(nx*nx + ny*ny + nz*nz);

        if(length > 0)
        {
            nx /= length;
            ny /= length;
            nz /= length;
        }

        // repeat for the 3 vertices
        for(int v=0; v<3; v++)
        {
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
    }

    return normals;
}

void writePLY(std::vector<float> vertices, std::vector<float> normals, std::string fileName){
    std::ofstream file(fileName);

    int vertexCount = vertices.size() / 3;
    int faceCount = vertexCount / 3;

    // header
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << vertexCount << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property float nx\n";
    file << "property float ny\n";
    file << "property float nz\n";
    file << "element face " << faceCount << "\n";
    file << "property list uchar int vertex_indices\n";
    file << "end_header\n";

    // write vertices + normals
    for(int i=0;i<vertexCount;i++)
    {
        file << vertices[i*3] << " "
             << vertices[i*3+1] << " "
             << vertices[i*3+2] << " "
             << normals[i*3] << " "
             << normals[i*3+1] << " "
             << normals[i*3+2] << "\n";
    }

    // write faces
    for(int i=0;i<faceCount;i++)
    {
        file << "3 "
             << i*3 << " "
             << i*3+1 << " "
             << i*3+2 << "\n";
    }

    file.close();
}
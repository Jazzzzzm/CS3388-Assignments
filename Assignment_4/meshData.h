#ifndef MESHDATA_H
#define MESHDATA_H

// struture to store vertex data
struct VertexData {
    float x, y, z;   
    float nx, ny, nz;       
    float r, g, b;         
    float u, v;             

    // constructor
    VertexData() :
        x(0), y(0), z(0),
        nx(0), ny(0), nz(0),
        r(0), g(0), b(0),
        u(0), v(0) {}
};

// strucutre to store triange faces
struct TriData {
    unsigned int v1, v2, v3;

    // constructor
    TriData(unsigned int a, unsigned int b, unsigned int c)
        : v1(a), v2(b), v3(c) {}
};

#endif
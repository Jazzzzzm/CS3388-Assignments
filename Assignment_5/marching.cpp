#include "marching.h"
#include "TriTable.hpp"

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

int marching_squares_lut[16][4] = {
	{-1, -1, -1, -1},
	{2, 3, -1, -1},
	{1, 2, -1, -1},
	{1, 3, -1, -1},
	{0, 1, -1, -1},
	{0, 1, 2, 3},
	{0, 2, -1, -1},
	{0, 3, -1, -1},
	{0, 3, -1, -1},
	{0, 2, -1, -1},
	{0, 3, 1, 2},
	{0, 1, -1, -1},
	{1, 3, -1, -1},
	{1, 2, -1, -1},
	{2, 3, -1, -1},
	{-1, -1, -1, -1}
};

std::vector<float> marching_cubes(
    std::function<float(float, float, float)> f,
    float isovalue,
    float min,
    float max,
    float stepsize
){
    std::vector<float> vertices;

    for(float x = min; x < max; x += stepsize)
    for(float y = min; y < max; y += stepsize)
    for(float z = min; z < max; z += stepsize)
    {
        float val[8];

        // cube corners
        float cx[8] = {x, x+stepsize, x+stepsize, x, x, x+stepsize, x+stepsize, x};
        float cy[8] = {y, y, y, y, y+stepsize, y+stepsize, y+stepsize, y+stepsize};
        float cz[8] = {z, z, z+stepsize, z+stepsize, z, z, z+stepsize, z+stepsize};

        // sample scalar field
        for(int i=0;i<8;i++)
            val[i] = f(cx[i],cy[i],cz[i]);

        // compute cube index
        int cubeIndex = 0;

        if(val[0] < isovalue) cubeIndex |= 1;
        if(val[1] < isovalue) cubeIndex |= 2;
        if(val[2] < isovalue) cubeIndex |= 4;
        if(val[3] < isovalue) cubeIndex |= 8;
        if(val[4] < isovalue) cubeIndex |= 16;
        if(val[5] < isovalue) cubeIndex |= 32;
        if(val[6] < isovalue) cubeIndex |= 64;
        if(val[7] < isovalue) cubeIndex |= 128;

        // generate triangles
        for(int i=0; marching_cubes_lut[cubeIndex][i] != -1; i++)
        {
            int edge = marching_cubes_lut[cubeIndex][i];

            float vx = x + vertTable[edge][0] * stepsize;
            float vy = y + vertTable[edge][1] * stepsize;
            float vz = z + vertTable[edge][2] * stepsize;

            vertices.push_back(vx);
            vertices.push_back(vy);
            vertices.push_back(vz);
        }
    }

    return vertices;
}
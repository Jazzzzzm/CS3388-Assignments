#ifndef MARCHING_H
#define MARCHING_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <iostream>

std::vector<float> marching_cubes(
    std::function<float(float, float, float)> f,
    float isovalue,
    float min,
    float max,
    float stepsize
);

#endif
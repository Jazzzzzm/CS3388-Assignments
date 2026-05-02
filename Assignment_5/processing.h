#ifndef PROCESSING_H
#define PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

std::vector<float> compute_normals(std::vector<float> verts);

void writePLY(std::vector<float> verts, std::vector<float> norms, std::string fileName);

#endif
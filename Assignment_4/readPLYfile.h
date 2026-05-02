#ifndef READPLYFILE_H
#define READPLYFILE_H

// Include standard headers
#include <vector>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>

// Include header files
#include "meshData.h"

void readPLYFile(
        std::string fname, 
        std::vector<VertexData>& vertices, 
        std::vector<TriData>& faces
    );

#endif
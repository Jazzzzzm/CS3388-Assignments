#ifndef TEXTUREDMESH_H
#define TEXTUREDMESH_H

#include <GL/glew.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>


class TexturedMesh {
    private:
        // OpenGL IDs
        GLuint vao;
        GLuint vbo_positions;
        GLuint vbo_texcoords;
        GLuint ebo_indices;
        GLuint textureID;
        GLuint shaderProgram;

        unsigned int indexCount;

    public:
        TexturedMesh(const std::string& plyPath, const std::string& bmpPath);
        void draw(const glm::mat4& MVP);
};

#endif
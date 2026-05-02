// enable glm experimental
#define GLM_ENABLE_EXPERIMENTAL

// include all needed .h files
#include "texturedMesh.h"
#include "LoadBitmap.h"
#include "meshData.h"
#include "readPLYfile.h"

// include needed libraries
#include <vector>

// TexturedMesh constructor
TexturedMesh::TexturedMesh(const std::string& plyPath, const std::string& bmpPath){
    
    // initialize VertexData and TriData vectors to store texture information
    std::vector<VertexData> vertices;
    std::vector<TriData> faces;

    // read given file and load vertices and faces
    readPLYFile(plyPath, vertices, faces);

    // convert to flat arrays
    std::vector<float> positions;
    std::vector<float> texcoords;
    std::vector<unsigned int> indices;

    for (auto& v : vertices) {
        positions.push_back(v.x);
        positions.push_back(v.y);
        positions.push_back(v.z);

        texcoords.push_back(v.u);
        texcoords.push_back(v.v);
    }

    for (auto& f : faces) {
        indices.push_back(f.v1);
        indices.push_back(f.v2);
        indices.push_back(f.v3);
    }

    indexCount = indices.size();

    // intializing vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // initializing position vbo
    glGenBuffers(1, &vbo_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(float),
                 positions.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // initializing texture vbo
    glGenBuffers(1, &vbo_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
    glBufferData(GL_ARRAY_BUFFER,
                 texcoords.size() * sizeof(float),
                 texcoords.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // initializing ebo
    glGenBuffers(1, &ebo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    // loading textures
    unsigned char* data;
    unsigned int width, height;

    loadARGB_BMP(bmpPath.c_str(), &data, &width, &height);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glTexImage2D(GL_TEXTURE_2D, 
                0, 
                GL_RGBA,           
                width, 
                height, 
                0, 
                GL_BGRA,           
                GL_UNSIGNED_BYTE, 
                data);

    glGenerateMipmap(GL_TEXTURE_2D);
    
    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // freeing memeory
    delete[] data;

    // vertex shader
    const char* vsSource =
        "#version 410 core\n"
        "layout(location=0) in vec3 position;\n"
        "layout(location=1) in vec2 texCoord;\n"
        "uniform mat4 MVP;\n"
        "out vec2 TexCoord;\n"
        "void main(){\n"
        "   gl_Position = MVP * vec4(position,1.0);\n"
        "   TexCoord = vec2(texCoord.x, texCoord.y);\n"
        "}";

    // fragment shader
    const char* fsSource =
        "#version 410 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D tex;\n"
        "void main(){\n"
        "   FragColor = texture(tex, TexCoord);\n"
        //"   FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);\n"
        "}";

    // create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);

    // error check
    GLint success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Vertex Shader Error: " << infoLog << std::endl;
    }

    // create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);

    // attach shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    // delete shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
}

// draw the texture
void TexturedMesh::draw(const glm::mat4& MVP){
    glUseProgram(shaderProgram);

    GLuint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLuint texLoc = glGetUniformLocation(shaderProgram, "tex");
    glUniform1i(texLoc, 0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
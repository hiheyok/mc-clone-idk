#pragma once
#ifndef IRENDERER_H_
#define IRENDERER_H_

#include <vector>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "../../../utils/functions/clock/Clock.h"
#include "../../../utils/functions/logging/log.h"
#include "../../../Server/world/World.h"
#include "../mesh/mesher.h"
#include "../camera/camera.h"
#include <unordered_map>
#include "../shaders/shaders.h"

class DataBuffer {
public:

    void genBuffers();

    /*void SetAttributeData(GLuint index, GLuint size, GLenum TYPE, GLboolean NORMAL, GLuint STRIDE, GLuint OFFSET);*/
    void SetElementAttributeData(GLuint index, GLuint size, GLenum TYPE, GLboolean NORMAL, GLuint STRIDE, GLuint OFFSET);
    void chunkDraw();
    void BindElements(std::vector<GLuint>* data);
    void BindData(std::vector<unsigned int>* data);

    void TSetElementAttributeData(GLuint index, GLuint size, GLenum TYPE, GLboolean NORMAL, GLuint STRIDE, GLuint OFFSET);
    void TchunkDraw();
    void TBindElements(std::vector<GLuint>* data);
    void TBindData(std::vector<unsigned int>* data);

    void clear() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &TVAO);
        glDeleteBuffers(1, &TEBO);
        glDeleteBuffers(1, &TVBO);
       
    }


    Shader** shaders = nullptr;

    GLuint TVAO = 0;
    GLuint TVBO = 0;
    GLuint TEBO = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    size_t verticesSize = 0;
    size_t TverticesSize = 0;

    LOG* logs = nullptr;

};

class IRenderer {
public:

    IRenderer() {
       

       /* glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,200000000,NULL,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 200000000, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    }

    void mutiDrawElements();
    void MutiChunkDrawElements(Camera* camera_);

    void ReloadShaderData(Shader* shader, uint8_t index);

    Camera* camera = nullptr;
    Shader* shaders[256]{};
    World* world = nullptr;
    MesherV2* mesher = nullptr;

    std::unordered_map<long long, DataBuffer> WorldRenderingDataContainer;

    std::mutex render_mutex;

protected:

    long long int ivec3To64xInt(int x, int y, int z) {
        //std::cout << x << ", " << y << ", " << z << "\n";
        return ((long long int)x * 10000000000) + ((long long int)y * 100000) + (long long int)z;
    }

    long long int ivec3To64xInt(glm::ivec3 vec) {
        //std::cout << vec.x << ", " << vec.y << ", " << vec.z << "\n";
        return ((long long int)vec.x * 10000000000) + ((long long int)vec.y * 100000) + (long long int)vec.z;
    }
    void BindShader(Shader shader0, uint8_t index);
    

    Shader* getShader(uint8_t index) {
        return shaders[index];
    }

    

   


    GLuint texture0 = 0;
    GLuint texturearray = 0;
    LOG* logs = nullptr;
    

private:
    std::vector<GLfloat>* verticesd = nullptr;
    std::vector<GLuint>* indicesd = nullptr;

};

#endif 



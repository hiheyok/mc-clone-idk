#include <vector>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "../../../utils/MathHelper.h"
#include "../gui/Settings/Settings.h"
#include "../frustum/frustum.h"
#include "IRenderer.h"
#include <fstream>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <tgmath.h>

using namespace std;
using namespace chrono;
using namespace glm;

#define TEXTURE_DIRT 0
#define TEXTURE_GRASS_SIDE 1
#define TEXTURE_GRASS_TOP_BW 2
#define TEXTURE_GRASS_TOP 3
#define TEXTURE_COBBLESTONE 4
#define TEXTURE_SAND 5
#define TEXTURE_WATER 6
#define TEXTURE_OAK_LOG_SIDE 7
#define TEXTURE_OAK_LOG_TOP 8
#define TEXTURE_OAK_LEAF 9
#define TEXTURE_OAK_SAPLING 10



vec2 RotVec2(vec2 pos, float rot) {
    rot = rot * (float)DEGTORAD;
    const float rotCos = sin(rot);
    const float rotSin = cos(rot);

    float yCos = pos.y * rotCos;
    float ySin = pos.y * rotSin;
    float xCos = pos.x * rotCos;
    float xSin = pos.x * rotSin;

    return vec2((yCos - xSin), (ySin + xCos));


}

vec3 RotVec3(vec3 pos, float yaw, float pitch) {
    yaw = yaw * (float)DEGTORAD;
    const float rotCosYaw = sin(yaw);
    const float rotSinYaw = cos(yaw);

    pitch = pitch * (float)DEGTORAD;

    const float rotCosPitch = cos(pitch);
    const float rotSinPitch = sin(pitch);

    float zCosYaw = pos.z * rotCosYaw;
    float zSinYaw = pos.z * rotSinYaw;
    float xCosYaw = pos.x * rotCosYaw;
    float xSinYaw = pos.x * rotSinYaw;

    return vec3((zCosYaw - xSinYaw), pos.y + (pos.y * (rotCosPitch - rotSinPitch)), (zSinYaw + xCosYaw));
}

void DataBuffer::genBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &TVAO);
    glGenBuffers(1, &TEBO);
    glGenBuffers(1, &TVBO);
}

void DataBuffer::chunkDraw() {
    shaders[0]->use();
    shaders[0]->setIVec3("ChunkPos", x, y, z);

    // std::cout << &VAO << "\n";
   //  std::cout << &EBO << "\n";

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, (GLsizei)verticesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DataBuffer::BindElements(std::vector<GLuint>* data) {
    verticesSize = data->size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(GLuint), data->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void DataBuffer::BindData(std::vector<GLuint>* data) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(GLuint), data->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void DataBuffer::TchunkDraw() {
    shaders[1]->use();
    shaders[1]->setIVec3("ChunkPos", x, y, z);

    glBindVertexArray(TVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)TverticesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DataBuffer::TBindElements(std::vector<GLuint>* data) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(GLuint), data->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    TverticesSize = data->size();


}

void DataBuffer::TBindData(std::vector<GLuint>* data) {

    glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(GLuint), data->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void IRenderer::ReloadShaderData(Shader* shader_, uint8_t index) {
    shaders[index] = shader_;
}

void IRenderer::BindShader(Shader shader0, uint8_t index) {
    shaders[index] = &shader0;
}

void DataBuffer::SetElementAttributeData(GLuint index, GLuint size, GLenum TYPE, GLboolean NORMAL, GLuint STRIDE, GLuint OFFSET) {


    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, STRIDE * sizeof(GLfloat), (void*)(OFFSET * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //  cout << x << ", " << y << ", " << z << "\n";




}
void DataBuffer::TSetElementAttributeData(GLuint index, GLuint size, GLenum TYPE, GLboolean NORMAL, GLuint STRIDE, GLuint OFFSET) {


    glBindVertexArray(TVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TEBO);
    glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, STRIDE * sizeof(GLfloat), (void*)(OFFSET * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //  cout << x << ", " << y << ", " << z << "\n";




}


void IRenderer::mutiDrawElements() {
    shaders[0]->use();
    for (const std::pair<long long int, DataBuffer>& element : WorldRenderingDataContainer) {
        glBindVertexArray(element.second.VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)element.second.verticesSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}


void IRenderer::MutiChunkDrawElements(Camera* camera_) {


    Camera cam = *camera_;

    CFrustum fr;
    fr.CalculateFrustum(&cam);

    int rd = getRENDER_DISTANCE();
    ivec3 Position = ivec3(-camera_->Position.x / CHUNK_SIZE, -camera_->Position.y / CHUNK_SIZE, -camera_->Position.z / CHUNK_SIZE);

    auto t0 = high_resolution_clock::now();
    std::unordered_map<long long int, DataBuffer> tmp = WorldRenderingDataContainer;
    duration<double, nano> time = high_resolution_clock::now() - t0;

    shaders[0]->use();
    for (const auto& data : tmp) {

        if (!(data.second.x + Position.x > rd || data.second.y + Position.y > rd || data.second.z + Position.z > rd || data.second.x + Position.x < -rd || data.second.y + Position.y < -rd || data.second.z + Position.z < -rd)) {//if (FindDistance(data.second.x, data.second.y, data.second.z, (int)x12 / CHUNK_SIZE, (int)y12 / CHUNK_SIZE, (int)z12 / CHUNK_SIZE) <= renderDistance) {
            if (fr.SphereInFrustum((float)data.second.x * CHUNK_SIZE, (float)data.second.y * CHUNK_SIZE, (float)data.second.z * CHUNK_SIZE, (float)CHUNK_SIZE * 2)) {
                WorldRenderingDataContainer[data.first].chunkDraw();
            }
        }
        else {
            mesher->unloadRenderingData(ivec3(data.second.x, data.second.y, data.second.z));
            WorldRenderingDataContainer[data.first].clear();
            WorldRenderingDataContainer.erase(ivec3To64xInt(data.second.x, data.second.y, data.second.z));

        }

    }

    for (const auto& data : tmp) {

        // std::cout << &data.second << "\n";

        if (!(data.second.x + Position.x > rd || data.second.y + Position.y > rd || data.second.z + Position.z > rd || data.second.x + Position.x < -rd || data.second.y + Position.y < -rd || data.second.z + Position.z < -rd)) {//if (FindDistance(data.second.x, data.second.y, data.second.z, (int)x12 / CHUNK_SIZE, (int)y12 / CHUNK_SIZE, (int)z12 / CHUNK_SIZE) <= renderDistance) {
            if (fr.SphereInFrustum((float)data.second.x * CHUNK_SIZE, (float)data.second.y * CHUNK_SIZE, (float)data.second.z * CHUNK_SIZE, (float)CHUNK_SIZE * 2)) {
                WorldRenderingDataContainer[data.first].TchunkDraw();
            }
            // dataSorted.push_back(RenderingDataContainer[data.first]);

            // RenderingDataContainer[data.first].TchunkDraw();
        }

    }
  
}


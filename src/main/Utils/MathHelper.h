#pragma once
#ifndef MATH_HELP_H_
#define MATH_HELP_H_

#define DEGTORAD 0.017453

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

float fastsqrt(float n);

double FindDistance(int x, int y, int z, int x1, int y1, int z1);
double FindDistance(int x, int y, int x1, int y1);

double FindDistance(glm::ivec3 pos0, glm::ivec3 pos1);
double FindDistance(glm::ivec2 pos0, glm::ivec2 pos1);

double FindDistance(glm::vec3 pos0, glm::vec3 pos1);
double FindDistance(glm::vec2 pos0, glm::vec2 pos1);

double FindDistance(int x, int x1);
double FindDistance(float x, float x1);
double FindDistance(double x, double x1);

float FastFindDistance(int x, int y, int z, int x1, int y1, int z1);
float FastFindDistance(int x, int y, int x1, int y1);

float FastFindDistance(double x, double y, double z, double x1, double y1, double z1);
float FastFindDistance(double x, double y, double x1, double y1);

long long int getChunkID(int x, int y, int z);
long long int getChunkID(glm::ivec3 vec);

//glm::ivec3 IntToIVec3(int n);
#endif
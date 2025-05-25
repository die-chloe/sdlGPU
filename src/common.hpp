#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include <SDL3/SDL.h>

#include "Renderer/Renderer.hpp"

void SDLException(const std::string& message);
extern const char* BasePath;


#define VERTEX_TYPE_NONE uint8_t(0)

#define VERTEX_TYPE_POSITION uint8_t(1)
typedef struct VertexPosition
{
    float x, y, z;
} VertexPosition;

#define VERTEX_TYPE_POSITION_COLOR uint8_t(2)
typedef struct VertexPositionColor
{
    float x, y, z;
    uint8_t r, g, b, a; // Color components
} VertexPositionColor;

#define VERTEX_TYPE_POSITION_TEXTURE uint8_t(3)
typedef struct VertexPositionTexture
{
    float x, y, z;
    float u, v; // Texture coordinates
} VertexPositionTexture;

#define VERTEX_TYPE_POSITION_COLOR_TEXTURE uint8_t(4)
typedef struct VertexPositionColorTexture
{
    float x, y, z;
    uint8_t r, g, b, a; // Color components
    float u, v; // Texture coordinates
} VertexPositionColorTexture;

#define VERTEX_TYPE_POSITION_NORMAL uint8_t(5)
typedef struct VertexPositionNormal
{
    float x, y, z;
    float nx, ny, nz; // Normal vector
} VertexPositionNormal;

#define VERTEX_TYPE_POSITION_NORMAL_TEXTURE uint8_t(6)
typedef struct VertexPositionNormalTexture
{
    float x, y, z;
    float nx, ny, nz; // Normal vector
    float u, v; // Texture coordinates
} VertexPositionNormalTexture;

#define VERTEX_TYPE_POSITION_NORMAL_COLOR uint8_t(7)
typedef struct VertexPositionNormalColor
{
    float x, y, z;
    float nx, ny, nz; // Normal vector
    uint8_t r, g, b, a; // Color components
} VertexPositionNormalColor;

#define VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE uint8_t(8)
typedef struct VertexPositionNormalColorTexture
{
    float x, y, z;
    float nx, ny, nz; // Normal vector
    uint8_t r, g, b, a; // Color components
    float u, v; // Texture coordinates
} VertexPositionNormalColorTexture;
#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

void SDLException(const std::string& message);
extern const char* BasePath;


typedef enum VERTEX_TYPE
{
    VERTEX_TYPE_NONE,
    VERTEX_TYPE_POSITION,
    VERTEX_TYPE_POSITION_COLOR,
    VERTEX_TYPE_POSITION_UV,
    VERTEX_TYPE_POSITION_COLOR_UV,
    VERTEX_TYPE_POSITION_NORMAL,
    VERTEX_TYPE_POSITION_NORMAL_UV,
    VERTEX_TYPE_POSITION_NORMAL_COLOR,
    VERTEX_TYPE_POSITION_NORMAL_COLOR_UV
} VertexType;

typedef struct VertexPosition
{
    glm::vec3 position; // Position in 3D space
} VertexPosition;


typedef struct VertexPositionColor
{
    glm::vec3 position; // Position in 3D space
    uint8_t r, g, b, a; // Color components
} VertexPositionColor;

typedef struct VertexPositionUV
{
    glm::vec3 position; // Position in 3D space
    glm::vec2 uv; // Texture coordinates
} VertexPositionUV;

typedef struct VertexPositionColorUV
{
    glm::vec3 position; // Position in 3D space
    uint8_t r, g, b, a; // Color components
    glm::vec2 uv; // Texture coordinates
} VertexPositionColorUV;

typedef struct VertexPositionNormal
{
    glm::vec3 position; // Position in 3D space
    glm::vec3 normal; // Normal vector
} VertexPositionNormal;

typedef struct VertexPositionNormalUV
{
    glm::vec3 position; // Position in 3D space
    glm::vec3 normal; // Normal vector
    glm::vec2 uv; // Texture coordinates
} VertexPositionNormalUV;

typedef struct VertexPositionNormalColor
{
    glm::vec3 position; // Position in 3D space
    glm::vec3 normal; // Normal vector
    uint8_t r, g, b, a; // Color components
} VertexPositionNormalColor;

typedef struct VertexPositionNormalColorUV
{
    glm::vec3 position; // Position in 3D space
    glm::vec3 normal; // Normal vector
    uint8_t r, g, b, a; // Color components
    glm::vec2 uv; // Texture coordinates
} VertexPositionNormalColorUV;
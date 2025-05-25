#pragma once
#include "common.hpp"
#include <SDL3/SDL_gpu.h>

class VertexBuffer
{
    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        static void GetVertexInfo(uint8_t vertexType,
            std::vector<SDL_GPUVertexAttribute>& attributes,
            std::vector<SDL_GPUVertexBufferDescription>& buffers,
            SDL_GPUVertexInputState& vertexInputState);

    private:
};
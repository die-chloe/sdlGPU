#pragma once
#include "common.hpp"
#include <SDL3/SDL_gpu.h>

class VertexBuffer
{
    public:
        VertexBuffer(SDL_GPUDevice* device, uint32_t bufferSize);
        virtual ~VertexBuffer();
        SDL_GPUBuffer* Buffer{nullptr};
        uint32_t Size{0};

        void Cleanup()
        {
            if (Buffer)
            {
                SDL_ReleaseGPUBuffer(m_Device, Buffer);
                Buffer = nullptr;
            }
        }

        static void GetVertexInfo(uint8_t vertexType,
            std::vector<SDL_GPUVertexAttribute>& attributes,
            std::vector<SDL_GPUVertexBufferDescription>& buffers,
            SDL_GPUVertexInputState& vertexInputState);



    private:
        SDL_GPUDevice* m_Device{nullptr};
};

class IndexBuffer
{
    public:
        IndexBuffer();
        virtual ~IndexBuffer();

    void Cleanup()
    {
        if (m_IndexBuffer)
        {
            SDL_ReleaseGPUBuffer(m_Device, m_IndexBuffer);
            m_IndexBuffer = nullptr;
        }
    }

    private:
        SDL_GPUDevice* m_Device{nullptr};
        SDL_GPUBuffer* m_IndexBuffer{nullptr};
        SDL_GPUTransferBuffer* m_TransferBuffer{nullptr};
};

class TransferBuffer
{
    public:
        TransferBuffer(SDL_GPUDevice* device, uint32_t bufferSize);
        virtual ~TransferBuffer();

        void Cleanup()
        {
            if (Buffer)
            {
                SDL_ReleaseGPUTransferBuffer(m_Device, Buffer);
                Buffer = nullptr;
            }
        }

        SDL_GPUTransferBuffer* Buffer{nullptr};
        uint32_t Size{0};

        void UploadVertexBuffer(VertexBuffer& vertexBuffer, const void* data);
    
    private:
        SDL_GPUDevice* m_Device{nullptr};
};
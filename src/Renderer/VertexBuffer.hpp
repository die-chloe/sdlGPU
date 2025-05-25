#pragma once
#include "common.hpp"
#include <SDL3/SDL_gpu.h>

class VertexBuffer
{
    public:
        VertexBuffer(SDL_GPUDevice* device, uint32_t bufferSize);
        virtual ~VertexBuffer();

        void Cleanup()
        {
            if (m_VertexBuffer)
            {
                SDL_ReleaseGPUBuffer(m_Device, m_VertexBuffer);
                m_VertexBuffer = nullptr;
            }
            // if (m_TransferBuffer)
            // {
            //     SDL_ReleaseGPUTransferBuffer(m_Device, m_TransferBuffer);
            //     m_TransferBuffer = nullptr;
            // }
        }

        static void GetVertexInfo(uint8_t vertexType,
            std::vector<SDL_GPUVertexAttribute>& attributes,
            std::vector<SDL_GPUVertexBufferDescription>& buffers,
            SDL_GPUVertexInputState& vertexInputState);

        void UploadData(const void* data, const uint32_t size);

        SDL_GPUBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

    private:
        SDL_GPUDevice* m_Device{nullptr};
        SDL_GPUBuffer* m_VertexBuffer{nullptr};
        SDL_GPUTransferBuffer* m_TransferBuffer{nullptr};
};
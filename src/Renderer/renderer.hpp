#pragma once

#include "common.hpp"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>


class Renderer
{
	public:
		Renderer(SDL_Window* window);
		virtual ~Renderer();
		
		void Init();
		void Cleanup();
		
		SDL_GPUDevice* Device{nullptr};

		SDL_GPUShader* LoadShader(
			const std::string& shaderSource,
			const uint32_t samplerCount = 0,
			const uint32_t uniformBufferCount = 0,
			const uint32_t storageBufferCount = 0,
			const uint32_t storageTextureCount = 0
		);
		
		SDL_GPUGraphicsPipeline* CreatePipeline(
			SDL_GPUShader* vertexShader,
			SDL_GPUShader* fragmentShader,
			uint8_t vertexType = 0
		);

		void ReleasePipeline(SDL_GPUGraphicsPipeline* pipeline);
		

		void InitCommandBuffer();

		void RenderPassDraw(SDL_GPUGraphicsPipeline* pipeline);

		void SubmitCommandBuffer();


	private:
		SDL_Window* m_Window{nullptr};

		SDL_GPUCommandBuffer* m_CommandBuffer{nullptr};

		SDL_GPUTexture* m_SwapchainTexture{nullptr};

};
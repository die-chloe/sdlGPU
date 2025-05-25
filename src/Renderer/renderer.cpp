#include "Renderer.hpp"
#include "VertexBuffer.hpp"


Renderer::Renderer(SDL_Window* window)
{
	m_Window = window;

	// Create a GPU device with SPIR-V, DirectX IL, and Metal shader formats enabled
	// and claim the window for the GPU device
	Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
							SDL_GPU_SHADERFORMAT_DXIL |
							SDL_GPU_SHADERFORMAT_MSL,
							true,nullptr);
	if (!Device)
		SDLException("Failed to create GPU device");

	if (!SDL_ClaimWindowForGPUDevice(Device, window))
	{
		SDLException("Failed to claim window for GPU device");
	}

	printf("Using GPU Driver: %s\n", SDL_GetGPUDeviceDriver(Device));
}

Renderer::~Renderer()
{
	Cleanup();
}



SDL_GPUShader* Renderer::LoadShader(const std::string& shaderSource, const uint32_t samplerCount, const uint32_t uniformBufferCount, const uint32_t storageBufferCount, const uint32_t storageTextureCount)
{

	SDL_GPUShaderStage stage;
	if (shaderSource.contains(".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (shaderSource.contains(".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDLException("Unrecognized shader type. Shader Filenames must contain either '.vert' or '.frag'");
		return nullptr;
	}

	std::string fullPath;
	std::string entryPoint;

	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(Device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
	{
		fullPath = std::string(BasePath) + "shaders/" + shaderSource + ".spv";
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entryPoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
	{
		fullPath = std::string(BasePath) + "shaders/" + shaderSource + ".dxil";
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entryPoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
	{
		fullPath = std::string(BasePath) + "shaders/" + shaderSource + ".msl";
		format = SDL_GPU_SHADERFORMAT_MSL;
		entryPoint = "main";
	}
	else
	{
		SDLException("No supported shader formats available");
		return nullptr;
	}

	size_t codeSize;
	uint8_t* code = static_cast<uint8_t*>(SDL_LoadFile(fullPath.c_str(), &codeSize));
	if (!code)
	{
		SDLException("Failed to load shader file: " + fullPath);
		return nullptr;
	}

	SDL_GPUShaderCreateInfo shaderInfo{};
	shaderInfo.code = code;
	shaderInfo.code_size = codeSize;
	shaderInfo.entrypoint = entryPoint.c_str();
	shaderInfo.format = format;
	shaderInfo.stage = stage;
	shaderInfo.num_samplers = samplerCount;
	shaderInfo.num_uniform_buffers = uniformBufferCount;
	shaderInfo.num_storage_buffers = storageBufferCount;
	shaderInfo.num_storage_textures = storageTextureCount;

	SDL_GPUShader* shader = SDL_CreateGPUShader(Device, &shaderInfo);
	if (!shader)
	{
		SDLException("Failed to create GPU shader");
		SDL_free(code);
		return nullptr;
	}

	SDL_free(code);
	return shader;

}

SDL_GPUGraphicsPipeline* Renderer::CreatePipeline(SDL_GPUShader* vertexShader, SDL_GPUShader* fragmentShader, uint8_t vertexType)
{
	SDL_GPUColorTargetDescription colorTargetDescription{};
	colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(Device, m_Window);
	std::vector colorTargetDescriptions{ colorTargetDescription };

	SDL_GPUGraphicsPipelineTargetInfo targetInfo{};
	targetInfo.color_target_descriptions = colorTargetDescriptions.data();
	targetInfo.num_color_targets = colorTargetDescriptions.size();
	
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.vertex_shader = vertexShader;
	pipelineCreateInfo.fragment_shader = fragmentShader;
	pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	pipelineCreateInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
	pipelineCreateInfo.target_info = targetInfo;
	
	std::vector<SDL_GPUVertexAttribute> vertexAttributes{};
	std::vector<SDL_GPUVertexBufferDescription> vertexBufferDescriptions{};
	if(vertexType != VERTEX_TYPE_NONE)
	{
		SDL_GPUVertexInputState vertexInputState{};
		VertexBuffer::GetVertexInfo(vertexType, vertexAttributes, vertexBufferDescriptions, vertexInputState);
		pipelineCreateInfo.vertex_input_state = vertexInputState;
	}
	
	auto pipeline = SDL_CreateGPUGraphicsPipeline(Device, &pipelineCreateInfo);

	SDL_ReleaseGPUShader(Device, vertexShader);
	SDL_ReleaseGPUShader(Device, fragmentShader);
	return pipeline;
}

void Renderer::ReleasePipeline(SDL_GPUGraphicsPipeline * pipeline)
{
	if(pipeline)
		SDL_ReleaseGPUGraphicsPipeline(Device, pipeline);
}



void Renderer::InitCommandBuffer()
{
	m_CommandBuffer = SDL_AcquireGPUCommandBuffer(Device);
	if (!m_CommandBuffer)
		SDLException("Failed to acquire GPU command buffer");

	
	SDL_WaitAndAcquireGPUSwapchainTexture(m_CommandBuffer, m_Window, &m_SwapchainTexture, nullptr, nullptr);
}

void Renderer::RenderPassDraw(SDL_GPUGraphicsPipeline *pipeline, VertexBuffer* vertexBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	if(!m_SwapchainTexture)
	{
		return;
	}

	SDL_GPUColorTargetInfo colorTarget{};
	colorTarget.texture = m_SwapchainTexture;
	colorTarget.store_op = SDL_GPU_STOREOP_STORE;
	colorTarget.load_op = SDL_GPU_LOADOP_CLEAR;
	colorTarget.clear_color = SDL_FColor{0.1f, 0.1f, 0.2f, 1.0f};

	std::vector colorTargets{colorTarget};

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(m_CommandBuffer, colorTargets.data(), colorTargets.size(), nullptr);

	SDL_BindGPUGraphicsPipeline(renderPass, pipeline);

	if(vertexBuffer)
	{
		SDL_GPUBufferBinding vertexBufferBinding{};
		vertexBufferBinding.buffer = vertexBuffer->GetVertexBuffer();
		vertexBufferBinding.offset = 0;
		
		SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBufferBinding, 1);
	}

	SDL_DrawGPUPrimitives(renderPass, vertexCount, instanceCount, firstVertex, firstInstance);
	
	SDL_EndGPURenderPass(renderPass);
}

void Renderer::SubmitCommandBuffer()
{
	if(!SDL_SubmitGPUCommandBuffer(m_CommandBuffer))
		SDLException("Failed to submit GPU command buffer");
}

void Renderer::Cleanup()
{
	if (Device)
	{
		SDL_DestroyGPUDevice(Device);
		Device = nullptr;
	}
}
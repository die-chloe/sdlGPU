#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <vector>


class SDLException final : public std::runtime_error {
public:
	explicit SDLException(const std::string& message) : std::runtime_error(message + '\n' + SDL_GetError()) {}
};

static const char* BasePath{};

void InitializeAssetLoader()
{
	BasePath = SDL_GetBasePath();
}

SDL_GPUShader* LoadShader(
	SDL_GPUDevice* device,
	const std::string& shaderSource,
	const uint32_t samplerCount,
	const uint32_t uniformBufferCount,
	const uint32_t storageBufferCount,
	const uint32_t storageTextureCount
) {
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
		throw SDLException{"Unrecognized shader type. Shader Filenames must contain either '.vert' or '.frag'."};
		return nullptr;
	}

	std::string fullPath;
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;

	std::string entryPoint;


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
		throw SDLException{"No supported shader formats available."};
		return nullptr;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath.c_str(), &codeSize);
	if (code == nullptr)
	{
		std::printf("Failed to load shader file: %s\n", fullPath.c_str());
		return nullptr;
	}


	SDL_GPUShaderCreateInfo shaderInfo{};
	shaderInfo.code = static_cast<const uint8_t*>(code);
	shaderInfo.code_size = codeSize;
	shaderInfo.entrypoint = entryPoint.c_str();
	shaderInfo.format = format;
	shaderInfo.stage = stage;
	shaderInfo.num_samplers = samplerCount;
	shaderInfo.num_uniform_buffers = uniformBufferCount;
	shaderInfo.num_storage_buffers = storageBufferCount;
	shaderInfo.num_storage_textures = storageTextureCount;


	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == nullptr)
	{
		throw SDLException{"Failed to create GPU shader: " + std::string(SDL_GetError())};
		SDL_free(code);
		return nullptr;
	}

	SDL_free(code);
	return shader;

}


int main(int argc, char* argv[]) {

	// Initialize SDL with video subsystem,
	// asset loader,
	// and create a window
	if(!SDL_Init(SDL_INIT_VIDEO))
		throw SDLException{"Failed to initialize SDL"};

	InitializeAssetLoader();

	SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1920, 1080, SDL_WINDOW_RESIZABLE);
	if (!window)
		throw SDLException{"Failed to create SDL window"};



	// Create a GPU device with SPIR-V, DirectX IL, and Metal shader formats enabled
	// and claim the window for the GPU device
	SDL_GPUDevice* device {
		SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
							SDL_GPU_SHADERFORMAT_DXIL |
							SDL_GPU_SHADERFORMAT_MSL,
							true,nullptr)
	};
	if (!device)
		throw SDLException{"Failed to create GPU device"};
	if (!SDL_ClaimWindowForGPUDevice(device, window))
		throw SDLException{"Failed to claim window for GPU device"};

	
	// Print GPU device information
	std::printf("Using GPU device driver: %s\n", SDL_GetGPUDeviceDriver(device));








	// Load vertex and fragment shaders -------------------------------------------------------------------
	// The shaders are expected to be in the "shaders" directory relative to the base path
	SDL_GPUShader* vertexShader{LoadShader(device, "test.vert", 0, 0, 0, 0)};
	if(!vertexShader)
		throw SDLException{"Failed to load vertex shader"};
	
	SDL_GPUShader* fragmentShader{LoadShader(device, "test.frag", 0, 0, 0, 0)};
	if(!fragmentShader)
		throw SDLException{"Failed to load fragment shader"};




	// Create a basic graphics pipeline ------------------------------------------------------------------------
	SDL_GPUColorTargetDescription colorTargetDescription{};
	colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(device, window);
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
	
	
	SDL_GPUGraphicsPipeline* pipeline{SDL_CreateGPUGraphicsPipeline(device, &pipelineCreateInfo)};
	
	if (!pipeline)
	throw SDLException{"Failed to create GPU graphics pipeline: " + std::string(SDL_GetError())};
	
	
	
	SDL_ReleaseGPUShader(device, vertexShader);
	SDL_ReleaseGPUShader(device, fragmentShader);
	
	
	
	// Main loop -----------------------------------------------------------------------------------------
	SDL_ShowWindow(window);
	bool running = true;
	SDL_Event event;


	while (running)
	{
		// Process events -------------------------------------------------------------------------------
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;

			default:
				break;
			}
		}


		// Process GPU commands ----------------------------------------------------------------------
		SDL_GPUCommandBuffer* commandBuffer{SDL_AcquireGPUCommandBuffer(device)};
		if (!commandBuffer)
			throw SDLException{"Failed to acquire GPU command buffer"};

		SDL_GPUTexture* swapchainTexture;
		SDL_AcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, nullptr, nullptr);
		if(swapchainTexture != NULL)
		{
			// Begin a render pass with the swapchain texture as the color target
			SDL_GPUColorTargetInfo colorTarget{};
			colorTarget.texture = swapchainTexture;
			colorTarget.store_op = SDL_GPU_STOREOP_STORE;
			colorTarget.load_op = SDL_GPU_LOADOP_CLEAR;
			colorTarget.clear_color = SDL_FColor(0.1f, 0.1f, 0.2f, 1.0f);

			std::vector colorTargets{colorTarget};
			SDL_GPURenderPass* renderPass{SDL_BeginGPURenderPass(commandBuffer, colorTargets.data(), colorTargets.size(), nullptr)};
			
			
			SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
			SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

		
			SDL_EndGPURenderPass(renderPass);
		}

		if(!SDL_SubmitGPUCommandBuffer(commandBuffer))
			throw SDLException{"Failed to submit GPU command buffer"};

		SDL_Delay(16); // Limit to ~60 FPS for now
	}


	// Cleanup
	SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
	SDL_DestroyGPUDevice(device);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main(__argc, __argv);
}
#endif
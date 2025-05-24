#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <stdexcept>


class SDLException final : public std::runtime_error {
public:
	explicit SDLException(const std::string& message) : std::runtime_error(message + '\n' + SDL_GetError()) {}
};



int main(int argc, char* argv[]) {

	// Initialize SDL with video subsystem
	// and create a window
	if(!SDL_Init(SDL_INIT_VIDEO))
		throw SDLException("Failed to initialize SDL");

	SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1920, 1080, SDL_WINDOW_RESIZABLE);
	if (!window)
		throw SDLException("Failed to create SDL window");



	// Create a GPU device with SPIR-V, DirectX IL, and Metal shader formats enabled
	// and claim the window for the GPU device
	SDL_GPUDevice* device {
		SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
							SDL_GPU_SHADERFORMAT_DXIL |
							SDL_GPU_SHADERFORMAT_MSL,
							true,nullptr)
	};
	if (!device)
		throw SDLException("Failed to create GPU device");
	if (!SDL_ClaimWindowForGPUDevice(device, window))
		throw SDLException("Failed to claim window for GPU device");



	
	
	// Main loop
	SDL_ShowWindow(window);
	bool running = true;
	SDL_Event event;


	while (running)
	{
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
		SDL_Delay(16); // Limit to ~60 FPS for now
	}


	// Cleanup
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
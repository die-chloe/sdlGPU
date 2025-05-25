#include "common.hpp"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

void SDLException(const std::string& message) {
    printf("%s: %s", message.c_str(), SDL_GetError());
    throw std::runtime_error(message);
}

const char* BasePath = nullptr;

void InitializeAssetLoader()
{
	BasePath = SDL_GetBasePath();
	if (!BasePath)
		SDLException("Failed to get base path for assets");
}


int main(int argc, char* argv[]) {

	// Initialize SDL with video subsystem,
	// asset loader,
	// and create a window
	if(!SDL_Init(SDL_INIT_VIDEO))
		SDLException("Failed to initialize SDL");

	InitializeAssetLoader();

	SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1920, 1080, SDL_WINDOW_RESIZABLE);
	if (!window)
		SDLException("Failed to create SDL window");


	// Initialize the Renderer
	// The renderer class will handle the GPU device and command buffer
	Renderer renderer(window);


	// Load vertex and fragment shaders
	// The shaders are expected to be in the "shaders" directory relative to the base path
	SDL_GPUShader* vertexShader{renderer.LoadShader("PositionColor.vert")};
	SDL_GPUShader* fragmentShader{renderer.LoadShader("Color.frag")};
	if (!vertexShader || !fragmentShader)
		SDLException("Failed to load shaders");


	// Create a basic graphics pipeline
	auto pipeline = renderer.CreatePipeline(vertexShader, fragmentShader, VERTEX_TYPE_POSITION_COLOR);
	if (!pipeline)
		SDLException("Failed to create graphics pipeline");






	
	// Main loop -----------------------------------------------------------------------------------------
	SDL_ShowWindow(window);
	bool running = true;
	SDL_Event event;


	while (running)
	{
		// Process events
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
		renderer.InitCommandBuffer();
		
		renderer.RenderPassDraw(pipeline);


		renderer.SubmitCommandBuffer();
		// End of GPU commands ----------------------------------------------------------------------

		SDL_Delay(16); // Limit to ~60 FPS for now
	}


	// Cleanup
	renderer.ReleasePipeline(pipeline);
	renderer.Cleanup();

	
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
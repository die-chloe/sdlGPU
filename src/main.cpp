#include "common.hpp"
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include "Renderer/Renderer.hpp"

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

	SDL_Window* window = SDL_CreateWindow("Triangle!", 1080, 1080, SDL_WINDOW_RESIZABLE);
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


	// Create a vertex buffer with the appropriate vertex type
	std::vector<VertexPositionColor> vertices = {
		{ glm::vec4{-0.8f, -0.8f, 0.0f, 1.0f}, 255, 0, 0, 255 }, // Bottom left red
		{ glm::vec4{ 0.8f, -0.8f, 0.0f, 1.0f}, 0, 255, 0, 255 }, // Bottom right green
		{ glm::vec4{ 0.0f,  0.8f, 0.0f, 1.0f}, 0, 0, 255, 255 }  // Top center blue
	};

	std::vector<glm::vec2> positions = {
		{ glm::sin(0) * 0.8f, glm::cos(0) * 0.8f },
		{ glm::sin(6.283f/3) * 0.8f, glm::cos(6.283f/3) * 0.8f },
		{ glm::sin(6.283f*2/3) * 0.8f, glm::cos(6.283f*2/3) * 0.8f }
	};

	glm::mat2x2 rotationMatrix = {
		{ glm::cos(0.016f), -glm::sin(0.016f) },
		{ glm::sin(0.016f),  glm::cos(0.016f) }
	};

	VertexBuffer vertexBuffer(renderer.Device, sizeof(VertexPositionColor) * vertices.size());
	TransferBuffer transferBuffer(renderer.Device, sizeof(VertexPositionColor) * vertices.size());



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
		
		int i = 0;
		for(auto& position : positions)
		{
			position = rotationMatrix * position; // Rotate the position
			// Update the vertex positions in the vertices vector
			vertices[i].position.x = position.x;
			vertices[i].position.y = position.y;
			vertices[i].position.z = 0.0f; // Keep z at 0 for 2D rendering

			i++;
		}


		// Process GPU commands ----------------------------------------------------------------------
		renderer.InitCommandBuffer();

		transferBuffer.UploadVertexBuffer(vertexBuffer, vertices.data());

		renderer.RenderPassDraw(pipeline, vertices.size(), &vertexBuffer, 1, 0, 0);

		renderer.SubmitCommandBuffer();
		// End of GPU commands ----------------------------------------------------------------------

		SDL_Delay(16); // Limit to ~60 FPS for now
	}


	// Cleanup
	transferBuffer.Cleanup();
	vertexBuffer.Cleanup();
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
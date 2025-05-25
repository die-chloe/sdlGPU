#include "VertexBuffer.hpp"

void VertexBuffer::GetVertexInfo(uint8_t vertexType, std::vector<SDL_GPUVertexAttribute>& vertexAttributes,
	std::vector<SDL_GPUVertexBufferDescription>& vertexBufferDescriptions, SDL_GPUVertexInputState& vertexInputState)
{

	SDL_GPUVertexBufferDescription vertexBufferDescription{};

	vertexInputState.num_vertex_buffers = 1;

	vertexBufferDescription.slot = 0;
	vertexBufferDescription.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
	vertexBufferDescription.instance_step_rate = 0;
	
	SDL_GPUVertexAttribute positionAttribute{};
	SDL_GPUVertexAttribute colorAttribute{};
	SDL_GPUVertexAttribute textureAttribute{};
	SDL_GPUVertexAttribute normalAttribute{};

	switch (vertexType)
	{
	case VERTEX_TYPE_POSITION:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);
		

		vertexBufferDescription.pitch = sizeof(VertexPosition);

		vertexInputState.num_vertex_attributes = 1;
		vertexInputState.vertex_attributes = vertexAttributes.data();
		break;
	

	case VERTEX_TYPE_POSITION_COLOR:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		colorAttribute.buffer_slot = 0;
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4;;
		colorAttribute.offset = sizeof(float) * 3; // After position
		colorAttribute.location = 1;

		vertexAttributes.push_back(colorAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionColor);

		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;


	case VERTEX_TYPE_POSITION_TEXTURE:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 3; // After position
		textureAttribute.location = 1;
		vertexAttributes.push_back(textureAttribute);


		vertexBufferDescription.pitch = sizeof(VertexPositionTexture);
		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	
	case VERTEX_TYPE_POSITION_COLOR_TEXTURE:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		colorAttribute.buffer_slot = 0;
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4;;
		colorAttribute.offset = sizeof(float) * 3; // After position
		colorAttribute.location = 1;

		vertexAttributes.push_back(colorAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 3 + sizeof(uint8_t) * 4; // After position and color
		textureAttribute.location = 2;

		vertexAttributes.push_back(textureAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionColorTexture);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	

	case VERTEX_TYPE_POSITION_NORMAL:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		normalAttribute.buffer_slot = 0;
		normalAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		normalAttribute.offset = sizeof(float) * 3; // After position
		normalAttribute.location = 1;

		vertexAttributes.push_back(normalAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormal);
		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	

	case VERTEX_TYPE_POSITION_NORMAL_TEXTURE:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		normalAttribute.buffer_slot = 0;
		normalAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		normalAttribute.offset = sizeof(float) * 3; // After position
		normalAttribute.location = 1;

		vertexAttributes.push_back(normalAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 6; // After position and normal
		textureAttribute.location = 2;

		vertexAttributes.push_back(textureAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormalTexture);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;


	case VERTEX_TYPE_POSITION_NORMAL_COLOR:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		normalAttribute.buffer_slot = 0;
		normalAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		normalAttribute.offset = sizeof(float) * 3; // After position
		normalAttribute.location = 1;

		vertexAttributes.push_back(normalAttribute);

		
		colorAttribute.buffer_slot = 0;
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4;;
		colorAttribute.offset = sizeof(float) * 6; // After position and normal
		colorAttribute.location = 2;

		vertexAttributes.push_back(colorAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormalColor);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;

	case VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE:
		positionAttribute.buffer_slot = 0;
		positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		
		vertexAttributes.push_back(positionAttribute);

		
		normalAttribute.buffer_slot = 0;
		normalAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		normalAttribute.offset = sizeof(float) * 3; // After position
		normalAttribute.location = 1;

		vertexAttributes.push_back(normalAttribute);

		
		colorAttribute.buffer_slot = 0;
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4;;
		colorAttribute.offset = sizeof(float) * 6; // After position and normal
		colorAttribute.location = 2;

		vertexAttributes.push_back(colorAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 10 + sizeof(uint8_t) * 4; // After position, normal, and color
		textureAttribute.location = 3;

		vertexAttributes.push_back(textureAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormalColorTexture);
		vertexInputState.num_vertex_attributes = 4;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;

		
	default:
		break;
	}

	vertexBufferDescriptions = { vertexBufferDescription };
	vertexInputState.vertex_buffer_descriptions = vertexBufferDescriptions.data();
}
#include "Buffer.hpp"

VertexBuffer::VertexBuffer(SDL_GPUDevice* device, uint32_t bufferSize):
	m_Device(device), Size(bufferSize)
{
	SDL_GPUBufferCreateInfo vertexBufferCreateInfo{};
	vertexBufferCreateInfo.size = bufferSize;
	vertexBufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;

	Buffer = SDL_CreateGPUBuffer(m_Device, &vertexBufferCreateInfo);
	if (!Buffer)
	{
		SDLException("Failed to create GPU vertex buffer");
	}
}

VertexBuffer::~VertexBuffer()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Retrieves vertex information based on the specified vertex type.
///
/// This function populates the vertex attributes, buffer descriptions,
/// and vertex input state based on the provided vertex type.
///
/// @param vertexType The type of vertex to retrieve information for.
/// @param vertexAttributes A vector to store the vertex attributes.
/// @param vertexBufferDescriptions A vector to store the vertex buffer descriptions.
/// @param vertexInputState The vertex input state to populate with the retrieved information.
////////////////////////////////////////////////////////////////////////////////
void VertexBuffer::GetVertexInfo(uint8_t vertexType,
								 std::vector<SDL_GPUVertexAttribute> &vertexAttributes,
                                 std::vector<SDL_GPUVertexBufferDescription> &vertexBufferDescriptions,
								 SDL_GPUVertexInputState &vertexInputState)
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
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
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
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
		colorAttribute.offset = sizeof(float) * 3; // After position
		colorAttribute.location = 1;

		vertexAttributes.push_back(colorAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 7; // After position and color
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
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
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
		colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
		colorAttribute.offset = sizeof(float) * 6; // After position and normal
		colorAttribute.location = 2;

		vertexAttributes.push_back(colorAttribute);

		
		textureAttribute.buffer_slot = 0;
		textureAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		textureAttribute.offset = sizeof(float) * 10; // After position, normal, and color
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



TransferBuffer::TransferBuffer(SDL_GPUDevice* device, uint32_t bufferSize):
m_Device(device), Size(bufferSize)
{
	SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo{};
	transferBufferCreateInfo.size = bufferSize;
	transferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	
	Buffer = SDL_CreateGPUTransferBuffer(m_Device, &transferBufferCreateInfo);
	if (!Buffer)
	{
		SDLException("Failed to create GPU transfer buffer");
	}
}

TransferBuffer::~TransferBuffer()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Uploads vertex data to the GPU using the transfer buffer.
/// 
/// SDL_GPU needs a transfer buffer of the same size as the vertex buffer
/// mapped in memory. So the buffer is mapped, data is copied to it,
/// then the upload command is submitted to the CommandBuffer.
/// 
/// @param vertexBuffer The vertex buffer to upload data to.
/// @param data Pointer to the vertex data to upload.
////////////////////////////////////////////////////////////////////////////////
void TransferBuffer::UploadVertexBuffer(VertexBuffer& vertexBuffer, const void* data)
{
	
	if (!Buffer || !vertexBuffer.Buffer)
	{
		SDLException("Vertex buffer or transfer buffer not initialized");
		return;
	}

	if (vertexBuffer.Size != Size)
	{
		SDLException("Vertex buffer size does not match transfer buffer size");
		return;
	}

	void* transferData = SDL_MapGPUTransferBuffer(m_Device, Buffer, false);
	if (!transferData)
	{
		SDLException("Failed to map GPU transfer buffer");
		return;
	}
	memcpy(transferData, data, vertexBuffer.Size);

	SDL_UnmapGPUTransferBuffer(m_Device, Buffer);

	SDL_GPUCommandBuffer* commandBuffer =  SDL_AcquireGPUCommandBuffer(m_Device);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

	SDL_GPUTransferBufferLocation transferLocation{};
	transferLocation.transfer_buffer = Buffer;
	transferLocation.offset = 0;

	SDL_GPUBufferRegion destinationRegion{};
	destinationRegion.buffer = vertexBuffer.Buffer;
	destinationRegion.offset = 0;
	destinationRegion.size = vertexBuffer.Size;

	SDL_UploadToGPUBuffer(copyPass, &transferLocation, &destinationRegion, false);

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(commandBuffer);

}


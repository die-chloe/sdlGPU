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
void VertexBuffer::GetVertexInfo(VERTEX_TYPE vertexType,
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
	SDL_GPUVertexAttribute uvAttribute{};
	SDL_GPUVertexAttribute normalAttribute{};

	
	positionAttribute.buffer_slot = 0;
	positionAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;

	
	colorAttribute.buffer_slot = 0;
	colorAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM;

			
	uvAttribute.buffer_slot = 0;
	uvAttribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;

	switch (vertexType)
	{
	case VERTEX_TYPE_POSITION:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPosition);
		vertexInputState.num_vertex_attributes = 1;
		vertexInputState.vertex_attributes = vertexAttributes.data();
		break;
	

	case VERTEX_TYPE_POSITION_COLOR:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		colorAttribute.offset = sizeof(glm::vec3); // After position
		colorAttribute.location = 1;
		vertexAttributes.push_back(colorAttribute);


		vertexBufferDescription.pitch = sizeof(VertexPositionColor);
		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;


	case VERTEX_TYPE_POSITION_UV:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);


		uvAttribute.offset = sizeof(glm::vec3); // After position
		uvAttribute.location = 1;
		vertexAttributes.push_back(uvAttribute);


		vertexBufferDescription.pitch = sizeof(VertexPositionUV);
		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	
	case VERTEX_TYPE_POSITION_COLOR_UV:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		colorAttribute.offset = sizeof(glm::vec3); // After position
		colorAttribute.location = 1;
		vertexAttributes.push_back(colorAttribute);

		uvAttribute.offset = sizeof(glm::vec3) + sizeof(uint8_t) * 4; // After position and color
		uvAttribute.location = 2;
		vertexAttributes.push_back(uvAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionColorUV);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	

	case VERTEX_TYPE_POSITION_NORMAL:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		normalAttribute.offset = sizeof(glm::vec3); // After position
		normalAttribute.location = 1;
		vertexAttributes.push_back(normalAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormal);
		vertexInputState.num_vertex_attributes = 2;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;
	

	case VERTEX_TYPE_POSITION_NORMAL_UV:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		normalAttribute.offset = sizeof(glm::vec3); // After position
		normalAttribute.location = 1;
		vertexAttributes.push_back(normalAttribute);

		uvAttribute.offset = sizeof(glm::vec3) * 2; // After position and normal
		uvAttribute.location = 2;
		vertexAttributes.push_back(uvAttribute);

		
		vertexBufferDescription.pitch = sizeof(VertexPositionNormalUV);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;


	case VERTEX_TYPE_POSITION_NORMAL_COLOR:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		normalAttribute.offset = sizeof(glm::vec3); // After position
		normalAttribute.location = 1;
		vertexAttributes.push_back(normalAttribute);

		colorAttribute.offset = sizeof(glm::vec3) * 2; // After position and normal
		colorAttribute.location = 2;
		vertexAttributes.push_back(colorAttribute);
		

		vertexBufferDescription.pitch = sizeof(VertexPositionNormalColor);
		vertexInputState.num_vertex_attributes = 3;
		vertexInputState.vertex_attributes = vertexAttributes.data();

		break;

	case VERTEX_TYPE_POSITION_NORMAL_COLOR_UV:
		positionAttribute.offset = 0;
		positionAttribute.location = 0;
		vertexAttributes.push_back(positionAttribute);

		normalAttribute.offset = sizeof(glm::vec3); // After position
		normalAttribute.location = 1;
		vertexAttributes.push_back(normalAttribute);

		colorAttribute.offset = sizeof(glm::vec3) * 2; // After position and normal
		colorAttribute.location = 2;
		vertexAttributes.push_back(colorAttribute);

		uvAttribute.offset = sizeof(glm::vec3) * 2 + sizeof(uint8_t) * 4; // After position, normal, and color
		uvAttribute.location = 3;
		vertexAttributes.push_back(uvAttribute);


		vertexBufferDescription.pitch = sizeof(VertexPositionNormalColorUV);
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


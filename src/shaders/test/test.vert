struct VSOutput
{
	float4 Position : SV_Position;
	[[vk::location(0)]] float3 Color : COLOR0;
};


VSOutput main(uint VertexIndex : SV_VertexID)
{
	float3 colors[] =
	{
		float3(1.0, 0.0, 0.0),
		float3(0.0, 1.0, 0.0),
		float3(0.0, 0.0, 1.0)
	};
	
	float2 positions[3] =
	{
		float2(0.0, -0.5),
		float2(0.5, 0.5),
		float2(-0.5, 0.5)
	};
	
	VSOutput output;
	output.Color = colors[VertexIndex];

	output.Position = float4(positions[VertexIndex], 0.0, 1.0);
	
	return output;
}
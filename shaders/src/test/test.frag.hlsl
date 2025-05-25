struct PSInput
{
	float3 color : TEXCOORD0; // Texture coordinate for color
};

float4 main(PSInput input) : SV_Target0
{	
	return float4(input.color, 1.0);
}
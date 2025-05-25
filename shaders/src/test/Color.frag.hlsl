struct PSInput
{
	float4 color : TEXCOORD0; // Texture coordinate for color
};

float4 main(PSInput input) : SV_Target0
{	
	return float4(input.color.xyz, 1.0);
}
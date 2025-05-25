struct Input
{
    float3 position : TEXCOORD0;
    float4 color : TEXCOORD1;
};

struct Output
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
};

Output main(Input input)
{
    Output output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}
struct PSInput
{
    float4 position : SV_POSITION;
};

struct Transform
{
    matrix transform;
};
ConstantBuffer<Transform> tran : register(b0);

struct CubeColors
{
	float4 colors[6];
};
ConstantBuffer<CubeColors> cubeColors : register(b0);

PSInput VSMain(float3 position : POSITION)
{
    PSInput result;

    result.position = mul(tran.transform, float4(position, 1));
    result.position.y *= -1;

    return result;
}

float4 PSMain(uint primitiveID : SV_PrimitiveID) : SV_TARGET
{
	return cubeColors.colors[primitiveID >> 1];
}

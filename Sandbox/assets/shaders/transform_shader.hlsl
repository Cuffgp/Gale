struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct Transform
{
    matrix transform;
};

ConstantBuffer<Transform> tran : register(b0);

PSInput VSMain(float3 position : POSITION, float3 color : COLOR)
{
    PSInput result;

    result.position = mul(tran.transform, float4(position, 1));
    result.position.y *= -1;
    result.color = float4(color, 1);

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUTPUT main_vs(float3 pos : POSITION)
{
	VS_OUTPUT output;
	output.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	output.uv = 0.5 * (pos.xy + float2(1.0, 1.0));
    return output;
}

cbuffer RTProperties : register(b0)
{
	float3 color;
};

float4 main_ps(VS_OUTPUT input) : SV_TARGET
{
	return float4(color, 1);
}

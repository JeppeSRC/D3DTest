Texture2D tex : register(t1);

SamplerState state {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressV = Wrap;
	AddressV = Wrap;
};

struct In {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
	float3 normal : NORMAL;
};

cbuffer Light : register(b1) {
	float4 lightDir;
};

float4 main(In i) : SV_TARGET
{
	float4 texCol = tex.Sample(state, i.uv);

	float brightness = dot(i.normal, -lightDir.xyz);

	return float4(brightness, brightness, brightness, 1);// *brightness;
}
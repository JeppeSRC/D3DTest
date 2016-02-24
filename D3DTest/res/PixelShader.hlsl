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
	float3 vertPos : VPOS;
};

cbuffer Light : register(b1) {
	float3 lightDir;
};

cbuffer PointLight : register(b2) {
	float3 l_lightPos;
	float3 l_lightColor;
	float l_constant;
	float l_linear;
	float l_exponent;
};

float4 main(In i) : SV_TARGET {

	float3 toLight = l_lightPos - i.vertPos;

	float distance = length(toLight);

	float atten = 1.0f / ((l_constant + l_linear * distance + l_exponent * (distance * distance)) +0.00000001f);

	float4 texCol = tex.Sample(state, i.uv);
	
	float brightness = dot(i.normal, -lightDir);
	
	//return float4(normalize(toLight), 1);
	return float4(texCol.xyz* brightness, 1);
}
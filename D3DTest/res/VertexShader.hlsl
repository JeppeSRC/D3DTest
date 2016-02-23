struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct In {
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : UV;
	float3 normal : NORMAL;
};

cbuffer Input : register(b0) {
	float4x4 projection;
	float4x4 model;
	float4x4 rotation;
};

Out main(In i)
{
	Out e;
	
	e.position = mul(projection, mul(model, mul(rotation, float4(i.position, 1))));
	e.normal = mul(rotation, float4(i.normal, 0));

	//e.color = float4(e.position.x * 0.5f + 0.5f, e.position.y * 0.5f + 0.5f, e.position.z * 0.5f + 0.5f, 1.0f);
	e.color = i.color;
	e.uv = i.uv;

	return e;
}
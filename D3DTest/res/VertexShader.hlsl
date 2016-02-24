struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 vertPos : VPOS;
};

struct In {
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : UV;
	float3 normal : NORMAL;
};

cbuffer Input : register(b0) {
	float4x4 projection;
	float4x4 view;
	float4x4 model;
};

Out main(In i)
{
	Out e;
	
	e.vertPos = mul(model, float4(i.position, 1)).xyz;
	e.position = mul(projection, mul(view, mul(model, float4(i.position, 1))));
	e.normal = mul(model, float4(i.normal, 0));

	//e.color = float4(e.position.x * 0.5f + 0.5f, e.position.y * 0.5f + 0.5f, e.position.z * 0.5f + 0.5f, 1.0f);
	e.color = i.color;
	e.uv = i.uv;

	return e;
}
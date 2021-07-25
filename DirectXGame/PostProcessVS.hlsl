struct VS_INPUT {
	float4 pos: POSITION0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT {
	float4 pos: SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = input.pos;
	output.texcoord = input.texcoord;

	return output;
}

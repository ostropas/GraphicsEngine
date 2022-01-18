Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);


struct PS_INPUT {
	float4 pos: SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer constant: register(b0)
{
	float m_distortion_level;
};

float2 distort(float2 p)
{
	float theta = atan2(p.y, p.x);
	float radius = length(p);

	radius = pow(radius, m_distortion_level);

	p.x = radius * cos(theta);
	p.y = radius * sin(theta);

	return 0.5 * (p + 1);
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float2 uv;
	float d = length(xy);

	if (d < 1)
	{
		uv = distort(xy);
	}
	else
	{
		uv = input.texcoord.xy;
	}

	float4 color = Texture.Sample(TextureSampler, uv);
	return color;
}

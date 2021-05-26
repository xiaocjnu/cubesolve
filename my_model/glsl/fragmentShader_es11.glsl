#version 300 es

precision mediump float;

uniform sampler2D sampler;
uniform bool enable_tex[2]; // texture enables



in vec4 v_texcoord0;
in vec4 v_texcoord1;
in vec4 v_front_color;
in vec4 v_back_color;
in float v_fog_factor;
in float v_ucp_factor;

out vec4 fragmentColor;

void main()
{
	if (enable_tex[0])
	{
		vec4 texColor = texture(sampler, v_texcoord0.xy);
		fragmentColor = v_front_color * texColor;
	}
	else
	{
		fragmentColor = v_front_color;
	}
	

	//fragmentColor = v_front_color;
	//fragmentColor.a = 1.0;
	//fragmentColor = vec4(0.5,0.5,0.5,1.0);
}
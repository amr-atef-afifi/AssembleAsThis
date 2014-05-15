#version 150
in  vec4 vColor;
out vec4 color;
in  vec2 exUV;
uniform float isDesk;
uniform sampler2D myTextureSampler;

void
main()
{
	if(isDesk < 0.5)
	{
		color = vColor;
	}
	else
	{
		color.rgb = texture2D( myTextureSampler, exUV ).rgb + vColor.rgb;
		color.a = 1.0;
	}
}

#version 430

layout(vertices = 3) out;
uniform float TessLevelInner;
uniform float TessLevelOuter;

void main(void)
{
	gl_TessLevelInner[0] = 1;

	gl_TessLevelOuter[0] = 1;
	gl_TessLevelOuter[1] = 1;
	gl_TessLevelOuter[2] = 1;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

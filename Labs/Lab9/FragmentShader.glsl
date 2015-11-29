#version 430

out vec4 out_color;

void main()
{
	float z = gl_FragCoord.z / gl_FragCoord.w;
	
	gl_FragColor = vec4(z,z,z,z);
	

	
}

#version 430 

layout(triangles, equal_spacing, ccw) in;

void main()
{	
	gl_Position.xyzw =	gl_in[0].gl_Position.xyzw * gl_TessCoord.x +
						gl_in[1].gl_Position.xyzw * gl_TessCoord.y +
						gl_in[2].gl_Position.xyzw * gl_TessCoord.z;
						
						
	//vec3 pos = normalize(vec3(gl_Position.x,gl_Position.y,gl_Position.z));
						
	
	vec3 pos = vec3(gl_Position.x,gl_Position.y,gl_Position.z);
	//normalize(pos);
	
	
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	
	float dx = x * sqrt(1.0 - (y*y/2.0) - (z*z/2.0) + (y*y*z*z/3.0));
	float dy = y * sqrt(1.0 - (z*z/2.0) - (x*x/2.0) + (z*z*x*x/3.0));
	float dz = z * sqrt(1.0 - (x*x/2.0) - (y*y/2.0) + (x*x*y*y/3.0));
	
	gl_Position.xyzw = vec4(dx/2,dy/2,dz/2, gl_Position.w);
						
}

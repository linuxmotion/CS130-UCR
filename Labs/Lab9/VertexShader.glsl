#version 430

in vec3 position;

void main()
{

							
	//vec3 pos = vec3(position.x,position.y,position.z);
						
	
	
	
	//float x = pos.x;
	//float y = pos.y;
	//float z = pos.z;
		
	
	//normalize(pos);
	
	//float dx = x * sqrt(1.0 - (y*y/2.0) - (z*z/2.0) + (y*y*z*z/3.0));
	//float dy = y * sqrt(1.0 - (z*z/2.0) - (x*x/2.0) + (z*z*x*x/3.0));
	//float dz = z * sqrt(1.0 - (x*x/2.0) - (y*y/2.0) + (x*x*y*y/3.0));
	
	gl_Position = vec4(position, 1.0);
	
	//gl_Position=vec4(pos,1.0);
}

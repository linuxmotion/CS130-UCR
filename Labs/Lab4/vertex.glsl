

float getLight(float normal, int light);
void main()
{
vec3 normal;
	
	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	/* now normalize the light's direction. Note that according to the.*/
    
    //gl_FrontColor = vec4(1,1,0,1)
    gl_FrontColor = getLight(normal, 0);
    gl_FrontColor += getLight(normal, 1);
    
	gl_Position = ftransform();
}


float getLight(float normal, int light){
    vec4 diffuse, ambient, globalAmbient, specular;
    vec3 lightDir;
	float NdotL, NdotHV;
    specular = 0;
    //OpenGL specification, the light is stored in eye space. Also since
	//we're talking about a directional light, the position field is actually
	//direction *///vec3(0,1,1));//
    lightDir = normalize(vec3(gl_LightSource[light].position));
	/* compute the cos of the angle between the normal and lights direction.

	The light is directional so the direction is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also
	need to clamp the result to the [0,1] range. */
	NdotL = max(dot(normal, lightDir), 0);
	
    /* Compute the diffuse term */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[light].diffuse;
    
    /* Compute the ambient and globalAmbient terms  */
	ambient = gl_FrontMaterial.ambient * gl_LightSource[light].ambient ;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
    
   /* compute the specular term if NdotL is  larger than zero */
	if (NdotL > 0.0) {

		NdotHV = max(dot(normal, normalize(gl_LightSource[light].halfVector.xyz)),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[light].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	return (NdotL * diffuse + globalAmbient + ambient + specular );
}

//varying vec4 diffuse,ambient;
//varying vec3 normal,halfVector;
 
//void main()
//{
    ///* first transform the normal into eye space and
    //normalize the result */
    //normal = normalize(gl_NormalMatrix * gl_Normal);
 
    ///* pass the halfVector to the fragment shader */
    //halfVector = gl_LightSource[0].halfVector.xyz;
 
    ///* Compute the diffuse, ambient and globalAmbient terms */
    //diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    //ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    //// add a little extra light
    //ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient ;
    //gl_Position = ftransform();
 
//}

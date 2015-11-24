/**
 * ray_tracer.cpp
 * CS230 Assignment 2, Winter 2012
 * -------------------------------
 * Implement ray tracer here.
 */

#define SET_RED(P, C)   (P = (((P) & 0x00ffffff) | ((C) << 24)))
#define SET_GREEN(P, C)  (P = (((P) & 0xff00ffff) | ((C) << 16)))
#define SET_BLUE(P, C) (P = (((P) & 0xffff00ff) | ((C) << 8)))

//#define NULL 0
#ifdef DEBUG
#include <iostream>
#define LOG(MESSAGE) cout << "LOGGING: " << __FILE__ << ": " << __LINE__ <<  ": "  << MESSAGE << endl;
#define STOP()  cin.get();
#else
#define LOG(MESSAGE)
#define STOP() 
#endif

#include "ray_tracer.h"

using namespace std;

const double Object::small_t=1e-6;
//--------------------------------------------------------------------------------
// utility functions
//--------------------------------------------------------------------------------
double sqr(const double x)
{
    return x*x;
}

Pixel Pixel_Color(const Vector_3D<double>& color)
{
    Pixel pixel=0;
    SET_RED(pixel,(unsigned char)(min(color.x,1.0)*255));
    SET_GREEN(pixel,(unsigned char)(min(color.y,1.0)*255));
    SET_BLUE(pixel,(unsigned char)(min(color.z,1.0)*255));
    return pixel;
}
//----------------------------------------------------------------------
// Color models
//----------------------------------------------------------------------
// Lambertian model
//Ld = kd I max(0, n · l)
Vector_3D<double> Lambertian_shading(const Ray& ray,
									 const Light* light,
									 const Vector_3D<double>& color_diffuse,
									 const Vector_3D<double>& intersection_point,
									 const Vector_3D<double>& same_side_normal){
	
	//LOG("calculating lambertian color")
	
	//LOG(color_diffuse)
	//LOG(intersection_point)
	//LOG(same_side_normal)
		Vector_3D<double> color;
		//Vector_3D<double> gazeRay = ray.direction;
		Vector_3D<double> lightPosition = light->position;
		//LOG(lightPosition)
		Vector_3D<double> lightRay = (intersection_point - lightPosition)*-1;
		lightRay.Normalize();
		//LOG(lightRay)
		double normaldotLight = Vector_3D<double>::Dot_Product(lightRay, same_side_normal);
		double maximum = (0 >= normaldotLight) ? 0 : normaldotLight;
    	Vector_3D<double> emittedLight = light->Emitted_Light(ray);
    	//LOG(maximum)
    	//LOG(emittedLight)
		color.x += emittedLight.x*color_diffuse.x*maximum;
		color.y += emittedLight.y*color_diffuse.y*maximum;
		color.z += emittedLight.z*color_diffuse.z*maximum;
		
		//color.x += 1*color_diffuse.x*maximum;
		//color.y += 1*color_diffuse.y*maximum;
		//color.z += 1*color_diffuse.z*maximum;
		//LOG(color)
		
		return color;
	}
	
// specular model
//Ls = ks I max(0, n · h) p ,
Vector_3D<double> Specular_shading(const Ray& ray,
									 const Light* light,
									 const Vector_3D<double>& color_specular,
									 const Vector_3D<double>& intersection_point,
									 const Vector_3D<double>& same_side_normal, 
									 const double specular_power){
	
	//LOG("calculating specular color")
	
	//LOG(color_specular)
	//LOG(intersection_point)
	//LOG(same_side_normal)
	Vector_3D<double> color;
	Vector_3D<double> gazeRay = ray.direction*-1;
	Vector_3D<double> lightPosition = light->position;
	Vector_3D<double> lightRay = lightPosition-intersection_point;
	lightRay.Normalize();
	
	
	
	//LOG(lightRay)
	Vector_3D<double> emittedLight = light->Emitted_Light(ray);
	//LOG(maximum)
	//LOG(emittedLight)
	Vector_3D<double> half = gazeRay + lightRay;
	
	half.Normalize();
	
	
	double NdotHalf = Vector_3D<double>::Dot_Product(same_side_normal,half);
	//NdotHalf  = pow(NdotHalf, specular_power);
	double maximum = (0 < NdotHalf) ? NdotHalf : 0;
	maximum = pow(maximum, specular_power);
	//LOG("max: " << maximum)
	color.x += emittedLight.x*color_specular.x*maximum;
	color.y += emittedLight.y*color_specular.y*maximum;
	color.z += emittedLight.z*color_specular.z*maximum;
	//LOG(color)
	return color;
}
//--------------------------------------------------------------------------------
// Shader
//--------------------------------------------------------------------------------
Vector_3D<double> Phong_Shader::
Shade_Surface(const Ray& ray,const Object& intersection_object,const Vector_3D<double>& intersection_point,const Vector_3D<double>& same_side_normal) const
{
    Vector_3D<double> color, color_am;
    

	color_am = this->color_ambient;//*emitted;
	// determine the coloring from each light
	// it is the sum of all light sources
    
	for(unsigned int i = 0; i < this->world.lights.size(); i++){
		//// determine the color
		Light *light = this->world.lights[i];
		color_am = color_am*light->Emitted_Light(ray);
		// If the point is in the light we want to add the shading models
		// if they are not we add the backround color
		
		Vector_3D<double> lightPosition = light->position;
		
		// LOG("light" << lightPosition)
		// LOG("p" << intersection_point)
		 
		 
	    Vector_3D<double> lightDirection = (lightPosition-intersection_point);
	    Vector_3D<double> lightSurface = intersection_point + lightDirection*Object::small_t;
		Ray lightRay(lightSurface, lightDirection);
	
	  
		
		//LOG("l" << lightDirection)
		//LOG("p+l'" << intersection_point + lightDirection)
		//LOG("lr.point" << lightRay.Point(1))

		bool inShadow = false;
		for(int i = 0; i < world.objects.size(); i++)
			if(world.objects[i]->Intersection(lightRay))
				inShadow = true;

		//const Object *closest = world.Closest_Intersection(lightRay);
		//LOG(closest)
		
		if(!inShadow){
			
			//LOG("The instersections light source was not blocked")
			Vector_3D<double> color_lambert = Lambertian_shading(ray, light, this->color_diffuse, intersection_point, same_side_normal);
			color += color_lambert;
		
			Vector_3D<double> color_specular = Specular_shading(ray, light, this->color_specular, intersection_point, same_side_normal, this->specular_power);		
			color += color_specular;			
		}//else{
		//	LOG("Intersection light source was blocked")
		//	}
				
	}
	// add blinn-phong and ambient
	color = color + color_am*0.5;
    
				
	
    return color;
}

Vector_3D<double> Reflective_Shader::
Shade_Surface(const Ray& ray,const Object& intersection_object,const Vector_3D<double>& intersection_point,const Vector_3D<double>& same_side_normal) const
{
    Vector_3D<double> color;

    // TODO: determine the color

    return color;
}

Vector_3D<double> Flat_Shader::
Shade_Surface(const Ray& ray,const Object& intersection_object,const Vector_3D<double>& intersection_point,const Vector_3D<double>& same_side_normal) const
{

    return color;
}

//--------------------------------------------------------------------------------
// Objects
//--------------------------------------------------------------------------------
// determine if the ray intersects with the sphere
// if there is an intersection, set t_max, current_object, and semi_infinite as appropriate and return true
bool Sphere::
Intersection(Ray& ray) const
{
	//LOG("Finding a sphere intersection")
	// now we can have a vector in the form camera+(t*ray)
	// we need to find the intersection if any such that t > 0 
	// and we have a point on our sphere if the following equation is 
	// satisfied, where d is our ray through the pixel, e is the camera
	// and c is the center of the sphere
	// (d · d)t 2 + 2*d·(e − c)t + (e − c)·(e − c) − R^2 = 0.
	Vector_3D<double> cameraPos = ray.endpoint;
	Vector_3D<double> spherePosition = this->center;
	Vector_3D<double> direction = ray.direction;
	Vector_3D<double> cameraSphere = cameraPos - spherePosition; // (e-c)
	
	
	//LOG("pos" << cameraPos )
	//LOG( "dir" << direction )
	//LOG( "sp_pos" << spherePosition )		
	
	//LOG( "cam sp" << cameraSphere )
	
	
	double raydotcamSphere = Vector_3D<double>::Dot_Product(direction*-1,cameraSphere); //(d·(e − c))
	double raydotray = Vector_3D<double>::Dot_Product(direction,direction); 				//(d·d)
	// than t = −d · (e − c) ± sqrt((d · (e − c))^2 − (d · d) ((e − c) · (e − c) − R^2 ))
	//	 divided by (d · d)
	// so 
	double discriminant = (sqr(raydotcamSphere) - raydotray*(Vector_3D<double>::Dot_Product(cameraSphere,cameraSphere) - sqr(this->radius)));
	
	//LOG("dis" << discriminant)
	//// if the discriminant is greater than or equal to zero we know that 
	//// we have a point on the sphere
	double closestHitpoint  = 0;
	if(discriminant >= 0){
		
		
		//LOG("We found a point on the sphere! Oh yeah")
		double hitPoints[2];
		// lets find the point of intersection
		//  t = −d · (e − c) ± sqrt((d · (e − c))^2 − (d · d) ((e − c) · (e − c) − R 2 ))
		//	 divided by (d · d)
		if(discriminant == 0){
			// we have one point of intersction
			hitPoints[0] = (raydotcamSphere)/raydotray;
			closestHitpoint = hitPoints[0];
		}else if(discriminant > 0){
			// we have two points of intersection
			// we want the closest one
			hitPoints[0] = (raydotcamSphere + sqrt(discriminant))/raydotray;
			hitPoints[1] = (raydotcamSphere - sqrt(discriminant))/raydotray;	
			if((hitPoints[0] > 0) && (hitPoints[1] > 0))
				closestHitpoint = (hitPoints[0] <  hitPoints[1]) ? hitPoints[0]:hitPoints[1];					
			if((hitPoints[0] > 0) && (hitPoints[1] < 0))
				closestHitpoint = hitPoints[0];	
			if((hitPoints[0] < 0) && (hitPoints[1] > 0))
				closestHitpoint = hitPoints[1];	
			// Now that we have the point of intersection
			
		}
		//LOG(raydotcamSphere)
		//LOG(discriminant)
		//LOG(raydotray)
		//LOG(closestHitpoint)
		if(closestHitpoint > small_t){
			ray.t_max = closestHitpoint; // Set our T value
			ray.semi_infinite = false; 	 // Indicate theat we stop at T
			ray.current_object = this;   // And that the object hit was this sphere
			// Check to see if theis really is a valid point for debug
			//LOG(ray.Point(ray.t_max))
			//Vector_3D<double> normal = this->Normal(ray.Point(ray.t_max));
			//double test = sqr(normal.x) + sqr(normal.y) + sqr(normal.z) - 1;
			//bool on_point = (test < small_t);
			//LOG(on_point)
			
			//LOG()
			//STOP()
			return true;
		}
		
	
	}
	//STOP()
    return false;
}

Vector_3D<double> Sphere::
Normal(const Vector_3D<double>& location) const
{
    Vector_3D<double> normal;
	// Now we need to compute the normal to this point
	// As discussed in Section 2.5.4, the normal vector at point p is given by the
	// gradient n = 2(p − c). The unit normal is (p − c)/R.
	// where P is the point of intersection and C is the center 
	// of the sphere
    normal =  (location - center)*(1/radius);
    //normal.Normalize();
    return normal;
}


// determine if the ray intersects with the sphere
// if there is an intersection, set t_max, current_object, and semi_infinite as appropriate and return true
bool Plane::
Intersection(Ray& ray) const
{
    // TODO
    // p0 is out plane definition point
    // l0 is the beginning of the ray
    // n is the normal to the plane
    // l is the direction of the ray
    //t=(p0−l0)⋅n/l⋅n
    
    double ldotn = -Vector_3D<double>::Dot_Product(ray.direction,this->normal);
    //LOG(ldotn)
    
    
    if(ldotn > Object::small_t ){
		//LOG("We have a potential plane hit")
		//LOG(x1)
		//LOG(ray.endpoint)	
		Vector_3D<double> planeRayPoint = x1 - ray.endpoint ;
		double numer =  Vector_3D<double>::Dot_Product(this->normal, planeRayPoint);
		//LOG("Ray plane dot product:" << numer)
		double hitPoint = -numer/ldotn;
		//LOG(hitPoint)
		if(hitPoint > small_t){
			//LOG("We have a planar intersection\n");
			ray.t_max = hitPoint; // Set our T value
			ray.semi_infinite = false; 	 // Indicate theat we stop at T
			ray.current_object = this;   // And that the object hit was this plane
			
			return true;
		}	
	} 
    return false;
}

Vector_3D<double> Plane::
Normal(const Vector_3D<double>& location) const
{
    return normal;
}
//--------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------
// Find the world position of the input pixel
Vector_3D<double> Camera::
World_Position(const Vector_2D<int>& pixel_index)
{
	// TODO 
	double WIDTH = this->film.pixel_grid.m;	// The width and height of our image plane
	double HEIGHT = this->film.pixel_grid.n;
	double left = -WIDTH/2;
	double right =-left;
	double top = HEIGHT/2;
	double bottom = -top;
	
	//Vector_3D<double> pixelgridCenter = this->film.focal_point;
	double u = left   + ((right-left)*(pixel_index.x + 0.5))/WIDTH;
	double v = bottom + ((top-bottom)*(pixel_index.y + 0.5))/HEIGHT;
	
	
	// A pixel on the image plane (ip) is therefore 
	// ip = e + d + uU + vV
	// where e is the camera origin
	// d is the focal length
	// U is the up vector on the image plane
	// V is the right vecotor on the image plane
	Vector_3D<double> imagePlanePosition = 
									this->focal_point + 
									this->vertical_vector*this->film.pixel_grid.dy*v +
									this->horizontal_vector*this->film.pixel_grid.dx*u;
									
	// We know have a position on our image plane defined from our camera
    Vector_3D<double> result = imagePlanePosition;	
    //LOG("u :" << u)
	//LOG("v :" << v)
    //LOG("position    :" << this->position)// camera position 
	//LOG("focal point :" << this->focal_point)// where the image plane is located
	//LOG("look vector :" << this->look_vector)// points from the position to the focal point - normalized
	//LOG("vertical    :" << this->vertical_vector) // point up in the image plane - normalized
	//LOG("horizontal  :" << this->horizontal_vector)// points to the right on the image plane - normalized
	//LOG("vertical*v  :" << this->vertical_vector*v)
	//LOG("horizontal*u:" << this->horizontal_vector*u)
	//LOG(result)
   // STOP();
    
    return result;
}
//--------------------------------------------------------------------------------
// Render_World
//--------------------------------------------------------------------------------
// Find the closest object of intersection and return a pointer to it
//   if the ray intersects with an object, then ray.t_max, ray.current_object, and ray.semi_infinite will be set appropriately
//   if there is no intersection do not modify the ray and return 0
const Object* Render_World::
Closest_Intersection(Ray& ray)
{

    // Iterate over all the scene object to see if a hit 
   const Object* closest_intersection = 0;
    //int intersectionIndex = -1;
    Ray tempRay = ray;
    for(unsigned int i = 0; i < this->objects.size(); i++){
		//LOG("testing for an intersection")
		if(this->objects[i]->Intersection(tempRay)){						
			//LOG("An intersection has been found")
			// Check our dummy variable to see if
			// we have a closer intersection than we currently have			
			if(tempRay.t_max < ray.t_max){
				//LOG("A new closer intersection has been found")
					// set the closest object
					ray.current_object = tempRay.current_object;
					// set the t value for that object hit
					ray.t_max = tempRay.t_max  ;
					closest_intersection = ray.current_object;
					//LOG(ray.endpoint)
					//LOG(ray.direction)
					//LOG("Intersection point: " << ray.endpoint + ray.direction*ray.t_max)
			}			
		} // we should now have our ray set to the closest object
		  // with the appropriate t value for hitting that object				
	}
    // If an intersection has taken place the pointer will != 0
    return closest_intersection;
}

// set up the initial view ray and call 
void Render_World::
Render_Pixel(const Vector_2D<int>& pixel_index)
{
    // TODO
    Ray ray; // TODO: set up the initial view ray here

	ray.endpoint = this->camera.position;// All rays start at the camera
	// find the direction
	ray.direction = this->camera.position - this->camera.World_Position(pixel_index);	
	ray.direction = ray.direction*-1;
	ray.direction.Normalize();// = ray.direction*(1/sqrt(Vector_3D<double>::Dot_Product(ray.direction, ray.direction)));
	ray.t_max = FLT_MAX;
	
    Ray dummy_root;
    Vector_3D<double> color=Cast_Ray(ray,dummy_root);
    camera.film.Set_Pixel(pixel_index,Pixel_Color(color));
}

// cast ray and return the color of the closest intersected surface point, 
// or the background color if there is no object intersection
Vector_3D<double> Render_World::
Cast_Ray(Ray& ray,const Ray& parent_ray)
{
	static int recursionDepth = 0;
	
    // TODO
    Vector_3D<double> color;
	// find the closest object in the seen for this view ray
	const Object *closest = Closest_Intersection(ray);
	// if an object has been hit, it is the closest
	//LOG("Testing the object, then determine color if exists")
	if(closest != NULL){
		//LOG("We found an intersection object")
		// By using the T value we can find the point of intersection
		// intersection = (cameraPos + closestHitpoint*ray);
		Vector_3D<double> intersection_point = ray.endpoint + ray.direction*ray.t_max;
		// Once we know the point of intersection we can find the normal to this point
		Vector_3D<double> same_side_normal = closest->Normal(intersection_point);
		//same_side_normal.z = same_side_normal.z*-1;
		same_side_normal.Normalize();
		color = closest->material_shader->Shade_Surface(ray,*closest,intersection_point,same_side_normal);
		
		
		// should we do a recursive ray trace
		if(recursionDepth < recursion_depth_limit){
			recursionDepth++;
			// We would need to find the new ray to direct
			// listing our current ray as the parent
			//Cast_Ray(new ray, parent);
		}

	}
	else{
		
			return background_shader->Shade_Surface(ray,*closest,Vector_3D<double>(),Vector_3D<double>());
	}
	
    return color;
}

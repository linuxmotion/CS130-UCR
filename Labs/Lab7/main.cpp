#include <GL/gl.h>
#include <SDL/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


// Classes
class vector3
{
public:
	vector3(){}
	vector3(double x, double y) : x(x), y(y), z(0) {}
	vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	vector3 operator+(vector3 v){ return vector3(x+v.x, y+v.y, z+v.z); }
	vector3 operator-(vector3 v){ return vector3(x-v.x, y-v.y, z-v.z); }
	vector3 operator*(double s){ return vector3(s*x, s*y, s*z); }
	bool operator!=(vector3 v){ return (x!=v.x or y!=v.y or z!=v.z); }	
	bool operator==(vector3 v){ return (x==v.x and y==v.y and z==v.z); }
public:
	double x,y,z;
};
vector3 operator-(vector3 v) {return vector3(0,0,0) - v;}
vector3 operator/(vector3 v, double s) {return v*(1.0/s);}
vector3 operator*(double s, vector3 v) {return v*s;}
double dot(vector3 a, vector3 b) { return (a.x*b.x + a.y*b.y + a.z*b.z); }
vector3 normal(vector3 v) { return (1.0/sqrt(dot(v,v)))*v; }
template<class T> T square(T a) { return a*a; }



//Some defines
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;

const int XMIN = -SCREEN_WIDTH/2;
const int XMAX = SCREEN_WIDTH/2;
const int YMIN = -SCREEN_HEIGHT/2;
const int YMAX = SCREEN_HEIGHT/2;



//Plots a point
void plot(int x, int y, float r, float g, float b)
{
	glBegin(GL_POINTS);
		glColor3f(r,g,b);
		glVertex2i(x,y);
		//cout << x << " " << y << endl;
	glEnd();
}



//Main Function
int main(int nArgs, char** args)
{
	//Initialize the window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //8 bits for red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); //8 bits for green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); //8 bits for blue
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //enable page flipping

	SDL_SetVideoMode(
						SCREEN_WIDTH, SCREEN_HEIGHT, 32,
						SDL_OPENGL
					);

	SDL_WM_SetCaption("CS130 Lab", NULL);

	//Set up the projection
	glMatrixMode(GL_PROJECTION);
	glOrtho(XMIN, XMAX, YMIN, YMAX, 0, 1);
	glMatrixMode(GL_MODELVIEW);



	//Main loop

	vector3 spherePosition(0,0,30);
	double sphereRadius=10;
	vector3 lightPosition(-20,-20,10);
	vector3 pixelgridCenter(0,0,5);
	vector3 cameraPos(0,0,0);
	vector3 ambientColor(.2f,.2f,.2f);
	vector3 diffuseColor(.5f,.5f,.5f);
	vector3 specularColor(.5f,.5f,.5f);
	vector3 x_incr(.025,0,0);
	vector3 y_incr(0,.025,0);
	vector3 viewUp(0,1,0);// Y axis is defined to be "up"
	while(true)
	{
		//Update platform specific stuff
		SDL_Event event;
		SDL_PollEvent(&event); //Get events
		if(event.type == SDL_QUIT) //if you click the [X] exit out
			break;


		vector3 gaze; 

		//Graphics
		glClear(GL_COLOR_BUFFER_BIT); //Clear the screen

		for(int x = XMIN; x <= XMAX; ++x)
		for(int y = YMIN; y <= YMAX; ++y)
		{
			// d = -dW+uU+vV
			
			
			
			int u = XMIN +(XMAX-XMIN)*(x + 0.5)/SCREEN_WIDTH;
			int v = YMIN +(YMAX-YMIN)*(y + 0.5)/SCREEN_HEIGHT;
			
			vector3 pixelPosition(x,y);
			pixelPosition = pixelgridCenter+x*x_incr+y*y_incr ;
			// The ray between camera and the pixel
			vector3 ray = cameraPos - pixelPosition; // our gaze direction

			// This first Vector for our camera basis
			gaze  = -normal(ray);
			
			// Do some calculation here
			// Specify our camera position
			
			// now we can have a vector in the form camera+(t*ray)
			// we need to find the intersection if any such that t > 0 
			// and we have a point on our sphere if the following equation is 
			// satisfied, where d is our ray through the pixel, e is the camera
			// and c is the center of the sphere
			// (d · d)t 2 + 2*d·(e − c)t + (e − c)·(e − c) − R^2 = 0.
			vector3 cameraSphere = cameraPos - spherePosition; // (e-c)
			double raydotcamSphere = dot(ray,cameraSphere); //(d·(e − c))
			double raydotray = dot(ray,ray); 				//(d·d)
			// than t = −d · (e − c) ± sqrt((d · (e − c))^2 − (d · d) ((e − c) · (e − c) − R 2 ))
			//	 divided by (d · d)
			// so 
			double discriminant = ((raydotcamSphere*raydotcamSphere) - raydotray*(dot(cameraSphere,cameraSphere) - sphereRadius*sphereRadius));
			//// if the discriminant is greater than or equal to zero we know that 
			//// we have a point on the sphere
			double closestHitpoint  = 0;
			if(discriminant >= 0){
				vector3 color(0,0,0); // fill this in with the appropriate colors
				//cout << "We found a point! yeah. \n";
				double hitPoints[2];
				// lets find the point of intersection
				//  t = −d · (e − c) ± sqrt((d · (e − c))^2 − (d · d) ((e − c) · (e − c) − R 2 ))
				//	 divided by (d · d)
				if(discriminant == 0){
					// we have one point of intersction
					hitPoints[0] = (-raydotcamSphere)/raydotray;
					closestHitpoint = hitPoints[0];
				}else if(discriminant > 0){
					// we have two points of intersection
					// we want the closest one
					hitPoints[0] = (-raydotcamSphere + sqrt(discriminant))/raydotray;
					hitPoints[1] = (-raydotcamSphere - sqrt(discriminant))/raydotray;					
					closestHitpoint = (hitPoints[0] <  hitPoints[1]) ? hitPoints[0]:hitPoints[1];
					// Now that we have the point of intersection
					
				}
				
				
				// Now we need to compute the normal to this point
				// As discussed in Section 2.5.4, the normal vector at point p is given by the
				// gradient n = 2(p − c). The unit normal is (p − c)/R.
				// where P is the point of intersection and C is the center 
				// of the sphere
				vector3 intersection = (cameraPos + closestHitpoint*ray);
				vector3 sphereNormal =  (intersection - spherePosition)/sphereRadius;
				vector3 lightRay = intersection - lightPosition;
				lightRay = normal(lightRay);
				
				double spheredotLight = dot(lightRay, sphereNormal);
				double maximum = (0 < spheredotLight) ? spheredotLight : 0;
				//sphereNormal = normal(sphereNormal);
				color = ambientColor;
				
				color.x += diffuseColor.x*maximum;
				color.y += diffuseColor.y*maximum;
				color.z += diffuseColor.z*maximum;
				
				vector3 half = gaze + lightRay;
				half = normal(half);
				double NdotHalf = dot(sphereNormal,half);
				NdotHalf  = pow(NdotHalf, 100);
				maximum = (0 < NdotHalf) ? NdotHalf : 0;
				color.x += specularColor.x*maximum;
				color.y += specularColor.y*maximum;
				color.z += specularColor.z*maximum;
				
				//cout << x << " " << y << endl;
				plot(x,y, color.x,color.y,color.z);
				//t = (-rayCam+sqrt(D))/rdotr;
			}
			// these might help
			// http://www.csee.umbc.edu/~olano/435f02/ray-sphere.html
			// http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
			// http://www.groovyvis.com/other/raytracing/basic.html
			
		    //plot(x,y,color.x,color.y,color.z);
			//if(t > 0)
				
		}

		SDL_GL_SwapBuffers(); //Finished drawing
	}



	//Exit out
	SDL_Quit();
	return 0;
}

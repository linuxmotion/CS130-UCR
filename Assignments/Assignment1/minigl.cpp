/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 * Do not use any additional files
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector> 
#include "minigl.h"

using namespace std;
#define PI 3.14159265

struct GLVertex{
	MGLfloat X;
	MGLfloat Y;
	MGLfloat Z;
	MGLfloat W;
	MGLpixel mPixelColor; 
} ;

class MGLMatrix{

private:
	MGLfloat mMatrix[16];
	
public:
	MGLMatrix(){
		SetIdentity();	
	}
	
	void SetIdentity(){
			
		for(int i = 0; i < 16; i++)
			mMatrix[i] = 0;
			
		mMatrix[0]=1;
		mMatrix[5]=1;
		mMatrix[10]=1;
		mMatrix[15]=1;
		
		return;
	}
	
	// we assume the matrix is a size 16 array
	void SetMatrix(const MGLfloat *newMatrix){			
		// Obliterates the current matrix with the new one
		for(int i = 0; i < 16; i++)
			mMatrix[i] = newMatrix[i];
	}
	
	void SetMatrix(MGLMatrix newMatrix){
			
		// Obliterates the current matrix with the new one
		for(int i = 0; i < 16; i++)
			mMatrix[i] = newMatrix.mMatrix[i];
	}
	
	void ScaleMatrix(MGLfloat x, MGLfloat y, MGLfloat z){
		
		//cout << "scaling by " << x << " " << y << " " << z << endl;
		MGLMatrix scalar;
		scalar.SetIdentity();
		scalar[0] = x;
		scalar[5] = y;
		scalar[10] = z;
		
		//scalar.seeMatrix();
		this->MultiplyMatrixR(scalar);
		//cout <<"new Matrix" << endl;
		//this->seeMatrix();
	}
	


	void RotateMatrix(MGLfloat angle, MGLfloat X, MGLfloat Y,MGLfloat Z ){
			
		MGLMatrix Rotate;
		
		MGLfloat normal = sqrt(X*X + Y*Y + Z*Z );
		MGLfloat x = X/normal; 
		MGLfloat y = Y/normal;
		MGLfloat z = Z/normal;
		
		MGLfloat c  = cos((angle*PI)/180);
		MGLfloat s = sin((angle*PI)/180); 
		
		Rotate[0] = ((x*x)*(1-c)) + c;
		Rotate[1] = ((y*x)*(1-c)) + z*s;
		Rotate[2] = ((x*z)*(1-c)) - y*s;
		Rotate[3] = 0;
		
		Rotate[4] = ((y*x)*(1-c)) - z*s;
		Rotate[5] = ((y*y)*(1-c)) + c;
		Rotate[6] = ((y*z)*(1-c)) + x*s;
		Rotate[7] =  0;
		
		Rotate[8]  = ((x*z)*(1-c)) + y*s;
		Rotate[9]  = ((y*z)*(1-c)) - x*s;
		Rotate[10] = ((z*z)*(1-c)) + c;
		Rotate[11] = 0;
				
		Rotate[12] = 0;
		Rotate[13] = 0;
		Rotate[14] = 0;
		Rotate[15] = 1;
        this->MultiplyMatrixR(Rotate);
	}
	
	void TranslateMatrix(MGLfloat x, MGLfloat y,MGLfloat z ){
		
			MGLMatrix translation;
			translation.SetIdentity();
			
			translation[12] = x;
			translation[13] = y;
			translation[14] = z;
			
            //translation.MultiplyMatrix(*this);
			this->MultiplyMatrixR(translation);
           // this->SetMatrix(translation);
			
		
	}
	
	// Matrix multiplication is done left to right
	// the matrix "multiply" will be applied from the left
	// i.e.  MultilplyMatrix(B) -> A = B*A
	MGLMatrix MultiplyMatrix(MGLMatrix multiple){
		
	MGLfloat temp[16];
	
		// for each row
		for(int i = 0; i < 4; i++){			
			// multiple against each column of new matrix
			for(int j = 0; j < 4; j++){
				int step = (j*4);
				temp[step+i]	= multiple[i]*mMatrix[step] + multiple[i+4]*mMatrix[step+1] + multiple[i+8]*mMatrix[step+2] + multiple[i+12]*mMatrix[step+3];
			}			
		}		
		// reassign new matrix
		for(int i = 0; i < 16; i++)
			mMatrix[i] = temp[i];	
			
		return *this;
	}
	// Matrix multiplication is done left to right
	// the matrix "multiply" will be applied from the left
	// i.e.  MultilplyMatrix(B) -> A = A*B
    MGLMatrix MultiplyMatrixR(MGLMatrix multiple){
		
	MGLfloat temp[16];
	
		// for each row
		for(int i = 0; i < 4; i++){			
			// multiple against each column of new matrix
			for(int j = 0; j < 4; j++){
				int step = (j*4);
				temp[step+i]	= multiple[step]*mMatrix[i] + multiple[step+1]*mMatrix[i+4] + multiple[step+2]*mMatrix[i+8] + multiple[step+3]*mMatrix[i+12];
			}			
		}		
		// reassign new matrix
		for(int i = 0; i < 16; i++)
			mMatrix[i] = temp[i];	
			
		return *this;
	}
	
	GLVertex MultVertex(GLVertex vertex){
		
		GLVertex newVertex;
		MGLfloat temp[4];
		// for each row
		for(int i = 0; i < 4; i++){			
			// multiple against each column of new matrix	
			// cout << mMatrix[i] << " " << mMatrix[i+4]<< " " <<  mMatrix[i+8] << " " << mMatrix[i+12] << "\n";
			temp[i]= mMatrix[i]*vertex.X + mMatrix[i+4]*vertex.Y + mMatrix[i+8]*vertex.Z + mMatrix[i+12]*vertex.W;				
		}	
		// set all calculated X,Y, Z, W
		newVertex.X = temp[0];
		newVertex.Y = temp[1];
		newVertex.Z = temp[2];
		newVertex.W = temp[3];
		// The new vertex needs the color component
		newVertex.mPixelColor = vertex.mPixelColor;
	
		return newVertex;
		
	}
	
	MGLfloat & operator[](int i){
		
		return mMatrix[i];
	}
	
	MGLMatrix operator =(MGLMatrix m){
	
		this->SetMatrix(m);
			
		return *this;
	}
	

	MGLMatrix operator *(MGLMatrix matrix){
		
			return this->MultiplyMatrix(matrix);
		
	}
	
	void seeMatrix(){
		
			for(int i =0; i < 4; i++){
					cout << mMatrix[i]  << " " << mMatrix[i+4] <<  " " << mMatrix[i+8] << " " << mMatrix[i+12]  << "\n"; 
			}
	
	}

};



// Function prototypes
void draw_line(MGLpixel color1,MGLpixel color2, int x0, int y0, int x1, int y1);
void draw_line(MGLpixel color1,MGLpixel color2, int x0, int y0, int x1, int y1, float depth1, float depth2);
void set_pixel(MGLpixel color, int x, int y);
void set_pixel(MGLpixel color, int x, int y, float depth);
void RenderTriangles(MGLMatrix viewport, bool wireframe);


//*********Global Variables**********//

int WIDTH = 0;
int HEIGHT = 0;
bool mHasBegun = false;
bool WIREFRAME = false;
bool Zbuffinit = false;

vector<GLVertex> mVertices;
MGLpixel *mFrameBuffer;
MGLfloat *mZBuffer;
MGLpixel mColor;


MGLMatrix mCurrentMatrix;
// We create out modelview stack
MGLMatrix mModelViewStack[32];
int mModelViewTracker = 0;
// We create our projection stack
MGLMatrix mProjectionStack[32];
int mProjectionTracker = 0;


int VIEW_STATE = 0;
int VIEW_POLY  = 0;


/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}


/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
	cout << "mglreadpixels\n";
	// initialize all of the variables
	WIDTH = width;
	HEIGHT = height;
	mFrameBuffer = new  MGLpixel[(WIDTH*HEIGHT)];
	mZBuffer = new  MGLfloat[(WIDTH*HEIGHT)];
	// initialize the two buffers 
	for(int i = 0; i < WIDTH*HEIGHT; i++){
		mZBuffer[i] =  INFINITY; // very far
		mFrameBuffer[i] = 0; // all black
	}
	
	
	// create our viewport matrix		
	// This defines our veiw port for every model
	// It translates from world cooridnates to pixel space	
	MGLMatrix viewport;
	viewport.SetIdentity();
    viewport.TranslateMatrix(-0.5, -0.5, -0.5);//mVertices[0].X, mVertices[0].Y, 0);
	viewport.ScaleMatrix(MGLfloat(WIDTH/2), MGLfloat(HEIGHT/2), 1);
	viewport.TranslateMatrix(1, 1, 1);
	
	
	// we pass the final tranformation matrix, the viewport.
	// The vertices are already in prepared for this transform
	RenderTriangles(viewport, WIREFRAME);
		
	for(int i = 0; i < (WIDTH*HEIGHT); i++){
		data[i] = mFrameBuffer[i];
	}
		
	
}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
	VIEW_POLY = mode;
	mHasBegun = true;
		

		
}

MGLfloat max(MGLfloat a, MGLfloat b){
	
	return (a > b)? a : b;
	
}

MGLfloat min(MGLfloat a, MGLfloat b){
	
	return (a < b)? a : b;
	
}

int orient2d(GLVertex a, GLVertex b, GLVertex c)
{
    return (b.X-a.X)*(c.Y-a.Y) - (b.Y-a.Y)*(c.X-a.X);
}

void BerrycentricRasterization(GLVertex vertex0,GLVertex vertex1,GLVertex vertex2, MGLpixel colors[3]){
	
	MGLfloat totalArea = orient2d(vertex0, vertex1, vertex2);//TriangleArea(vertex0, vertex1, vertex2);
	GLVertex point; // this is a point given in device pixels, we do not need a divisor
		point.W = 1;
	
	MGLfloat alpha = 0;
	MGLfloat beta = 0;
	MGLfloat gamma = 0;
	
	MGLfloat minY  = min(vertex0.Y, min(vertex1.Y, vertex2.Y));
	MGLfloat minX  = min(vertex0.X, min(vertex1.X, vertex2.X));
	MGLfloat maxX  = max(vertex0.X, max(vertex1.X, vertex2.X));
	MGLfloat maxY  = max(vertex0.Y, max(vertex1.Y, vertex2.Y));
	
	MGLpixel color = 000000;
	
	MGLbyte red1 = MGL_GET_RED(colors[0]);
    MGLbyte green1 = MGL_GET_GREEN(colors[0]);
    MGLbyte blue1 = MGL_GET_BLUE(colors[0]);

	MGLbyte red2 = MGL_GET_RED(colors[1]);
    MGLbyte green2 = MGL_GET_GREEN(colors[1]);
    MGLbyte blue2 = MGL_GET_BLUE(colors[1]);
 
	MGLbyte red3 = MGL_GET_RED(colors[2]);
    MGLbyte green3 = MGL_GET_GREEN(colors[2]);
    MGLbyte blue3 = MGL_GET_BLUE(colors[2]);

	for(int i = minX; i <= maxX; i++){ // loop from [min x, max x]
				point.X = i;
				for(int j = minY; j < maxY; j++){ // loop from [min y, max y]
									
					point.Y = j;
										
					alpha = orient2d(vertex1, vertex2, point);				
					beta = orient2d(vertex2, vertex0, point);
					gamma = orient2d(vertex0, vertex1, point);
					
				
					if( (alpha >= 0) && (beta  >= 0 ) && (gamma >= 0)){
							
							// Normalize our areas
							alpha /= totalArea;				
							beta /= totalArea;
							gamma /= totalArea;
							
							MGLbyte r = MGLbyte((alpha*red1)+(beta*red2)+ (gamma*red3));
							MGLbyte g = MGLbyte((alpha*green1)+(beta*green2)+ (gamma*green3));
							MGLbyte b = MGLbyte((alpha*blue1)+(beta*blue2) + (gamma*blue3));
							
							float depth = alpha*vertex0.Z + beta*vertex1.Z + gamma*vertex2.Z;

							MGL_SET_BLUE(color, b);
							MGL_SET_RED(color, r);
							MGL_SET_GREEN(color, g);
							//cout << "setting pixel " << i << " " << j << " to color r = " << MGL_GET_RED(color) << " g = " << MGL_GET_GREEN(color) <<
							// " blue = " <<  MGL_GET_BLUE(color) <<  " \n";
							set_pixel(color, i, j, depth);
					}
				}
					
			}
	
	return;
	
}
/**
 * Stop specifying the vertices for a group of primitives.
 */ 
 void RenderTriangles(MGLMatrix viewport, bool wireframe){
	 
		
		// This is where we actually do the drawing
		// but we need to go from vertices to viewport*vertices irst
		//cout << "viewport matrix\n";
		//viewport.seeMatrix();
		
		int size = mVertices.size();
		
		// Are vertices only need the viewport matrix applied
		GLVertex vertex0 = viewport.MultVertex(mVertices[0]);
		GLVertex vertex1 = viewport.MultVertex(mVertices[1]);
		GLVertex vertex2 = viewport.MultVertex(mVertices[2]);
			
		MGLpixel colors[3];
		colors[0] = mVertices[0].mPixelColor;
		colors[1] = mVertices[1].mPixelColor;
		colors[2] = mVertices[2].mPixelColor;
		//cout <<"colors :" << colors[0] << " " << colors[1] << "  "  << colors[2] << endl;
		if(wireframe){
			
				// Draw the lines, wireframe triangle for now
			for(int i = 0; i < (size)-1; i = (i+3)){
				
				//cout << "mVertices[" << i   <<"] = ( " << mVertices[i].X   << "," << mVertices[i].Y   << "," << mVertices[i].Z   << ") -> (" << vertex0.X << ","  << vertex0.Y << ","  << vertex0.Z << ")\n";
				//cout << "mVertices[" << i+1 <<"] = ( " << mVertices[i+1].X << "," << mVertices[i+1].Y << "," << mVertices[i+1].Z << ") -> (" << vertex1.X << ","  << vertex1.Y << ","  << vertex1.Z << ")\n";
				//cout << "mVertices[" << i+2 <<"] = ( " << mVertices[i+2].X << "," << mVertices[i+2].Y << "," << mVertices[i+2].Z << ") -> (" << vertex2.X << ","  << vertex2.Y << ","  << vertex2.Z << ")\n";
					
				draw_line(mVertices[i].mPixelColor, mVertices[i+1].mPixelColor,vertex0.X/vertex0.W, vertex0.Y/vertex0.W, vertex1.X/vertex1.W, vertex1.Y/vertex1.W,vertex0.Z, vertex1.Z);	
				draw_line(mVertices[i+1].mPixelColor, mVertices[i+2].mPixelColor,vertex1.X/vertex1.W, vertex1.Y/vertex1.W, vertex2.X/vertex2.W, vertex2.Y/vertex2.W,vertex1.Z,vertex2.Z);	
				draw_line(mVertices[i+2].mPixelColor, mVertices[i].mPixelColor,vertex2.X/vertex2.W, vertex2.Y/vertex2.W, vertex0.X/vertex0.W, vertex0.Y/vertex0.W,vertex2.Z,vertex0.Z);	
					
				vertex0 = viewport.MultVertex(mVertices[i+3]);
				vertex1 = viewport.MultVertex(mVertices[i+4]);
				vertex2 = viewport.MultVertex(mVertices[i+5]);
			}
			
		
		}
		else{
			for(int i = 0; i < (size)-1; i = (i+3)){ // for every three vertices
				

				
				// Do W division
				vertex0.X =  vertex0.X/vertex0.W;
				vertex0.Y =  vertex0.Y/vertex0.W;
				vertex1.X =  vertex1.X/vertex1.W;
				vertex1.Y =  vertex1.Y/vertex1.W;
				vertex2.X =  vertex2.X/vertex2.W;
				vertex2.Y =  vertex2.Y/vertex2.W;

				colors[0] = mVertices[i].mPixelColor;
				colors[1] = mVertices[i+1].mPixelColor;
				colors[2] = mVertices[i+2].mPixelColor;
				
				//cout << "mVertices[" << i   <<"] = ( " << mVertices[i].X   << "," << mVertices[i].Y   << "," << mVertices[i].Z   << ") -> (" << vertex0.X << ","  << vertex0.Y << ","  << vertex0.Z << ")\n";
				//cout << "mVertices[" << i+1 <<"] = ( " << mVertices[i+1].X << "," << mVertices[i+1].Y << "," << mVertices[i+1].Z << ") -> (" << vertex1.X << ","  << vertex1.Y << ","  << vertex1.Z << ")\n";
				//cout << "mVertices[" << i+2 <<"] = ( " << mVertices[i+2].X << "," << mVertices[i+2].Y << "," << mVertices[i+2].Z << ") -> (" << vertex2.X << ","  << vertex2.Y << ","  << vertex2.Z << ")\n";
				
				//Rasterize triangle
				BerrycentricRasterization(vertex0, vertex1, vertex2, colors);
				vertex0 = viewport.MultVertex(mVertices[i+3]);
				vertex1 = viewport.MultVertex(mVertices[i+4]);
				vertex2 = viewport.MultVertex(mVertices[i+5]);
				
			}
			
			
		}
		
	
		// We've dealt with all of the vertices in the current stack
		//for(int i = 0; i < size; i++){
				//mVertices.pop_back();
			
		//}
		
	 
}
	 
 void RenderQuads(){
 

	//cout << "=========RenderQuads============\n";
	
	//int size = 
		// reverticize the vertex list
		int size = mVertices.size();
		vector<GLVertex> newVertices;
		newVertices.push_back(mVertices[size-4]); //quad 1	
		newVertices.push_back(mVertices[size-3]); //quad 2
		newVertices.push_back(mVertices[size-2]); //quad 3
	
		newVertices.push_back(mVertices[size-2]); //quad 3
		newVertices.push_back(mVertices[size-1]); //quad 4
		newVertices.push_back(mVertices[size-4]); //quad 1
		
		// pop the last four vertices, this was the quad
		mVertices.pop_back();
		mVertices.pop_back();
		mVertices.pop_back();
		mVertices.pop_back();
	
		// push the next 6, this is two triangles
		mVertices.push_back(newVertices[0]);
		mVertices.push_back(newVertices[1]);
		mVertices.push_back(newVertices[2]);
		mVertices.push_back(newVertices[3]);
		mVertices.push_back(newVertices[4]);
		mVertices.push_back(newVertices[5]);
		
	
		return;
}
void mglEnd()
{
	
	if(!mHasBegun){
			
			MGL_ERROR("Must call mglBegin before mglEnd!");
			return;
	}
	
	//cout << "========Screen matrix\n";
	//screenMatrix.seeMatrix();
	
	if(VIEW_POLY == MGL_TRIANGLES){
		
		int size = mVertices.size();
		int chop = size % 3;
		//cout << "Chopped " << chop << " vertices\n";
		for(int i = 0; i < chop; ++i){
			mVertices.pop_back();
		}
	
			
			
		return;
	}
	else if(VIEW_POLY == MGL_QUADS){
		
			RenderQuads();
		return;
	}
	
	mHasBegun = false;
	//if we reached this point something went qrong, throw an error
	MGL_ERROR("A valid polygon type was not chosen");
	
	

}

void set_pixel(MGLpixel color, int x, int y){
	set_pixel(color, x, y, 0);	
}
void set_pixel(MGLpixel color, int x, int y, float depth)
{

    // Clip all pixels that are not in the view port
    if((x < 0) || (x > WIDTH) || (y < 0) || (y > HEIGHT)){ // not a vaild pixel, clip it, may need for rasterizer
		//cout << "Clipping pixel outside of viewport\n";
		return ;
	}
	// the new pixel is deeper than the old pixel
    // do not replace	
	if(depth < mZBuffer[(WIDTH*y) + x]){
		//cout << "Pixel " << (WIDTH*int(y)) + int(x) << " is set in the framebuffer to color " << color << "\n"; 
		mZBuffer[(WIDTH*y) + x] = depth;
		mFrameBuffer[(WIDTH*y) + x] = color;
		return;	
	}
}


float distancei(int x0, int y0, int x1, int y1){
	return sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
}
	
	
void draw_line(MGLpixel pixelColor1,MGLpixel pixelColor2, int x0, int y0, int x1, int y1){
	draw_line(pixelColor1,pixelColor2, x0, y0, x1, y1, 0, 0);
}
void draw_line(MGLpixel pixelColor1,MGLpixel pixelColor2, int x0, int y0, int x1, int y1, float depth1, float depth2)

{
	//cout << "X0 = " << x0 << " Y0 = " << y0 << "\n";
	//cout << "X1 = " << x1 << " Y0 = " << y1 << "\n";
	//cout << "Interpolating color " << pixelColor1<< " and " << pixelColor2 << endl;

    float dx = x1 - x0;
    float dy = y1 - y0;
    
    float alpha = 0;
    float beta  = 0;
    
    MGLbyte red1 = MGL_GET_RED(pixelColor1);
    MGLbyte green1 = MGL_GET_GREEN(pixelColor1);
    MGLbyte blue1 = MGL_GET_BLUE(pixelColor1);
    
    MGLbyte red2 = MGL_GET_RED(pixelColor2);
    MGLbyte green2 = MGL_GET_GREEN(pixelColor2);
    MGLbyte blue2 = MGL_GET_BLUE(pixelColor2);
    
    MGLpixel color = 000000;
    float depth = 0;
    if(depth1 == depth2){
		//cout << "Not going to do depth interpolation\n";
		//cout << "depth1 = " << depth1 << " depth2 = " << depth2 << "\n";
		depth = depth1;
	}
    
    float distance = distancei(x0, y0, x1, y1);
    if(dx == 0){
       // cout << "DX = 0 \n";
;		if(y0 < y1){
			for(int y = y0; y < y1; ++y){
				alpha = (distancei(x0, y0, x1, y)/distance);
				beta = 1 - alpha;		
				
				//cout << "alpha = " << alpha << " beta = " << beta << endl;
					
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				//cout << " The calculated color = " << color << endl;
				if(depth1 == depth2)
				{
					depth = depth1;
						
				}
				else
					depth = alpha*depth2 + beta*depth1;
					
					set_pixel(color, int(x0), int(y), depth);
				}
		}else if(y0 > y1){
			
			for(int y = y0; y > y1; --y){
				
				alpha = (distancei(x0, y0, x1, y)/distance);
				beta = 1 - alpha;			
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
				{
					depth = depth1;
				}
				else
					depth = alpha*depth2 + beta*depth1;
					
				set_pixel(color, int(x0), int(y), depth);
				
				}
		}
		return;

    }
    
   

    float m = dy/dx;
    float b = y0 - m*x0;
    //cout << "m = " << m << "\n";
    if(x0 < x1){ 
		//cout << "x0 < x1 \n";
        if((-1 <= m) && (m <= 1)){
            for(int x = x0; x < x1; ++x){
                //cout << "-1 <= m <= 1 \n";
                alpha = (distancei(x0, y0, x, m*x + b)/distance);
				beta = 1 - alpha;	
				
				//cout << "alpha -> " << alpha << " beta -> " << beta << "\n";		
				
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                set_pixel(color, int(x), int(m*x + b), depth);
                }
        }else if((m >= 1) || (m <= -1)){ // y = m*x+b -> (y-b)/m = x
            //cout << "abs Slope is greater than 1\n";
            if(y0 > y1){
				
				//cout << "y0 > y1 \n";
                for(int y = y0; y > y1; --y){
                
                
					if(m <= -1){
							//cout << " m <= -1\n";
							alpha = (distancei(x0, y0, ((y-b)/m),y)/distance);
							beta = 1 - alpha;			
						
							MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
							MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
							MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
							if(depth1 == depth2)
								depth = depth1;
							else
								depth = alpha*depth2 + beta*depth1;
							set_pixel(color, int((y-b)/m), int(y), depth);
					}
						//cout << "y = " << y << " x = " << ((y-b)/m) << "\n";

                }
                
            }else{
                
                for(int y = y0; y < y1; ++y){
					if(m <= -1){
						alpha = (distancei(x0, y0, ((y-b)/m),y)/distance);
						beta = 1 - alpha;			
						MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
						MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
						MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
						if(depth1 == depth2)
							depth = depth1;
						else
							depth = alpha*depth2 + beta*depth1;
						//cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
						set_pixel(color, int((y-b)/m), int(y), depth);
						
						}
                }
                
            }
            
            
        }
    } else if(x1 < x0){
		
		//cout << "x1 < x0 \n";
        if((-1 <= m ) && (m <= 1)){
            for(int x = x0; x > x1; --x){
				
				alpha = (distancei(x0, y0, x, m*x + b)/distance);
				beta = 1 - alpha;			
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                set_pixel(color, int(x), int(m*x + b), depth);
                
                }
        }else if((m >= 1) || (m <= -1)){ 
            //cout << "where am i 1\n";
            if(y0 < y1){
                for(int y = y0; y < y1; ++y){
				
				alpha = (distancei(x0, y0, ((y-b)/m), y)/distance);
				beta = 1 - alpha;			
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                //cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
                //cout << "where am i " << y << "|" << y1 << "\n";
					set_pixel(color, int((y-b)/m), int(y), depth);
             }
                
            }else{
                // y = m*x+b -> (y-b)/m = x
                for(int y = y0; y > y1; --y){
					alpha = (distancei(x0, y0, ((y-b)/m), y)/distance);
				beta = 1 - alpha;			
				MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
					set_pixel(color, int((y-b)/m), int(y), depth);
					
					}
           }   
        }
    }
    
    //cout << "alpha -> " << alpha << " beta -> " << beta << "\n";
    return;
}


/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y)              
{
	if(!mHasBegun)MGL_ERROR("mglBegin must be called first");
	mglVertex3(x,y,0);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
	
	if(!mHasBegun)MGL_ERROR("mglBegin must be called first");
	
	switch(VIEW_STATE){
		case MGL_MODELVIEW:{	
			mModelViewStack[mModelViewTracker] = mCurrentMatrix;			
			break;
		}
		case MGL_PROJECTION:{
			 mProjectionStack[mProjectionTracker] = mCurrentMatrix;
			break;
		}
	}
	GLVertex vertex; // create the gl coordinate vertex
	vertex.X = x;
	vertex.Y = y;
	vertex.Z = z;
	vertex.W = 1;
	vertex.mPixelColor = mColor;
	//cout << vertex.X << " " << vertex.Y  << " " <<  vertex.Z << endl;
	// we need to transform the vertecies without the final viewport
	MGLMatrix model = mModelViewStack[mModelViewTracker];
	MGLMatrix proj = mProjectionStack[mProjectionTracker];
	// Check our matrices for an easy one
	//cout << "=======Projection matrix\n";
	//mProjectionStack[mProjectionTracker].seeMatrix();
	//cout << "=======Modelveiw matrix\n";
	//mModelViewStack[mModelViewTracker].seeMatrix();
	// next we need to take the vertex cooridinates into screen space
	// To do this we apply the modelview and projection matrices
	//model.MultiplyMatrix();		
	MGLMatrix screenMatrix = model.MultiplyMatrix(proj);


	// transform by multiply the vertex and the matrix
	mVertices.push_back(screenMatrix.MultVertex(vertex));
		
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
	
	switch(mode){
		case MGL_MODELVIEW:{
			if(VIEW_STATE != mode)
				mProjectionStack[mProjectionTracker] = mCurrentMatrix ;
			mCurrentMatrix = mModelViewStack[mModelViewTracker];				
			break;
		}
		case MGL_PROJECTION:{
			
			// save the topmost current matrix for the prevois stack type
			if(VIEW_STATE != mode)
				mModelViewStack[mModelViewTracker] = mCurrentMatrix ;	
				
			mCurrentMatrix = mProjectionStack[mProjectionTracker];
			break;
		}
	}
	
	VIEW_STATE = mode;
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
		
	switch(VIEW_STATE){
		case MGL_MODELVIEW:{
			//cout << "Pushing a  modelview matrix\n";
			
			//mCurrentMatrix.seeMatrix();
			mModelViewStack[mModelViewTracker++] = mCurrentMatrix;	
			mCurrentMatrix = mModelViewStack[mModelViewTracker-1] ;			
			break;
		}
		case MGL_PROJECTION:{
			//cout << "Pushing a projection matrix\n";
			//mCurrentMatrix.seeMatrix();
			 mProjectionStack[mProjectionTracker++] = mCurrentMatrix;
			 mCurrentMatrix = mProjectionStack[mProjectionTracker++];
			break;
		}
	}
	//cout << "=========new matrix=======\n";
	//mCurrentMatrix.seeMatrix();
	//cout << "End pushing a matrix\n";
	

}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
	
	//cout << "Popping a matrix\n";
	//mCurrentMatrix.seeMatrix();
	
	switch(VIEW_STATE){
		case MGL_MODELVIEW:{
			if(mModelViewTracker == 0)
				MGL_ERROR("Cannot pop the last matrix on the MODELVIEW stack");
			
			mCurrentMatrix = mModelViewStack[--mModelViewTracker];				
			break;
		}
		case MGL_PROJECTION:{
			if(mProjectionTracker == 0)
				MGL_ERROR("Cannot pop the last matrix on the PROJECTION stack");
			
			mCurrentMatrix = mProjectionStack[--mProjectionTracker] ;
			break;
		}
	}
	//cout << "=============\n";

	//mCurrentMatrix.seeMatrix();
	//cout << "end popping sequence\n";
	}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
	
	mCurrentMatrix.SetIdentity();
}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix)
{
	mCurrentMatrix.SetMatrix(matrix);
	
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix)
{
	MGLMatrix m;
	m.SetMatrix(matrix);
	mCurrentMatrix.MultiplyMatrix(m);
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
	mCurrentMatrix.TranslateMatrix(x,y,z);
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
	mCurrentMatrix.RotateMatrix(angle,x,y,z);
	return ;
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
	mCurrentMatrix.ScaleMatrix(x,y,z);
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far)
{
	MGLMatrix perspective;
	
	perspective.SetIdentity();
	perspective[0] = (2*near)/(right - left);
	perspective[5] = (2*near)/(top - bottom);
	
	perspective[8] =  (right+left)/(right - left);
	perspective[9] = (top+bottom)/(top - bottom);
	perspective[10] = -((far+near)/(far - near));
	perspective[11] = -1;
	
	perspective[14] = (-2*far*near)/(far - near);
	perspective[15] = 0;
	
	mCurrentMatrix.MultiplyMatrixR(perspective);
	
	
	
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
 
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far)
{
	MGLMatrix orthographic;
	
	orthographic.SetIdentity();
	orthographic[0] = 2/(right - left);
	orthographic[5] = 2/(top - bottom);
	orthographic[10] = -2/(far - near);
	
	orthographic[12] =  -(right+left)/(right - left);
	orthographic[13] = -(top+bottom)/(top - bottom);
	orthographic[14] = -(far+near)/(far - near);


	mCurrentMatrix.MultiplyMatrixR(orthographic);

	
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red,
              MGLbyte green,
              MGLbyte blue)
{
	mColor = 0;
    MGL_SET_RED(mColor,red);
    MGL_SET_GREEN(mColor,green);
    MGL_SET_BLUE(mColor,blue);

	
	// cout << "color r = " << MGL_GET_RED(mColor) << " g = " << MGL_GET_GREEN(mColor) <<
	//						 " blue = " <<  MGL_GET_BLUE(mColor) <<  " \n";
							 
	
	
}

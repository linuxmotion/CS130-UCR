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

struct ColorRGB{
	MGLbyte r;
	MGLbyte g;
	MGLbyte b;
	
}mColorRGB;

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
		
		MGLMatrix scalar;
		scalar.SetIdentity();
		scalar[0] = x;
		scalar[5] = y;
		scalar[10] = z;
		
        //scalar.MultiplyMatrix(*this);
		this->MultiplyMatrixR(scalar);
       // this->SetMatrix(scalar);
		
		
		
	}
	
#define PI 3.14159265
	#include <cmath>
	void RotateMatrix(MGLfloat angle, MGLfloat X, MGLfloat Y,MGLfloat Z ){
		
		
		MGLMatrix Rotate;
		

		
		MGLfloat normal = sqrt(X*X + Y*Y + Z*Z );
		MGLfloat x = X/normal; 
		MGLfloat y = Y/normal;
		MGLfloat z = Z/normal;
		
		MGLfloat c  = cos((angle*PI)/180);
		MGLfloat s = sin((angle*PI)/180); 
		//float cosh = cos(angle);
		//float sin  = sin(angle);
		
		//mMatrix[0] = 
		
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
		cout << "Rotating matrix\n";
		Rotate.seeMatrix();
		
		//Rotate.MultiplyMatrix(*this);
		//this->SetMatrix(Rotate);
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
		//cout << "Input vertex\n";
		//cout << "X = " << vertex.X << " Y =  " << vertex.Y  << " Z =  " << vertex.Z << "\n" ;
		// for each row
		for(int i = 0; i < 4; i++){			
			// multiple against each column of new matrix	
			// cout << mMatrix[i] << " " << mMatrix[i+4]<< " " <<  mMatrix[i+8] << " " << mMatrix[i+12] << "\n";
			temp[i]= mMatrix[i]*vertex.X + mMatrix[i+4]*vertex.Y + mMatrix[i+8]*vertex.Z + mMatrix[i+12]*vertex.W;				
		}	
		newVertex.X = temp[0];
		newVertex.Y = temp[1];
		newVertex.Z = temp[2];
		newVertex.W = temp[3];
		//cout << "Output vertex\n";
		//cout << newVertex.X << " " << newVertex.Y  << " " << newVertex.Z << "\n";
		
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
	
	//MGLMatrix operator *(GLVertex &vertex){
	//	
	//		return this->MultVertex(vertex);
		
	//}
	
	
};




void draw_line(MGLpixel color1,MGLpixel color2, int x0, int y0, int x1, int y1);
void draw_line(MGLpixel color1,MGLpixel color2, int x0, int y0, int x1, int y1, float depth1, float depth2);
void set_pixel(MGLpixel color, float x, float y);
void set_pixel(MGLpixel color, float x, float y, float depth);
	 


//*********Global Variables**********//
bool mHasBegun = false;
#define WIDTH 320
#define HEIGHT 240

MGLMatrix mCurrentMatrix;

vector<GLVertex> mVertices;
MGLpixel mFrameBuffer[(WIDTH*HEIGHT)];
MGLfloat mZBuffer[(WIDTH*HEIGHT)];
MGLpixel mColor;



// We create out modelview stack
MGLMatrix mModelViewStack[32];
int mModelViewTracker = 0;
// We create our projection stack
MGLMatrix mProjectionStack[32];
int mProjectionTracker = 0;

//vector<GLMatrix*>
//vector<MGLfloat> mZbuffer;
int VIEW_STATE = 0;
int VIEW_POLY  = 0;

float wDivisor = 1;


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

	printf("mgl read pixels\n");
	MGLpixel pix = 0;
	MGL_SET_BLUE(pix, 255);
	//for(int i = 0; i < (height-1); i++)
	//	mFrameBuffer[(width*i) + i] = pix;
		
	for(int i = 0; i < (width*height); i++){
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
	
	
	for(int i = 0; i < WIDTH*HEIGHT; i++){
		mZBuffer[i] = 0;
	}
	
	
}

MGLfloat max(MGLfloat a, MGLfloat b){
	
	return (a > b)? a : b;
	
}

MGLfloat min(MGLfloat a, MGLfloat b){
	
	return (a < b)? a : b;
	
}

MGLfloat TriangleArea(GLVertex p1, GLVertex p2, GLVertex p3){
	
	
	
	return ((p1.X*p2.Y + p2.X*p3.Y + p3.X*p1.Y - p2.X*p1.Y - p3.X*p2.X - p1.X*p3.Y)/2);

}

/**
 * Stop specifying the vertices for a group of primitives.
 */ 
 void RenderTriangles(MGLMatrix screenMatrix, bool wireframe){
	 
		int size = mVertices.size();
		int chop = size % 3;
		cout << "Chopped " << chop << " vertices\n";
		for(int i = 0; i < chop; ++i){
			mVertices.pop_back();
		}
	
		//cout << "Screen matrix\n";
		//screenMatrix.seeMatrix();
		
		GLVertex vertex0 = screenMatrix.MultVertex(mVertices[0]);
		GLVertex vertex1 = screenMatrix.MultVertex(mVertices[1]);
		GLVertex vertex2 = screenMatrix.MultVertex(mVertices[2]);
		
		MGLfloat minY  = min(vertex0.Y, min(vertex1.Y, vertex2.Y));
		MGLfloat minX  = min(vertex0.X, min(vertex1.X, vertex2.X));
		MGLfloat maxX  = max(vertex0.X, max(vertex1.X, vertex2.X));
		MGLfloat maxY  = max(vertex0.Y, max(vertex1.Y, vertex2.Y));
		//cout << minY << "|" << minX << "|" << maxY << "|"<< maxX << "\n";
		
		MGLfloat alpha = 0;
		MGLfloat beta = 0;
		MGLfloat gamma = 0;
		
		MGLfloat totalArea = TriangleArea(vertex0, vertex1, vertex2);
		GLVertex point;
		
		if(!wireframe){
		//Rasterize triangles
		for(int z = 0; z < (size)-1; z = (z+3)){ // for every three vertices
		
			for(int i = minX; i <= maxX; i++){ // loop from [min x, max x]
				point.X = i;
				for(int j = minY; j < maxY; j++){ // loop from [min y, max y]
					
					
					//point.Y = j;
					/*
					 * alpha = (TriangleArea(point, vertex1,vertex2)/totalArea);
					beta = (TriangleArea(point, vertex2,vertex0)/totalArea);
					*/
					alpha = ((vertex1.Y - vertex2.Y)*i + (vertex1.X - vertex2.X)*j + (vertex2.X*vertex1.Y) + (vertex1.X*vertex2.Y))/
						((vertex1.Y - vertex2.Y)*vertex0.X + (vertex1.X - vertex2.X)*vertex0.Y + (vertex2.X*vertex1.Y) + (vertex1.X*vertex2.Y));
					beta = ((vertex2.Y - vertex0.Y)*i + (vertex2.X - vertex0.X)*j + (vertex2.X*vertex0.Y) + (vertex0.X*vertex2.Y))/
						((vertex2.Y - vertex0.Y)*vertex1.X + (vertex2.X - vertex0.X)*vertex1.Y + (vertex2.X*vertex0.Y) + (vertex0.X*vertex2.Y));;
					
					
					gamma = 1 - alpha - gamma;
					
					MGLpixel color =0 ; 
					MGL_SET_RED(color,MGLbyte(alpha*mColorRGB.r) );
					MGL_SET_GREEN(color,MGLbyte(alpha*mColorRGB.g) );
					MGL_SET_BLUE(color,MGLbyte(alpha*mColorRGB.b) );
	
					cout << alpha << "|" << beta << "|" << gamma << "\n";
					
					if((alpha < 1) && (beta  < 1) && (gamma < 1) && (alpha >= 0) && (beta  >= 0 ) && (gamma >= 0)){
						cout << "setting pixel " << i << " " << j << " to color" << color << "\n";
						set_pixel(color, float(i), float(j));
					}
				}
					
			}
			vertex0 = screenMatrix.MultVertex(mVertices[z+3]);
			vertex1 = screenMatrix.MultVertex(mVertices[z+4]);
			vertex2 = screenMatrix.MultVertex(mVertices[z+5]);
			
		}
		
		for(int i = 0; i < size; i++){
				mVertices.pop_back();
			
		}
		return;
	}
		
		// Draw the lines, wireframe triangle for now
		for(int i = 0; i < (size)-1; i = (i+3)){
				
				cout << "mVertices[" << i   <<"] = ( " << mVertices[i].X   << "," << mVertices[i].Y   << "," << mVertices[i].Z   << ") -> (" << vertex0.X << ","  << vertex0.Y << ","  << vertex0.Z << ")\n";
				cout << "mVertices[" << i+1 <<"] = ( " << mVertices[i+1].X << "," << mVertices[i+1].Y << "," << mVertices[i+1].Z << ") -> (" << vertex1.X << ","  << vertex1.Y << ","  << vertex1.Z << ")\n";
				cout << "mVertices[" << i+2 <<"] = ( " << mVertices[i+2].X << "," << mVertices[i+2].Y << "," << mVertices[i+2].Z << ") -> (" << vertex2.X << ","  << vertex2.Y << ","  << vertex2.Z << ")\n";
					
				draw_line(mVertices[i].mPixelColor, mVertices[i+1].mPixelColor,vertex0.X, vertex0.Y, vertex1.X, vertex1.Y,vertex0.Z, vertex1.Z);	
				draw_line(mVertices[i+1].mPixelColor, mVertices[i+2].mPixelColor,vertex1.X, vertex1.Y, vertex2.X, vertex2.Y,vertex1.Z,vertex2.Z);	
				draw_line(mVertices[i+2].mPixelColor, mVertices[i].mPixelColor,vertex2.X, vertex2.Y, vertex0.X, vertex0.Y,vertex2.Z,vertex0.Z);	
					
				vertex0 = screenMatrix.MultVertex(mVertices[i+3]);
				vertex1 = screenMatrix.MultVertex(mVertices[i+4]);
				vertex2 = screenMatrix.MultVertex(mVertices[i+5]);
		}
		
		for(int i = 0; i < size; i++){
				mVertices.pop_back();
			
		}
		
	 
	 }
	 
 void RenderQuads(MGLMatrix screenMatrix, bool triangulate ){
 
	int size = mVertices.size();

	cout << "=========RenderQuads============\n";
	//screenMatrix.seeMatrix();
	

	
	float divisor0 = -mVertices[0].Z;
	float divisor1 = -mVertices[1].Z;	
	float divisor2 = -mVertices[2].Z;	
	
	if(divisor0 == 0)
			divisor0 = 1;
			
	if(divisor1 == 0)
			divisor1 = 1;
			
	if(divisor2 == 0)
		divisor2 = 1;
	
		GLVertex vertex0 = screenMatrix.MultVertex(mVertices[0]);
		GLVertex vertex1 = screenMatrix.MultVertex(mVertices[1]);
		GLVertex vertex2 = screenMatrix.MultVertex(mVertices[2]);


		if(triangulate){	
				
					
					// draw first three
				draw_line(mVertices[0].mPixelColor, mVertices[1].mPixelColor, vertex0.X/divisor0, vertex0.Y/divisor0, vertex1.X/divisor1, vertex1.Y/divisor1);	
				draw_line(mVertices[1].mPixelColor, mVertices[2].mPixelColor, vertex1.X/divisor1, vertex1.Y/divisor1, vertex2.X/divisor2, vertex2.Y/divisor2);	
				draw_line(mVertices[2].mPixelColor, mVertices[0].mPixelColor, vertex2.X/divisor2, vertex2.Y/divisor2, vertex0.X/divisor0, vertex0.Y/divisor0);	
					
					
				vertex0 = screenMatrix.MultVertex(mVertices[2]);
				vertex1 = screenMatrix.MultVertex(mVertices[3]);
				vertex2 = screenMatrix.MultVertex(mVertices[0]);
				
					float divisor0 = -mVertices[2].Z;
					float divisor1 = -mVertices[3].Z;	
					float divisor2 = -mVertices[0].Z;	
				// draw second three
				draw_line(mVertices[2].mPixelColor, mVertices[3].mPixelColor, vertex0.X/divisor0, vertex0.Y/divisor0, vertex1.X/divisor1, vertex1.Y/divisor1);	
				draw_line(mVertices[3].mPixelColor, mVertices[4].mPixelColor, vertex1.X/divisor1, vertex1.Y/divisor1, vertex2.X/divisor2, vertex2.Y/divisor2);	
				draw_line(mVertices[4].mPixelColor, mVertices[2].mPixelColor, vertex2.X/divisor2, vertex2.Y/divisor2, vertex0.X/divisor0, vertex0.Y/divisor0);		
		
		for(int i = 0; i < size; i++){
				mVertices.pop_back();
			
		}
		
		/////////////////////////////////////////////////////////
	

		return;
	}
	
	// Draw the lines, wireframe quad for now
	for(int i = 0; i < size-1; i++){
						
			vertex0 = screenMatrix.MultVertex(mVertices[i]);
			vertex1 = screenMatrix.MultVertex(mVertices[i+1]);
			
			divisor0 = -mVertices[i].Z;
			divisor1 = -mVertices[i+1].Z;	
			
			
			if(divisor0 == 0 && divisor1 == 0){	
				if(vertex0.Z == vertex1.Z || (vertex0.Z+vertex1.Z) == 1){
					divisor0 = 1;
					divisor1 = 1;
				}
				else{
					divisor0 = vertex0.Z;
					divisor1 = vertex1.Z;
				
				}
				
			}
			//if(divisor1 == 0)
			//	divisor1 = 1;
			cout << "======Line " << i+1 << "=====\n";
		    cout << "mVertices[" << i   <<"] = ( " << mVertices[i].X   << "," << mVertices[i].Y    << "," << mVertices[i].Z  << ") -> (" << vertex0.X/divisor0 << ","  << vertex0.Y/divisor0 << "," <<vertex0.Z <<")\n";
			cout << "mVertices[" << i+1 <<"] = ( " << mVertices[i+1].X << "," << mVertices[i+1].Y  << "," << mVertices[i+1].Z<< ") -> (" << vertex1.X/divisor1 << ","  << vertex1.Y/divisor1 << "," <<vertex1.Z  << ")\n";
			cout << "divisor0 = " << divisor0 << " divisor1 = " << divisor1 << "\n"; 
			draw_line(mVertices[i].mPixelColor, mVertices[i+1].mPixelColor, vertex0.X/divisor0 , vertex0.Y/divisor0, vertex1.X/divisor1, vertex1.Y/divisor1, vertex0.Z, vertex1.Z);	
					
			
	}
	
	
	// Draw the last line back to the first
	vertex0 = screenMatrix.MultVertex(mVertices[size-1]);
	vertex1 = screenMatrix.MultVertex(mVertices[0]);	
	
		divisor0 = -mVertices[size-1].Z;
		divisor1 = -mVertices[0].Z;	
	
		if(divisor0 == 0 && divisor1 == 0){	
				if(vertex0.Z == vertex1.Z || (vertex0.Z+vertex1.Z) == 1){
					divisor0 = 1;
					divisor1 = 1;
				}
				else{
					divisor0 = vertex0.Z;
					divisor1 = vertex1.Z;
				
				}
				
			}
		
	cout << "======Line " << size << "=====\n";
	 cout << "mVertices[" << size-1   <<"] = ( " << mVertices[size-1].X   << "," << mVertices[size-1].Y    << "," << mVertices[size-1].Z  << ") -> (" << vertex0.X/divisor0 << ","  << vertex0.Y/divisor0 << "," <<vertex0.Z <<")\n";
			cout << "mVertices[" << 0<<"] = ( " << mVertices[0].X << "," << mVertices[0].Y  << "," << mVertices[0].Z<< ") -> (" << vertex1.X/divisor1 << ","  << vertex1.Y/divisor1 << "," <<vertex1.Z  << ")\n";
				
	draw_line(mVertices[size-1].mPixelColor, mVertices[0].mPixelColor, vertex0.X/divisor0 , vertex0.Y/divisor0, vertex1.X/divisor1, vertex1.Y/divisor1, vertex0.Z, vertex1.Z);	
	
	for(int i = 0; i < size; i++){
				mVertices.pop_back();			
	}
		
	 
}
void mglEnd()
{
	
	if(!mHasBegun){
			
			MGL_ERROR("Must call mglBegin before mglEnd!");
			return;
	}
	
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
	//for(int i = 0; i < (WIDTH*HEIGHT); i++){
		//cout << "mFrameBuffer = " <<  mFrameBuffer[i] << endl;
	//}
	mHasBegun = false;
	// This is where we actually do the drawing
	// but we need to go from vertices to vertices*viewmode
	
	// This defines our veiw port for every model
	// It translates from object cooridnates to eye view
	MGLMatrix viewport;
	MGLMatrix model = mModelViewStack[mModelViewTracker];
	MGLMatrix proj = mProjectionStack[mProjectionTracker];
	
	viewport.SetIdentity();
    viewport.TranslateMatrix(-0.5, -0.5, -0.5);//mVertices[0].X, mVertices[0].Y, 0);
	viewport.ScaleMatrix(MGLfloat(WIDTH/2), MGLfloat(HEIGHT/2), 1);
	viewport.TranslateMatrix(1, 1, 1);
		
	// Check our matrices for an easy one
	cout << "=======Projection matrix\n";
	mProjectionStack[mProjectionTracker].seeMatrix();
	cout << "=======Modelveiw matrix\n";
	mModelViewStack[mModelViewTracker].seeMatrix();
	// next we need to take the vertex cooridinates into screen space
	// To do this we apply the modelview and projection matrices
	//model.MultiplyMatrix();		
	MGLMatrix screenMatrix = model.MultiplyMatrix(proj).MultiplyMatrix(viewport);
    //= model.MultiplyMatrix(proj).MultiplyMatrix(viewport); //.MultiplyMatrix();
		
	cout << "========Screen matrix\n";
	screenMatrix.seeMatrix();
	
	if(VIEW_POLY == MGL_TRIANGLES){
			RenderTriangles(screenMatrix, true);
			
		return;
	}
	else if(VIEW_POLY == MGL_QUADS){
		
			RenderQuads(screenMatrix, false);
		return;
	}
	//if we reached this point something went qrong, throw an error
	MGL_ERROR("A valid polygon type was not chosen");
	
	

}

void set_pixel(MGLpixel color, float x, float y){
	set_pixel(color, x, y, 0);	
}
void set_pixel(MGLpixel color, float x, float y, float depth)
{
    //float col[] = { 1.0, 1.0, 1.0 };
    //set_pixel(x,y,col);
    

    // Clip all pixels that are not in the view port
    if((x < 0) || (x > WIDTH) || (y < 0) || (y > HEIGHT)){ // not a vaild pixel, clip it, may need for rasterizer
		//cout << "Clipping pixel outside of viewport\n";
		return ;
	}
	
	// the new pixel is deeper than the old pixel
    // do not replace	
	if(depth < mZBuffer[(WIDTH*int(y)) + int(x)]){
		//cout << "Pixel " << (WIDTH*int(y)) + int(x) << " is ocluded\n";
		return;
		
	}
	
	//cout << "setting pixel " << (WIDTH*int(y)) + int(x) << " to depth = "<< depth << "\n";
    mZBuffer[(WIDTH*int(y)) + int(x)] = depth;
    mFrameBuffer[(WIDTH*int(y)) + int(x)] = color;
    
    //mFrameBuffer[(WIDTH*int(y)) + int(x)] = 10001000;
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
		cout << "Not going to do depth interpolation\n";
		cout << "depth1 = " << depth1 << " depth2 = " << depth2 << "\n";
		depth = depth1;
	}
    
    float distance = distancei(x0, y0, x1, y1);
    if(dx == 0){
       // cout << "DX = 0 \n";
;		if(y0 < y1){
			for(int y = y0; y < y1; ++y){
				alpha = (distancei(x0, y0, x1, y)/distance);
				beta = 1 - alpha;			
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				
				if(depth1 == depth2)
				{
					depth = depth1;
						
				}
				else
					depth = alpha*depth2 + beta*depth1;
					
				set_pixel(color, x0, y, depth);
				}
		}else if(y0 > y1){
			
			for(int y = y0; y > y1; --y){
				
				alpha = (distancei(x0, y0, x1, y)/distance);
				beta = 1 - alpha;			
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
				{
					depth = depth1;
				}
				else
					depth = alpha*depth2 + beta*depth1;
					
				set_pixel(color, x0, y, depth);
				
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
				
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                set_pixel(color, x, m*x + b, depth);
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
						
							color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
							color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
							color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
							if(depth1 == depth2)
								depth = depth1;
							else
								depth = alpha*depth2 + beta*depth1;
							set_pixel(color, ((y-b)/m), y, depth);
					}
						//cout << "y = " << y << " x = " << ((y-b)/m) << "\n";

                }
                
            }else{
                
                for(int y = y0; y < y1; ++y){
					if(m <= -1){
						alpha = (distancei(x0, y0, ((y-b)/m),y)/distance);
						beta = 1 - alpha;			
						color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
						color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
						color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
						if(depth1 == depth2)
							depth = depth1;
						else
							depth = alpha*depth2 + beta*depth1;
						//cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
						set_pixel(color, ((y-b)/m), y, depth);
						
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
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                set_pixel(color, x, m*x + b, depth);
                
                }
        }else if((m >= 1) || (m <= -1)){ 
            //cout << "where am i 1\n";
            if(y0 < y1){
                for(int y = y0; y < y1; ++y){
				
				alpha = (distancei(x0, y0, ((y-b)/m), y)/distance);
				beta = 1 - alpha;			
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
                //cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
                //cout << "where am i " << y << "|" << y1 << "\n";
					set_pixel(color, ((y-b)/m), y, depth);
             }
                
            }else{
                // y = m*x+b -> (y-b)/m = x
                for(int y = y0; y > y1; --y){
					alpha = (distancei(x0, y0, ((y-b)/m), y)/distance);
				beta = 1 - alpha;			
				color = MGL_SET_BLUE(color, MGLpixel((alpha*blue2)+(beta*blue1)));
				color = MGL_SET_RED(color, MGLpixel((alpha*red2)+(beta*red1)));
				color = MGL_SET_GREEN(color, MGLpixel((alpha*green2)+(beta*green1)));
				if(depth1 == depth2)
					depth = depth1;
				else
					depth = alpha*depth2 + beta*depth1;
					set_pixel(color, ((y-b)/m), y, depth);
					
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
	GLVertex vertex;
	vertex.X = x;
	vertex.Y = y;
	vertex.Z = z;
	vertex.w = 1;
	vertex.mPixelColor = mColor;
	//cout << vertex.X << " " << vertex.Y  << " " <<  vertex.Z << endl;
	mVertices.push_back(vertex);
		
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
		
	
	//mCurrentMatrix.seeMatrix();
	
	switch(VIEW_STATE){
		case MGL_MODELVIEW:{
			cout << "Pushing a  modelview matrix\n";
			
			mCurrentMatrix.seeMatrix();
			mModelViewStack[mModelViewTracker++] = mCurrentMatrix;	
			mCurrentMatrix = mModelViewStack[mModelViewTracker-1] ;			
			break;
		}
		case MGL_PROJECTION:{
			cout << "Pushing a projection matrix\n";
			mCurrentMatrix.seeMatrix();
			 mProjectionStack[mProjectionTracker++] = mCurrentMatrix;
			 mCurrentMatrix = mProjectionStack[mProjectionTracker++];
			break;
		}
	}
	cout << "=========new matrix=======\n";
	mCurrentMatrix.seeMatrix();
	cout << "End pushing a matrix\n";
	
	
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
	
	cout << "Popping a matrix\n";
	mCurrentMatrix.seeMatrix();
	
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
	cout << "=============\n";

	mCurrentMatrix.seeMatrix();
	cout << "end popping sequence\n";
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
	
	mColorRGB.b = blue;
	mColorRGB.r = red;
	mColorRGB.g = green;
}

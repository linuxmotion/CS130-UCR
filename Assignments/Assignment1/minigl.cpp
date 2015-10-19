/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 * Do not use any additional files
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector> // must replace
#include "minigl.h"

using namespace std;

struct GLVertex{
	MGLfloat X;
	MGLfloat Y;
	MGLfloat Z;
} ;

class Vector{
	
	MGLfloat vector[3];
	
	Vector(MGLfloat x, MGLfloat y, MGLfloat z){
		
		vector[0] = x; 
		vector[1] = y; 
		vector[2] = z;
	}
	
	MGLfloat operator [](int i){
		
	return vector[i];
	}
	
	
};

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
		
		this->MultiplyMatrix(scalar);
		
		
		
	}
	
	void RotateMatrix(MGLfloat angle, MGLfloat x, MGLfloat y,MGLfloat z ){
		
		
		MGLMatrix zrotate, yrotate, xrotate;
		
		zrotate.SetIdentity();
		yrotate.SetIdentity();
		xrotate.SetIdentity();
		
		//float cosh = cos(angle);
		//float sin  = sin(angle);
		
		//mMatrix[0] = 
		
		
			
		
	}
	
	void TranslateMatrix(MGLfloat x, MGLfloat y,MGLfloat z ){
		
			MGLMatrix translation;
			translation.SetIdentity();
			
			translation[12] = x;
			translation[13] = y;
			translation[14] = z;
			
			this->MultiplyMatrix(translation);
			
		
	}
	
	// Matrix multiplication is done left to right
	// the matrix "multiply" will be applied from the left
	// i.e.  A = B*A
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
	
	
	GLVertex MultVertex(GLVertex vertex){
		
		GLVertex newVertex;
		MGLfloat temp[4];
		//cout << "Input vertex\n";
		//cout << "X = " << vertex.X << " Y =  " << vertex.Y  << " Z =  " << vertex.Z << "\n" ;
		// for each row
		for(int i = 0; i < 4; i++){			
			// multiple against each column of new matrix	
			// cout << mMatrix[i] << " " << mMatrix[i+4]<< " " <<  mMatrix[i+8] << " " << mMatrix[i+12] << "\n";
			temp[i]= mMatrix[i]*vertex.X + mMatrix[i+4]*vertex.Y + mMatrix[i+8]*vertex.Z + mMatrix[i+12]*1;				
		}	
		newVertex.X = temp[0];
		newVertex.Y = temp[1];
		newVertex.Z = temp[2];
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
		
		cout << "\n====================\n";
			for(int i =0; i < 4; i++){
					cout << mMatrix[i]  << " " << mMatrix[i+4] <<  " " << mMatrix[i+8] << " " << mMatrix[i+12]  << "\n"; 
			}
	
	}
	
	//MGLMatrix operator *(GLVertex &vertex){
	//	
	//		return this->MultVertex(vertex);
		
	//}
	
	
};




void draw_line(int x0, int y0, int x1, int y1);

	 


//*********Global Variables**********//
bool mHasBegun = false;
#define WIDTH 320
#define HEIGHT 240

MGLMatrix mCurrentMatrix;

vector<GLVertex> mVertices;
MGLpixel mFrameBuffer[(WIDTH*HEIGHT)];

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
	
	
}

/**
 * Stop specifying the vertices for a group of primitives.
 */
 
 void RenderTriangles(MGLMatrix screenMatrix){
	 
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
		
		
		// Draw the lines, wireframe triangle for now
		for(int i = 0; i < (size/3); i = (i+2)){
				
				cout << "mVertices[" << i <<"] = ( " << mVertices[i].X << "," << mVertices[i].Y << ") -> (" << vertex0.X << ","  << vertex0.Y << ")\n";
				cout << "mVertices[" << i+1 <<"] = ( " << mVertices[i+1].X << "," << mVertices[i+1].Y << ") -> (" << vertex1.X << ","  << vertex1.Y << ")\n";
				cout << "mVertices[" << i+2 <<"] = ( " << mVertices[i+2].X << "," << mVertices[i+2].Y << ") -> (" << vertex2.X << ","  << vertex2.Y << ")\n";
					
				draw_line(vertex0.X , vertex0.Y, vertex1.X, vertex1.Y);	
				draw_line(vertex1.X , vertex1.Y, vertex2.X, vertex2.Y);	
				draw_line(vertex2.X , vertex2.Y, vertex0.X, vertex0.Y);	
					
				vertex0 = screenMatrix.MultVertex(mVertices[i+2]);
				vertex1 = screenMatrix.MultVertex(mVertices[i+3]);
				vertex2 = screenMatrix.MultVertex(mVertices[i+4]);
		}
		
		for(int i = 0; i < size; i++){
				mVertices.pop_back();
			
		}
		
	 
	 }
	 
 void RenderQuads(MGLMatrix screenMatrix ){
	 
		int size = mVertices.size();

		//cout << "Screen matrix\n";
		//screenMatrix.seeMatrix();
		
		GLVertex vertex0 = screenMatrix.MultVertex(mVertices[0]);
		GLVertex vertex1 = screenMatrix.MultVertex(mVertices[1]);		
		// Draw the lines, wireframe quad for now
		for(int i = 0; i < size; i++){
			
			    cout << "mVertices[" << i <<"] = ( " << mVertices[i].X << "," << mVertices[i].Y << ") -> (" << vertex0.X << ","  << vertex0.Y << ")\n";
				
				draw_line(vertex0.X , vertex0.Y, vertex1.X, vertex1.Y);	
							
				vertex0 = vertex1;
				vertex1 = screenMatrix.MultVertex(mVertices[i+1]);
		}
		// Draw the last line back to the first
		draw_line( screenMatrix.MultVertex(mVertices[size-1]).X, screenMatrix.MultVertex(mVertices[size-1]).Y, screenMatrix.MultVertex(mVertices[0]).X, screenMatrix.MultVertex(mVertices[0]).Y);
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
	//for(int i = 0; i < (WIDTH*HEIGHT); i++){
		//cout << "mFrameBuffer = " <<  mFrameBuffer[i] << endl;
	//}
	mHasBegun = false;
	// This is where we actually do the drawing
	// but we need to go from vertices to vertices*viewmode
	
	// This defines our veiw port for every model
	// It translates from object cooridnates to eye view
	MGLMatrix view;
	MGLMatrix model = mModelViewStack[mModelViewTracker];
	MGLMatrix proj = mProjectionStack[mProjectionTracker];
	
	view.SetIdentity();
	view.TranslateMatrix(1, 1, 1);
	view.ScaleMatrix(MGLfloat(WIDTH/2), MGLfloat(HEIGHT/2), 1);
	view.TranslateMatrix(-0.5, -0.5, -0.5);//mVertices[0].X, mVertices[0].Y, 0);
		
	// Check our matrices for an easy one
	cout << "=======Projection matrix\n";
	mProjectionStack[mProjectionTracker].seeMatrix();
	cout << "=======Modelveiw matrix\n";
	mModelViewStack[mModelViewTracker].seeMatrix();
	// next we need to take the vertex cooridinates into screen space
	// To do this we apply the modelview and projection matrices
	model.MultiplyMatrix(view);		
	MGLMatrix screenMatrix = proj.MultiplyMatrix(model); //.MultiplyMatrix();
		
	cout << "Screen matrix\n";
	screenMatrix.seeMatrix();
	
	if(VIEW_POLY == MGL_TRIANGLES){
			RenderTriangles(screenMatrix);
		return;
	}
	else if(VIEW_POLY == MGL_QUADS){
		
			RenderQuads(screenMatrix);	
		return;
	}
	//if we reached this point something went qrong, throw an error
	MGL_ERROR("A valid polygon type was not chosen");
	
	
	
}

void set_pixel(float x, float y)
{
    //float col[] = { 1.0, 1.0, 1.0 };
    //set_pixel(x,y,col);
    if((x < 0) || (x > WIDTH) || (y < 0) || (y > HEIGHT)) // not a vaild pixel, clip it, may need for rasterizer
		return ;
    
    mFrameBuffer[(WIDTH*int(y)) + int(x)] = 10001000;
}


void draw_line(int x0, int y0, int x1, int y1)
{
	//cout << "X0 = " << x0 << " Y0 = " << y0 << "\n";
	//cout << "X1 = " << x1 << " Y0 = " << y1 << "\n";

    float dx = x1 - x0;
    float dy = y1 - y0;
    
    if(dx == 0){
        
            if(y0 < y1){
                for(int y = y0; y < y1; ++y)
                    set_pixel(x0, y);
            }else if(y0 > y1){
                
                for(int y = y0; y > y1; --y)
                    set_pixel(x0, y);
            }
            return;
    
    }
    

    float m = dy/dx;
    float b = y0 - m*x0;
    //cout << "m = " << m << "\n";
    if(x1 > x0){ 
        if((-1 <= m) && (m <= 1)){
            for(int x = x0; x < x1; ++x)
                set_pixel(x, m*x + b);
        }else if((m >= 1) || (m <= -1)){ // y = m*x+b -> (y-b)/m = x
            //cout << "abs Slope is greater than 1\n";
            if(y0 > y1){
                for(int y = y0; y > y1; --y){
                if(m <= -1)
                    //cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
                set_pixel(((y-b)/m), y);
                }
                
            }else{
                
                for(int y = y0; y < y1; ++y){
                if(m <= -1)
                    //cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
                set_pixel(((y-b)/m), y);
                }
                
            }
            
            
        }
    } else if(x1 < x0){
        if((-1 <= m ) && (m <= 1)){
            for(int x = x0; x > x1; --x)
                set_pixel(x, m*x + b);
        }else if((m >= 1) || (m <= -1)){ 
            //cout << "where am i 1\n";
            if(y0 < y1){
               // cout << "where am i 2\n";
                for(int y = y0; y < y1; ++y){
                //cout << "y = " << y << " x = " << ((y-b)/m) << "\n";
                set_pixel(((y-b)/m), y);
                }
                
            }else{
                // y = m*x+b -> (y-b)/m = x
                for(int y = y0; y > y1; --y)
                set_pixel(((y-b)/m), y);
                }
            
            
            
        }
    }
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
	cout << vertex.X << " " << vertex.Y  << " " <<  vertex.Z << endl;
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
		
	cout << "Pushing a matrix\n";
	mCurrentMatrix.seeMatrix();
	
	switch(VIEW_STATE){
		case MGL_MODELVIEW:{
			mModelViewStack[mModelViewTracker++] = mCurrentMatrix;	
			mCurrentMatrix = mModelViewStack[mModelViewTracker-1] ;			
			break;
		}
		case MGL_PROJECTION:{
			 mProjectionStack[mProjectionTracker++] = mCurrentMatrix;
			 mCurrentMatrix = mProjectionStack[mProjectionTracker++];
			break;
		}
	}
	
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
	perspective[0] = 2/(right - left);
	perspective[5] = 2/(top - bottom);
	perspective[10] = -2/(far - near);
	
	perspective[8] =  -(right+left)/(right - left);
	perspective[9] = -(top+bottom)/(top - bottom);
	perspective[14] = (-2*far*near)/(far - near);
	perspective[15] = 0;
	
	mCurrentMatrix.MultiplyMatrix(perspective);
	
	cout << "Perspective \n";
	mCurrentMatrix.seeMatrix();
	
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


	//orthographic.seeMatrix();
	
	//mCurrentMatrix = 
	mCurrentMatrix.MultiplyMatrix(orthographic);
	cout << "Orthographic \n";
	mCurrentMatrix.seeMatrix();
	
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red,
              MGLbyte green,
              MGLbyte blue)
{
}

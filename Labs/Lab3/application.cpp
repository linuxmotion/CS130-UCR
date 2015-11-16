#include "application.h"

#include <iostream>
#include <cassert>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void draw_grid();
    
application::application() 
    : solid(true)
{
}

application::~application()
{
}

// triggered once after the OpenGL context is initialized
void application::init_event()
{

    cout << "CAMERA CONTROLS: \n  LMB: Rotate \n  MMB: Pan \n  LMB: Dolly" << endl;
    cout << "KEYBOARD CONTROLS: \n  '=': Toggle wireframe mode" << endl;

    const GLfloat ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    const GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    // enable a light
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);
    
    // enable depth-testing, colored materials, and lighting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    // normalize normals so lighting calculations are correct
    // when using GLUT primitives
    glEnable(GL_NORMALIZE);
    
    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    // set the cameras default coordinates
    camera.set_distance(20);
    camera.set_elevation(35);
    camera.set_twist(45);

    t.reset();
}

void application::draw_student_lab(){
    
    // make a disc
    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(0, 8, 0);
    glRotatef(90,0,100,0);
    solid ? glutSolidTorus(0.5, 1, 20, 20) : glutWireTorus(0.5, 1, 20, 20);
    glPopMatrix();
  
    //Sun
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 1, 0);
    glRotatef(t.elapsed()*90, 1, 1, 1);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    
    // Mercury
    glPushMatrix();
    glColor3f(1, 0, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    glRotatef(t.elapsed()*180, 0, 1, 0);
    glTranslatef(0, 1, 12);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    
    // Venus
    glPushMatrix();
    glColor3f(0, 1, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(t.elapsed()*100, 0, 1, 0);
    glTranslatef(0, 1, 10);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    
    glPopMatrix();
    
    
    //Earth
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(t.elapsed()*120, 0, 1, 0);
    glTranslatef(0, 1, 15);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
   
    
    //Earths moon
    // we are procedding here using the earths coorinates as our new
    // origin
    glPushMatrix();
    
    glColor3f(1, 1, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(t.elapsed()*120, 0, 1, 0);
    glTranslatef(0, 0,20);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    glPopMatrix();
    
    //Mars
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 0, 0);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(t.elapsed()*110, 0, 1, 0);
    glTranslatef(0, 1, 33);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
     glPushMatrix();
    
    //Mars - Deimos
    glColor3f(1,0 , 1);
    glTranslatef(0, 0, 0);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(t.elapsed()*70, 0, 1, 0);
    glTranslatef(0, 0,20);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    //Mars - Phobos
    glColor3f(1, 1, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(t.elapsed()*120, 0, 1, 0);
    glTranslatef(0, 0,25);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    glPopMatrix();
    
    
    //Jupiter
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.7, 0.7, 0.7);
    glRotatef(t.elapsed()*90, 0, 1, 0);
    glTranslatef(0, 1, 20);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    
    //Saturn
    glPushMatrix();
    glColor3f(5, 5, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.55, 0.55, 0.55);
    glRotatef(t.elapsed()*90, 0, 1, 0);
    glTranslatef(0, 1, 35);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    //Uranus
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(0, 0, 0);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(t.elapsed()*90, 0, 1, 0);
    glTranslatef(0, 1,60);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    //Neptune
    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, 0);
    glScalef(0.44, 0.44, 0.44);
    glRotatef(t.elapsed()*90, 0, 1, 0);
    glTranslatef(0, 1,70);
    if(sphere) solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    else solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    glPopMatrix();
    
    
    
}

// triggered each time the application needs to redraw
void application::draw_event()
{
    // apply our camera transformation
    camera.apply_gl_transform();
    
    // set the position of the light
    const GLfloat light_pos1[] = { 0.0, 10.0, 0.0, 1 };   
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    
    // draws the grid and frame at the origin
    draw_grid();

    draw_student_lab();
    //
    // create some various objects in the world
    //
    //glPushMatrix();
    //glColor3f(1, 0, 0);
    //glTranslatef(5, 1, -5);
    //solid ? glutSolidSphere(1, 20, 20) : glutWireSphere(1, 20, 20);
    //glPopMatrix();
    
    //glPushMatrix();
    //glColor3f(0, 1, 1);
    //glTranslatef(-5, 1, 5);
    //solid ? glutSolidTorus(0.5, 1, 20, 20) : glutWireTorus(0.5, 1, 20, 20);
    //glPopMatrix();
    
    //glPushMatrix();
    //glColor3f(0, 0, 1);
    //glTranslatef(-5, 1, -5);
    //solid ? glutSolidCone(1, 2, 10, 10) : glutWireCone(1, 2, 10, 10);
    //glPopMatrix();
    
    //glPushMatrix();
    //glColor3f(0, 1, 0);
    //glTranslatef(5, 0.5, 5);
    //solid ? glutSolidCube(1) : glutWireCube(1);
    //glPopMatrix();

    //glPushMatrix();
    //glColor3f(1, 1, 0);
    //glTranslatef(0, 2, 0);
    //// rotate 180 degrees/second about the y-axis
    //glRotatef(t.elapsed()*180, 0, 1, 0);
    //glScalef(0.5, 0.5, 0.5);
    //solid ? glutSolidDodecahedron() : glutWireDodecahedron();
    //glPopMatrix();
}

// triggered when mouse is clicked
void application::mouse_click_event(
    mouse_button button, mouse_button_state button_state, 
    int x, int y
    )
{
}
    
// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
    if (key == '=')
        solid = !solid;
    if(key == '-')
        sphere = !sphere;
}

void draw_grid()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);

    //
    // Draws the coordinate frame at origin
    //
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5); 
    glBegin(GL_LINES);

    // x-axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    
    // y-axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    
    // z-axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glPopMatrix();
    
    //
    // Draws a grid along the x-z plane
    //
    glLineWidth(1.0);
    glColor3f(.20, .20, .20);
    glBegin(GL_LINES);

    int ncells = 20;
    int ncells2 = ncells/2;

    for (int i= 0; i <= ncells; i++)
    {
        int k = -ncells2;
        k +=i;
        glVertex3f(ncells2,0,k);
        glVertex3f(-ncells2,0,k);
        glVertex3f(k,0,ncells2);
        glVertex3f(k,0,-ncells2);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

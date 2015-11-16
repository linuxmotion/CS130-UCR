#include "application.h"

#include <iostream>
#include <cassert>

using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// set pixel (x,y) in framebuffer to color col, where
// col is a float array of three values between 0 and 1
// which specify the amount of red, green, and blue to mix (e.g.
// RED: (1,0,0) GREEN: (0,1,0) BLUE: (0,0,1) 
// YELLOW: (1,1,0) MAGENTA: (1,0,1) CYAN: (0,1,1)
// )
//
void set_pixel(int x, int y, float col[3])
{
    // write a 1x1 block of pixels of color col to framebuffer
    // coordinates (x, y)
    glRasterPos2i(x, y);
    glDrawPixels(1, 1, GL_RGB, GL_FLOAT, col);
}

void set_pixel(int x, int y)
{
    float col[] = { 1.0, 1.0, 1.0 };
    set_pixel(x,y,col);
}


//
// This is where the line drawing algorithm gets called
// Fix it so that there are no gaps and that it works for all cases
// This lab requires you to use dda to render the line: 
// http://en.wikipedia.org/wiki/Digital_differential_analyzer_%28graphics_algorithm%29
//
void draw_line(int x0, int y0, int x1, int y1)
{
    //New code is here
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






// triggered each time the application needs to redraw
void application::draw_event()
{
    //
    // This code draws a single red horizontal line across
    // the the middle of the window using the function
    // set_pixel. Replace this code with your DDA
    // implementation that can draw a line of any slope. You
    // should only need to call set_pixel within your
    // routine.
    //
    float col[] = { 1.0, 0.0, 0.0 };
    for (int i = 0; i < this->width; ++i)
    {
        set_pixel(i, (int)this->height/2, col);
    }
    
    //
    // This code will draw each of the lines
    //
    glColor3f(1.0, 1.0, 1.0);
    for (size_t i = 0; i < lines.size(); ++i)
    {
	draw_line(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1);
    }
}

// triggered when mouse is clicked
void application::mouse_click_event(
    mouse_button button, mouse_button_state button_state, 
    int x, int y
    )
{
    // Each time the mouse button is pushed down, add a new
    // line segment to our list.
    if (button == LEFT_BUTTON && button_state == BUTTON_DOWN) {
        lines.push_back(line_segment(x, y, x, y));
    }
    
    //cout << "button event: " << button << " " << button_state
    //    << " " << x << " " << y << endl;
}
    
// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
    // While the mouse button is down, update the last
    // endpoint in our list to reflect the current location
    // of the mouse.
    lines[lines.size()-1].x1 = x;
    lines[lines.size()-1].y1 = y;
    
    //cout << "move event: " << x << " " << y << endl;
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
    // clear our stored clicks
    if (key == 'c')
        lines.clear();

    //cout << "keyboard event: " << key << " " 
    //    << x << " " << y << endl;
}

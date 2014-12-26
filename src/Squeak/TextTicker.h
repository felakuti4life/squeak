//
//  TextTicker.h
//  Squeak
//
//  Created by Andrew Forsyth on 11/22/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef Squeak_TextTicker_h
#define Squeak_TextTicker_h

//#include <GLUT/glut.h>
#include <GL/freeglut.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


//#include <FTGL/ftgl.h>

class TextTicker{
    vector<string> log;
    GLfloat x; //left x
    GLfloat y; //bottom y
    int numLines;
    GLint fontHeightInPixels;
    GLint bottom_offset;
    GLint right_offset;
    GLint left_offset;
    GLfloat text_spacing = 1.5;
    GLfloat color_delta;
    void *font;
public:
    //params windowx, windowy, font, right margin, left margin, bottom margin
    
    TextTicker(GLint _x, GLint _y, void *font, GLint rightOff, GLint leftOff, GLint bottomOff);
    void updateLog(string toAdd);
    void updateFont(void *font);
    void drawLog();
    void clearLog();
    //params windowx, windowy, right margin, left margin, bottom margin
    void reshape(GLint new_x, GLint new_y, GLint rightOff, GLint leftOff, GLint bottomOff);
    void RenderString(float x, float &y, void *font, string str, GLfloat r, GLfloat g, GLfloat b);
};

#endif

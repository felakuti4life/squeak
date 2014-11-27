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
    GLint bottom_offset = 10;
    GLint right_offset = 10;
    GLint left_offset = 10;
    GLfloat text_spacing = 1.5;
    GLfloat color_delta;
    void *font;
public:
    TextTicker(GLint _x, GLint _y, void *font);
    void updateLog(string toAdd);
    void drawLog();
    void clearLog();
    void reshape(GLint new_x, GLint new_y);
private:
    void RenderString(float x, float &y, void *font, string str, GLfloat r, GLfloat g, GLfloat b);
};

#endif

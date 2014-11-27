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
using namespace std;


//#include <FTGL/ftgl.h>

class TextTicker{
    vector<string> log;
    GLfloat x; //left x
    GLfloat y; //bottom y
public:
    TextTicker(GLint _x, GLint _y);
    void updateLog(string toAdd);
    void drawLog();
    void clearLog();
    void reshape(GLint new_x, GLint new_y);
private:
    void RenderString(float x, float y, void *font, string str, GLfloat r, GLfloat g, GLfloat b);
};

#endif

//
//  GraphicsTools.h
//  Squeak
//
//  Created by Ethan Geller on 11/29/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef __Squeak__GraphicsTools__
#define __Squeak__GraphicsTools__

#include "x-vector3d.h"
#include "RtAudio.h"
#include "chuck_fft.h"
using namespace std;




enum movement {
    IDLE, FORWARD, BACKWARD, LEFT, RIGHT
};


GLuint LoadTexture(const char *filename);
void draw_osc(GLfloat *z, GLfloat *zinc, int g_bufferSize, int g_windowSize, float* g_buffer, int g_height);
void draw_fft(GLfloat *z, GLfloat *zinc, complex *cbuf, int g_windowSize);
void drawCube(float zGeMotion, float zLynchMotion, float yTopMotion, float yBottomMotion);
void actionRotate();
void actionMove();

#endif /* defined(__Squeak__GraphicsTools__) */

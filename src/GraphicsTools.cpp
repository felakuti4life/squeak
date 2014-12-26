//
//  GraphicsTools.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/29/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "GraphicsTools.h"
#include "GL/freeglut.h"
float g_step = 0.05;
float victory_score = 200;
float victory_threshold = 0.0008;

Vector3D cube_rotation;
movement direction = IDLE;

//-----------------------------------------------------------------------------
// name: LoadTexture(const char * filename)
// desc: loads bmp as texture
//-----------------------------------------------------------------------------
GLuint LoadTexture(const char *filename) {
    
    GLuint texture;
    
    int width, height;
    
    unsigned char *data;
    
    FILE *file;
    
    file = fopen(filename, "rb");
    
    if (file == NULL) return 0;
    width = 1028;
    height = 1028;
    data = (unsigned char *) malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, 54, 1, file);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    
    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char B, G, R;
        B = data[index];
        G = data[index + 1];
        R = data[index + 2];
        
        data[index] = R;
        data[index + 1] = G;
        data[index + 2] = B;
        
    }
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    
    return texture;
    
}


void draw_osc(GLfloat *z, GLfloat *zinc, int g_bufferSize, int g_windowSize, float* g_buffer, int g_height) {
    glPushMatrix();
    // translate
    glTranslatef(0, 2, 0);
    // start primitive
    glBegin(GL_LINE_STRIP);
    // loop over buffer
    float HalfBuf = g_bufferSize/2;
    for (float i = 0; i < g_bufferSize; i++) {
        // plot
        float col = i/HalfBuf;
        if(i < HalfBuf)
            glColor3f(col, col, col);
        
        else glColor3f(1-col, 1-col, 1-col);
        
        glVertex2f((GLfloat) i*4, (GLfloat) (g_height - 20) + 40 * g_buffer[(int)i]);
        // increment z
        *z += *zinc;
    }
    // end primitive
    glEnd();
    // pop
    glPopMatrix();
}

void draw_fft(GLfloat* z, GLfloat *zinc, complex *cbuf, int g_windowSize) {
    float win = g_windowSize;
    float gradient = 0.0;
    glPushMatrix();
    // translate
    glTranslatef(0, -2, 0);
    // color
    glColor3f(0, 0.2, 0.2);
    // start primitive
    glBegin(GL_LINE_STRIP);
    // loop over buffer to draw spectrum
    for (float i = 0; i < g_windowSize; i++) {
        // plot the magnitude,
        // with scaling, and also "compression" via pow(...)
        glColor3f(0, 0.6 - (i / (win)), 0.3);
        glVertex2f(i*4, (GLfloat) (10 * pow(cmp_abs(cbuf[(int)i]), .5)+30));
        // increment x
        *z += *zinc;
    }
    // end primitive
    glEnd();
    // restore transformations
    glPopMatrix();
}

void drawCube(float zGeMotion, float zLynchMotion, float yTopMotion, float yBottomMotion) {
    
    glBegin(GL_QUADS);
    // Front Face (ge face)
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f - zGeMotion);
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f - zGeMotion);
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f - zGeMotion);
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f - zGeMotion);
    glEnd();
    
    glBegin(GL_QUADS);
    
    // Back Face (lynch face)
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f + zLynchMotion);
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f + zLynchMotion);
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f + zLynchMotion);
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f + zLynchMotion);
    glEnd();
    
    glBegin(GL_QUADS);
    // Top Face
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f + yTopMotion, -1.0f);
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f + yTopMotion, 1.0f);
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f + yTopMotion, 1.0f);
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f + yTopMotion, -1.0f);
    glEnd();
    
    glBegin(GL_QUADS);
    // Bottom Face
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f - yBottomMotion, -1.0f);
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f - yBottomMotion, -1.0f);
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f - yBottomMotion, 1.0f);
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f - yBottomMotion, 1.0f);
    glEnd();
}

void actionRotate() {
    glRotatef(cube_rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(cube_rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(cube_rotation.z, 0.0f, 0.0f, 1.0f);
}

void actionMove() {
    switch (direction) {
        case IDLE:
            break;
        case FORWARD:
            glTranslatef(0, 0, g_step);
            break;
        case BACKWARD:
            glTranslatef(0, 0, 0 - g_step);
        case LEFT:
            glTranslatef((0 - g_step), 0, 0);
            break;
        case RIGHT:
            glTranslatef(g_step, 0, 0);
        default:
            break;
    }
    direction = IDLE;
}
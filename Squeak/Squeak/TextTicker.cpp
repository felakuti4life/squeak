//
//  TextTicker.cpp
//  Squeak
//
//  Created by Andrew Forsyth on 11/22/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "TextTicker.h"

TextTicker::TextTicker(GLint _x, GLint _y){
    x = _x;
    y = _y;
}

void TextTicker::RenderString(GLfloat x, GLfloat y, void *font, string str, GLfloat r, GLfloat g, GLfloat b)
{
    const unsigned char *c = (const unsigned char*)str.c_str();
    
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    
    glutBitmapString(font, c);
}

void TextTicker::updateLog(string toAdd){
    log.push_back(toAdd);
}

void TextTicker::reshape(GLint new_x, GLint new_y){
    x = new_x;
    y = new_y;
}

void TextTicker::drawLog(){
    GLfloat draw_x = 0.0;
    GLfloat draw_y = y - 10;
    
    for(int i = log.size()-1; i >= 0; i--){
        std::printf("the x val is: %f and the y val is: %f \n", draw_x, draw_y);
        RenderString(draw_x, draw_y, GLUT_BITMAP_TIMES_ROMAN_24, log[i], 1.0, 1.0, 1.0);
        //RenderString(100.0, 100.0, GLUT_BITMAP_TIMES_ROMAN_24, log[i], 1.0, 1.0, 1.0);
        draw_y -= 24;
    }
}

void TextTicker::clearLog(){
    log.clear();
}
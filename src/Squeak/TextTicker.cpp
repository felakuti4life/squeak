//
//  TextTicker.cpp
//  Squeak
//
//  Created by Andrew Forsyth on 11/22/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "TextTicker.h"

TextTicker::TextTicker(GLint _x, GLint _y, void *_font, GLint rightOff, GLint leftOff, GLint bottomOff){
    x = _x;
    y = _y;
    right_offset = rightOff;
    left_offset = leftOff;
    bottom_offset = bottomOff;
    font = _font;
    fontHeightInPixels = glutBitmapHeight(font);
    numLines = y / fontHeightInPixels;
    color_delta = 1.0/(float)numLines;
}

void TextTicker::RenderString(GLfloat draw_x, GLfloat &draw_y, void *font, string str, GLfloat r, GLfloat g, GLfloat b)
{
    const unsigned char *c = (const unsigned char*)str.c_str();
    ostringstream os;
    glColor3f(r, g, b);
    
    //do we need to leave room for text wrapping?
    if(glutBitmapLength(font, c) > x - (left_offset + right_offset)){
        int timesLonger = int(glutBitmapLength(font, c)/(x- (left_offset + right_offset)));
        draw_y -= fontHeightInPixels * timesLonger;
    }
    //set the position of the start of the string
    glRasterPos2f(draw_x, draw_y);
    float x_pos = 0.0;
    for(int i = 0; i < str.length(); i++){
        x_pos += glutBitmapWidth(font, c[i]);
        os << c[i];
        if(x_pos > x - (left_offset + right_offset)){
            os << '\n';
            x_pos = 0.0;
        }
    }
    
    //finally, print the damn thing
    glutBitmapString(font, (const unsigned char *)os.str().c_str());
}

void TextTicker::updateLog(string toAdd){
    log.push_back(toAdd);
}

void TextTicker::reshape(GLint new_x, GLint new_y, GLint rightOff, GLint leftOff, GLint bottomOff){
    x = new_x;
    y = new_y;
    right_offset = rightOff;
    left_offset = leftOff;
    bottom_offset = bottomOff;
    numLines = y/fontHeightInPixels;
    color_delta = 1.5/(float)numLines;
}

void TextTicker::updateFont(void *_font){
    font = _font;
    fontHeightInPixels = glutBitmapHeight(font);
    numLines = y / fontHeightInPixels;
}

void TextTicker::drawLog(){
    GLfloat draw_x = left_offset;
    GLfloat draw_y = y - bottom_offset;
    GLfloat r = 1.0, g = 1.0, b = 1.0;
    
    for(int i = log.size()-1; i >= 0; i--){
        
        //std::printf("the x val is: %f and the y val is: %f \n", draw_x, draw_y);
        RenderString(draw_x, draw_y, font, log[i], r, g, b);
        //RenderString(100.0, 100.0, GLUT_BITMAP_TIMES_ROMAN_24, log[i], 1.0, 1.0, 1.0);
        draw_y -= fontHeightInPixels * text_spacing;
        r -= color_delta; g -= color_delta; b -= color_delta;
    }
}

void TextTicker::clearLog(){
    log.clear();
}
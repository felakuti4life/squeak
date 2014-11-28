//
//  main.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/19/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//
#include "x-vector3d.h"
#include "RtAudio.h"
#include "chuck_fft.h"
#include "RoomGen.h"
#include "SoundSourceGen.h"
#include "Map.h"

using namespace std;

//#ifdef __MACOSX_CORE__
#include <GLUT/glut.h>
//#include <openglut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#else
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
*/
//#endif



//MARK: MAP AUDIO FILES
string soundFPath1 = "/Users/Ethan/syncbox/SQUEAK/sound/singles/blender.wav";
string soundFPath2 = "/Users/Ethan/syncbox/SQUEAK/sound/singles/squeak_1.wav";
string soundFPath3 = "/Users/Ethan/syncbox/SQUEAK/sound/singles/squeak_2.wav";

string mouseChirpFPath = "/Users/Ethan/syncbox/SQUEAK/sound/singles/squeak_3.wav";

string roomFPath1 = "/Users/Ethan/syncbox/SQUEAK/sound/impulses/chapel.wav";
string roomFPath2 = "/Users/Ethan/syncbox/SQUEAK/sound/impulses/basement.wav";

//MARK: INITIALIZE GENERATORS
//sound sources
SoundSourceGen blender = SoundSourceGen(soundFPath1);
SoundSourceGen squeak1 = SoundSourceGen(soundFPath2);
SoundSourceGen squeak2 = SoundSourceGen(soundFPath3);
SoundSourceGen mouseChirp = SoundSourceGen(mouseChirpFPath);
//vector<AudioGen*> chain1, chain2, chain3;
vector<AudioGen*> chain1 = {&blender};
vector<AudioGen*> chain2 = {&squeak1};


//impulses
RoomGen chapel = RoomGen(roomFPath1, chain1);

RoomGen basement = RoomGen(roomFPath2, chain2);
vector<AudioGen*> chain3 = {&squeak2, &chapel};
RoomGen basement2 = RoomGen(roomFPath2, chain3);

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void initGfx();

void idleFunc();

void displayFunc();

void reshapeFunc(GLsizei width, GLsizei height);

void keyboardFunc(unsigned char, int, int);

void mouseFunc(int button, int state, int x, int y);

// our datetype
void actionMove();

void actionRotate();

void drawCube(float zGeMotion, float zLynchMotion, float yTopMotion, float yBottomMotion);

void draw_fft(GLfloat z, GLfloat zinc, complex *cbuf);

void draw_osc(GLfloat z, GLfloat zinc);

#define SAMPLE float
#define AUDIO_FORMAT RTAUDIO_FLOAT32
#define SAMPLE_RATE 44100
#define NCHANNELS 1
#define PIE 3.14159265358979

enum movement {
    IDLE, FORWARD, BACKWARD, LEFT, RIGHT
};
movement direction = IDLE;
float g_step = 0.05;
float ge_size, lynch_size;
float victory_score = 200;
float victory_threshold = 0.0008;
// width and height
int g_width = 1024;
int g_height = 720;

long g_last_width = g_width;
long g_last_height = g_height;

GLuint ge_texture, lynch_texture;

Vector3D cube_rotation = Vector3D();

void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{
    char *c;
    
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    
    //glutBitmapString(font, string);
}
//called like this:


// window
SAMPLE *g_window = NULL;
unsigned long g_windowSize;


GLboolean g_fullscreen = FALSE;

//fudge factor for accentuating verticality of time signal drawing
int g_vertical_stretch = 3;

// global buffer
SAMPLE *g_buffer = NULL;
long g_bufferSize;

// fft buffer
SAMPLE *g_fftBuf = NULL;
bool g_draw_dB = false;


float score;

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

//-----------------------------------------------------------------------------
// name: usage()
// desc: prints usage for Squeak
//-----------------------------------------------------------------------------
void usage() {
    cout << "Ethan Geller and Andrew Forsyth Presents\n"
    << "---------------------\n\n"
    << "SQUEAK\n"
    << "===========\n\n"
    << "KEY COMMANDS:\n"
    << "-------------\n\n"
    << "q: give UP\n"
    << "w: move FORWARD\n"
    << "s: move BACKWARD\n"
    << "a: move LEFT\n"
    << "d: move RIGHT\n"
    << "space: play TAPE\n"
    << "r: record SOUND\n"
    << endl;
}

//-----------------------------------------------------------------------------
// name: callback()
// desc: audio callback
//-----------------------------------------------------------------------------
double g_t = 0;
int callback(void *outputBuffer, void *inputBuffer, unsigned int numFrames,
             double streamTime, RtAudioStreamStatus status, void *data) {
    cerr << "!";
    // cast!
    SAMPLE *input = (SAMPLE *) inputBuffer;
    SAMPLE *output = (SAMPLE *) outputBuffer;
    chapel.synthesize2(input, output, numFrames);
    //for( int i = 0; i < numFrames; i++ )
    //{
    //    output[i*2] = output[i*2+1] = sin(3.1415*2*440/44100*g_t);
    //    g_t++;
    //}
    //cout << *output << endl;
    return 0;
}


//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main(int argc, char **argv) {
    // instantiate RtAudio object
    RtAudio audio;
    // variables
    unsigned int bufferBytes = 0;
    // frame size
    unsigned int bufferFrames = 1024;
    
    // check for audio devices
    if (audio.getDeviceCount() < 1) {
        // nopes
        cout << "no audio devices found!" << endl;
        exit(1);
    }
    usage();
    // initialize GLUT
    glutInit(&argc, argv);
    // init gfx
    initGfx();
    //MARK: gui tests
    RenderString(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Hello", 1.0f, 0.0f, 0.0f);
    ge_texture = LoadTexture("ge.bmp");
    lynch_texture = LoadTexture("lynch.bmp");
    
    //MARK: map test
    Map theMap = Map();
    
    // let RtAudio print messages to stderr.
    audio.showWarnings(true);
    // set input and output parameters
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = audio.getDefaultInputDevice();
    iParams.nChannels = NCHANNELS;
    iParams.firstChannel = 0;
    oParams.deviceId = audio.getDefaultOutputDevice();
    oParams.nChannels = NCHANNELS;
    oParams.firstChannel = 0;
    
    // create stream options
    RtAudio::StreamOptions options;
    
    // go for it
    try {
        // open a stream
        audio.openStream(&oParams, &iParams, AUDIO_FORMAT, SAMPLE_RATE, &bufferFrames, &callback, (void *) &bufferBytes, &options);
    }
    catch (RtError &e) {
        // error!
        cout << e.getMessage() << endl;
        exit(1);
    }
    
    // compute
    bufferBytes = (unsigned int) (bufferFrames * NCHANNELS * sizeof(SAMPLE));
    // allocate global buffer
    g_bufferSize = bufferFrames;
    g_buffer = new SAMPLE[g_bufferSize];
    g_fftBuf = new SAMPLE[g_bufferSize];
    memset(g_buffer, 0, sizeof(SAMPLE) * g_bufferSize);
    memset(g_fftBuf, 0, sizeof(SAMPLE) * g_bufferSize);
    
    // allocate buffer to hold window
    g_windowSize = bufferFrames;
    g_window = new SAMPLE[g_windowSize];
    // generate the window
    //hanning(g_window, g_windowSize);
    
    // go for it
    try {
        // start stream
        audio.startStream();
        
        // let GLUT handle the current thread from here
        glutMainLoop();
        
        // stop the stream.
        audio.stopStream();
    }
    catch (RtError &e) {
        // print error message
        cout << e.getMessage() << endl;
        goto cleanup;
    }
    
cleanup:
    // close if open
    if (audio.isStreamOpen())
        audio.closeStream();
    
    // done
    return 0;
}


//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void initGfx() {
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // initialize the window size
    glutInitWindowSize(g_width, g_height);
    // set the window postion
    glutInitWindowPosition(100, 100);
    // create the window
    glutCreateWindow("Sound Peaks");
    glShadeModel(GL_SMOOTH);
    
    // set the idle function - called when idle
    glutIdleFunc(idleFunc);
    // set the display function - called when redrawing
    glutDisplayFunc(displayFunc);
    // set the reshape function - called when client area changes
    glutReshapeFunc(reshapeFunc);
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc(keyboardFunc);
    // set the mouse function - called on mouse stuff
    glutMouseFunc(mouseFunc);
    
    // set clear color
    glClearColor(1, 0.1, 0.1, 1);
    // enable color material
    glEnable(GL_COLOR_MATERIAL);
    // enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
}


//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc(GLsizei w, GLsizei h) {
    // save the new window size
    g_width = w;
    g_height = h;
    // map the view port to the client area
    glViewport(0, 0, w, h);
    // set the matrix mode to project
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix
    glLoadIdentity();
    // create the viewing frustum
    gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0);
    // set the matrix mode to modelview
    glMatrixMode(GL_MODELVIEW);
    // load the identity matrix
    glLoadIdentity();
    // position the view point
    gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}


//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'Q':
            exit(1);
            break;
        case 'q':
            exit(1);
            break;
            
        case 'w':
            direction = FORWARD;
            break;
            
        case 's':
            direction = BACKWARD;
            break;
            
        case 'a':
            direction = LEFT;
            break;
            
        case 'd':
            direction = RIGHT;
            break;
            
        case 'p':
            ge_size += 1.0f;
            break;
            
        case 'l':
            ge_size -= 1.0f;
            break;
            
        case 'o':
            lynch_size += 1.0f;
            break;
            
        case 'k':
            lynch_size -= 1.0f;
            break;
            
        case 'i':
            g_vertical_stretch += 2.0f;
            break;
            
        case 'j':
            g_vertical_stretch -= 2.0f;
            break;
            
        case 'f': {
            // check fullscreen
            if (!g_fullscreen) {
                g_last_width = g_width;
                g_last_height = g_height;
                glutFullScreen();
            }
            else
                glutReshapeWindow(g_last_width, g_last_height);
            
            // toggle variable value
            g_fullscreen = !g_fullscreen;
        }
            break;
        default:
            break;
    }
    
    glutPostRedisplay();
}


//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        // when left mouse button is down
        if (state == GLUT_DOWN) {
            cube_rotation.y = y / 60 - g_height / 60;
            cout << cube_rotation.x << "y: " << cube_rotation.y;
        }
        else {
            cube_rotation.x = 0.0f;
            cube_rotation.y = 0.0f;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        // when right mouse button down
        if (state == GLUT_DOWN) {
            
        }
        else {
        }
    }
    else {
    }
    
    glutPostRedisplay();
}


//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc() {
    // render the scene
    glutPostRedisplay();
}


//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc() {
    if (g_vertical_stretch < 0) g_vertical_stretch = 0;
    else if (g_vertical_stretch > 20) g_vertical_stretch = 20;
    
    
    // local state
    static GLfloat zrot = 0.0f, c = 0.0f;
    
    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    actionRotate();
    actionMove();
    
    //glRotatef(45,1.0f,0.0f,0.0f);
    //glRotatef(45,0.0f,1.0f,0.0f);
    // line width
    glLineWidth(4.0);
    // define a starting point
    GLfloat z = -5;
    // compute increment
    GLfloat zinc = ::fabs(z * 2 / g_bufferSize);
    
    // color
    glColor3f(.5, .5, 1);
    // apply window to buf
    apply_window(g_buffer, g_window, g_windowSize);
    draw_osc(z, zinc);
    
    // copy into the fft buf
    memcpy(g_fftBuf, g_buffer, sizeof(SAMPLE) * g_bufferSize);
    
    // take forward FFT (time domain signal -> frequency domain signal)
    rfft(g_fftBuf, g_windowSize / 2, FFT_FORWARD);
    // cast the result to a buffer of complex values (re,im)
    complex *cbuf = (complex *) g_fftBuf;
    
    // define a starting point
    z = -5;
    // compute increment
    zinc = (GLfloat) ::fabs(z * 2 / (g_windowSize / 2));
    float ge_agitator = cmp_abs(cbuf[24]);
    float zGeMotion = 1.0f * ge_agitator * g_vertical_stretch * 360.0f;
    float lynch_agitator = cmp_abs(cbuf[32]);
    float zLynchMotion = 1.0f * lynch_agitator * g_vertical_stretch * 360.0f;
    float top_agitator = cmp_abs(cbuf[16]);
    float yTopMotion = 1.0f * top_agitator * g_vertical_stretch * 360.0f;
    float bottom_agitator = cmp_abs(cbuf[8]);
    float yBottomMotion = 1.0f * bottom_agitator * g_vertical_stretch * 360.0f;
    
    if (ge_agitator > victory_threshold &&
        lynch_agitator > victory_threshold &&
        top_agitator > victory_threshold &&
        bottom_agitator > victory_threshold)
        score += 10;
    else if (score > 0) score -= 1;
    // save transformation state
    draw_fft(z, zinc, cbuf);
    drawCube(zGeMotion, zLynchMotion, yTopMotion, yBottomMotion);
    glClearColor(1.0f - (score / victory_score), 0.1, 0.1, 1);
    glFlush();
    
    glutSwapBuffers();
}

void draw_osc(GLfloat z, GLfloat zinc) {
    float r = 3.0f;
    glPushMatrix();
    // translate
    glTranslatef(0, 2, 0);
    // start primitive
    glBegin(GL_LINE_STRIP);
    // loop over buffer
    for (int i = 0; i < g_bufferSize; i++) {
        // plot
        glVertex3f((GLfloat) ((r * cos(i)) + 0), (GLfloat) ((r * sin(i)) + 3 * g_buffer[i] - 2), z);
        // increment z
        z += zinc;
    }
    // end primitive
    glEnd();
    // pop
    glPopMatrix();
}

void draw_fft(GLfloat z, GLfloat zinc, complex *cbuf) {
    z = -2;
    float gradient = score / victory_score;
    glPushMatrix();
    // translate
    glTranslatef(0, -2, 0);
    // color
    glColor3f(0, 1 - gradient, gradient);
    // start primitive
    glBegin(GL_LINE_STRIP);
    // loop over buffer to draw spectrum
    for (int i = 0; i < g_windowSize; i++) {
        // plot the magnitude,
        // with scaling, and also "compression" via pow(...)
        glVertex3f(2 * sin(z), (GLfloat) (2 * cos(z) + (1.5 + (10 * pow(cmp_abs(cbuf[i]), .5)))), z * 3);
        // increment x
        z += zinc;
    }
    // end primitive
    glEnd();
    // restore transformations
    glPopMatrix();
}

void drawCube(float zGeMotion, float zLynchMotion, float yTopMotion, float yBottomMotion) {
    
    glBindTexture(GL_TEXTURE_2D, ge_texture);
    glBegin(GL_QUADS);
    
    // Front Face (ge face)
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f - ge_size, -1.0f - ge_size, 1.0f - zGeMotion);
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f + ge_size, -1.0f - ge_size, 1.0f - zGeMotion);
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f + ge_size, 1.0f + ge_size, 1.0f - zGeMotion);
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f - ge_size, 1.0f + ge_size, 1.0f - zGeMotion);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, lynch_texture);
    glBegin(GL_QUADS);
    
    // Back Face (lynch face)
    // Bottom Right
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f - lynch_size, -1.0f - lynch_size, -1.0f + zLynchMotion);
    // Top Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f - lynch_size, 1.0f + lynch_size, -1.0f + zLynchMotion);
    // Top Left
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f + lynch_size, 1.0f + lynch_size, -1.0f + zLynchMotion);
    // Bottom Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f + lynch_size, -1.0f - lynch_size, -1.0f + zLynchMotion);
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

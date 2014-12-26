//
//  main.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/19/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//


//#include <GL/freeglut.h>

#include "x-vector3d.h"
#include "RtAudio.h"
#include "chuck_fft.h"
#include "RoomGen.h"
#include "SoundSourceGen.h"
#include "Tape.h"
#include "EnGen.h"
#include "TextTicker.h"
#include "GraphicsTools.h"
#include <GL/freeglut.h>
using namespace std;
//#ifdef __MACOSX_CORE__
//#include <GLUT/glut.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#else
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
*/
//#endif
//MARK: drug types
enum Drug  {SOBER, MOUSE_TRANQUILIZER, MOUSE_ENERGIZER, MOUSE_INTELLIGIZER};
int nDrugs = 4;
int drugTicker = 0;
int turnTicker = 0;
int chapterTicker = 0;
Drug currentDrug = SOBER;
Drug drugOnHand = SOBER;
bool onDrugs = false;
//MARK: background music
BackgroundGen introMusic = BackgroundGen("sound/ambient/theme.wav");
SoundSourceGen doorSound = SoundSourceGen("sound/singles/door_transition.wav");

//dry sources
SoundSourceGen dryDoorOpen = SoundSourceGen("sound/singles/door_open.wav");
SoundSourceGen dryDoorClose = SoundSourceGen("sound/singles/door_close.wav");


SoundSourceGen DoorClose = SoundSourceGen("sound/singles/door_open.wav");

bool shouldContinue = false;

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
#define NCHANNELS 2
#define PIE 3.14159265358979



// width and height
GLint g_width = 1024;
GLint g_height = 720;

GLint g_last_width = g_width;
GLint g_last_height = g_height;

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


EnGen engine;
TextTicker* g_log;
Room* startingRoom;
Room currentRoom;
string currentRoomName = "Ch. 0: Beginnings";
int nObjectsInCurrentRoom;
vector<Thing> thingsInCurrentRoom;
float* currentImpulse;
float currentImpulseSize;

int nNeighbors;
vector<Room*> neighbors;

//amount of recordings player has taken thus far
int nRecordings;
vector<Room*> recordingRooms;
vector<int> recordingThingIdx;

int stepsInTape;
vector<int> tapeObjectsIdx;
vector<Room> tapeRooms;
SoundSourceGen tapeAudio;

bool gameStarted = false;
float score;


//MARK: text ticker offsets:
int lOffset = 20;
int rOffset = 100;
int botOffset = 100;

GLfloat roomNameXPos = 0;
GLfloat roomNameYPos = 16;

//-----------------------------------------------------------------------------
// name: usage()
// desc: prints usage for Squeak
//-----------------------------------------------------------------------------
void usage() {
    cout << "Ethan Geller and Andrew Forsyth Present\n"
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


    SAMPLE *input = (SAMPLE *) inputBuffer;
    SAMPLE *output = (SAMPLE *) outputBuffer;
    engine.synthesize2(input, output, numFrames);
//    for (float f = 0; f < numFrames*2; f++) {
//        output[(int) f] = sin(440*2*PIE*(g_t/SAMPLE_RATE));
//        g_t++;
//    }
    memcpy(g_buffer, output, numFrames*2);
    return 0;
}

void setUpDoorSounds(){
    int doorClosetSize;
    RoomGen r = currentRoom.getAudio();
    float* doorCloseSound = r.getSoundInRoom(dryDoorClose.getSound(), dryDoorClose.getSize(), &doorClosetSize);
    DoorClose.setSound(doorCloseSound);
    DoorClose.setSize(doorClosetSize/2);
}

//MARK: Drug stuff
void setDrug(){
    if(currentDrug == MOUSE_ENERGIZER){
        engine.setSoundPlayback(1.4);
        engine.setAmbPlayback(0.6);
        g_log->updateLog("Squeak suddenly feel much more energized.");
        g_log->updateLog("\'YEAH BABY!\' Squeak yells, while  galloping around the room in concentric circles. \'I want to put the WORLD in my pocket!!\'");
    }
    else if (currentDrug == MOUSE_TRANQUILIZER){
        engine.setSoundPlayback(0.6);
        engine.setAmbPlayback(1.4);
        g_log->updateLog("Squeak suddenly feel much more relaxed.");
        g_log->updateLog("...Too relaxed, in fact. \'I haven\'t felt this weird since I tried to watch Tusk.\'");
    }
    else if (currentDrug == MOUSE_INTELLIGIZER){
        g_log->updateFont(GLUT_BITMAP_TIMES_ROMAN_24);
        g_log->updateLog("Squeak suddenly feels much smarter. \'The area of a circle is wherever I roam!\' Squeak cries to the heavens. \'Truly this is the year of the Rat.\'");
    }
    else if(currentDrug == SOBER){
        g_log->updateFont(GLUT_BITMAP_9_BY_15);
        engine.setSoundPlayback(1.0);
        engine.setAmbPlayback(1.0);
        g_log->updateLog("Squeak feels himself sobering up. \'Just when I was beginning to feel some semblence of peace,\' Squeak mutters. He fixes himself an old fashioned to reestablish his disgruntled aesthetic.");
        onDrugs = false;
    }
}

void showDrugs(){
    if(onDrugs && drugTicker == 0){
        currentDrug = SOBER;
        setDrug();
    }
    if(drugOnHand != SOBER){
        g_log->updateLog("You can always press [m] to take those meds you found.");
    }
    if(turnTicker == 4){
        drugOnHand = MOUSE_TRANQUILIZER;
        g_log->updateLog("Squeak finds a box of flakes labeled \'MOUSE TRANQUILIZER\'. \'This would probably only hurt my investigation,\' Squeak says to himself. \'I see absolutely no reason to take these.\'");
        g_log->updateLog("press [m] to take the medication.");
    }
    
    else if(turnTicker == 10){
        drugOnHand = MOUSE_INTELLIGIZER;
        g_log->updateLog("Squeak finds a box of flakes labeled \'MOUSE INTELLIGIZER\'. \'Man, I really don\'t want to try one of these again\' Squeak says to himself. \'They really tend to make my job nearly impossible.\'");
        g_log->updateLog("press [m] to take the medication.");
    }
    
    else if(turnTicker == 16){
        drugOnHand = MOUSE_ENERGIZER;
        g_log->updateLog("Squeak finds a box of flakes labeled \'MOUSE ENERGIZER\'. \'I hate these things!\' Squeak says to himself. \'I think I would rather play change a lightbulb in the Hagia Sofia.\'");
        g_log->updateLog("press [m] to take the medication.");
    }
}

//MARK: Room Stuff
void goToRoom(Room* r){
    engine.stopPlayingAmbience();
    chapterTicker++;
    string chapterNum = to_string(chapterTicker);
    currentRoom = *r;
    thingsInCurrentRoom = currentRoom.getThingsInRoom();
    currentRoomName = "Chapter " + chapterNum + ": The " + currentRoom.getName();
    nObjectsInCurrentRoom = thingsInCurrentRoom.size();
    neighbors = currentRoom.getNeighbors();
    nNeighbors = neighbors.size();
    RoomGen rGen = currentRoom.getAudio();
    currentImpulse = rGen.getImpulse();
    currentImpulseSize = rGen.getSize();
    //set up ambience!
    if(currentRoom.getHasAmbience()){
        engine.setCurrentAmbience(currentRoom.getAmbience());
    }
    
    //g_buffer = currentRoom.getAudio().getSound();
    //g_bufferSize = currentRoom.getAudio().getSize();
}

void neighborOptions(){
    int ascii_one = 49;
    for(int i = 0; i < nNeighbors; i++){
        Room r = *neighbors[i];
        char c = ascii_one + i;
        string key;
        key = c;
        string str = "[" + key + "]: The " + r.getName();
        g_log->updateLog(str);
    }
}
vector<int> ascii_qrow = {113, 119, 101, 114, 116, 121};
vector<int> ascii_arow = {97, 115, 100, 102, 103, 104};
void inspectOptions(){
    for(int i = 0; i < nObjectsInCurrentRoom; i++){
        Thing t = thingsInCurrentRoom[i];
        char c = ascii_qrow[i];
        string inspect_key;
        inspect_key = c;
        c = ascii_arow[i];
        string rec_key;
        rec_key = c;
        string str = "[" + inspect_key + "] to inspect, [" + rec_key +"] to record: The " + t.getName();
        g_log->updateLog(str);
    }
}

void giveOptions(){
    g_log->updateLog("Looking around, you see that there are a few rooms you can drunkenly stumble over to:");
    neighborOptions();
    g_log->updateLog("You use all of your energy dragging your glazed eyes across the myriad of objects Crumbs could have fucked with:");
    inspectOptions();
}

void roomEntrance(){
    g_log->updateLog("");
    g_log->updateLog(currentRoom.getName());
    g_log->updateLog("--------------------");
    
    //engine.playSound(dryDoorOpen);
    setUpDoorSounds();
    engine.playSound(DoorClose);
    g_log->updateLog("");
    g_log->updateLog(currentRoom.getDescription());
    g_log->updateLog("The way you see it, there are a few options:");
    giveOptions();
    turnTicker++;
    if(drugTicker) drugTicker--;
    showDrugs();
}

void inspectThing(Thing t){
    g_log->updateLog(t.getDescription());
    engine.playSound(t.getAudio());
    turnTicker++;
    if(drugTicker) drugTicker--;
    showDrugs();
}


bool promptForNewGame = false;
void endGame(){
    bool hasWon = true;
    g_log->updateLog("This should do it. Now to retrace my steps...");
    for (int i = 0; i < nRecordings; i++) {
        cerr << "allocating rooms" << endl;
        Room recR = *recordingRooms[i];
        Room tapeR = tapeRooms[i];
        //it's correct
        cerr << "comparing rooms" << endl;
        if(recR.getName() == tapeR.getName()){
            cerr << "allocating tapes" << endl;
            g_log->updateLog("Crumbs was in " + recR.getName() + "!");
            Thing recT = tapeR.getThingsInRoom()[recordingThingIdx[i]];
            Thing tapeT = tapeR.getThingsInRoom()[tapeObjectsIdx[i]];
            //was the right thing
            if (recordingThingIdx[i] == tapeObjectsIdx[i]) {
                g_log->updateLog("and it was definitely the " + tapeT.getName() + " in the tape...");
            }
            else {
                g_log->updateLog("But it couldn't have been the " + recT.getName() + "... it sounds almost more like the " + tapeT.getName() + "... still, at least we're in the right place.");
            }
        }
        else {
            g_log->updateLog("This is definitely not the " + recR.getName() + " on the tape... it sounds more like the " + tapeR.getName() + "... I have lost crumbs forever.");
            hasWon = false;
            break;
        }
    }
    if(!hasWon) g_log->updateLog("GAME OVER :(");
    else g_log->updateLog("Crumbs was definitely- he\'s here! GAME OVER");
    promptForNewGame = true;
}

void recordThing(int idx){
    Thing t = thingsInCurrentRoom[idx];
    g_log->updateLog("\'This is that thing from the tape,\' You sigh. It must be. Crumbs must have been here.");
    g_log->updateLog("Gingerly, you record the " + t.getName());
    recordingRooms.push_back(&currentRoom);
    recordingThingIdx.push_back(idx);
    nRecordings++;
    engine.playSound(t.getAudio());
    if(nRecordings == stepsInTape) endGame();
    string left = to_string(stepsInTape - nRecordings);
    g_log->updateLog("The way I see it, there are about " + left + " sounds left to identify in this tape.");
    turnTicker++;
    if(drugTicker) drugTicker--;
    showDrugs();
    g_log->updateLog("What now?");
    
}

void showKeyboardShortcuts(){
    g_log->updateLog("You can always press [p] to play Crumb's last dance, or [l] to show current commands.");
}
//-----------------------------------------------------------------------------
// name: main()
// desc: entry point
//-----------------------------------------------------------------------------
int main(int argc, char **argv) {
    doorSound.setSize(doorSound.getSize()/2);

    // instantiate RtAudio object
    RtAudio audio;
    // variables
    unsigned int bufferBytes = 0;
    // frame size
    unsigned int bufferFrames = 512;
    
    // check for audio devices
    if (audio.getDeviceCount() < 1) {
        // nopes
        cout << "no audio devices found!" << endl;
        exit(1);
    }
    //usage();
    
    //MARK: map test
    Map theMap = Map();
    startingRoom = theMap.getRoomAddress(0, 0);
    Tape theTape = Tape(theMap);
    stepsInTape = theTape.getSteps();
    tapeObjectsIdx = theTape.getObjectIndices();
    tapeRooms = theTape.getRooms();
    tapeAudio = theTape.getAudio();
    // initialize GLUT
    glutInit(&argc, argv);
    
    // init gfx
    initGfx();
    
    //initialize the log
    g_log = new TextTicker(g_width, g_height, GLUT_BITMAP_9_BY_15, rOffset, lOffset, botOffset);
    //set up background music
    engine.setCurrentAmbience(introMusic);
    
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
    hanning(g_window, g_windowSize);
    
    
    g_log->updateLog("SQUEAK");
    g_log->updateLog("-----");
    g_log->updateLog("A game by Andrew Forsyth and Ethan Geller");
    g_log->updateLog("------------------------------------------");
    g_log->drawLog();
    g_log->updateLog("");
    g_log->updateLog("");
    g_log->updateLog("Detective Squeak was making his particularly generous version of Irish coffee when Pvt. Muffins decided to make his presence known.\'We\'ve got another one for you, Squeak. Hitting the bottle so soon?\'");
    g_log->updateLog("\'I am only a mouse, Muffs. I don\'t pretend to be anything else.\'");
    g_log->updateLog("\'Well you\'re the best goddamn mouse detective this side of the Pacific, and trust me when I say that your crusty whiskers would be the last thing I wanted to see right now if we did not need your help again.\'");
    g_log->updateLog("\'I told you I\'m out, Muffs. I can\'t let myself get roped up in all this again. The cheese is\'nt worth it.\'");
    g_log->updateLog("\'I know Squeaks, but this time... They got Crumbs, Squeaks.\'");
    
    g_log->updateLog("The name Crumbs had awoken something deep inside Squeaks, something that Squeaks assumed had left him along with his ex wife back in 1987. Crumbs, the informant from heaven. Crumbs, who could set up a hammock between a cats ears and sleep in it. Maybe he was brave, maybe he was stupid. But he finally got caught now, and Squeaks knew that he owed Crumbs one final rescue.");
    
    showKeyboardShortcuts();
    g_log->updateLog("press [space] to start");
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
    glutCreateWindow("SQUEAK");
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
    glClearColor(0.0, 0.0, 0.0, 1);
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
    
    //update the log on the height and width of our window
    g_log->reshape(g_width, g_height, rOffset, lOffset, botOffset);
    // set the matrix mode to project
    glMatrixMode(GL_PROJECTION);
    // load the identity matrix
    glLoadIdentity();
    // create the viewing frustum
    gluOrtho2D(0, w, h, 0);
    // set the matrix mode to modelview
    glMatrixMode(GL_MODELVIEW);
    // load the identity matrix
    glLoadIdentity();
    // map the view port to the client area
    glViewport(0, 0, w, h);
    // position the view point
    //gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void playTape(){
    engine.playSound(tapeAudio);
    g_log->updateLog("You play the tape back. \'What have you gotten yourself into this time, Crumbs...\' you mutter. You feel a strange mixture of genuine worry and macabre curiosity.");
}

void neighborKeyPress(int key){
    if (nNeighbors > key-1) {
        goToRoom(neighbors[key-1]);
        roomEntrance();
    }
    else g_log->updateLog("You tried to enter something you obviously cannot reach");
}

void inspectKeyPress(int key){
    if (nObjectsInCurrentRoom > key-1) {
        Thing t = currentRoom.getThingsInRoom()[key-1];
        inspectThing(t);
    }
    else g_log->updateLog("You tried to grasp for something you obviously cannot reach");
}

void recordKeyPress(int key){
    if (nObjectsInCurrentRoom > key-1) {
        recordThing(key-1);
    }
    else g_log->updateLog("You tried to record something you obviously cannot reach");
}

void drugAction(){
    if(drugOnHand == SOBER){
        g_log->updateLog("Alas, Squeak\'s metaphorical pockets are empty.");
    }
    else{
        currentDrug = drugOnHand;
        onDrugs = true;
        setDrug();
        drugTicker = 4;
        drugOnHand = SOBER;
    }
}

//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            if(!gameStarted){
                goToRoom(startingRoom);
                engine.stopPlayingAmbience();
                roomEntrance();
                gameStarted = true;
                engine.setCurrentAmbience(currentRoom.getAmbience());
            }
            else if (promptForNewGame){
                cout << "BETA SORRY BYE" << endl;
            }
            break;
        
            //room neighbor triggers
        case '1':
            neighborKeyPress(1);
            break;
        case '2':
            neighborKeyPress(2);
            break;
        case '3':
            neighborKeyPress(3);
            break;
        case '4':
            neighborKeyPress(4);
            break;
            
            //inspect triggers
        case 'q':
            inspectKeyPress(1);
            break;
        case 'w':
            inspectKeyPress(2);
            break;
        case 'e':
            inspectKeyPress(3);
            break;
        case 'r':
            inspectKeyPress(4);
            break;
        case 't':
            inspectKeyPress(5);
            break;
        case 'y':
            inspectKeyPress(6);
            break;
            //record things
        case 'a':
            recordKeyPress(1);
            break;
        case 's':
            recordKeyPress(2);
            break;
        case 'd':
            recordKeyPress(3);
            break;
        case 'f':
            recordKeyPress(4);
            break;
        case 'g':
            recordKeyPress(5);
            break;
        case 'h':
            recordKeyPress(6);
            break;
            //utilities
        case 'l':
            showKeyboardShortcuts();
            giveOptions();
            break;
        case 'p':
            playTape();
            break;
        case 'm':
            drugAction();
            break;
//        case 'z':
//            exit(1);
//        case 'x': {
//            // check fullscreen
//            if (!g_fullscreen) {
//                g_last_width = g_width;
//                g_last_height = g_height;
//                glutFullScreen();
//            }
//            else
//                glutReshapeWindow(g_last_width, g_last_height);
//            
//            // toggle variable value
//            g_fullscreen = !g_fullscreen;
//        }
//            break;
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

    //nothing here for now
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
    
    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //draw the log
    g_log->drawLog();
    g_log->RenderString(roomNameXPos, roomNameYPos, GLUT_BITMAP_HELVETICA_18, currentRoomName, 0.9, 0.9, 0.9);
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
    draw_osc(&z, &zinc, g_bufferSize, g_windowSize, g_buffer, g_height);
    // copy into the fft buf
    memcpy(g_fftBuf, g_buffer, sizeof(SAMPLE) * g_bufferSize);
    
    // take forward FFT (time domain signal -> frequency domain signal)
    rfft(g_fftBuf, g_windowSize / 2, FFT_FORWARD);
    // cast the result to a buffer of complex values (re,im)
    complex *cbuf = (complex *) g_fftBuf;
    //if (gameStarted) draw_osc(z, zinc, g_bufferSize, g_windowSize, g_buffer);
    draw_fft(&z, &zinc, cbuf, g_windowSize);
    
    //current impulse
    glPushMatrix();
    glTranslatef(0, 2, 0);
    glBegin(GL_LINE_STRIP);
    for(float i = 0; i < currentImpulseSize; i++){
        glColor3f((i/currentImpulseSize) * 0.5, 0, (1- i/currentImpulseSize)*0.5);
        
        glVertex2f((GLfloat) (g_width-10) + 40 * currentImpulse[(int)i],(GLfloat) (i/currentImpulseSize)*g_height);
        // increment z
        z += zinc;
    }
    // end primitive
    glEnd();
    // pop
    glPopMatrix();
    
    glRasterPos2d(1, 1);
    //glutBitmapString(, currentRoom.getName());
    glClearColor(0.0f, 0.1, 0.1, 1);
    glFlush();
    
    glutSwapBuffers();
}




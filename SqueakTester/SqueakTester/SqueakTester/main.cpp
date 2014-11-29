//
//  main.cpp
//  SqueakTester
//
//  Created by Ethan Geller on 11/17/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include "RtAudio.h"
#include "RtError.h"
#include "RoomGen.h"
#include "SoundSourceGen.h"
using namespace std;

#define NCHANNELS 2
#define FORMAT RTAUDIO_FLOAT32
#define SAMPLE_RATE 44100



//MARK: MAP AUDIO FILES
//TODO: set up relative pathing
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
vector<AudioGen*> chain1 = {&blender};
vector<AudioGen*> chain2 = {&squeak1};

//impulses
RoomGen chapel = RoomGen(roomFPath1, chain1);
RoomGen basement = RoomGen(roomFPath2, chain2);
vector<AudioGen*> chain3 = {&squeak2, &chapel};
RoomGen basement2 = RoomGen(roomFPath2, chain3);


double g_t = 0;
//MARK: callback
int callback(void *outputBuffer, void *inputBuffer, unsigned int numFrames,
             double streamTime, RtAudioStreamStatus status, void *data) {
    cerr << "!";
    // cast!
    SAMPLE *input = (SAMPLE *) inputBuffer;
    SAMPLE *output = (SAMPLE *) outputBuffer;
    blender.synthesize2(input, output, numFrames);
    return 0;
}

int main(int argc, const char * argv[]) {
    RtAudio adac;
    unsigned int bufferBytes;
    unsigned int bufferFrames = 512;
    
    //get mouse chirp, convolve it through the chapel, and feed it back into the mouse chirp
    SAMPLE* s = blender.getSound();
    int sSize = blender.getSize();
    int tSize;
    SAMPLE* convolvedSound = chapel.getSoundInRoom(s, sSize, &tSize);
    cout << "tSize: " << tSize << endl;
    blender.setSize(tSize);
    blender.setSound(convolvedSound);
    blender.setPlaybackRate(1.0);
    mouseChirp.setPlaybackRate(0.5);
    cout << "blender Size:" << blender.getSize() << endl;
    chapel.prepareConvolvedAudio();
    //MARK: SET UP RTAUDIO
    
    if(adac.getDeviceCount() < 1){
        cout << "No audio devices found!" << endl;
        return 1;
    }
    
    adac.showWarnings(true);
    RtAudio::StreamParameters inputParameters, outputParameters;
    inputParameters.deviceId = adac.getDefaultInputDevice();
    inputParameters.nChannels = NCHANNELS;
    inputParameters.firstChannel = 0;
    outputParameters.deviceId = adac.getDefaultOutputDevice();
    outputParameters.nChannels = NCHANNELS;
    outputParameters.firstChannel = 0;
    
    RtAudio::StreamOptions options;
    
    try {
        adac.openStream(&outputParameters, &inputParameters, FORMAT, SAMPLE_RATE, &bufferFrames, &callback, (void *) &bufferBytes, &options);
    }
    catch (RtError& e) {
        cerr << e.getMessage() << endl;
        exit(1);
    }
    
    bufferBytes = bufferFrames * NCHANNELS * sizeof(SAMPLE);
    
    cout << "stream latency: " << adac.getStreamLatency() << " frames" << endl;
    
    try {
        adac.startStream();
        // get input
        char input;
        std::cout << "running... press <enter> to quit (buffer frames: " << bufferFrames << ")" << endl;
        std::cin.get(input);
        
        // stop the stream.
        adac.stopStream();
    }
    catch (RtError& e ) {
        cerr << e.getMessage() << endl;
        goto cleanup;
    }
    
    
    
cleanup:
    if(adac.isStreamOpen())
        adac.closeStream();
    
    return 0;
}

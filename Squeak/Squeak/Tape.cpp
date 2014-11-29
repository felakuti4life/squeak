//
//  Tape.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/28/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Tape.h"

//MARK: some settings
int xStart = 0;
int yStart = 0;
string tapeName = "Crumb\'s\\ last\\ dance";

Tape::Tape(Map map){
    //TODO: tweak path length for difficulty
    int pathLength = map.getWidth();
    //get our starting room
    Room* rAddr = map.getRoomAddress(xStart, yStart);
    for (int i = 0; i < pathLength; i++){
        //FIXME: address to room in map
        rooms.push_back(rAddr);
        //find a random thing in this room
        Room r = *rAddr;
        int thingIdx = rand() % r.getThingsInRoom().size();
        objectIndices.push_back(thingIdx);
        //go to a neighbor
        int neighIdx = rand() % r.getNeighbors().size();
        rAddr = r.getNeighbors()[neighIdx];
    }
    
    writeAudio();
}

void Tape::writeAudio(){
    //TODO: generate the tape audio, write it to a file
    //get total size of tape float*
    int totalSize;
    vector<float*> sounds;
    vector<int> sizes;
    for (int i = 0; i <  rooms.size(); i++) {
        Room r = *rooms[i];
        Thing t = r.getThingsInRoom()[objectIndices[i]];
        SoundSourceGen aud = t.getAudio();
        float* snd = aud.getSound();
        //FIXME: here is where we are going to convolute it
        int sz = aud.getSize();
        sizes.push_back(sz);
        sounds.push_back(snd);
        totalSize += sz;
    }
    //Great! let's copy it over
    SAMPLE* tapeAudio = new SAMPLE[totalSize];
    int counter;
    for (int i = 0; i < sounds.size(); i++) {
        for(int k = 0;k < sizes[i]; k++){
            tapeAudio[counter] = sounds[i][k];
            counter++;
        }
    }
    handler.writeFile(tapeName, tapeAudio, totalSize, 44100);
    audio = SoundSourceGen(tapeName);
}
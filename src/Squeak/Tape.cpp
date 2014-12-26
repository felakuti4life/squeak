//
//  Tape.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/28/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Tape.h"
#include <random>
using namespace std;

bool tapeDebug = false;
//MARK: some settings
int xStart = 0;
int yStart = 0;
string tapeName = "/Users/Ethan/CrumbsLastDance.wav";


Tape::Tape(Map map){
    //TODO: tweak path length for difficulty
    int pathLength = map.getWidth();
    //get our starting room
    Room* rAddr = map.getRoomAddress(xStart, yStart);
    Room* prevAddr = nullptr;
    for (int i = 0; i < pathLength; i++){
        //FIXME: address to room in map
        
        //find a random thing in this room
        Room r = *rAddr;
        rooms.push_back(r);
        int thingIdx = rand() % r.getThingsInRoom().size();
        objectIndices.push_back(thingIdx);
        //go to a neighbor
        int neighIdx;
        Room* newAddr;
        neighIdx = rand() % r.getNeighbors().size();
        newAddr = r.getNeighbors()[neighIdx];
        if (r.getNeighbors().size() > 1) {
            while (newAddr == prevAddr) {
                cout << "loop" << endl;
                neighIdx = rand() % r.getNeighbors().size();
                newAddr = r.getNeighbors()[neighIdx];
            }
        }
        prevAddr = rAddr;
        rAddr = newAddr;
    }
    
    writeAudio();
    if(tapeDebug) printTapeInfo();
}

int Tape::getSteps(){return rooms.size();}
vector<int> Tape::getObjectIndices(){return objectIndices;}
vector<Room> Tape::getRooms(){return rooms;}

void Tape::writeAudio(){
    //TODO: generate the tape audio, write it to a file
    //get total size of tape float*
    int totalSize = 0;
    vector<float*> sounds;
    vector<int> sizes;
    for (int i = 0; i <  rooms.size(); i++) {
        Room r = rooms[i];
        Thing t = r.getThingsInRoom()[objectIndices[i]];
        SoundSourceGen aud = t.getAudio();
        float* snd = aud.getSound();
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
    //make the file
    handler.writeFile(tapeName, tapeAudio, totalSize, 44100);
    //make a sound source gen from that file
    audio = SoundSourceGen(tapeName);
    audio.setSize(audio.getSize()/4);
}

SoundSourceGen Tape::getAudio(){return audio;}

void Tape::printTapeInfo(){
    for (int i = 0; i < rooms.size(); i++) {
        Room r = rooms[i];
        int idx = objectIndices[i];
        Thing t = r.getThingsInRoom()[idx];
        cout << i+1 << ": Crumbs hit the " << t.getName() << " in the " << r.getName() << endl;
    }
}
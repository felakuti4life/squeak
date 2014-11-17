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
#include "RtAudio.h"
#include "RtError.h"
#include "RoomGen.h"
#include "SoundSourceGen.h"
using namespace std;

int main(int argc, const char * argv[]) {
    //MARK: MAP AUDIO FILES
    string soundFPath1 = "";
    string soundFPath2 = "";
    string soundFPath3 = "";
    
    string roomFPath1 = "";
    string roomFPath2 = "";
    string roomFPath3 = "";
    
    //MARK: INITIALIZE GENERATORS
    //sound sources
    SoundSourceGen source1 = SoundSourceGen(soundFPath1);
    SoundSourceGen source2 = SoundSourceGen(soundFPath2);
    SoundSourceGen source3 = SoundSourceGen(soundFPath3);
    vector<AudioGen*> chain1, chain2, chain3;
    chain1 = {&source1};
    chain2 = {&source2};
    chain3 = {&source3};
    
    //impulses
    RoomGen room1 = RoomGen(roomFPath1, chain1);
    RoomGen room2 = RoomGen(roomFPath2, chain2);
    RoomGen room3 = RoomGen(roomFPath3, chain3);
    
    return 0;
}

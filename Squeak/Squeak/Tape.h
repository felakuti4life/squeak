//
//  Tape.h
//  Squeak
//
//  Created by Ethan Geller on 11/28/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef __Squeak__Tape__
#define __Squeak__Tape__

#include <stdio.h>
#include "Map.h"
#include "SoundSourceGen.h"
#include "AudioFileHandler.h"

class Tape {
    vector<int> objectIndices;
    vector<Room *> rooms;
    
    SoundSourceGen audio;
    AudioFileHandler handler;
    
public:
    Tape(Map map);
    void writeAudio();
};

#endif /* defined(__Squeak__Tape__) */

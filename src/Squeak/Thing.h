//
//  Thing.h
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef __Squeak__Thing__
#define __Squeak__Thing__

#include <stdio.h>
#include "SoundSourceGen.h"
#include <string>

class Thing {
    string name;
    string description;
    SoundSourceGen audio;
    
public:
    //MARK: INITIALIZERS
    Thing();
    Thing(string name, string descrip);
    Thing(string name, string audio_fpath, string descrip);
    
    //MARK: SETTERS and GETTERS
    string getName();
    string getDescription();
    void setName(string n);
    
    SoundSourceGen getAudio();
    void setAudio(SoundSourceGen soundSource);
    void setAudio(string audio_fpath);
    
};

#endif /* defined(__Squeak__Thing__) */

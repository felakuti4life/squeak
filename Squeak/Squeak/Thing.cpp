//
//  Thing.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Thing.h"
//placeholder names
vector<string> placeholder_names = {
    "Suspicious dish",
    "Suspicious food",
    "Suspicious thing"
};

Thing::Thing(){
    int idx = rand() % placeholder_names.size();
    name = placeholder_names[idx];
}

Thing::Thing(string name){
    this->name = name;
}

Thing::Thing(string name, string audio_fpath){
    this->name = name;
    this->audio = SoundSourceGen(audio_fpath);
}

string Thing::getName(){return name;}
void Thing::setName(string n){name = n;}

SoundSourceGen Thing::getAudio(){return audio;}
void Thing::setAudio(SoundSourceGen soundSource){audio = soundSource;}
void Thing::setAudio(string audio_fpath){audio = SoundSourceGen(audio_fpath);}

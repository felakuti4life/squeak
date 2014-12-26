//
//  Thing.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Thing.h"
//placeholder names
vector<string> placeholder_thing_names = {
    "Suspicious dish",
    "Suspicious food",
    "Suspicious thing"
};

Thing::Thing(){
    int idx = rand() % placeholder_thing_names.size();
    name = placeholder_thing_names[idx];
}

Thing::Thing(string name, string descrip){
    this->name = name;
    description = descrip;
}

Thing::Thing(string name, string audio_fpath, string descrip){
    this->name = name;
    description = descrip;
    this->audio = SoundSourceGen(audio_fpath);
}

string Thing::getName(){return name;}
string Thing::getDescription(){return description;}
void Thing::setName(string n){name = n;}

SoundSourceGen Thing::getAudio(){return audio;}
void Thing::setAudio(SoundSourceGen soundSource){audio = soundSource;}
void Thing::setAudio(string audio_fpath){audio = SoundSourceGen(audio_fpath);}

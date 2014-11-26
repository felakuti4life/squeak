//
//  Room.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Room.h"
#include <random>

//Random placeholder strings
vector<string> placeholder_names ={
    "Dry Parlor",
    "Anechoic Paradise",
    "Foamy Studyroom",
    "Generic Roomscape"
};

Room::Room(){
    int idx = rand() % placeholder_names.size();
    name = placeholder_names[idx];
}

Room::Room(string name){
    this->name = name;
}

Room::Room(string name, string impulse_fpath){
    this->name = name;
    audio = RoomGen(impulse_fpath);
}

string Room::getName(){return name;}
void Room::setName(string n){name=n;}

RoomGen Room::getAudio(){return audio;}
void Room::setAudio(RoomGen audio){this->audio = audio;}
void Room::setAudio(string impulse_fpath){
    audio = RoomGen(impulse_fpath);
}

vector<Room*> Room::getNeighbors(){return neighbors;}
void Room::setNeighbors(vector<Room*> n){
    neighbors = n;
}




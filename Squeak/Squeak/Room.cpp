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
vector<string> placeholder_room_names ={
    "Dry Parlor",
    "Anechoic Paradise",
    "Foamy Studyroom",
    "Generic Roomscape"
};

Room::Room(){
    int idx = rand() % placeholder_room_names.size();
    name = placeholder_room_names[idx];
}

Room::Room(string name){
    this->name = name;
}

Room::Room(string name, string impulse_fpath){
    this->name = name;
    audio = RoomGen(impulse_fpath);
}

Room::Room(string name, string impulse_fpath, vector<Thing> things){
    this->name = name;
    audio = RoomGen(impulse_fpath);
    thingsInRoom = things;
}

string Room::getName(){return name;}
void Room::setName(string n){name=n;}

RoomGen Room::getAudio(){return audio;}
void Room::setAudio(RoomGen audio){this->audio = audio;}
void Room::setAudio(string impulse_fpath){
    audio = RoomGen(impulse_fpath);
}

vector<Thing> Room::getThingsInRoom(){return thingsInRoom;}
void Room::setThingsInRoom(vector<Thing> things){thingsInRoom = things;}

vector<Room*> Room::getNeighbors(){return neighbors;}
void Room::setNeighbors(vector<Room*> n){
    neighbors = n;
}




//
//  Model.h
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef Squeak_Model_h
#define Squeak_Model_h

//#include <stdio>

#include "Room.h"
#include "Thing.h"

//name reserved for designated non-rooms on the map
string nullRoomName = "";
int minimumThingsInRoom = 1;
int maximumThingsInRoom = 4;

//Here are the things you can put in rooms:
//first argument: name, second argument: file path to audio source
vector<Thing> allTheThings = {
    Thing("", ""),
    Thing("", ""),
    Thing("", ""),
    Thing("", ""),
    Thing("", "")
};

//Here are the rooms!:
//first argument: name, second argument: file path to audio source
vector<Room> allTheRooms = {
    Room("", ""),
    Room("", ""),
    Room("", ""),
    Room("", ""),
    Room("", ""),
    Room("", "")
};

#endif

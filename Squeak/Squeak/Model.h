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
    Thing("Rusty Blender", "/Users/Ethan/syncbox/SQUEAK/sound/singles/blender.wav"),
    Thing("Scary Thing 2", "scarything_fpath"),
    Thing("thing 3", "thing_fpath"),
    Thing("thing 4", "thing_fpath"),
    Thing("thing 5", "thing_fpath")
};

//Here are the rooms!:
//first argument: name, second argument: file path to audio source
vector<Room> allTheRooms = {
    Room("Horrible Chapel", "/Users/Ethan/syncbox/SQUEAK/sound/impulses/chapel.wav"),
    Room("The All-Too-Bright Basement", "/Users/Ethan/syncbox/SQUEAK/sound/impulses/basement.wav"),
    Room("Room 3", ""),
    Room("Room 4", ""),
    Room("Room 5", ""),
    Room("Room 6", "")
};

#endif

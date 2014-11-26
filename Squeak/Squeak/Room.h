//
//  Room.h
//  Squeak
//
//  Created by Ethan Geller on 11/25/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef __Squeak__Room__
#define __Squeak__Room__

#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
#include "Thing.h"
#include "RoomGen.h"

//Room class: contains roomgen audio object, room name, and objects that are in the room
class Room {
    string name;
    RoomGen audio;
    vector<Thing> thingsInRoom;
    vector<Room*> neighbors;
    
public:
    //MARK: INITIALIZERS
    Room();
    Room(string name);
    Room(string name, string impulse_fpath);
    Room(string name, string impulse_fpath, vector<Thing> things);
    
    
    //MARK: GETTERS and SETTERS
    string getName();
    void setName(string n);
    
    RoomGen getAudio();
    void setAudio(RoomGen audio);
    void setAudio(string impulse_fpath);
    
    vector<Thing> getThingsInRoom();
    void setThingsInRoom(vector<Thing> things);
    
    vector<Room*> getNeighbors();
    void setNeighbors(vector<Room*> n);
    
    
    //MARK: Actions
    
    
};

#endif /* defined(__Squeak__Room__) */

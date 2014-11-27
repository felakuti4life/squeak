//
//  Map.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/26/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Map.h"

Map::Map(){
    //generate the map
    
}

void Map::fillRoomsWithThings(){
    //two dimensional for loop scanning through the map
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[i].size(); j++) {
            //check to make sure there is a room here
            if (table[i][j].getName() != nullRoomName) {
                //get number of things to put in the room
                int thingsInTheRoom = rand() % maximumThingsInRoom + minimumThingsInRoom;
                //set up a pile of things
                vector<Thing> someThings = {};
                //get some random things for the pile
                for (int k = 0; k < thingsInTheRoom; k++) {
                    //find a random thing and throw it in the pile
                    int thingIdx = rand() % allTheThings.size();
                    someThings.push_back(allTheThings[thingIdx]);
                    //TODO: some minor random playback rates on things
                }
                //put the pile in the room
                table[i][j].setThingsInRoom(someThings);
            }
        }
    }
}
//
//  Map.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/26/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Map.h"
#include <math.h>

//MARK: some settings

//whether we are filling the mansion with random, possibly redundant rooms
bool maniacMansion = false;

Thing getRandomThing(){
    int thingIdx = rand() % allTheThings.size();
    return allTheThings[thingIdx];
}

Room getRandomRoom(){
    int roomIdx = rand() % allTheRooms.size();
    return allTheRooms[roomIdx];
}

Map::Map(){
    //generate the map
    //how many possible rooms the map has
    double sz = allTheRooms.size();
    //width of map in each dimension
    int width = floor(sqrt(sz));
    //start positions
    int xPos = 0, yPos = 0;
    
    //preallocate the vectors
    for (int i = 0; i < width; i++)
        for (int k = 0; k < width; k++) {
            table[i][k] = Room(nullRoomName);
        }
    //raster style!
    for (int i = 0; i < allTheRooms.size() - 2; i++) {
        //make sure there is not a room here
        if (table[xPos][yPos].getName() == nullRoomName) {
            if (maniacMansion)
                table[xPos][yPos] = getRandomRoom();
            else
                table[xPos][yPos] = allTheRooms[i];
            if (rand() % 2) xPos++;
            else yPos++;
        }
        else {
            if (rand()%2) xPos -= 2;
            else yPos -=2;
        }
        //realign xPos and yPos in case they are out of bounds
        if (xPos < 0 || xPos > width){
            //if it's out of bounds, put us somewhere crazy!
            xPos = rand() % width;
        }
        if (yPos < 0 || yPos > width){
            //same as the xPos
            yPos = rand() % width;
        }
    }
}


//Horrifyingly deep!
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
//
//  Map.cpp
//  Squeak
//
//  Created by Ethan Geller on 11/26/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#include "Map.h"
#include "Model.h"
#include <math.h>

//MARK: some settings

//whether we are filling the mansion with random, possibly redundant rooms
bool maniacMansion = false;
//Are we testing the map?
bool mapDebug = true;


//MARK: map generation test!
void Map::testMap(){
    cout << "TEST:\n\n" << "Map:" << endl;
    for (int i = 0; i < table.size(); i++) {
        for (int k = 0; k < table[i].size(); k++) {
            cout << i << " " << k << " " << table[i][k].getName() << "\t";
        }
        cout << endl;
    }
    cout << "NEIGHBORS:"<< endl;
    for (int i = 0; i < table.size(); i++) {
        for (int k = 0; k < table[i].size(); k++) {
            cout << "Room: " << table[i][k].getName() << endl;
            vector<Room*> neighbors = table[i][k].getNeighbors();
            for (int j = 0; j < neighbors.size(); j++) {
                Room aRoom = *neighbors[j];
                cout << "Neighbor " << j << ": " << aRoom.getName() << endl;
            }
            cout << endl;
        }
        cout << "\n\n";
    }
    cout << "Things:"<< endl;
    for (int i = 0; i < table.size(); i++) {
        for (int k = 0; k < table[i].size(); k++) {
            cout << "Room: " << table[i][k].getName() << endl;
            vector<Thing> things = table[i][k].getThingsInRoom();
            for (int j = 0; j < things.size(); j++) {
                Thing aThing = things[j];
                cout << "Thing " << j << ": " << aThing.getName() << endl;
            }
            cout << endl;
        }
        cout << "\n\n";
    }
}

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
    cout << "width: " << width << endl;
    //start positions
    int xPos = 0, yPos = 0;
    
    //preallocate the vectors
    for (int x = 0; x <= width; x++){
        vector<Room> rooms;
        for (int y = 0; y <= width; y++) {
            rooms.push_back(Room(nullRoomName));
        }
        table.push_back(rooms);
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
        cout << "x position:" << xPos << " y position: " << yPos << endl;
    }
    fillRoomsWithThings();
    setNeighbors();
    if(mapDebug) testMap();
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

void Map::setNeighbors(){
    for (int x = 0; x < table.size(); x++)
        for (int y = 0; y <table[x].size(); y++)
            //ensure there's a room here
            if(table[x][y].getName() != nullRoomName){
                vector<Room*> neighbors;
                //to the left?
                if(x-1 >=0)
                    if(table[x-1][y].getName() != nullRoomName)
                        neighbors.push_back(&table[x-1][y]);
                //to the right?
                if(x+1 < table.size())
                    if(table[x+1][y].getName() != nullRoomName)
                        neighbors.push_back(&table[x+1][y]);
                //down below?
                if(y-1 >=0)
                    if(table[x][y-1].getName() != nullRoomName)
                        neighbors.push_back(&table[x][y-1]);
                //up above?
                if(y+1 < table[x].size())
                    if(table[x][y+1].getName() != nullRoomName)
                        neighbors.push_back(&table[x][y+1]);
                //we've got our neighbors.
                table[x][y].setNeighbors(neighbors);
            }
        
}
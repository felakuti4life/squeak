//
//  Map.h
//  Squeak
//
//  Created by Ethan Geller on 11/26/14.
//  Copyright (c) 2014 Ethan Geller. All rights reserved.
//

#ifndef __Squeak__Map__
#define __Squeak__Map__

#include <stdio.h>
#include "Room.h"
#include <vector>
class Map {
    
    
public:
    typedef vector<Room> column;
    typedef vector<column> Table;
    //Map randomly generated on initialization
    Map();
    Map(int seed);
    
    //getter
    Room getRoom(int x, int y);
    Room* getRoomAddress(int x, int y);
    
    int getWidth();
    int getHeight();
    
    void fillRoomsWithThings();
    void setNeighbors();
    
    void convoluteSounds();
    
private:
    Table table;
    
    void testMap();
    
    
};

#endif /* defined(__Squeak__Map__) */

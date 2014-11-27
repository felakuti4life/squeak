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
#include "Model.h"
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
    Room getRoom(Room* r);
    int getWidth();
    int getHeight();
    
    //fill rooms with things
    void fillRoomsWithThings();
    
    
private:
    Table table;
    
};

#endif /* defined(__Squeak__Map__) */

//
// Created by Ludovico on 28/06/2018.
//

#include "ProceduralMap.h"
#include "time.h"

ProceduralMap::ProceduralMap(): xsize(800), ysize(800), objects(10), oldSeed(0), chanceRoom(80), chanceCorridor(20),
minRoomHeight(5), minRoomWidth(5), maxRoomSide(xsize/4) {
    levelMap = 0;
}

ProceduralMap::~ProceduralMap() {
if(levelMap)
    delete [] levelMap;
}

int ProceduralMap::getRand(int x, int y){
    long seed = time(NULL) + oldSeed;
    oldSeed = seed;
}

TileType ProceduralMap::getCell(int x, int y) const {
    return levelMap[x + xsize * y];
}

void ProceduralMap::setCell(int x, int y, TileType tile) {
    int rand;
    if(tile == TileType::Wall){
        rand = getRand(1,3);
        switch (rand){
            case 1:
                levelMap[x + xsize * y] = TileType::Wall;
                break;
            case 2:
                levelMap[x + xsize * y] = TileType::dirtWall;
                break;
            case 3:
                levelMap[x + xsize * y] = TileType::brokenWall;
                break;
            default:
                levelMap[x + xsize * y] = TileType::Wall;
                break;
        }
    }
    else if (tile == TileType::Floor){
        rand = getRand(1,3);
        switch (rand){
            case 1:
                levelMap[x + xsize * y] = TileType::Floor;
                break;
            case 2:
                levelMap[x + xsize * y] = TileType::dirtFloor;
                break;
            case 3:
                levelMap[x + xsize * y] = TileType::brokenFloor;
                break;
            default:
                levelMap[x + xsize * y] = TileType::Floor;
                break;
        }

    }
    else if (tile == TileType::Door){
        rand = getRand(1,2);
        switch (rand){
            case 1:
                levelMap[x + xsize * y] = TileType::Door;
                break;
            case 2:
                levelMap[x + xsize * y] = TileType::BrokenDoor;
                break;
            default:
                levelMap[x + xsize * y] = TileType::Door;
                break;
        }
    }
}


bool ProceduralMap::makeBossRoom(int x, int y) {
    //TODO
}


bool ProceduralMap::makeRoom(int x, int y, int xlength, int ylength, int direction) {
// rooms will respect the min and max width and length, the walkable tiles will be 2 less for length and 2 less for width
    int xLength = getRand(minRoomWidth, maxRoomSide);
    int yLength = getRand(minRoomHeight, maxRoomSide);
    //two cases in room creation. I'll manage what tile version of wall or floor to take in SetCell
    TileType wall = TileType::Wall;
    TileType floor = TileType ::Floor;
    int dir = 0;
    if (direction > 0 && direction < 4)
        dir = direction;

    switch (dir) {
        case 0:
            //north
            //Check if there's enough space left for it. It checks a row and a column more than what we need
            for (int ytemp = y; ytemp > (y - yLength); ytemp--) {  //decrementing y means to go north
                if (ytemp < 0 || ytemp > ysize)
                    return false;
                //if xLength is odd the room will be one more in the right half
                for (int xtemp = (x - xLength / 2); xtemp < (x + (xLength + 1) / 2); xtemp++) {
                    if (xtemp < 0 || xtemp > xsize)
                        return false;
                    if (getCell(xtemp, ytemp) != TileType::Unused)
                        return false; //no space left
                }
            }

            //we're still here, build
            for (int ytemp = y; ytemp > (y - yLength); ytemp--) {
                for (int xtemp = (x - xLength / 2); xtemp < (x + (xLength + 1) / 2); xtemp++) {
                    //start with the walls
                    if (xtemp == (x - xLength / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (xtemp == (x + (xLength - 1) / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == y)
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y - yLength + 1))
                        setCell(xtemp, ytemp, wall);
                        //and then fill with the floor
                    else
                        setCell(xtemp, ytemp, floor);
                }
            }
            break;
        case 1:
            //east
            for (int ytemp = (y - yLength / 2); ytemp < (y + (yLength + 1) / 2);
                 ytemp++) {
                if (ytemp < 0 || ytemp > ysize)
                    return false;
                for (int xtemp = x; xtemp < (x + xLength); xtemp++) {
                    if (xtemp < 0 || xtemp > xsize)
                        return false;
                    if (getCell(xtemp, ytemp) != TileType::Unused)
                        return false;
                }
            }

            for (int ytemp = (y - yLength / 2); ytemp < (y + (yLength + 1) / 2);
                 ytemp++) {
                for (int xtemp = x; xtemp < (x + xLength); xtemp++) {

                    if (xtemp == x)
                        setCell(xtemp, ytemp, wall);
                    else if (xtemp == (x + xLength - 1))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y - yLength / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y + (yLength - 1) / 2))
                        setCell(xtemp, ytemp, wall);
                    else
                        setCell(xtemp, ytemp, floor);
                }
            }
            break;
        case 2:
            //south
            for (int ytemp = y; ytemp < (y + yLength); ytemp++) {
                if (ytemp < 0 || ytemp > ysize)
                    return false;
                for (int xtemp = (x - xLength / 2); xtemp < (x + (xLength + 1) / 2);
                     xtemp++) {
                    if (xtemp < 0 || xtemp > xsize)
                        return false;
                    if (getCell(xtemp, ytemp) != TileType::Unused)
                        return false;
                }
            }

            for (int ytemp = y; ytemp < (y + yLength); ytemp++) {
                for (int xtemp = (x - xLength / 2); xtemp < (x + (xLength + 1) / 2);
                     xtemp++) {

                    if (xtemp == (x - xLength / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (xtemp == (x + (xLength - 1) / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == y)
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y + yLength - 1))
                        setCell(xtemp, ytemp, wall);
                    else
                        setCell(xtemp, ytemp, floor);
                }
            }
            break;
        case 3:
            //west
            for (int ytemp = (y - yLength / 2); ytemp < (y + (yLength + 1) / 2);
                 ytemp++) {
                if (ytemp < 0 || ytemp > ysize)
                    return false;
                for (int xtemp = x; xtemp > (x - xLength); xtemp--) {
                    if (xtemp < 0 || xtemp > xsize)
                        return false;
                    if (getCell(xtemp, ytemp) != TileType::Unused)
                        return false;
                }
            }

            for (int ytemp = (y - yLength / 2); ytemp < (y + (yLength + 1) / 2);
                 ytemp++) {
                for (int xtemp = x; xtemp > (x - xLength); xtemp--) {

                    if (xtemp == x)
                        setCell(xtemp, ytemp, wall);
                    else if (xtemp == (x - xLength + 1))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y - yLength / 2))
                        setCell(xtemp, ytemp, wall);
                    else if (ytemp == (y + (yLength - 1) / 2))
                        setCell(xtemp, ytemp, wall);
                    else
                        setCell(xtemp, ytemp, floor);
                }
            }
            break;
    }

    // successful build
    return true;
}


bool ProceduralMap::makeCorridor(int x, int y, int lenght, int direction) {
//define the dimensions of the corridor (er.. only the width and height..)
    int len = getRand(2, lenght);
    TileType floor = TileType::Floor;
    int dir = 0;
    if (direction > 0 && direction < 4)
        dir = direction;

    int xtemp = 0;
    int ytemp = 0;

    switch (dir) {
        case 0:
            //north
            //check if there's enough space for the corridor
            //start with checking it's not out of the boundaries
            if (x < 0 || x > xsize)
                return false;
            else
                xtemp = x;

            //same thing here, to make sure it's not out of the boundaries
            for (ytemp = y; ytemp > (y - len); ytemp--) {
                if (ytemp < 0 || ytemp > ysize)
                    return false; // out of boundaries
                if (getCell(xtemp, ytemp) != TileType::Unused)
                    return false;
            }

            //if we're still here, let's start building
            for (ytemp = y; ytemp > (y - len); ytemp--) {
                setCell(xtemp, ytemp, floor);
            }
            break;
        case 1:
            //east
            if (y < 0 || y > ysize)
                return false;
            else
                ytemp = y;

            for (xtemp = x; xtemp < (x + len); xtemp++) {
                if (xtemp < 0 || xtemp > xsize)
                    return false;
                if (getCell(xtemp, ytemp) != TileType::Unused)
                    return false;
            }

            for (xtemp = x; xtemp < (x + len); xtemp++) {
                setCell(xtemp, ytemp, floor);
            }
            break;
        case 2:
            //south
            if (x < 0 || x > xsize)
                return false;
            else
                xtemp = x;

            for (ytemp = y; ytemp < (y + len); ytemp++) {
                if (ytemp < 0 || ytemp > ysize)
                    return false;
                if (getCell(xtemp, ytemp) != TileType::Unused)
                    return false;
            }

            for (ytemp = y; ytemp < (y + len); ytemp++) {
                setCell(xtemp, ytemp, floor);
            }
            break;
        case 3:
            //west
            if (ytemp < 0 || ytemp > ysize)
                return false;
            else
                ytemp = y;

            for (xtemp = x; xtemp > (x - len); xtemp--) {
                if (xtemp < 0 || xtemp > xsize)
                    return false;
                if (getCell(xtemp, ytemp) != TileType::Unused)
                    return false;
            }

            for (xtemp = x; xtemp > (x - len); xtemp--) {
                setCell(xtemp, ytemp, floor);
            }
            break;
    }

    // Successful build
    return true;
}


bool ProceduralMap::createLevel(int inx, int iny) {
    towerLevel++;
    objects = getRand(5, 2*towerLevel);
    levelMap = new TileType[xsize * ysize];
    for (int y = 0; y < ysize; y++) {
        for (int x = 0; x < xsize; x++) {
            // making the borders of unwalkable walls
            if (y == 0)
                setCell(x, y, TileType::Wall);
            else if (y == ysize - 1)
                setCell(x, y, TileType::Wall);
            else if (x == 0)
                setCell(x, y, TileType::Wall);
            else if (x == xsize - 1)
                setCell(x, y, TileType::Wall);

                //and fill the rest with unused
            else
                setCell(x, y, TileType::Unused);
        }
    }

    //TODO
}

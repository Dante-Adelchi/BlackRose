//
// Created by Ludovico on 28/06/2018.
//

#include "ctime"
#include "../include/ProceduralMap.h"
//TODO add a fixup for walls and for metalFloor
ProceduralMap::ProceduralMap(const TextureHolder &textures, Tile::BackGroundType backGroundType, sf::Vector2u
    windowSize): xsize(windowSize.x), ysize(windowSize.y), objects(10), chanceRoom(80), chanceCorridor(20),
    minRoomSide(5), maxRoomSide(10), textures(textures) {

    for (int i=0; i<25; i++) {
        for  (int j=0; j<25; j++) {
            tileMap.emplace_back(std::make_shared<Tile>(textures, sf::Vector2f(i,j), backGroundType));
        }
    }
    srand(time(NULL));
    /*
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 25; x++) {
            // making the borders of unwalkable walls
            if (y == 0)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (y == 25 - 1)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (x == 0)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (x == 25 - 1)
                setCell(x, y, Tile::BackGroundType::metalWall);
        }
    }



    for(int i=0; i<5; i++){
        int a = getRand(1,23);
        int b = getRand(1,23);
        setCell(a, b, Tile::BackGroundType::labFloor);
    }

    int a = getRand(1,24);
    int b = getRand(1,24);
    setCell(a, b, Tile::BackGroundType::labFloor);
    if ( getCell(a,b)->backGround == Tile::BackGroundType::labFloor){
        setCell(0,0,getCell(a,b)->backGround);
    }

    int a = getRand(1,24);
    int b = getRand(1,24);
    int c = getRand(0,3);
    makeRoom( a, b, c);

    //makeCorridor(a, b, 6, 2);
    int a,b,c,tries;
    tries = 0;
    while (tries < 100) {
        a = getRand(1, 24);
        b = getRand(1, 24);
        c = getRand(0, 3);
        if (makeRoom(a, b, c)) {
            tries = 200;
            //random num of doors for each room
            //where
        } else
            tries++;
    }
    */
    createLevel(10,15);


}

ProceduralMap::~ProceduralMap() {

}

int ProceduralMap::getRand(int x, int y){

    //return rand() % (y-x) + x;
    if(x > y){
        getRand(y, x);
    } else
        return rand() % (y-x) + x;

}

void ProceduralMap::setCell(int x, int y, Tile::BackGroundType inputTile) {

    std::shared_ptr<Tile> tmp = std::make_shared<Tile>(textures, sf::Vector2f(x,y), inputTile);

    std::shared_ptr<Tile>(tmp).swap(tileMap[y+x*25]);

}

std::shared_ptr<Tile> ProceduralMap::getCell(int x, int y) {
    return tileMap[y+x*25];
}


bool ProceduralMap::makeBossRoom(int x, int y) {

    //TODO
    bossRoom = true;
    return true;
}


bool ProceduralMap::makeRoom(int x, int y, int direction) {
    /*
    //check if a bossRoom is needed
    if (!bossRoom){
        //I want bossRoom will be build only in a central part of the map
        if((x >= xsize/4 && x <= (xsize/4)*3) && (y >= ysize/4 && y <= (ysize/4)*3)){
            makeBossRoom(x, y);
        }
    }
    */
    // rooms will respect the min and max width and length, the walkable tiles will be 2 less for length and 2 less for width
    int xLength = getRand(minRoomSide, maxRoomSide);
    int yLength = getRand(minRoomSide, maxRoomSide);
    //two cases in room creation. I'll manage what tile version of wall or floor to take in SetCell
    int dir = 0;
    if (direction > 0 && direction < 4)
        dir = direction;
    /*
    int nDoors = getRand(0, 100);
    if(nDoors >= 0 && nDoors < chance1Door)
        nDoors = 1;
    else if( nDoors >= chance1Door && nDoors < chance2Door)
        nDoors = 2;
    else if( nDoors >= chance2Door && nDoors < chance3Door)
        nDoors = 3;
    else if( nDoors >= chance3Door && nDoors < chance4Door)
        nDoors = 4;
    */

    int nDoors = 1; //test 2 door each room
    int l;
    int wallDir;
    int i;
    bool doorDone = false;
    int northWall, eastWall, southWall, westWall;
    //a door function cannot be done in a convenient way because the system changes depending on the direction of the room
    switch (dir) {
        case 0:
            //north
            //Check if there's enough space left for it. It checks a row and a column more than what we need
            for (int yTemp = y; yTemp > (y - yLength); yTemp--) {  //decrementing y means to go north
                if (yTemp < 0 || yTemp > ysize)
                    return false; // not enough length space, out from makeroom and the calling function will call again makeroom avoiding the same direction
                //if xLength is odd the room will be one more in the right half
                for (int xTemp = (x - xLength/2); xTemp < (x + ((xLength + 1) / 2)); xTemp++) {
                    if (xTemp < 0 || xTemp > xsize)
                        return false;  //not enough width space
                    if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                        return false; //no space left
                }
            }

            //we're still here, build
            for (int yTemp = y; yTemp > (y - yLength); yTemp--) {
                for (int xTemp = (x - xLength / 2); xTemp < (x + ((xLength + 1) / 2)); xTemp++) {
                    //start with the walls
                    if (xTemp == (x - xLength / 2)) //westwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (xTemp == (x + (xLength - 1) / 2)) //eastwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == y) //southwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y - yLength + 1)) //northwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                        //and then fill with the floor
                    else {
                        setCell(xTemp, yTemp, Tile::BackGroundType::labFloor);

                    }
                }
            }
            for (i = 0; i < nDoors; i++) {
                wallDir = getRand(0, 3);
                //yNorthWall = y-yLength+1;
                //xEastWall = x+(xLength-1)/2;
                //ySouthWall = y;
                //xOvestWall = x-xLength/2;
                while (!doorDone) {
                    if (wallDir == 0) {
                        l = getRand( x-xLength/2 + 1, x+(xLength+1)/2 - 1);
                        if (getCell(l, y-yLength )->backGround != Tile::BackGroundType::metalWall && getCell(l, y-yLength +1)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell(l, y-yLength +1, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                    if (wallDir == 1) {
                        l = getRand(y - 1, (y-yLength +2));
                        if (getCell(x+(xLength)/2 + 1, l)->backGround != Tile::BackGroundType::metalWall && getCell(x+(xLength-1)/2, l)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell(x+(xLength-1)/2, l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);

                    }
                    if (wallDir == 2) {
                        l = getRand(x-xLength/2 + 1, x+(xLength+1)/2 - 1);
                        if (getCell(l, (y + 1))->backGround != Tile::BackGroundType::metalWall && getCell(l, y)->backGround == Tile::BackGroundType::metalWall) {
                            setCell(l, y, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                    if (wallDir == 3) {
                        l = getRand((y - 1), (y+yLength + 1));
                        if (getCell(x-xLength/2 - 1, l)->backGround != Tile::BackGroundType::metalWall && getCell(x-xLength/2, l)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell(x-xLength/2, l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                }
            }
            break;
        case 1:
            //east
            //yNorthWall = y-yLength/2;
            //xEastWall = x+xLength;
            //ySouthWall = y + yLength/2;
            //xOvestWall = x;
            for (int yTemp = (y - yLength / 2); yTemp < (y + (yLength +1 ) / 2); yTemp++) {
                if (yTemp < 0 || yTemp > ysize)
                    return false;
                for (int xTemp = x; xTemp < (x + xLength); xTemp++) {
                    if (xTemp < 0 || xTemp > xsize)
                        return false;
                    if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                        return false;
                }
            }

            for (int yTemp = (y - yLength / 2); yTemp < (y + (yLength +1) / 2); yTemp++) {
                for (int xTemp = x; xTemp < (x + xLength); xTemp++) {

                    if (xTemp == x) //westwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (xTemp == (x + xLength - 1)) //eastwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y - yLength / 2)) //northwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y + (yLength - 1) / 2)) //southwall
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else
                        setCell(xTemp, yTemp, Tile::BackGroundType::labFloor);
                }
            }

            for (i = 0; i < nDoors; i++) {
                wallDir = getRand(0, 3);
                //yNorthWall = y-yLength/2;
                //xEastWall = x+xLength -1;
                //ySouthWall = y+(yLength-1)/2;
                //xOvestWall = x;

                while (!doorDone) {
                    if (wallDir == 0) {
                        l = getRand( x + 1, (x+xLength - 2));
                        if (getCell(l, (y - yLength/2 -1))->backGround != Tile::BackGroundType::metalWall && getCell(l, y - yLength/2)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell(l, (y - yLength / 2), Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                    if (wallDir == 1) {
                        l = getRand((y - yLength/2 + 1), (y + (yLength-1)/2 - 1));
                        if (getCell(x + xLength, l)->backGround != Tile::BackGroundType::metalWall && getCell(x + xLength -1, l)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell((x + xLength -1), l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);

                    }
                    if (wallDir == 2) {
                        l = getRand(x + 1, (x + xLength - 2));
                        if (getCell(l, (y+(yLength-1)/2 + 1))->backGround != Tile::BackGroundType::metalWall && getCell(l, y+(yLength-1)/2)->backGround == Tile::BackGroundType::metalWall) {
                            setCell(l, y+(yLength-1)/2 , Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                    if (wallDir == 3) {
                        l = getRand((y+(yLength-1)/2 - 1), (y - yLength/2 + 1));
                        if (getCell(x - 1, l)->backGround != Tile::BackGroundType::metalWall && getCell(x, l)->backGround == Tile::BackGroundType ::metalWall) {
                            setCell(x, l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                }
            }

            break;
        case 2:
            //south
            northWall = y;
            eastWall = x+(xLength-1)/2;
            southWall = y+yLength-1;
            westWall = x-xLength/2;
            for (int yTemp = y; yTemp < (y + yLength); yTemp++) {
                if (yTemp < 0 || yTemp > ysize)
                    return false;
                for (int xTemp = (x - xLength / 2); xTemp < (x + (xLength + 1) / 2);
                     xTemp++) {
                    if (xTemp < 0 || xTemp > xsize)
                        return false;
                    if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                        return false;
                }
            }

            for (int yTemp = y; yTemp < (y + yLength); yTemp++) {
                for (int xTemp = (x - xLength / 2); xTemp < (x + (xLength + 1) / 2);
                     xTemp++) {

                    if (xTemp == (x - xLength / 2))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (xTemp == (x + (xLength - 1) / 2))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == y)
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y + yLength - 1))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else
                        setCell(xTemp, yTemp, Tile::BackGroundType::labFloor);
                }
            }
            for (i = 0; i < nDoors; i++) {
                wallDir = getRand(0, 3);
                while (!doorDone) {
                    //SOUTH
                    if (wallDir == 0) {
                        l = getRand(westWall + 1, eastWall - 1);
                        if (getCell(l, (northWall - 1))->backGround != Tile::BackGroundType::metalWall && getCell(l, northWall)->backGround == Tile::BackGroundType::metalWall){
                            setCell(l, northWall, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);

                    }
                    if (wallDir == 1) {
                        l = getRand(northWall +1 , (southWall -1));
                        if (getCell(eastWall +1, l)->backGround != Tile::BackGroundType::metalWall && getCell(eastWall, l)->backGround == Tile::BackGroundType::metalWall) {
                            setCell(eastWall, l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);
                    }
                    if (wallDir == 2) {
                        l = getRand(westWall + 1, eastWall - 1);
                        if (getCell(l, southWall +1)->backGround != Tile::BackGroundType::metalWall && getCell(l, southWall)->backGround == Tile::BackGroundType::metalWall) {
                            setCell(l, southWall, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);


                    }
                    if (wallDir == 3) {
                        l = getRand(southWall -1, northWall +1);
                        if (getCell(eastWall -1, l)->backGround != Tile::BackGroundType::metalWall && getCell(eastWall, l)->backGround == Tile::BackGroundType::metalWall) {
                            setCell(eastWall, l, Tile::BackGroundType::cementFloor);
                            doorDone = true;
                        } else
                            wallDir = getRand(0, 3);

                    }
                }
            }
            break;
        case 3:
            //west
            northWall = y-yLength/2;
            eastWall = x;
            southWall = y + (yLength-1)/2;
            westWall = x - xLength +1;
            for (int yTemp = (y - yLength / 2); yTemp < (y + (yLength + 1) / 2);
                 yTemp++) {
                if (yTemp < 0 || yTemp > ysize)
                    return false;
                for (int xTemp = x; xTemp > (x - xLength); xTemp--) {
                    if (xTemp < 0 || xTemp > xsize)
                        return false;
                    if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                        return false;
                }
            }
            for (int yTemp = (y - yLength / 2); yTemp < (y + (yLength + 1) / 2);
             yTemp++) {
                for (int xTemp = x; xTemp > (x - xLength); xTemp--) {

                    if (xTemp == x)
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (xTemp == (x - xLength + 1))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y - yLength / 2))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else if (yTemp == (y + (yLength - 1) / 2))
                        setCell(xTemp, yTemp, Tile::BackGroundType::metalWall);
                    else
                        setCell(xTemp, yTemp, Tile::BackGroundType::labFloor);
                }
            }
                for (i = 0; i < nDoors; i++) {
                    wallDir = getRand(0, 3);
                    while (!doorDone) {
                        if (wallDir == 0) {
                            l = getRand(westWall +1, eastWall -1);
                            if (getCell(l, northWall -1)->backGround != Tile::BackGroundType::metalWall && getCell(l, northWall)->backGround == Tile::BackGroundType::metalWall) {
                                setCell(l, northWall, Tile::BackGroundType::cementFloor);
                                doorDone = true;
                            } else
                                wallDir = getRand(0, 3);
                        }
                        if (wallDir == 1) {
                            l = getRand(southWall -1, northWall +1);
                            if (getCell(eastWall + 1, l)->backGround != Tile::BackGroundType::metalWall && getCell(eastWall, l)->backGround == Tile::BackGroundType::metalWall) {
                                setCell(eastWall , l, Tile::BackGroundType::cementFloor);
                                doorDone = true;
                            } else
                                wallDir = getRand(0, 3);
                        }
                        if (wallDir == 2) {
                            l = getRand(westWall +1, eastWall -1);
                            if (getCell(l, southWall +1)->backGround != Tile::BackGroundType::metalWall && getCell(l, southWall)->backGround == Tile::BackGroundType::metalWall) {
                                setCell(l, southWall, Tile::BackGroundType::cementFloor);
                                doorDone = true;
                            } else
                                wallDir = getRand(0, 3);
                        }
                        if (wallDir == 3) {
                            l = getRand(southWall -1, northWall +1);
                            if (getCell(westWall -1, l)->backGround == Tile::BackGroundType::metalWall && getCell(westWall, l)->backGround == Tile::BackGroundType ::metalWall) {
                                setCell(westWall, l, Tile::BackGroundType::cementFloor);
                                doorDone = true;
                            } else
                                wallDir = getRand(0, 3);
                        }
                    }
                }
            break;
    }
    // successful build
    return true;
}




bool ProceduralMap::makeCorridor(int x, int y, int length, int direction) {
//define the dimensions of the corridor (er.. only the width and height..)
    int len = getRand(2, length);
    int dir = 0;
    if (direction > 0 && direction < 4)
        dir = direction;

    int xTemp = 0;
    int yTemp = 0;

    switch (dir) {
        case 0:
            //north
            //check if there's enough space for the corridor
            //start with checking it's not out of the boundaries
            if (x-1 < 0 || x+1 > xsize)
                return false;
            else
                xTemp = x;

            //same thing here, to make sure it's not out of the boundaries
            for (yTemp = y; yTemp > (y - len); yTemp--) {
                if (yTemp < 0 || yTemp > ysize)
                    return false; // out of boundaries
                if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                    return false; //no space left
            }

            //if we're still here, let's start building
            for (yTemp = y; yTemp > (y - len); yTemp--) {
                setCell(xTemp, yTemp, Tile::BackGroundType::woodFloor);
                setCell(xTemp-1, yTemp, Tile::BackGroundType::metalWall);
                setCell(xTemp+1, yTemp, Tile::BackGroundType::metalWall);
            }
            break;

        case 1:
            //east
            if (y < 0 || y > ysize)
                return false;
            else
                yTemp = y;

            for (xTemp = x; xTemp < (x + len); xTemp++) {
                if (xTemp < 0 || xTemp > xsize)
                    return false;
                if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                    return false;
            }

            for (xTemp = x; xTemp < (x + len); xTemp++) {
                setCell(xTemp, yTemp, Tile::BackGroundType::woodFloor);
                setCell(xTemp, yTemp+1, Tile::BackGroundType::metalWall);
                setCell(xTemp, yTemp-1, Tile::BackGroundType::metalWall);
            }
            break;

        case 2:
            //south
            if (x < 0 || x > xsize)
                return false;
            else
                xTemp = x;

            for (yTemp = y; yTemp < (y + len); yTemp++) {
                if (yTemp < 0 || yTemp > ysize)
                    return false;
                if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                    return false;
            }

            for (yTemp = y; yTemp < (y + len); yTemp++) {

                setCell(xTemp, yTemp, Tile::BackGroundType::woodFloor);
                setCell(xTemp-1, yTemp, Tile::BackGroundType::metalWall);
                setCell(xTemp+1, yTemp, Tile::BackGroundType::metalWall);
            }
            break;
        case 3:
            //west
            if (yTemp < 0 || yTemp > ysize)
                return false;
            else
                yTemp = y;

            for (xTemp = x; xTemp > (x - len); xTemp--) {
                if (xTemp < 0 || xTemp > xsize)
                    return false;
                if (getCell(xTemp, yTemp)->backGround != Tile::BackGroundType::baseFloor)
                    return false;
            }

            for (xTemp = x; xTemp > (x - len); xTemp--) {
                setCell(xTemp, yTemp, Tile::BackGroundType::woodFloor);
                setCell(xTemp, yTemp+1, Tile::BackGroundType::metalWall);
                setCell(xTemp, yTemp-1, Tile::BackGroundType::metalWall);
            }
            break;
    }

    // Successful build
    return true;
}

//TODO va adattata alla struttura dati

bool ProceduralMap::createLevel(int inx, int iny) {
    towerLevel++;
    objects = getRand(10, 100);


    for (int y = 0; y < numSideCell; y++) {
        for (int x = 0; x < numSideCell; x++) {
            // making the borders of unwalkable walls
            if (y == 0)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (y == numSideCell - 1)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (x == 0)
                setCell(x, y, Tile::BackGroundType::metalWall);
            else if (x == numSideCell - 1)
                setCell(x, y, Tile::BackGroundType::metalWall);

                //and fill the rest with unused
            else
                setCell(x, y, Tile::BackGroundType::baseFloor);
        }
    }

    int a,b,c;

    int tries = 0;
    a = getRand(1,24);
    b = getRand(1,24);
    c = getRand(0,3);


    while (tries < 800){
        a = getRand(1,24);
        b = getRand(1,24);
        c = getRand(0,3);
        if( makeRoom( a, b, c)){
            tries = 800;
        //random num of doors for each room
        //where
        } else
            tries++;
    }

    //now we have a room (or a bossRoom)
    int buildSpace = getRand(0, 100);

    //keep count of the number of "objects" we've made
    int currentFeatures = 1; //+1 for the first room we just made

    //then we start the main loop
    for (int countingTries = 0; countingTries < 1000; countingTries++) {
        //check if we've reached our quota
        if (currentFeatures == objects) {
            break;
        }

        //start with a random wall
        int newx = 0;
        int xmod = 0;
        int newy = 0;
        int ymod = 0;
        int validTile = -1;
        //1000 chances to find a suitable object (room or corridor)..
        //(yea, i know it's kinda ugly with a for-loop... -_-')
        for (int testing = 0; testing < 1000; testing++) {
            newx = getRand(1, numSideCell - 2);
            newy = getRand(1, numSideCell - 2);
            validTile = -1;
            //cout<<"tempx: " << newx << "\ttempy: " << newy<<endl;
            if (getCell(newx, newy)->backGround == Tile::metalWall
                || getCell(newx, newy)->backGround == Tile::woodFloor) {
                //check if we can reach the place
                if (getCell(newx, newy + 1)->backGround == Tile::labFloor
                    || getCell(newx, newy + 1)->backGround == Tile::woodFloor) {
                    validTile = 0; //
                    xmod = 0;
                    ymod = -1;
                } else if (getCell(newx - 1, newy)->backGround == Tile::labFloor
                           || getCell(newx - 1, newy)->backGround == Tile::woodFloor) {
                    validTile = 1; //
                    xmod = +1;
                    ymod = 0;
                } else if (getCell(newx, newy - 1)->backGround == Tile::labFloor
                           || getCell(newx, newy - 1)->backGround == Tile::woodFloor) {
                    validTile = 2; //
                    xmod = 0;
                    ymod = +1;
                } else if (getCell(newx + 1, newy)->backGround == Tile::labFloor
                           || getCell(newx + 1, newy)->backGround == Tile::woodFloor) {
                    validTile = 3; //
                    xmod = -1;
                    ymod = 0;
                }

                //check that we haven't got another door nearby, so we won't get a lot of openings besides
                //each other
                if (validTile > -1) {
                    if (getCell(newx, newy + 1)->backGround == Tile::cementFloor) //north
                        validTile = -1;
                    else if (getCell(newx - 1, newy)->backGround == Tile::cementFloor) //east
                        validTile = -1;
                    else if (getCell(newx, newy - 1)->backGround == Tile::cementFloor) //south
                        validTile = -1;
                    else if (getCell(newx + 1, newy)->backGround == Tile::cementFloor) //west
                        validTile = -1;
                }

                //if we can, jump out of the loop and continue with the rest
                if (validTile > -1)
                    break;
            }
        }
        if (validTile > -1) {
            //choose what to build now at our newly found place, and at what direction
            int feature = getRand(0, 100);
            if (feature <= chanceRoom) { //a new room
                if (makeRoom((newx + xmod), (newy + ymod), validTile)) {
                    currentFeatures++; //add to our quota

                    //then we mark the wall opening with a door
                    setCell(newx, newy, Tile::cementFloor);

                    //clean up in front of the door so we can reach it
                    setCell((newx + xmod), (newy + ymod), Tile::labFloor);
                }
            } else if (feature >= chanceRoom) { //new corridor
                if (makeCorridor((newx + xmod), (newy + ymod), 6, validTile)) {
                    //same thing here, add to the quota and a door
                    currentFeatures++;

                    setCell(newx, newy, Tile::cementFloor);
                }
            }
        }
    }

    //second part

    //sprinkle out the bonus stuff (stairs, chests etc.) over the map
    int newx = 0;
    int newy = 0;
    int ways = 0; //from how many directions we can reach the random spot from
    int state = 0; //the state the loop is in, start with the stairs
    while (state != 10) {
        for (int testing = 0; testing < 1000; testing++) {
            newx = getRand(1, numSideCell - 1);
            newy = getRand(1, numSideCell - 2); //cheap bugfix, pulls down newy to 0<y<24, from 0<y<25

            //cout<<"x: " << newx << "\ty: " << newy<<endl;
            ways = 4; //the lower the better

            //check if we can reach the spot
            if (getCell(newx, newy + 1)->backGround == Tile::labFloor
                || getCell(newx, newy + 1)->backGround == Tile::woodFloor) {
                //north
                if (getCell(newx, newy + 1)->backGround != Tile::cementFloor)
                    ways--;
            }
            if (getCell(newx - 1, newy)->backGround == Tile::labFloor
                || getCell(newx - 1, newy)->backGround == Tile::woodFloor) {
                //east
                if (getCell(newx - 1, newy)->backGround != Tile::cementFloor)
                    ways--;
            }
            if (getCell(newx, newy - 1)->backGround == Tile::labFloor
                || getCell(newx, newy - 1)->backGround == Tile::woodFloor) {
                //south
                if (getCell(newx, newy - 1)->backGround != Tile::cementFloor)
                    ways--;
            }
            if (getCell(newx + 1, newy)->backGround == Tile::labFloor
                || getCell(newx + 1, newy)->backGround == Tile::woodFloor) {
                //west
                if (getCell(newx + 1, newy)->backGround != Tile::cementFloor)
                    ways--;
            }

            if (state == 0) {
                if (ways == 0) {
                    //we're in state 0, let's place a "upstairs" thing
                    setCell(newx, newy, Tile::metalFloor); //upstairs
                    state = 1;
                    break;
                }
            } else if (state == 1) {
                if (ways == 0) {
                    //state 1, place a "downstairs"
                    setCell(newx, newy, Tile::metalFloor); //downstairs
                    state = 10;
                    break;
                }
            }
        }
    }

    return true;
}

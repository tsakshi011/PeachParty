#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//class Actor;

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        void setBank(int value){m_bank = value;}
        void addBank(int count){m_bank += count;}
        int getBank(){return m_bank;}
        bool isEmpty(int x, int y); //check if square does not exist at this point
        bool overlapDirectional(int x, int y, int& dir);
        bool atFork(int player_num, int x, int y, int& new_direction);
        Board* getBoard(){return &m_board;}
        Peach* getPeach(){return peach;} //returns pointer to the Peach Avatar
        Yoshi* getYoshi(){return yoshi;} //returns pointer to the Yoshi Avatar
        void createDroppingSquare(int x, int y); //creates a dropping square at the coordinate
        bool overlapVortex(int x, int y);
        void createVortex(int x, int y, int dir, int player);

    private:
        std::vector<Actor*> actors; //vector holding all the actors on the board except with peach and yoshi
        int m_bank; //all bank squares have a "central bank" that holds one value
        Board m_board;
        Peach* peach;
        Yoshi* yoshi;
};

#endif // STUDENTWORLD_H_

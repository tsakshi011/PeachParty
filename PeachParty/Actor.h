#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor: public GraphObject
{
    public:
        Actor(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, int depth, int size): GraphObject(imageID, startX, startY, dir, depth, size){
            ticks_to_move = 0;
            this->studentWorld = studentWorld;
            m_direction = right;
            m_alive = true;
        };
        virtual void doSomething() = 0;
        StudentWorld* getWorld() const {return studentWorld;}
        virtual bool isCoinSquare()const {return false;}
        int getMovingDirection(){return m_direction;}
        void setMovingDirection(int dir){m_direction = dir;}
        virtual bool isDirectional(){return false;}
        bool isAlive() const{return m_alive;}
        void setAlive(bool state){m_alive = state;}
        int getTicks()const {return ticks_to_move;}
        void setTicks(int ticks){ticks_to_move = ticks;}
        virtual bool isAvatar(){return false;}
        bool is_active() const{return active;}
        void set_active(bool state){active = state;}
        virtual bool is_a_square(){return false;}
        virtual bool canMove(int dir);
        virtual bool atFork();
        virtual void moveRandom();
        virtual bool can_be_hit_by_vortex(){return false;};
        void setTeleported(bool value){m_teleported = value;}
        bool getTeleported(){return m_teleported;}
        virtual void teleport(){return;}
    private:
        StudentWorld* studentWorld;
        bool m_teleported;
        int ticks_to_move;
        bool active;
        bool m_alive;
        int m_direction;
};


class Avatar: public Actor //Avatars have states either waiting or walking
{
    public:
        Avatar(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, int depth, int size): Actor(studentWorld, imageID,startX,startY, dir, depth, size){
            state = true;
            m_coins = 0;
            //m_direction = right;
            m_alive = true;
            m_start = true;
            is_landed = true;
            has_vortex = false;
            die = 0;
        };
        virtual void doSomething();
        bool isAlive() const {return m_alive;}
        void setAlive(bool alive){m_alive = alive;}
        bool waitingToRoll(){return state;}
        void setState(bool state){ this->state = state;}
        virtual int getPlayer() = 0;
        //virtual bool canMove(int dir);
        //bool atFork();
        //bool atCorner();
        bool has_a_vortex(){return has_vortex;}
        void set_vortex(bool val){has_vortex = val;}
        virtual bool isAvatar(){return true;}
        int countDir();
        void addCoin(int count){m_coins+=count;}
        void setCoin(int value){m_coins = value;}
        int getCoins(){return m_coins;}
        void addStar(){m_stars++;}
        void setStar(int val){m_stars = val;}
        int getStar(){return m_stars;}
        void setLanding(bool state){is_landed = state;}
        bool getLanding(){return is_landed;}
        //bool isStart(){return m_start;}
        //void setStart(){m_start = false;}
    private:
        int die;
        bool m_alive;
        bool state;
        int m_coins;
        int m_stars;
        bool is_landed;
        bool m_start;
        bool has_vortex;
};


class Peach: public Avatar
{
    public:
        Peach(StudentWorld* studentWorld, int startX, int startY): Avatar(studentWorld, IID_PEACH, startX, startY, right, 0, 1.0)
        {
            setState(true);
            setLanding(true);
        }
        int getPlayer(){return 1;}
};


class Yoshi: public Avatar
{
    public:
        Yoshi(StudentWorld* studentWorld, int startX, int startY): Avatar(studentWorld, IID_YOSHI,startX, startY, right, 0, 1.0)
        {
            setState(true);
            setLanding(true);
        }
        int getPlayer(){return 2;}
};


class ActivatingObject: public Actor
{
    public:
        ActivatingObject(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, double size, int depth, bool activate_when_go_lands): Actor(studentWorld, imageID, startX, startY, dir, size, depth)
        {
            m_active = false;
        }
        virtual void doSomething();
        bool is_a_square(){return true;}
        virtual bool isAlive() const {return true;}
        void activate(){m_active = true;}
        void deactivate(){m_active = false;}
        bool getActive(){return m_active;}
    private:
        bool m_active;
};


class CoinSquare: public ActivatingObject
{
    public:
        /*ActivatingObject(StudentWorld* studentWorld, int imageID, int startX, int startY, bool isBlue): Actor(studentWorld, imageID, startX, startY, right, 1.0, 1,false)*/
        CoinSquare(StudentWorld* studentWorld, int imageID, int startX, int startY,bool isBlue): ActivatingObject(studentWorld, imageID, startX, startY, right, 1, 1.0, false)
        {
            set_active(false);
            m_blue = isBlue;
            m_alive = true;
            /*if(!m_blue)
            {
                m_coin = -3;
            }
            m_coin = 3;*/
        }
        bool isAlive() const {return m_alive;}
        bool isBlue(){return m_blue;}
        bool isCoinSquare(){return true;}
        //int getCoin(){return m_coin;}
        void doSomething();
    private:
        bool m_alive;
        bool m_blue;
        //int m_coin;
};

class ActivateOnPlayer: public ActivatingObject
{
    public:
        ActivateOnPlayer(StudentWorld* studentWorld, int imageID, int startX, int startY,int dir, double size, int depth, bool activate_when_go_lands): ActivatingObject(studentWorld, imageID, startX, startY, dir, size, depth, activate_when_go_lands){}
        //virtual void doSomething();
        virtual bool isCoinSquare(){return true;}
    private:
        
};

class StarSquare : public ActivateOnPlayer {
    public:
    StarSquare(StudentWorld *studentWorld,int startX, int startY): ActivateOnPlayer(studentWorld, IID_STAR_SQUARE,startX, startY, right, 1, 1.0, true){};
        bool is_a_square() const;
        bool can_be_hit_by_vortex() const;
        virtual void doSomething();
};

class DirectionalSquare : public ActivateOnPlayer {
    public:
        DirectionalSquare(StudentWorld* studentWorld, int startX, int startY, int dir, int angle): ActivateOnPlayer(studentWorld, IID_DIR_SQUARE, startX, startY, right, 1, 1.0, false){};
        void doSomething();
        bool is_a_square() const;
        bool can_be_hit_by_vortex() const;
        virtual bool isDirectional(){return true;}
};

class BankSquare : public ActivateOnPlayer {
    public:
        BankSquare(StudentWorld* studentWorld, int startX, int startY): ActivateOnPlayer(studentWorld, IID_BANK_SQUARE, startX, startY, right, 1, 1.0, false){};
        bool is_a_square() const;
        bool can_be_hit_by_vortex() const;
        virtual void doSomething();
    
};

class EventSquare : public ActivateOnPlayer {
    public:
        EventSquare(StudentWorld* studentWorld, int startX, int startY): ActivateOnPlayer(studentWorld, IID_EVENT_SQUARE, startX, startY, right, 1, 1.0, false){};
        bool is_a_square() const;
        bool can_be_hit_by_vortex() const;
        void doSomething();
        void swap();
};

class DroppingSquare: public ActivateOnPlayer
{
    public:
        DroppingSquare(StudentWorld* studentWorld, int startX, int startY):ActivateOnPlayer(studentWorld, IID_DROPPING_SQUARE, startX, startY, right, 1.0, 0, false){};
        bool is_a_square() const;
        virtual void doSomething();
};

class Enemy: public ActivateOnPlayer
{
    public:
        Enemy(StudentWorld* studentWorld, int imageID, int startX, int startY, int dir, double size, int depth, bool activate_when_go_lands): ActivateOnPlayer(studentWorld,imageID,startX,startY,dir,size,depth,activate_when_go_lands)
        {
            m_counter = 180;
            impact_vortex = false;
        }
        bool is_a_square() {return false;}
        bool can_be_hit_by_vortex() const;
        virtual bool can_be_hit_by_vortex(){return true;};
        void setCounter(int count){m_counter = count;}
        int getCounter(){return m_counter;}
        bool isPaused(){return paused;}
        void setPaused(bool state){paused = state;}
        void setSquares(int squares){squares_to_move = squares;}
        int getSquares(){return squares_to_move;}
        void teleport();
        
    private:
        bool impact_vortex;
        bool paused;
        int m_counter;
        int squares_to_move;
};

class Bowser: public Enemy
{
    public:
        Bowser(StudentWorld* studentWorld,int startX, int startY): Enemy(studentWorld, IID_BOWSER, startX, startY, right, 0, 1, false)
        {
            setCounter(180);
            setPaused(true);
            set_active(false);
        }
    virtual void doSomething();
};

class Boo: public Enemy
{
    public:
        Boo(StudentWorld* studentWorld,int startX, int startY): Enemy(studentWorld, IID_BOO, startX, startY, right, 0, 1, false)
        {
            setPaused(true);
        }
        void swapCoins();
        void swapStars();
        virtual void doSomething();
};

class Vortex: public ActivatingObject
{
    public:
        Vortex(StudentWorld* studentWorld, int startX, int startY, int dir, int player):ActivatingObject(studentWorld, IID_VORTEX, startX, startY, dir, 0, 1, true)
        {
            activate();
            setMovingDirection(dir);
            m_player = player;
        }
        int get_player(){return m_player;}
        virtual void doSomething();
    private:
        int m_player;
};

#endif // ACTOR_H_
/*
 
 TREE:
 
              ACTOR
           /        \
        AVATAR          ACTIVATING OBJECTS
       /    \           /       /        \
    PEACH   YOSHI  VORTEX     COIN_SQUARE       ACTIVATE_ON_PLAYER
                                            /     |     |     |    |    \
                                    DIRECTIONAL  STAR  BANK  ENEMY EVENT DROPPING
                                                             /   \
                                                        BOO        BOWSER
 */

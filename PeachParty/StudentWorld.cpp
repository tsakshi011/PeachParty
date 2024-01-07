#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() //destructor for studentWorld call cleanup
{
    cleanUp();
    delete peach;
    delete yoshi;
}

int StudentWorld::init() //create board and place all the actors at set positions identified by files
{
        string board_file = assetPath() + "/board0" + to_string(getBoardNumber()) + ".txt";
        Board::LoadResult result = m_board.loadBoard(board_file);
        if(result == Board::load_fail_file_not_found){
            cerr << "Could not find board01.txt data file\n";
        }else if (result == Board::load_fail_bad_format){
            cerr << "Your board was improperly formatted\n";
        }else if(result == Board::load_success){
            cerr << "Successfully loaded board\n";
            for(int i = 0; i < BOARD_WIDTH; i++){
                for(int j = 0; j < BOARD_HEIGHT; j++){
                    Board::GridEntry ge = m_board.getContentsOf(i, j);
                    switch(ge){
                        case Board::empty:
                            //cerr << "Location "<< i << " " << j << " is empty\n";
                            break;
                        case Board::boo:
                            actors.push_back(new Boo(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH,j * SPRITE_HEIGHT, true));
                            break;
                        case Board::bowser:
                            actors.push_back(new Bowser(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH,j * SPRITE_HEIGHT, true));
                            break;
                        case Board::player:
                            peach = new Peach(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                            yoshi = new Yoshi(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                            actors.push_back(new CoinSquare(this,  IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH,j * SPRITE_HEIGHT, true));
                            break;
                        case Board::red_coin_square:
                            actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, i * SPRITE_WIDTH,j * SPRITE_HEIGHT, false));
                            break;
                        case Board::blue_coin_square:
                            actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, true));
                            //actors.push_back(new BankSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            break;
                        case Board::star_square:
                            actors.push_back(new StarSquare(this, i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            break;
                        case Board::bank_square:
                            actors.push_back(new BankSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            break;
                        case Board::event_square:
                            actors.push_back(new EventSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT));
                            break;
                        case Board::up_dir_square:
                            actors.push_back(new DirectionalSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT, ActivateOnPlayer::up, 90));
                            actors.back()->setDirection(90);
                            break;
                        case Board::down_dir_square:
                            actors.push_back(new DirectionalSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT, ActivateOnPlayer::down, 270));
                            actors.back()->setDirection(270);
                            break;
                        case Board::right_dir_square:
                            actors.push_back(new DirectionalSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT, ActivateOnPlayer::right, 0));
                            actors.back()->setDirection(0);
                            break;
                        case Board::left_dir_square:
                            actors.push_back(new DirectionalSquare(this,i * SPRITE_WIDTH,j * SPRITE_HEIGHT, ActivateOnPlayer::left, 180));
                            actors.back()->setDirection(180);
                            break;
                    }
                }
            }
        }
    m_bank = 0;
	startCountdownTimer(99);  // game runtime
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() 
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    if(peach->isAlive()) peach->doSomething();
    if(yoshi->isAlive()) yoshi->doSomething();
    for(Actor* actor: actors) //traverse actors vector if alive then execute doSomething function
    {
        if(actor->isAlive())
        {
            actor->doSomething();
        }
    }
    
    vector<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        if(!(*it)->isAlive()) //if not alive delete from vector and delete
        {
            delete *it;
            it = actors.erase(it);
        }else{
            it++;
        }
    }
    
    ostringstream oss;
    oss << "\n";
    int player1_die = getPeach()->getTicks()/8;
    int player1_stars = getPeach()->getStar();
    int player1_coins = getPeach()->getCoins();
    int time_remaining = timeRemaining();
    int bank = m_bank;
    int player2_die = getYoshi()->getTicks()/8;
    int player2_stars = getYoshi()->getStar();
    int player2_coins = getYoshi()->getCoins();
    oss << "P1 Roll: " << setw(1) << player1_die << " Stars: " << player1_stars << " $$: " << player1_coins;
    if(getPeach()->has_a_vortex()){
        oss << " VOR";
    }
    oss << " | Time: " << time_remaining << " | Bank: " << bank << " | P2 Roll: " << player2_die << " Stars: " << player2_stars << " $$: " << player2_coins;
    if(getYoshi()->has_a_vortex()){
        oss << " VOR";
    }
    oss << endl;
    string text = oss.str();
    setGameStatText(text);
    if (timeRemaining() <= 1){
        playSound(SOUND_GAME_FINISHED); //Game has finished when no more time is remaining
        if(player1_stars > player2_stars) //peach won if it has more stars
        {
            setFinalScore(player1_stars, player1_coins);
            return GWSTATUS_PEACH_WON;
        }else if(player2_stars > player1_stars)//yoshi won if it has more stars
        {
            setFinalScore(player2_stars, player2_coins);
            return GWSTATUS_YOSHI_WON;
        }else if(player2_stars == player1_stars)
        {
            if(player1_coins > player2_coins)//peach won if it has more coins - same amount of stars
            {
                setFinalScore(player1_stars, player1_coins);
                return GWSTATUS_PEACH_WON;
            }else if(player1_coins < player2_coins)//yoshi won if it has more coins - same amount of stars
            {
                setFinalScore(player2_stars, player2_coins);
                return GWSTATUS_YOSHI_WON;
            }else if(player1_coins == player2_coins)
            {
                int chance = randInt(0,1); //same coins and stars choose random winner
                if(chance == 0)
                {
                    setFinalScore(player1_stars, player1_coins);
                    return GWSTATUS_PEACH_WON;
                }
                if(chance == 1){
                    setFinalScore(player2_stars, player2_coins);
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isEmpty(int x, int y)
{
    for(Actor* actor: actors)
    {
        if(actor->is_a_square() && (abs(actor->getX()-x) < SPRITE_WIDTH) && (abs(actor->getY()-y) < SPRITE_HEIGHT)){
            return false;
        }
    }
    return true;
}

bool StudentWorld::overlapDirectional(int x, int y, int &dir)
{
    for(Actor* actor: actors)
    {
        if(actor->isDirectional() && actor->getX() == x && actor->getY() == y){
            dir = actor->getDirection();
            return true;
        }
    }
    return false;
}

bool StudentWorld::overlapVortex(int x, int y)
{
    bool foundBaddie = false;
    for(Actor* actor: actors)//check if overlap with vortex
    {
        if(actor->can_be_hit_by_vortex()){
            if((abs(actor->getX()-x) < SPRITE_WIDTH) && (abs(actor->getY()-y) < SPRITE_HEIGHT))
            {
                if(!foundBaddie) //if the baddie has already been found then don't execute teleportation for the enemy
                {
                    actor->teleport();
                    foundBaddie = true;
                }
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::createDroppingSquare(int x, int y)
{
    for(Actor* actor: actors)
    {
        if(actor->is_a_square() && actor->getX() == x && actor->getY() == y){
            actor->setAlive(false);
            break;
        }
    }
    actors.push_back(new DroppingSquare(this, x, y)); //create a dropping square at this point
}

void StudentWorld::createVortex(int x, int y, int dir, int player)
{
    actors.push_back(new Vortex(this, x, y, dir, player)); //create a vortex at this point by a certain player in the direction that they point to
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = actors.begin(); //delete all actors in the vector/from game board
    while(it != actors.end())
    {
        delete *it;
        it = actors.erase(it);
        //it++;
        it = actors.begin();
    }
}


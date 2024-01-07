#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

bool Actor::canMove(int dir)
{
    int x_new = getX();
    int y_new = getY();
    getPositionInThisDirection(dir, SPRITE_WIDTH, x_new, y_new);
    return (!getWorld()->isEmpty(x_new,y_new)); //check that square in the new direction actaully exists
}

/*bool Actor::canMove(int dir)
{
    int x_new = getX();
    int y_new = getY();
    if(dir == right)
    {
        if(!getWorld()->isEmpty(x_new + 16, y_new))
            return true;
    }else if(dir == left){
        if(!getWorld()->isEmpty(x_new - 16, y_new))
            return true;
    }else if(dir == up){
        if(!getWorld()->isEmpty(x_new, y_new + 16))
            return true;
    }else if(dir == down){
        if(!getWorld()->isEmpty(x_new, y_new-16))
            return true;
    }
    return false;
}*/

bool Actor::atFork()
{
    int count = 0;
    int x_new = getX(); //check in each direction of whether a square exists there
    int y_new = getY();
    if(x_new%16 != 0 || y_new%16 != 0) return false;
    getPositionInThisDirection(right, SPRITE_WIDTH, x_new, y_new);
    if(getWorld()->getBoard()->getContentsOf(x_new/16,y_new/16) != Board::empty)count ++;
    getPositionInThisDirection(left, SPRITE_WIDTH, x_new, y_new);
    if(getWorld()->getBoard()->getContentsOf(x_new/16,y_new/16) != Board::empty)count ++;
    getPositionInThisDirection(up, SPRITE_WIDTH, x_new, y_new);
    if(getWorld()->getBoard()->getContentsOf(x_new/16,y_new/16) != Board::empty)count ++;
    getPositionInThisDirection(down, SPRITE_WIDTH, x_new, y_new);
    if(getWorld()->getBoard()->getContentsOf(x_new/16,y_new/16) != Board::empty)count ++;
    return count > 2; //if more than two directions exist player avatar at a fork
}

void Actor::moveRandom()
{
    bool move = false;
    while(!move){ //while player cannot move continue checking random directions
        int chance = randInt(1,4); //generate random number
        if(chance == 1 && canMove(right) && !move){ //check each direction if the num was picked
            setMovingDirection(right);
            setDirection(0);
            move = true;
        }
        if(chance == 2 && canMove(left) && !move){
            setMovingDirection(left);
            setDirection(180);
            move = true;
        }
        if(chance == 3 && canMove(up) && !move){
            setMovingDirection(up);
            setDirection(0);
            move = true;
        }
        if(chance == 4 && canMove(down) && !move){
            setMovingDirection(down);
            setDirection(0);
            move = true;
        }
    }
}


void Avatar::doSomething()
{
    if(waitingToRoll()) //player is not currently walking
    {
        if(getTeleported()) //if was teleported move randomly no longer teleported
        {
            moveRandom();
            setTeleported(false);
        }
        int action = getWorld()->getAction(getPlayer());
        if(action == ACTION_ROLL) //checking that user pressed either Tab or Return for Peach and Yoshi, respectively
        {
            int die = randInt(1,10) * 8; //roll dice
            setTicks(die);
            setState(false); //player now is walking
        }else if(action == ACTION_FIRE){
            if(has_a_vortex())
            {
                getWorld()->createVortex(getX(), getY(), getMovingDirection(), getPlayer());
            }
        }else if(action == ACTION_NONE){ //no proper key was pressed
            return;
        }
    }else if(!waitingToRoll()){ //player in walking state
        int dir = getDirection();
        if(getWorld()->overlapDirectional(getX(), getY(), dir)){ //if at a directional square
            setMovingDirection(dir);
            if(getMovingDirection() == left){
                setDirection(180);
            }else{
                setDirection(0);
            }
        }else if(atFork()){ //execute when player avatar at a fork
            switch(getWorld()->getAction(getPlayer()))
            {
                case ACTION_RIGHT: //execute actions based on what the user pressed key unless cannot move in that direction
                    if(getMovingDirection() != left && canMove(right))
                    {
                        setMovingDirection(right);
                        setDirection(0);
                    }else{
                        return;
                    }
                    break;
                case ACTION_LEFT:
                    if(getMovingDirection() != right && canMove(left))
                    {
                        setMovingDirection(left);
                        setDirection(180);
                    }else{
                        return;
                    }
                    break;
                case ACTION_UP:
                    if(getMovingDirection() != down && canMove(up))
                    {
                        setMovingDirection(up);
                        setDirection(0);
                    }else{
                        return;
                    }
                    break;
                case ACTION_DOWN:
                    if(getMovingDirection() != up && canMove(down))
                    {
                        setMovingDirection(down);
                        setDirection(0);
                    }else{
                        return;
                    }
                    break;
                default:
                    return;
            }
        }else{ //player avatar at a corner chooses up over down and right over left
              int x_new, y_new;
              getPositionInThisDirection(getMovingDirection(), SPRITE_WIDTH, x_new, y_new); //check position in
              
              if(getWorld()->isEmpty(x_new, y_new)){
                  if(getMovingDirection() == right || getMovingDirection() == left)
                  {
                      getPositionInThisDirection(down, SPRITE_HEIGHT, x_new, y_new);
                      if((getWorld()->isEmpty(x_new, y_new))) //if no square down move up
                      {
                          setDirection(up);
                          setMovingDirection(up);
                      }else{
                          getPositionInThisDirection(up, SPRITE_HEIGHT, x_new, y_new);
                          if(getWorld()->isEmpty(x_new, y_new)) //if no square up move down
                          {
                              setDirection(down);
                              setMovingDirection(down);
                          }
                      }
                  }else if(getMovingDirection() == up || getMovingDirection() == down)
                  {
                      getPositionInThisDirection(left, SPRITE_WIDTH, x_new, y_new);
                      if(getWorld()->isEmpty(x_new, y_new)) //if no square to the left move right
                      {
                          setDirection(right);
                          setMovingDirection(right);
                      }else{
                          getPositionInThisDirection(right, SPRITE_WIDTH, x_new, y_new);
                          if(getWorld()->isEmpty(x_new, y_new)) //if no square to the right move left
                          {
                              setDirection(left);
                              setMovingDirection(left);
                          }
                      }
                      
                  }
              }
        }
        if(getMovingDirection() == left){
            setDirection(180);
        }else{
            setDirection(0);
        }
        moveAtAngle(getMovingDirection(), 2); //moving two pixels in walking direction of Avatar
        setTicks(getTicks()-1); //decrement ticks_to_move
        if(getTicks() <= 0){ //end of ticks_to_move Avatar returns to waiting for die roll state
            setState(true);
            setLanding(true);
        }
    }
}

void DirectionalSquare::doSomething()
{
    return; //already implemented in the player do something function when a player lands on a directional Square
}

void CoinSquare::doSomething()
{
    if(!isAlive())return;
    if(getWorld()->getPeach()->getX()%16==0 && getWorld()->getPeach()->getY()%16==0 && getWorld()->getPeach()->waitingToRoll() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY() && isBlue())
    { //peach landed on blue square
        if(getWorld()->getPeach()->getLanding()){ //check that has landed
            getWorld()->getPeach()->addCoin(3); //add 3 coins to peach
            getWorld()->playSound(SOUND_GIVE_COIN); //play noise of coin
            getWorld()->getPeach()->setLanding(false); //no longer newly landed player avatar
        }
    }
    if(getWorld()->getPeach()->getX()%16==0 && getWorld()->getPeach()->getY()%16==0 && getWorld()->getPeach()->waitingToRoll() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY() && !isBlue())
    {
        if(getWorld()->getPeach()->getLanding()){ //check landed on red coin
            if(getWorld()->getPeach()->getCoins() < 3) //if player avatar does not have 3 coins then take away all their coins
            {
                getWorld()->getPeach()->setCoin(0);
            }
            else{
                getWorld()->getPeach()->addCoin(-3); //player avatar has more than to 3 coins then remove 3 coins
            }
            getWorld()->playSound(SOUND_TAKE_COIN);
            getWorld()->getPeach()->setLanding(false);
        }
    }
    
    if(getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY() && isBlue()) //repeat for Yoshi's avatar
    {
        if(getWorld()->getYoshi()->getLanding()){
            getWorld()->getYoshi()->addCoin(3);
            getWorld()->playSound(SOUND_GIVE_COIN);
            getWorld()->getYoshi()->setLanding(false);
        }
    }
    if(getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY() && !isBlue())
    {
        if(getWorld()->getYoshi()->getLanding()){
            if(getWorld()->getYoshi()->getCoins() < 3)
            {
                getWorld()->getYoshi()->setCoin(0);
            }
            else{
                getWorld()->getYoshi()->addCoin(-3);
            }
            getWorld()->playSound(SOUND_TAKE_COIN);
            getWorld()->getYoshi()->setLanding(false);
        }
    }
}

void StarSquare::doSomething()
{
    if(getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY() && !is_active())
    {
        getWorld()->playSound(SOUND_GIVE_STAR);
        if(getWorld()->getPeach()->getCoins() < 20){
            return;
        }else{
            getWorld()->getPeach()->addCoin(-20);
            getWorld()->getPeach()->addStar();
        }
    }
    
    if(getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY() && !is_active() )
    {
        getWorld()->playSound(SOUND_GIVE_STAR);
        if(getWorld()->getYoshi()->getCoins() < 20){
            return;
        }else{
            getWorld()->getYoshi()->addCoin(-20);
            getWorld()->getYoshi()->addStar();
        }
    }
}

void BankSquare::doSomething()
{
    if(getWorld()->getPeach()->getX()%16==0 && getWorld()->getPeach()->getY()%16==0 && getWorld()->getPeach()->waitingToRoll() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY())
    {
        if(getWorld()->getPeach()->getLanding()){ //check exactly on the bank square
            getWorld()->getPeach()->addCoin(getWorld()->getBank()); //give all coins from bank to avatar
            getWorld()->setBank(0); //bank is now 0
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
            getWorld()->getPeach()->setLanding(false); //no longer newly landed avatar on bank square
        }
    }else if(!getWorld()->getPeach()->getLanding() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY()){
        if(getWorld()->getPeach()->getCoins() < 5) //if player avatar passing over transfer 5 coins or less based on what player avatar has over to the "central bank"
        {
            getWorld()->addBank(getWorld()->getPeach()->getCoins());
            getWorld()->getPeach()->setCoin(0);
        }else{
            getWorld()->addBank(5);
            getWorld()->getPeach()->addCoin(-5);
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
    
    if(getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY()) //execute the same for Yoshi
    {
        if(getWorld()->getYoshi()->getLanding()){
            getWorld()->getYoshi()->addCoin(getWorld()->getBank());
            getWorld()->setBank(0);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
            getWorld()->getYoshi()->setLanding(false);
        }
    }else if(!getWorld()->getYoshi()->getLanding() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY()){
        if(getWorld()->getYoshi()->getCoins() < 5)
        {
            getWorld()->addBank(getWorld()->getYoshi()->getCoins());
            getWorld()->getYoshi()->setCoin(0);
        }else{
            getWorld()->addBank(5);
            getWorld()->getYoshi()->addCoin(-5);
        }
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
}

void EventSquare::swap()
{
    int temp_x = getWorld()->getPeach()->getX(); //swap positions of the player
    int temp_y = getWorld()->getPeach()->getY();
    int x = getWorld()->getYoshi()->getX();
    int y = getWorld()->getYoshi()->getY();
    getWorld()->getPeach()->moveTo(x,y);
    getWorld()->getYoshi()->moveTo(temp_x, temp_y);
    
    
    int temp_ticks = getWorld()->getPeach()->getTicks(); //swap ticks
    getWorld()->getPeach()->setTicks(getWorld()->getYoshi()->getTicks());
    getWorld()->getYoshi()->setTicks(temp_ticks);
    
    int temp_dir = getWorld()->getPeach()->getMovingDirection(); //swap moving direction
    getWorld()->getPeach()->setMovingDirection(getWorld()->getYoshi()->getMovingDirection());
    getWorld()->getYoshi()->setMovingDirection(temp_dir);
    
    int temp_spriteDir = getWorld()->getPeach()->getDirection(); //swap sprite's directions
    getWorld()->getPeach()->setDirection(getWorld()->getYoshi()->getDirection());
    getWorld()->getYoshi()->setDirection(temp_spriteDir);
    
    bool temp_state = getWorld()->getPeach()->waitingToRoll(); //swap avatar's states
    getWorld()->getPeach()->setState(getWorld()->getYoshi()->waitingToRoll());
    getWorld()->getYoshi()->setState(temp_state);
    
    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}

void EventSquare::doSomething()
{
    if(!getActive() && getWorld()->getPeach()->getX()%16==0 && getWorld()->getPeach()->getY()%16==0 && getWorld()->getPeach()->waitingToRoll() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY())
    {
        activate();
        int random = randInt(1,3);
        if(random == 1) //33% chance of teleporting to random square on the baord
        {
            while(!getWorld()->getPeach()->getTeleported()){
                int x = rand() % 15;
                int y = rand() % 15;
                if(!getWorld()->isEmpty(x*16,y*16)){
                    getWorld()->getPeach()->moveTo(x*16,y*16);
                    getWorld()->getPeach()->setTeleported(true);
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    return;
                }
            }
        }else if(random == 2) //33% chance of swapping everything with other player avatar including position, direction, coins, and stars
        {
            swap();
        }else if(random == 3) //33% chance of dropping a vortex when it has one
        {
            if(!getWorld()->getPeach()->has_a_vortex())
            {
                getWorld()->getPeach()->set_vortex(true);
            }
        }
    }else if(!getActive() && getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY())
    {
        activate();
        int random = randInt(1,3);
        if(random == 1)
        {
            while(!getWorld()->getYoshi()->getTeleported()){
                int x = rand() % 15;
                int y = rand() % 15;
                if(!getWorld()->isEmpty(x*16,y*16)){
                    getWorld()->getYoshi()->moveTo(x*16,y*16);
                    getWorld()->getYoshi()->setTeleported(true);
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    return;
                }
            }
        }else if(random == 2)
        {
            swap();
        }else if(random == 3)
        {
            if(!getWorld()->getYoshi()->has_a_vortex())
            {
                getWorld()->getYoshi()->set_vortex(true);
            }
        }
    }
    if(getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && (getWorld()->getYoshi()->getX() != getX() || getWorld()->getYoshi()->getY() != getY()))
    {
        deactivate();
    }
    if(!getActive() && getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY())
    {
        deactivate();
    }
}

void DroppingSquare::doSomething()
{
    if(getWorld()->getPeach()->getX()%16==0 && getWorld()->getPeach()->getY()%16==0 && getWorld()->getPeach()->waitingToRoll() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY())
    {
        int random = randInt(0,1);
        if(random == 0) //randomly choose whether to remove 10 coins from the player avatar
        {
            if(getWorld()->getPeach()->getCoins() < 10)
            {
                getWorld()->getPeach()->setCoin(0);
            }else{
                getWorld()->getPeach()->addCoin(-10);
            }
        }else if(random == 1) //or remove one star from player avatar if it has one
        {
            if(getWorld()->getPeach()->getStar() >= 1)
            {
                getWorld()->getPeach()->setStar(getWorld()->getPeach()->getStar()-1);
            }
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
    
    
    if(getWorld()->getYoshi()->getX()%16==0 && getWorld()->getYoshi()->getY()%16==0 && getWorld()->getYoshi()->waitingToRoll() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY())
    {
        int random = randInt(0,1);
        if(random == 0)
        {
            if(getWorld()->getYoshi()->getCoins() < 10)
            {
                getWorld()->getYoshi()->addCoin(-1 * (10-getWorld()->getYoshi()->getCoins()));
            }else{
                getWorld()->getYoshi()->addCoin(-10);
            }
        }else if(random == 1)
        {
            if(getWorld()->getYoshi()->getStar() >= 1)
            {
                getWorld()->getYoshi()->setStar(getWorld()->getYoshi()->getStar()-1);
            }
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
        
}

void Bowser::doSomething()
{
    if(isPaused())
    {
        int chance = randInt(0,1);
        if(!getActive() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY() && getWorld()->getPeach()->waitingToRoll())
        {
            activate(); //50% chance removing all coins and stars of avatar landed on Bowser
            if(chance == 0)
            {
                getWorld()->getPeach()->setCoin(0);
                getWorld()->getPeach()->setStar(0);
            }
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
        }
        if(!getActive() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY() && getWorld()->getYoshi()->waitingToRoll())
        {
            activate();
            if(chance == 0)
            {
                getWorld()->getYoshi()->setCoin(0);
                getWorld()->getYoshi()->setStar(0);
            }
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
        }
        if((getWorld()->getYoshi()->getY() != getY() || getWorld()->getYoshi()->getX() != getX()) && (getWorld()->getPeach()->getX() != getX() || getWorld()->getPeach()->getY() != getY()))
        {
            deactivate();
        }
        setCounter(getCounter()-1);
        if(getCounter() <= 0)
        {
            setSquares(randInt(1,10));
            setTicks(getSquares() * 8);
            moveRandom();
            if(getMovingDirection() == left){
                setDirection(180);
            }else{
                setDirection(0);
            }
            setPaused(false);
        }
    }else if(!isPaused()) //avatar's do something
    {
        if(getX() % 16 == 0 && getY() % 16 == 0 && !getWorld()->isEmpty(getX(), getY()) && atFork()){
            moveRandom(); //move randomly if at a fork
        }else{
            int x_new, y_new;
            getPositionInThisDirection(getMovingDirection(), SPRITE_WIDTH, x_new, y_new); //check position in
            
            if(getWorld()->isEmpty(x_new, y_new)){
                if(getMovingDirection() == right || getMovingDirection() == left)
                {
                    getPositionInThisDirection(down, SPRITE_HEIGHT, x_new, y_new);
                    if((getWorld()->isEmpty(x_new, y_new))) //if no square down move up
                    {
                        setDirection(up);
                        setMovingDirection(up);
                    }else{
                        getPositionInThisDirection(up, SPRITE_HEIGHT, x_new, y_new);
                        if(getWorld()->isEmpty(x_new, y_new)) //if no square up move down
                        {
                            setDirection(down);
                            setMovingDirection(down);
                        }
                    }
                }else if(getMovingDirection() == up || getMovingDirection() == down)
                {
                    getPositionInThisDirection(left, SPRITE_WIDTH, x_new, y_new);
                    if(getWorld()->isEmpty(x_new, y_new)) //if no square to the left move right
                    {
                        setDirection(right);
                        setMovingDirection(right);
                    }else{
                        getPositionInThisDirection(right, SPRITE_WIDTH, x_new, y_new);
                        if(getWorld()->isEmpty(x_new, y_new)) //if no square to the right move left
                        {
                            setDirection(left);
                            setMovingDirection(left);
                        }
                    }
                }
            }
        }
        if(getMovingDirection() == left){
            setDirection(180);
        }else{
            setDirection(0);
        }
        moveAtAngle(getMovingDirection(), 2); //moving two pixels in walking direction of Avatar
        setTicks(getTicks()-1); //decrement ticks_to_move
        if(getTicks() <= 0){ //end of ticks_to_move Avatar returns to waiting for die roll state
            setPaused(true);
            
            setCounter(180);
            int count = randInt(1,4);
            if(count == 1) //25% chance of dropping a square at its point
            {
                getWorld()->createDroppingSquare(getX(), getY());
                getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
            }
        }
    }
}

void Boo::swapCoins() //swapping coins of the avatars
{
    int temp_coins = getWorld()->getPeach()->getCoins();
    getWorld()->getPeach()->setCoin(getWorld()->getYoshi()->getCoins());
    getWorld()->getYoshi()->setCoin(temp_coins);
}

void Boo::swapStars() //swapping stars of the avatars
{
    int temp_stars = getWorld()->getPeach()->getStar();
    getWorld()->getPeach()->setStar(getWorld()->getYoshi()->getStar());
    getWorld()->getYoshi()->setStar(temp_stars);
}

void Boo::doSomething()
{
    if(isPaused())
    {
        int chance = randInt(0,1);
        if(!getActive() && getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY() && getWorld()->getPeach()->waitingToRoll())
        {
            activate(); //50% chance thant landing on Boo will either swap coins of avatars or stars
            if(chance == 0)
            {
                swapCoins();
            }else if(chance == 1)
            {
                swapStars();
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
        }else if(!getActive() && getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY() && getWorld()->getYoshi()->waitingToRoll())
        {
            activate();
            if(chance == 0)
            {
                swapCoins();
            }else if(chance == 1)
            {
                swapStars();
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
        }
        if((getWorld()->getYoshi()->getY() != getY() || getWorld()->getYoshi()->getX() != getX()) && (getWorld()->getPeach()->getX() != getX() || getWorld()->getPeach()->getY() != getY()))
        {
            deactivate();
        }
        setCounter(getCounter()-1);
        if(getCounter() <= 0)
        {
            setSquares(randInt(1,3));
            setTicks(getSquares() * 8);
            moveRandom();
            if(getMovingDirection() == left){
                setDirection(180);
            }else{
                setDirection(0);
            }
            setPaused(false);
        }
    }else if(!isPaused()) //move like an avatar's do something
    {
        if(getX() % 16 == 0 && getY() % 16 == 0 && !getWorld()->isEmpty(getX(), getY()) && atFork()){
            moveRandom();
        }else{
            int x_new, y_new;
            getPositionInThisDirection(getMovingDirection(), SPRITE_WIDTH, x_new, y_new); //check position in
            
            if(getWorld()->isEmpty(x_new, y_new)){
                if(getMovingDirection() == right || getMovingDirection() == left)
                {
                    getPositionInThisDirection(down, SPRITE_HEIGHT, x_new, y_new);
                    if((getWorld()->isEmpty(x_new, y_new))) //if no square down move up
                    {
                        setDirection(up);
                        setMovingDirection(up);
                    }else{
                        getPositionInThisDirection(up, SPRITE_HEIGHT, x_new, y_new);
                        if(getWorld()->isEmpty(x_new, y_new)) //if no square up move down
                        {
                            setDirection(down);
                            setMovingDirection(down);
                        }
                    }
                }else if(getMovingDirection() == up || getMovingDirection() == down)
                {
                    getPositionInThisDirection(left, SPRITE_WIDTH, x_new, y_new);
                    if(getWorld()->isEmpty(x_new, y_new)) //if no square to the left move right
                    {
                        setDirection(right);
                        setMovingDirection(right);
                    }else{
                        getPositionInThisDirection(right, SPRITE_WIDTH, x_new, y_new);
                        if(getWorld()->isEmpty(x_new, y_new)) //if no square to the right move left
                        {
                            setDirection(left);
                            setMovingDirection(left);
                        }
                    }
                }
            }
        }
        if(getMovingDirection() == left){
            setDirection(180);
        }else{
            setDirection(0);
        }
        moveAtAngle(getMovingDirection(), 2); //moving two pixels in walking direction of Avatar
        setTicks(getTicks()-1); //decrement ticks_to_move
        if(getTicks() <= 0){ //end of ticks_to_move Avatar returns to waiting for die roll state
            setPaused(true);
            setCounter(180);
        }
    }
}


void ActivatingObject::doSomething()
{
    return;
}

void Enemy::teleport()
{
    while(!getTeleported()){ //keep checking for random valid squares for avatar to move to until found the loop goes on
        int x = rand() % 15;
        int y = rand() % 15;
        if(!getWorld()->isEmpty(x*16,y*16)){
            moveTo(x*16,y*16);
            setTeleported(true);
            return;
        }
    }
}

void Vortex::doSomething()
{
    if(!isAlive()) return; //make sure the vortex is alive
    moveAtAngle(getMovingDirection(),2); //move the vortex in the direction it was created
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
    { //checking if the vortex has left the screen then kill the vortex - delete
        if(get_player() == 1)
        {
            getWorld()->getPeach()->set_vortex(false);
        }else if(get_player() == 2)
        {
            getWorld()->getYoshi()->set_vortex(false);
        }
        setAlive(false);
    }
    if(getWorld()->overlapVortex(getX(),getY()))
    {//check that the vortex overlaps with any BADDIEs then kill it - delete
        if(get_player() == 1)
        {
            getWorld()->getPeach()->set_vortex(false);
        }else if(get_player() == 2)
        {
            getWorld()->getYoshi()->set_vortex(false);
        }
        setAlive(false);
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}

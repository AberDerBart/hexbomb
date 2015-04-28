#ifndef PLAYER_H
#define PLAYER_H
class Player;
#include "Direction.h"
#include "Hex.h"
#include "Item.h"
#include "Bomb.h"
class Player:public Item{
public:
	Player();
	Player(Hex*,int sockFd=-1);
	~Player();
	bool placeBomb();
	void removeBomb(Bomb*);
	void setBombCountMax(int);
	int getBombRange();
	int getBombPower();
	bool move();
	void moveFinished();
	bool isMoving();
	void tick(int);
	void pickupItem(Item*);
	void setSocket(int);
	void processCommand();
	void closeSocket();
	void sendMessage(char*);
	//override
	void setCurrentHex(Hex*);
	void damage(int);
private:
	//bombs
	int bombRange;
	int bombPower;
	int bombCountMax;
	bool layBomb;
	Bomb** bombs;
	//network/control
	int sockFd;
	int id;
	char idStr[6];
	char* idStrP;
	//movement
	Hex* targetHex;
	Hex* nextTargetHex;
	char nextTargetHexDir;
	double speed;
	double moveProg;
	bool moving;
	Direction dir;
	//itemPickup
	Item* tempItem;
public:
	//getter/setter
	int getId();
	void setId(int);
	char* getIdStr();
};
#endif

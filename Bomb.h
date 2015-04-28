#ifndef BOMB_H
#define BOMB_H
class Bomb;
#include "Item.h"
#include "Player.h"

class Bomb:public Item{
public:
	Bomb();
	Bomb(Hex*,Player*);
	~Bomb();
	void tick(int);
	double getDecayTime();
private:
	void explode();

	double decayTime;
	Player* owner;
};
#endif

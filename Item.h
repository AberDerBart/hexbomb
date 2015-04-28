#ifndef ITEM_H
#define ITEM_H
class Item;
#include "Hex.h"
#ifndef NULL
#define NULL 0
#endif

class Item{
public:
	Item();
	Item(Hex*);
	virtual ~Item();
	
	Hex* getCurrentHex();
	virtual void setCurrentHex(Hex*);
	virtual void tick(int);
	bool isBlocking();
	virtual void damage(int);
protected:
	Hex* currentHex;
	bool blocking;
};
#endif

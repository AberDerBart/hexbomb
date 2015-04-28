#ifndef HEX_H
#define HEX_H
class Hex;
#include "Direction.h"
#include "Item.h"
#include "HexType.h"

class Hex{
public:
	Hex(HexType type=empty);
	~Hex();
	
	void setNeighbour(Direction,Hex*);
	void setX(int);
	void setY(int);
	void setItem(Item*);
	bool isWalkable();
	Hex* getNeighbour(Direction);
	HexType getType();
	void setType(HexType);
	int getX();
	int getY();
	Item* getItem();
private:
	int durability;
	HexType type;
	Item* item;
	Hex* neighbours[6];
	int x;
	int y;
};
#endif

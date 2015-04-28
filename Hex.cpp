#include "Hex.h"

Hex::Hex(HexType type){
	this->type=type;
	neighbours[0]=NULL;
	neighbours[1]=NULL;
	neighbours[2]=NULL;
	neighbours[3]=NULL;
	neighbours[4]=NULL;
	neighbours[5]=NULL;
}

Hex::~Hex(){

}

void Hex::setNeighbour(Direction dir, Hex* hex){
	neighbours[dir]=hex;
}

void Hex::setX(int x){
	this->x=x;
}

void Hex::setY(int y){
	this->y=y;
}

void Hex::setItem(Item* item){
	this->item=item;
}

Item* Hex::getItem(){
	return item;
}

Hex* Hex::getNeighbour(Direction dir){
	if(dir==null){
		return this;
	}
	return neighbours[dir];
}

HexType Hex::getType(){
	return type;
}

void Hex::setType(HexType type){
	this->type=type;
}

int Hex::getX(){
	return x;
}

int Hex::getY(){
	return y;
}

bool Hex::isWalkable(){
	if(type!=empty){
		return false;
	}
	if(item){
		if(item->isBlocking()){
			return false;
		}
	}
	return true;
}

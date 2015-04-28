#include "Item.h"

Item::Item(){
	currentHex=new Hex(empty);
	currentHex->setItem(this);
}

Item::Item(Hex* hex){
	currentHex=hex;
	hex->setItem(this);
}

Item::~Item(){

}

Hex* Item::getCurrentHex(){
	return currentHex;
}

void Item::setCurrentHex(Hex* hex){
	if(currentHex!=NULL){
		if(currentHex->getItem()==this){
			currentHex->setItem(NULL);
		}
	}
	currentHex=hex;
	if(currentHex){
		currentHex->setItem(this);
	}
}

void Item::tick(int micsec){
	
}

bool Item::isBlocking(){
	return blocking;
}

void Item::damage(int dmg){

}

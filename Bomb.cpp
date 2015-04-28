#include "Bomb.h"
#include "Direction.h"
#include "TcpServer.h"
#include <stdio.h>

Bomb::Bomb():Item(){
	blocking=true;
	decayTime=3.;
}

Bomb::Bomb(Hex* hex, Player* player):Item(hex){
	owner=player;
	blocking=true;
	decayTime=3.;
}


Bomb::~Bomb(){

}

void Bomb::tick(int micsec){
	decayTime-=(double)micsec/1000000.;
	//printf("%f\n",decayTime);
	if(decayTime<=0){
		explode();
	}
}

double Bomb::getDecayTime(){
	return decayTime;
}

void Bomb::explode(){
	char msg[512];
	printf("bomb exploded at (%d,%d) with power %d and range %d\n",currentHex->getX(), currentHex->getY(),owner->getBombPower(),owner->getBombRange());
	sprintf(msg,"%d,x,%d,%d\0",owner->getId(),currentHex->getX(), currentHex->getY());
	TcpServer::getInstance()->notify(msg);
	int range;
	int power;
	Hex* hex;
	if(currentHex==owner->getCurrentHex()){
		owner->damage(1);
	}
	for(int i=0;i<6;i++){
		range=owner->getBombRange()-1;
		power=owner->getBombPower();
		hex=currentHex->getNeighbour((Direction)i);
		while(power>0 && range>0){
			if(hex){
				switch(hex->getType()){
				case empty:
					if(hex->getItem()!=NULL){
						hex->getItem()->damage(1);
					}
					hex=hex->getNeighbour((Direction)i);
					break;
				case wall:
					hex->setType(empty);
					power--;
					break;
				case strongWall:
					hex->setType(wall);
					power--;
					break;
				default:
					range=0;
					break;
				}
			}else{
				range=0;
			}
			range--;
		}
	}
	decayTime=-1;
	setCurrentHex(NULL);
}

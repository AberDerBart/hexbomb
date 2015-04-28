#include "Player.h"
#include "Bomb.h"
#include "Map.h"
#include "TcpServer.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

Player::Player():Item(){
	targetHex=currentHex;
	nextTargetHex=currentHex;
	blocking=true;
	layBomb=false;
	speed=1;
	bombCountMax=1;
	bombRange=2;
	bombPower=1;
	bombs=new Bomb*[bombCountMax];
	for(int i=0;i<bombCountMax;i++){
		bombs[i]=NULL;
	}
	sockFd=-1;
}

Player::Player(Hex* hex,int sockFd):Item(hex){
	targetHex=currentHex;
	nextTargetHex=currentHex;
	blocking=true;
	layBomb=false;
	targetHex=getCurrentHex();
	nextTargetHex=getCurrentHex();
	speed=1;
	bombRange=2;
	bombPower=1;
	bombs=new Bomb*[bombCountMax];
	for(int i=0;i<bombCountMax;i++){
		bombs[i]=NULL;
	}
	setSocket(sockFd);
}

Player::~Player(){

}

bool Player::placeBomb(){
	if(targetHex){
		layBomb=true;
		return true;
	}
	return false;
}

void Player::removeBomb(Bomb* bomb){
	int i=0;
	while(i<bombCountMax){
		if(bomb==bombs[i]){
			//delete bombs[i];
			bombs[i]=NULL;
		}
		i++;
	}
}

void Player::setBombCountMax(int count){
	Bomb** tempBombs=new Bomb*[count];
	if(count>bombCountMax){
		for(int i=0;i<bombCountMax;i++){
			tempBombs[i]=bombs[i];
		}
		for(int i=bombCountMax;i<count;i++){
			tempBombs[i]=NULL;
		}
	}else{
		for(int i=0;i<count;i++){
			tempBombs[i]=bombs[i];
		}
		for(int i=count;i<bombCountMax;i++){
			delete bombs[i];
		}
	}
	delete bombs;
	bombs=tempBombs;
	bombCountMax=count;
}

int Player::getBombRange(){
	return bombRange;
}

int Player::getBombPower(){
	return bombPower;
}

bool Player::move(){
	if(targetHex==NULL){
		return false;
	}
	if(dir==-1){
		nextTargetHex=targetHex;
		return true;
	}
	nextTargetHex=targetHex->getNeighbour(dir);
	nextTargetHexDir=dir;
	return true;
}

void Player::moveFinished(){
	if(currentHex->getItem()==this){
		currentHex->setItem(NULL);
	}
	currentHex=targetHex;
	moveProg=0;
	pickupItem(tempItem);
	if(currentHex==targetHex){
		moving=false;
	}
}

bool Player::isMoving(){
	return moving;
}

void Player::tick(int micsec){
	char msg[5];
	if(currentHex==targetHex){
		if(nextTargetHex!=currentHex){
			if(nextTargetHex!=NULL){
				if(nextTargetHex->isWalkable()){
					targetHex=nextTargetHex;
					printf("Moving to %d,%d\n",targetHex->getX(),targetHex->getY());
					tempItem=targetHex->getItem();
					targetHex->setItem(this);
					msg[0]=',';
					msg[1]='m';
					msg[2]=',';
					msg[3]=nextTargetHexDir+0x30;
					msg[4]=0;
					TcpServer::getInstance()->notify(this,msg);
				}else{
					printf("Cannot move to %d,%d\n",nextTargetHex->getX(),nextTargetHex->getY());
					nextTargetHex=targetHex;
				}
			}else{
				nextTargetHex=targetHex;
				printf("Cannot move: end of Map\n");
			}
		}
	}
	if(currentHex!=targetHex){
		moving=true;
		moveProg+=((double)micsec)*speed/1000000.;
		if(moveProg>=1){
			moveFinished();
			move();
		}
	}
	for(int i=0;i<bombCountMax;i++){
		if(bombs[i]){
			if(bombs[i]->getDecayTime()<=-1){
				delete bombs[i];
				bombs[i]=NULL;
			}else{
				bombs[i]->tick(micsec);
			}
		}
	}
	if(currentHex==targetHex){
		if(layBomb){
			int i=0;
			if(currentHex->getItem()==this){
				while(i<bombCountMax){
					if(bombs[i]==NULL){
						printf("placing bomb at %d,%d\n",currentHex->getX(),currentHex->getY());
						TcpServer::getInstance()->notify(this,(char*)",b\0");
						bombs[i]=new Bomb(currentHex,this);
						break;
					}
					i++;
				}
				layBomb=false;
			}
		}
	}
}

void Player::pickupItem(Item* item){
	delete item;
}

void Player::setSocket(int sockFd){
	this->sockFd=sockFd;
	fcntl(sockFd,F_SETFL,O_NONBLOCK);
}

void Player::processCommand(){
	char buffer[512];
	char* message;
	int end=read(sockFd,buffer,512);
	if(end>0){
		buffer[end-1]=0;
		message=buffer;
		//printf("processing message \"%s\"\n",message);
		int playerId=atoi(message);
		if(playerId!=id){
			return;
		}
		while(*message!=','){
			if(*message==0){
				return;
			}
			message++;
		}
		message++;
		char command=message[0];
		if(command=='m'){
			message++;
			if(message[0]!=','){
				return;
			}
			message++;
			dir=(Direction)atoi(message);
			message++;
			if(message[0]!=0){
				return;
			}
			move();
			return;
		}
		if(command=='b'){
			message++;
			if(message[0]!=0){
				return;
			}
			placeBomb();
			return;
		}
		if(command=='c'){
			delete TcpServer::getInstance();
			exit(0);
		}
	}
}

void Player::closeSocket(){
	printf("closing player %d's socket...\n",id);
	close(sockFd);
}

void Player::sendMessage(char* msg){
	int len=0;
	while(msg[len++]);
	int i=write(sockFd,msg,len);
	if(i==-1){
		fprintf(stderr,"Failed to send message\n");
	}
}

void Player::setCurrentHex(Hex* hex){
	Item::setCurrentHex(hex);
	if(targetHex){
		if(targetHex->getItem()==this){
			targetHex->setItem(NULL);
		}
	}
	targetHex=hex;
	nextTargetHex=hex;
}

void Player::damage(int dmg){
	if(dmg>0){
		printf("Player died\n");
		TcpServer::getInstance()->notify(this,(char*)",d");
		setCurrentHex(NULL);
		targetHex=NULL;
	}
}

int Player::getId(){
	return id;
}

void Player::setId(int id){
	this->id=id;
	int len=0;
	idStr[0]=(id/10000)%10+0x30;
	idStr[1]=(id/1000)%10+0x30;
	idStr[2]=(id/100)%10+0x30;
	idStr[3]=(id/10)%10+0x30;
	idStr[4]=id%10+0x30;
	idStr[5]=0;
	idStrP=idStr-sizeof(char);
	while(*(++idStrP)=='0' && idStrP != &idStr[4]);
}

char* Player::getIdStr(){
	return idStrP;
}

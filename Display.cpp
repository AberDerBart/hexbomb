#include "Display.h"
#include <stdio.h>
#include <unistd.h>
#include "Settings.h"
#include "Hex.h"
#include "Map.h"
#include "TcpServer.h"
#include "PlayerServer.h"

Display::Display(){
	sockFd=-1;
}

Display::~Display(){
	
}

void Display::sendMessage(char* msg){
	int len=0;
	while(msg[len++]);
	int i=write(sockFd,msg,len);
	if(i==-1){
		fprintf(stderr,"Display - Failed to send message\n");
	}
}

void Display::setSocket(int sockFd){
	this->sockFd=sockFd;
}

void Display::sendState(){
	char buffer[1024];
	int playerCount=PlayerServer::getInstance()->getPlayerCount();
	sprintf(buffer,"p,%d",playerCount);
	sendMessage(buffer);
	sprintf(buffer,"s,%d,%d",Settings::getHeight(),Settings::getWidth());
	sendMessage(buffer);
	sendMessage(Map::getInstance()->serialize());

	Hex* tmpHex;
	for(int i=0;i<PlayerServer::getInstance()->getPlayerCount();i++){
		tmpHex=PlayerServer::getInstance()->getPlayers()[i]->getCurrentHex();
		if(tmpHex){
			sprintf(buffer,"%d,l,%d,%d",i,tmpHex->getX(),tmpHex->getY());
		}else{
			sprintf(buffer,"%d,d",i);
		}
		sendMessage(buffer);
	}
}

#include "TcpServer.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "Direction.h"
#include "Map.h"
#include "Settings.h"
#include "DisplayServer.h"
#include "PlayerServer.h"

TcpServer::TcpServer(){
	printf("hexbomb deamon v0.2\n");
}

TcpServer::~TcpServer(){
}

TcpServer* TcpServer::getInstance(){
	if(instance==NULL){
		instance=new TcpServer();
	}
	return instance;
}

TcpServer* TcpServer::instance=NULL;

void TcpServer::mainloop(){
	struct timeval currentTime;
	long tickTime;
	gettimeofday(&lastTime,NULL);
	gettimeofday(&currentTime,NULL);
	int i=0;
	mutex.lock();
	while(true){
		PlayerServer::getInstance()->processCommands();
		tickTime=currentTime.tv_usec-lastTime.tv_usec;
		lastTime=currentTime;
		gettimeofday(&currentTime,NULL);
		if(tickTime<=0){
			tickTime+=1000000;
		}
		mutex.unlock();
		PlayerServer::getInstance()->tick(tickTime);
		mutex.lock();
		usleep(2000);
		fflush(stdout);
	}
}

Player* TcpServer::getPlayers(){
	return players;
}

int TcpServer::getListenSocket(){
	return listenSocket;
}

void TcpServer::notify(Player* player,char* msg){
	char msgConv[15];
	char* iter=msgConv;
	char* id=player->getIdStr();
	while(*id)*(iter++)=*(id++);
	while(*msg)*(iter++)=*(msg++);
	*iter=0;
	DisplayServer::sendMessage(msgConv);
}

void TcpServer::notify(char* msg){
	DisplayServer::sendMessage(msg);
}

void TcpServer::lockMutex(){
	mutex.lock();
}

void TcpServer::unlockMutex(){
	mutex.unlock();
}

bool TcpServer::processMessage(char* message){

}

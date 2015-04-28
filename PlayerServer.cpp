#include "PlayerServer.h"
#include "Settings.h"
#include "Map.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "TcpServer.h"

PlayerServer::~PlayerServer(){
	
}

PlayerServer* PlayerServer::getInstance(){
	if(!instance){
		instance=new PlayerServer();
	}
	return instance;
}

PlayerServer* PlayerServer::instance=NULL;

PlayerServer::PlayerServer(){
	struct addrinfo hints;
	struct addrinfo* addresses;
	struct addrinfo* ai;
	
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;
	hints.ai_protocol=0;
	hints.ai_canonname=NULL;
	hints.ai_addr=NULL;
	hints.ai_next=NULL;

	if(getaddrinfo(NULL,Settings::getPort(),&hints,&addresses)!=0){
		fprintf(stderr,"failed to get address info\n");
		exit(-1);
	}

	for(ai=addresses;ai!=NULL;ai=ai->ai_next){
		listenSocket=socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);
		if(listenSocket==-1){
			continue;
		}

		if(bind(listenSocket,ai->ai_addr,ai->ai_addrlen)==0){
			break;
		}
		close(listenSocket);
	}

	if(ai==NULL){
		fprintf(stderr,"failed to bind port %s\n",Settings::getPort());
		exit(-1);
	}

	freeaddrinfo(addresses);

	listen(listenSocket,5);

	acceptThread=new std::thread(&PlayerServer::acceptPlayers,this);
}

void PlayerServer::acceptPlayers(){
	Player* tmpPlayer;
	int id=0;
	int socket;
	while(1){
		tmpPlayer=new Player();
		socket=accept(listenSocket,NULL,NULL);
		TcpServer::getInstance()->lockMutex();
			tmpPlayer->setSocket(socket);
			printf("Player connected.\n",socket);
			tmpPlayer->setId(id);
			id++;
			tmpPlayer->setCurrentHex(Map::getInstance()->getHex(5,id));
			tmpPlayer->sendMessage(tmpPlayer->getIdStr());
			players.push_back(tmpPlayer);
		TcpServer::getInstance()->unlockMutex();
	}
}

void PlayerServer::processCommands(){
	for(int i=0;i<players.size();i++){
		players[i]->processCommand();
	}
}

void PlayerServer::tick(long tickTime){
	for(int i=0;i<players.size();i++){
		players[i]->tick(tickTime);
	}
}

std::vector<Player*> PlayerServer::getPlayers(){
	return players;
}

int PlayerServer::getPlayerCount(){
	return players.size();
}

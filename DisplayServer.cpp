#include "DisplayServer.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "null.h"
#include "Settings.h"
#include "Map.h"
#include "TcpServer.h"

DisplayServer::~DisplayServer(){
	
}

DisplayServer* DisplayServer::getInstance(){
	if(!instance){
		instance=new DisplayServer();
	}
	return instance;
}

DisplayServer* DisplayServer::instance=NULL;

DisplayServer::DisplayServer(){
	//listen on TCP port 
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

	if(getaddrinfo(NULL,Settings::getDisplayPort(),&hints,&addresses)!=0){
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
		fprintf(stderr,"failed to bind port %s\n",Settings::getDisplayPort());
		exit(-1);
	}

	freeaddrinfo(addresses);

	listen(listenSocket,5);

	printf("Listening for displays on port %s\n",Settings::getDisplayPort());

	Map::getInstance()->generate(Settings::getWidth(),Settings::getHeight());

	acceptThread=new std::thread(&DisplayServer::acceptDisplays,this);
}

void DisplayServer::acceptDisplays(){
	Display tmpDisplay;
	while(1){
		tmpDisplay.setSocket(accept(listenSocket,NULL,NULL));

		TcpServer::getInstance()->lockMutex();
			tmpDisplay.sendState();
			displays.push_back(tmpDisplay);
		TcpServer::getInstance()->unlockMutex();
	}	
}

void DisplayServer::sendMessage(char* message){
	std::vector<Display> tmpDisplays=getInstance()->displays;
	for(int i=0;i<tmpDisplays.size();i++){
		tmpDisplays[i].sendMessage(message);
	}
}

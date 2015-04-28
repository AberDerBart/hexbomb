#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Settings.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

Settings::Settings(){
	strcpy(port,"     ");
	strcpy(displayPort,"     ");
	width=0;
	height=0;
	players=0;
	int fd=open("config.ini",0,O_RDONLY);
	char buffer[1024];
	std::string line;
	buffer[read(fd,buffer,1023)]=0;
	close(fd);
	int i=0;
	int equ=0;
	while(buffer[i]){
		equ=strncmp(buffer+i,"width=",6);
		if(!equ){
			i+=6;
			width=atoi(buffer+i);
			//printf("width:%d\n",width);
		}
		equ=strncmp(buffer+i,"height=",7);
		if(!equ){
			i+=7;
			height=atoi(buffer+i);
			//printf("height:%d\n",height);
		}
		equ=strncmp(buffer+i,"players=",8);
		if(!equ){
			i+=8;
			players=atoi(buffer+i);
			//printf("players:%d\n",players);
		}
		equ=strncmp(buffer+i,"logSize=",8);
		if(!equ){
			i+=8;
			logSize=atoi(buffer+i);
			//printf("logSize:%d\n",logSize);
		}
		equ=strncmp(buffer+i,"controlport=",12);
		if(!equ){
			i+=12;
			strncpy(port,buffer+i,5);
			int j=0;
			while(port[j]>='0' && port[j]<='9'){
				j++;
			}
			port[j]=0;
			//printf("port:%s\n",port);
		}
		equ=strncmp(buffer+i,"displayport=",12);
		if(!equ){
			i+=12;
			strncpy(displayPort,buffer+i,5);
			int j=0;
			while(displayPort[j]>='0' && displayPort[j]<='9'){
				j++;
			}
			displayPort[j]=0;
			//printf("displayport:%s\n",displayPort);
		}
		while(buffer[i++]!=10);
	}
}

Settings::~Settings(){

}

Settings* Settings::getInstance(){
	if(instance==NULL){
		instance=new Settings();
	}
	return instance;
}

Settings* Settings::instance=NULL;

int Settings::getPlayerCount(){
	return getInstance()->players;
}

char* Settings::getPort(){
	return getInstance()->port;
}

char* Settings::getDisplayPort(){
	return getInstance()->displayPort;
}

int Settings::getWidth(){
	return getInstance()->width;
}

int Settings::getHeight(){
	return getInstance()->height;
}

int Settings::getLogSize(){
	return getInstance()->logSize;
}

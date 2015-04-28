#include "Map.h"
#include "Settings.h"
#include <stdio.h>
#ifndef NULL
#define NULL 0
#endif

Map::Map(){
	serialized=NULL;
}

Map::~Map(){

}

Map* Map::getInstance(){
	if(instance==NULL){
		instance=new Map();
	}
	return instance;
}

Map* Map::instance=NULL;

Hex* Map::getHex(int x,int y){
	return &data[x][y];
}

int Map::getWidth(){
	return width;
}

int Map::getHeight(){
	return height;
}

void Map::generate(int width,int height){
	mutex.lock();
	if(width==-1){
		width=Settings::getHeight();
	}
	if(height==-1){
		height=Settings::getWidth();
	}
	serialized=new char[width*height*2+2];
	this->height=height;
	this->width=width;
	data=new Hex*[width];
	for(int x=0;x<width;x++){
		data[x]=new Hex[height];
	}
	//defining neighbours
	//even
	for(int x=0;x<width;x+=2){
		for(int y=0;y<height;y++){
			data[x][y].setX(x);
			data[x][y].setY(y);
			if(y==0){
				data[x][y].setNeighbour(top,NULL);
			}else{
				data[x][y].setNeighbour(top,&data[x][y-1]);
			}
			if(x+1==width || y==0){
				data[x][y].setNeighbour(topright,NULL);
			}else{
				data[x][y].setNeighbour(topright,&data[x+1][y-1]);
			}
			if(x+1==width){
				data[x][y].setNeighbour(bottomright,NULL);
			}else{
				data[x][y].setNeighbour(bottomright,&data[x+1][y]);
			}
			if(y+1==height){
				data[x][y].setNeighbour(bottom,NULL);
			}else{
				data[x][y].setNeighbour(bottom,&data[x][y+1]);
			}
			if(x==0){
				data[x][y].setNeighbour(bottomleft,NULL);
			}else{
				data[x][y].setNeighbour(bottomleft,&data[x-1][y]);
			}
			if(x==0 || y==0){
				data[x][y].setNeighbour(topleft,NULL);
			}else{
				data[x][y].setNeighbour(topleft,&data[x-1][y-1]);
			}
		}
	}
	//uneven
	for(int x=1;x<width;x+=2){
		for(int y=0;y<height;y++){
			data[x][y].setX(x);
			data[x][y].setY(y);
			if(y==0){
				data[x][y].setNeighbour(top,NULL);
			}else{
				data[x][y].setNeighbour(top,&data[x][y-1]);
			}
			if(x+1==width){
				data[x][y].setNeighbour(topright,NULL);
			}else{
				data[x][y].setNeighbour(topright,&data[x+1][y]);
			}
			if(x+1==width || y+1==height){
				data[x][y].setNeighbour(bottomright,NULL);
			}else{
				data[x][y].setNeighbour(bottomright,&data[x+1][y+1]);
			}
			if(y+1==height){
				data[x][y].setNeighbour(bottom,NULL);
				data[x][y].setNeighbour(bottomleft,NULL);
			}else{
				data[x][y].setNeighbour(bottom,&data[x][y+1]);
				data[x][y].setNeighbour(bottomleft,&data[x-1][y+1]);
			}
			data[x][y].setNeighbour(topleft,&data[x-1][y]);
		}
	}
	mutex.unlock();
}

char* Map::serialize(){
	serialized[0]='m';
	int i=1;
	mutex.lock();
	for(int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			i+=sprintf(serialized+i,",%d",data[x][y].getType());
		}
	}
	mutex.unlock();
	return serialized;
}

#ifndef MAP_H
#define MAP_H
#include "Hex.h"
#include <mutex>
class Map{
private:
	Map();
public:
	~Map();
	static Map* getInstance();
private:
	static Map* instance;

public:
	//methods
	void generate(int width=-1,int height=-1);
	Hex* getHex(int,int);
	int getHeight();
	int getWidth();
	char* serialize();
private:
	//instance vars
	Hex** data;
	int height;
	int width;
	char* serialized;
	std::mutex mutex;
};
#endif

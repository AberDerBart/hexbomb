#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdio.h>
#include <string>

class Settings{
private:
	Settings();
	static Settings* instance;
public:
	~Settings();
public:
	static int getPlayerCount();
	static char* getPort();
	static char* getDisplayPort();
	static int getWidth();
	static int getHeight();
	static int getLogSize();
private:
	static Settings* getInstance();
	char port[6];
	char displayPort[6];
	int width;
	int height;
	int players;
	int logSize;
};

#endif

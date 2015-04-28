#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H
#include "Display.h"
//#include <pthread.h>
#include <vector>
#include <thread>
#include <mutex>

class DisplayServer{
public:
	virtual ~DisplayServer();
	static DisplayServer* getInstance();

	static void sendMessage(char* message);
	void acceptDisplays();
private:
	static DisplayServer* instance;
	DisplayServer();
	
	std::vector<Display> displays;
	int listenSocket;

	std::thread* acceptThread;
	std::mutex mutex;
};

#endif

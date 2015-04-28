#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <sys/socket.h>
#include <netinet/ip.h>
#include "Player.h"
#include <sys/time.h>
#include <netdb.h>
#include <mutex>

class TcpServer{
private:
	TcpServer();
public:
	~TcpServer();
	static TcpServer* getInstance();

	void mainloop();
	Player* getPlayers();
	int getListenSocket();
	void notify(Player*,char*);
	void notify(char*);
	void lockMutex();
	void unlockMutex();
private:
	bool processMessage(char*);
	int listenSocket;
	struct sockaddr_in address;
	Player* players;
	int playerC;
	struct timeval lastTime;
	static TcpServer* instance;
	std::mutex mutex;
};
#endif

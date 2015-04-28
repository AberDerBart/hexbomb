#ifndef PLAYERSERVER_H
#define PLAYERSERVER_H
#include "Player.h"
#include <vector>
#include <thread>
#include <mutex>

class PlayerServer{
public:
	virtual ~PlayerServer();
	static PlayerServer* getInstance();

	void acceptPlayers();
	void processCommands();
	void tick(long);

	std::vector<Player*> getPlayers();
	int getPlayerCount();
private:
	static PlayerServer* instance;
	PlayerServer();

	std::vector<Player*> players;

	int listenSocket;
	std::thread* acceptThread;
	std::mutex mutex;
};

#endif

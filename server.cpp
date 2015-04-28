#include "TcpServer.h"
#include "DisplayServer.h"
#include "Map.h"
#include "PlayerServer.h"

int main(void){
	TcpServer::getInstance();
	Map::getInstance()->generate();
	DisplayServer::getInstance();
	TcpServer::getInstance()->mainloop();
	return 0;
}

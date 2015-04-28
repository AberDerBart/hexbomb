CC=g++
LIBS=
FLAGS=-O2 -std=c++11 -pthread
hexbombd:	Map.o Hex.o TcpServer.o server.cpp Player.o Item.o Bomb.o Settings.o Display.o DisplayServer.o PlayerServer.o
	$(CC) $(FLAGS) -o hexbombd server.cpp Map.o Hex.o TcpServer.o Player.o Item.o Bomb.o Settings.o Display.o DisplayServer.o PlayerServer.o
TcpServer.o: TcpServer.cpp
	$(CC) $(FLAGS) -c TcpServer.cpp
Settings.o: Settings.cpp
	$(CC) $(FLAGS) -c Settings.cpp
Player.o: Player.cpp
	$(CC) $(FLAGS) -c Player.cpp
Map.o: Map.cpp
	$(CC) $(FLAGS) -c Map.cpp
Item.o: Item.cpp
	$(CC) $(FLAGS) -c Item.cpp
Bomb.o: Bomb.cpp
	$(CC) $(FLAGS) -c Bomb.cpp
Hex.o: Hex.cpp
	$(CC) $(FLAGS) -c Hex.cpp
Display.o: Display.cpp
	$(CC) $(FLAGS) -c Display.cpp
DisplayServer.o: DisplayServer.cpp
	$(CC) $(FLAGS) -c DisplayServer.cpp
PlayerServer.o: PlayerServer.cpp
	$(CC) $(FLAGS) -c PlayerServer.cpp
clean:
	rm -f *.o
	rm -f hexbombd
install:
	cp hexbombd /usr/local/bin

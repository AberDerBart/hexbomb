#ifndef DISPLAY_H
#define DISPLAY_H

class Display{
public:
	Display();
	virtual ~Display();

	void sendMessage(char*);
	void setSocket(int);
	void sendState();
private:
	int sockFd;
};

#endif

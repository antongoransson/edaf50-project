#include "connection.h"
#include "connectionclosedexception.h"
#include <string>
#include <memory>


class MessageHandler {
	public:
	MessageHandler(std::shared_ptr<Connection>& c);
	void sendCode(int code);
	void sendInt(int value);
	void sendIntParameter(int param);
	void sendStringParameter(std::string param);
	int recvCode();
	int recvInt();
	int recvIntParameter();
	std::string recvStringParameter();
private:
	std::shared_ptr<Connection> conn; // the connection
	// Logger logWindow; // the log window
	unsigned char recvByte();
  void sendByte(int code);
};

/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using std::cout;
using std::endl;
using std::cerr;
using std::stoi;
using std::exception;
using std::shared_ptr;
using std::make_shared;
using std::string;
/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}

void get_list_newsgroups(){
	mh.sendCode(Protocol::ANS_LIST_NG);
	vector<Pair<int,string> v = list_news_groups();
	int size = sizeof(v) / sizeof(v[0]);
	mh.sendIntParameter(size);
	result += " ";
	for(int i = 0; i < size; i++){
		mh.sendIntParameter(v[i].first);
		mh.sendStringParameter(v[i].second);
	}
	mh.sendCode(Protocol::ANS_END);
}

void create_newsgroup(string s){
	mh.sendCode(Protocol::ANS_CREATE_NG);
	if(create_news_group(s)){
		mh.sendStringParameter(ANS_ACK);
	}
	else{
		mh.sendStringParameter(ANS_NAK ERR_NG_ALREADY_EXISTS);
	}
	mh.sendCode(Protocol::ANS_END);
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				MessageHandler mh(conn);
				Protocol nbr = static_cast<Protocol>(mh.recvCode());
				string result = "";
        switch (nbr) {
          case Protocol::COM_LIST_NG: get_list_newsgroups(); break;

          case Protocol::COM_CREATE_NG: create_newsgroup(recvStringParameter()); break;

          case Protocol::COM_DELETE_NG: break;

          case Protocol::COM_LIST_ART: break;
          case Protocol::COM_CREATE_ART: break;
          case Protocol::COM_DELETE_ART: break;
          case Protocol::COM_GET_ART: break;
          case Protocol::COM_END: break;
          default: break;
        }
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();

			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}

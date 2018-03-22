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


void handle_list_articles(MessageHandler& mh) {
	 mh.send_code(Protocol::ANS_LIST_ART);
	 int nbr = mh.recv_int_parameter();
	 // pair<vector<pair<int,string>, bool> articles = db.list_articles();
	 if (!false) {
	 	mh.send_code(Protocol::ANS_NAK);
	  mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
	 } else {

	 }
	 mh.send_code(Protocol::ANS_END);
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
				Protocol nbr = static_cast<Protocol>(mh.recv_code());
				string result = "";
        switch (nbr) {
          case Protocol::COM_LIST_NG: mh.send_code(Protocol::ANS_LIST_NG); mh.send_int_parameter(0); mh.send_code(Protocol::ANS_END); break;
          case Protocol::COM_CREATE_NG:result ="CREATE NG"; break;
          case Protocol::COM_DELETE_NG: break;
          case Protocol::COM_LIST_ART: handle_list_articles(mh); break;
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

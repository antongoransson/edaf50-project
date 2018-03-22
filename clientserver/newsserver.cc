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
#include "databaseinterface.h"
#include "database.h"

using std::cout;
using std::endl;
using std::cerr;
using std::stoi;
using std::exception;
using std::shared_ptr;
using std::make_shared;
using std::string;


void handle_list_articles(MessageHandler& mh, DatabaseInterface& db) {
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

void get_list_newsgroups(MessageHandler& mh, DatabaseInterface& db){
	mh.send_code(Protocol::ANS_LIST_NG);
	vector<pair<int,string>> v = db.list_news_groups();
	int size = v.size();
	mh.send_int_parameter(size);
	for(int i = 0; i < size; i++){
		mh.send_int_parameter(v[i].first);
		mh.send_string_parameter(v[i].second);
	}
	mh.send_code(Protocol::ANS_END);
}

void handle_create_news_group(MessageHandler& mh, DatabaseInterface& db){
	mh.send_code(Protocol::ANS_CREATE_NG);
	if(db.create_news_group(mh.recv_string_parameter())){
		mh.send_code(Protocol::ANS_ACK);
	}
	else{
		mh.send_code(Protocol::ANS_NAK);
		mh.send_code(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	mh.send_code(Protocol::ANS_END);
}

void handle_delete_news_group(MessageHandler& mh, DatabaseInterface& db){
	mh.send_code(Protocol::ANS_DELETE_NG);
	if(db.delete_news_group(mh.recv_int_parameter())){
		mh.send_code(Protocol::ANS_ACK);
	}
	else{
		mh.send_code(Protocol::ANS_NAK);
		mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.send_code(Protocol::ANS_END);
}

void handle_create_article(MessageHandler& mh, DatabaseInterface& db){
	mh.send_code(Protocol::ANS_CREATE_ART);
	if(create_article(mh.recv_int_parameter(), mh.recv_string_parameter(), mh.recv_string_parameter(), mh.recv_string_parameter())){
		mh.send_code(Protocol::ANS_ACK);
	}
	else{
		mh.send_code(Protocol::ANS_NAK);
		mh.send_code(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	mh.send_code(Protocol::ANS_END);
}

void handle_delete_article(MessageHandler& mh, DatabaseInterface& db){
	mh.send_code(Protocol::ANS_DELETE_ART);
	int delete_int = delete_article(mh.recv_int_parameter(), mh.recv_int_parameter());
	if(delete_int == 1){
		mh.send_code(Protocol::ANS_ACK);
	}
	else if(delete_int == 2){
		mh.send_code(Protocol::ANS_NAK);
		mh.send_code(Protocol::ERR_ART_DOES_NOT_EXIST);
	}
	else{
		mh.send_code(Protocol::ANS_NAK);
		mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.send_code(Protocol::ANS_END);
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	Database db;
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
          case Protocol::COM_LIST_NG: get_list_newsgroups(mh, db); break;
          case Protocol::COM_CREATE_NG: handle_create_news_group(mh, db); break;
          case Protocol::COM_DELETE_NG: handle_delete_news_group(mh, db); break;
					case Protocol::COM_LIST_ART: handle_list_articles(mh, db); break;
          case Protocol::COM_CREATE_ART: handle_create_article(mh, db); break;
          case Protocol::COM_DELETE_ART: handle_delete_article(mh, db); break;
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

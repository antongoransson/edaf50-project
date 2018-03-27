/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::exception;
using std::stoi;

void handle_list_newsgroups(MessageHandler& mh) {
	mh.send_code(Protocol::COM_LIST_NG);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	int nbr = mh.recv_int_parameter();
	if (nbr == 0 ) {
		cout << "No groups exists use command: 2 to create one" << endl;
	} else {
		for (int i = 0; i != nbr; ++i) {
			int grpID = mh.recv_int_parameter();
			string name = mh.recv_string_parameter();
			cout << "Group id:  " << grpID << "\t   " << "Name: " << name << endl;
		}
	}
	mh.recv_code();
}

void handle_create_newsgroups(MessageHandler& mh) {
	string name, temp;
	cout << "Enter name of newsgroup: ";
	cin.ignore();
	getline(cin,name);
	mh.send_code(Protocol::COM_CREATE_NG);
	mh.send_string_parameter(name);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		cout << "Newsgroup with name: " << name << " created" << endl;
	} else {
		cout << "Newsgroup with name: " << name << "alrady exists" << endl;
	}
	mh.recv_code();
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	MessageHandler mh(conn);
	cout << "Welcome to our news system!" << endl << "Avaible commands:" << endl;
	cout << "1. List newsgroups" << endl;
	cout << "2. Create newsgroup" << endl;
	cout << "3. Delete newsgroup" << endl;
	cout << "4. List articles" << endl;
	cout << "5. Create an article" << endl;
	cout << "6. Delete an article" << endl;
	cout << "7. Get an article" << endl;
	cout << "8. To list these instructions again" << endl;
	cout << "------------------------------------" << endl;

	int command;
	while (cin >> command) {
		try {
			switch (command) {
				case 1: handle_list_newsgroups(mh); break;
				case 2: handle_create_newsgroups(mh); break;
				case 3: break;
				case 4: break;
				case 5: break;
				case 6: break;
				case 7: break;
				case 8: break;
				default: break;

			}
			cout << "------------------------------------" << endl;
			// writeNumber(conn, nbr);
			// string reply = readString(conn);
			// cout << " " << reply << endl;
			// cout << "Type another number: ";
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

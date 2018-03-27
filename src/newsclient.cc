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

void error_message_handler(Protocol& res) {
	switch (res) {
		case Protocol::ERR_NG_DOES_NOT_EXIST:
			cout << "Newsgroup does not exist" << endl;
		 	break;
		case Protocol::ERR_NG_ALREADY_EXISTS:
			cout << "Newsgroup already exists" << endl;
			break;
		case Protocol::ERR_ART_DOES_NOT_EXIST:
			cout << "Article does not exist" << endl;
		 	break;
		default:
			cout << "Something went wrong" << endl;
			break;
	}
}
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
			cout << "Group id: " << grpID << "\t   " << "Name: " << name << endl;
		}
	}
	mh.recv_code();
}

void handle_create_newsgroups(MessageHandler& mh) {
	string name;
	cout << "Enter name of newsgroup: ";
	cin.ignore();
	getline(cin, name);
	mh.send_code(Protocol::COM_CREATE_NG);
	mh.send_string_parameter(name);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		cout << "Newsgroup with name: " << name << " created" << endl;
	} else {
		res = mh.recv_code();
		error_message_handler(res);
	}
	mh.recv_code();
}

void handle_delete_newsgroup(MessageHandler& mh) {
	string name, temp;
	int id;
	cout << "Enter id of newsgroup to delete: ";
	cin >> id;
	mh.send_code(Protocol::COM_DELETE_NG);
	mh.send_int_parameter(id);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		cout << "Newsgroup with id: " << id << " deleted" << endl;
	} else {
		res = mh.recv_code();
		error_message_handler(res);
	}
	mh.recv_code();
}

void handle_list_articles(MessageHandler& mh) {
	int id;
	cout << "Enter number of newsgroup: ";
	cin >> id;

	mh.send_code(Protocol::COM_LIST_ART);
	mh.send_int_parameter(id);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		int nbr = mh.recv_int_parameter();
		if (nbr == 0 ) {
			cout << "No artcles exists in that group use command: 5 to create one" << endl;
		} else {
			for (int i = 0; i != nbr; ++i) {
				int grpID = mh.recv_int_parameter();
				string name = mh.recv_string_parameter();
				cout << "Article id:  " << grpID << "\t   " << "Name: " << name << endl;
			}
		}
	} else {
		res = mh.recv_code();
		error_message_handler(res);
	}
	mh.recv_code();
}

void handle_delete_article(MessageHandler& mh) {
	int grpID;
	cout << "Enter id of the article's newsgroup : ";
	cin >> grpID;
	int artID;
	cout << "Enter id of the article : ";
	cin >> artID;
	mh.send_code(Protocol::COM_DELETE_ART);
	mh.send_int_parameter(grpID);
	mh.send_int_parameter(artID);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		cout << "Article deleted" << endl;
	} else {
		res = mh.recv_code();
		error_message_handler(res);
	}
	mh.recv_code();
}


void handle_create_article(MessageHandler& mh) {
	string title, author, text;
	int id;
	cout << "Enter id of newsgroup: ";
	cin >> id;

	cout << "Enter title of article: ";
	cin.ignore();
	getline(cin,title);
	cout << endl;
	cout << "Enter author of article: ";
	cin.ignore();
	getline(cin,author);
	cout << endl;
	cout << "Enter text of article: ";
	cin.ignore();
	getline(cin,text);
	cout << endl;
	mh.send_code(Protocol::COM_CREATE_ART);
	mh.send_int_parameter(id);
	mh.send_string_parameter(title);
	mh.send_string_parameter(author);
	mh.send_string_parameter(text);
	mh.send_code(Protocol::COM_END);
	mh.recv_code();
	Protocol res = mh.recv_code();
	if (res == Protocol::ANS_ACK) {
		cout << "Article with title: " << title << " created" << endl;
	} else {
		res = mh.recv_code();
		error_message_handler(res);
	}
	mh.recv_code();
}

void list_instructions() {
	cout << "1. List newsgroups" << endl;
	cout << "2. Create newsgroup" << endl;
	cout << "3. Delete newsgroup" << endl;
	cout << "4. List articles" << endl;
	cout << "5. Create an article" << endl;
	cout << "6. Delete an article" << endl;
	cout << "7. Get an article" << endl;
	cout << "8. To list these instructions again" << endl;
	cout << "0. To exit the program" << endl;
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
 	cout << "Welcome to our news system!" << endl << "Avaible commands:" << endl;
	list_instructions();
	cout << "------------------------------------" << endl;
	cout << "Enter command: ";

	MessageHandler mh(conn);
	int command;
	while (cin >> command) {
		try {
			switch (command) {
				case 1: handle_list_newsgroups(mh); break;
				case 2: handle_create_newsgroups(mh); break;
				case 3: handle_delete_newsgroup(mh); break;
				case 4: handle_list_articles(mh); break;
				case 5: handle_create_article(mh); break;
				case 6: handle_delete_article(mh); break;
				case 7: break;
				case 8: list_instructions(); break;
				case 0:
					cout << "Exiting... Thanks for your visit!";
					exit(0); break;
				default:
				 	cout << "Non existing command use on of the following:" << endl;
					list_instructions();
				break;

			}
			cout << "------------------------------------" << endl;
			cout << "Enter new command: " ;
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

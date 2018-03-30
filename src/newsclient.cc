#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::exception;
using std::stoi;
using std::numeric_limits;
using std::streamsize;

void error_message_handler(const Protocol& res) {
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
      cout << "Protocol was violated, please try again" << endl << "Exiting... " << endl;
      exit(1);
  }
}
// This should never be needed, but better safe than sorry
void handle_protocol_ans(const Protocol& res, const Protocol& expected) {
  if (res != expected) {
    cout << endl << "Protocol was violated, please try again" << endl << "Exiting..." << endl;
    exit(1);
  }
}

void check_for_eof() {
  if (cin.eof()) {
    cout << endl << "Exiting... Thanks for your visit!" << endl;
    exit(0);
  }
}

int read_int_cin() {
  int temp;
  while (!(cin >> temp)) {
    check_for_eof();
    cout << "Please write a number: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  return temp;
}

string read_line_cin() {
  string temp;
  getline(cin, temp);
  while(temp.size() == 0) {
    cout << "Please write at least one character: ";
    getline(cin, temp);
  }
  check_for_eof();
  return temp;
}

void handle_list_newsgroups(MessageHandler& mh) {
  mh.send_code(Protocol::COM_LIST_NG);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_LIST_NG);
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
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}

void handle_create_newsgroups(MessageHandler& mh) {
  cout << "Enter name of newsgroup: ";
  cin.ignore();
  string name = read_line_cin();
  mh.send_code(Protocol::COM_CREATE_NG);
  mh.send_string_parameter(name);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_CREATE_NG);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    cout << "Newsgroup with name: " << name << " created" << endl;
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}

void handle_delete_newsgroup(MessageHandler& mh) {
  cout << "Enter id of newsgroup to delete: ";
  int grpID = read_int_cin();
  mh.send_code(Protocol::COM_DELETE_NG);
  mh.send_int_parameter(grpID);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_DELETE_NG);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    cout << "Newsgroup with id: " << grpID << " deleted" << endl;
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}

void handle_list_articles(MessageHandler& mh) {
  cout << "Enter id of newsgroup: ";
  int grpID = read_int_cin();
  mh.send_code(Protocol::COM_LIST_ART);
  mh.send_int_parameter(grpID);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_LIST_ART);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    int nbr = mh.recv_int_parameter();
    if (nbr == 0 ) {
      cout << "No articles exists in that group, use command: 5 to create one" << endl;
    } else {
      for (int i = 0; i != nbr; ++i) {
      int artID = mh.recv_int_parameter();
      string name = mh.recv_string_parameter();
      cout << "Article id: " << artID << "\t   " << "Name: " << name << endl;
      }
    }
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}



void handle_create_article(MessageHandler& mh) {
  cout << "Enter id of newsgroup: ";
  int grpID = read_int_cin();
  cout << "Enter title of article: ";
  cin.ignore();
  string title = read_line_cin();
  cout << "Enter author of article: ";
  string author = read_line_cin();
  cout << "Enter text of article (Finish with a '#done' on an empy line): " << endl;
  string text, line;
  while (getline(cin, line) && line != "#done") {
    text += line + "\n";
  }
  check_for_eof();
  cin.clear();
  text.pop_back(); // Removes last newline
  mh.send_code(Protocol::COM_CREATE_ART);
  mh.send_int_parameter(grpID);
  mh.send_string_parameter(title);
  mh.send_string_parameter(author);
  mh.send_string_parameter(text);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_CREATE_ART);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    cout << "Article with title: " << title << " created" << endl;
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}

void handle_delete_article(MessageHandler& mh) {
  cout << "Enter id of the article's newsgroup : ";
  int grpID = read_int_cin();
  cout << "Enter id of the article : ";
  int artID = read_int_cin();
  mh.send_code(Protocol::COM_DELETE_ART);
  mh.send_int_parameter(grpID);
  mh.send_int_parameter(artID);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_DELETE_ART);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    cout << "Article deleted" << endl;
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}

void handle_get_article(MessageHandler& mh) {
  cout << "Enter id of the article's newsgroup : ";
  int grpID = read_int_cin();
  cout << "Enter id of the article : ";
  int artID = read_int_cin();
  mh.send_code(Protocol::COM_GET_ART);
  mh.send_int_parameter(grpID);
  mh.send_int_parameter(artID);
  mh.send_code(Protocol::COM_END);
  Protocol ans = mh.recv_code();
  handle_protocol_ans(ans, Protocol::ANS_GET_ART);
  Protocol res = mh.recv_code();
  if (res == Protocol::ANS_ACK) {
    string title = mh.recv_string_parameter();
    string author = mh.recv_string_parameter();
    string text = mh.recv_string_parameter();
    cout << "------------------------------------" << endl;
    cout << "Title: " << title << endl << "Author: " << author << endl << text << endl;
  } else {
    res = mh.recv_code();
    error_message_handler(res);
  }
  Protocol end = mh.recv_code();
  handle_protocol_ans(end, Protocol::ANS_END);
}


void list_instructions() {
  cout << "1. List newsgroups" << endl;
  cout << "2. Create newsgroup" << endl;
  cout << "3. Delete newsgroup" << endl;
  cout << "4. List articles" << endl;
  cout << "5. Create an article" << endl;
  cout << "6. Delete an article" << endl;
  cout << "7. Get an article" << endl;
  cout << "8. List these instructions again" << endl;
  cout << "0. Exit the program (Ctrl+D) " << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cerr << "Usage: newclient host-name port-number" << endl;
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
  while (true) {
    if (cin >> command) {
      try {
        switch (command) {
          case 1: handle_list_newsgroups(mh); break;
          case 2: handle_create_newsgroups(mh); break;
          case 3: handle_delete_newsgroup(mh); break;
          case 4: handle_list_articles(mh); break;
          case 5: handle_create_article(mh); break;
          case 6: handle_delete_article(mh); break;
          case 7: handle_get_article(mh); break;
          case 8: list_instructions(); break;
          case 0:
            cout << "Exiting... Thanks for your visit!";
            exit(0);
          default:
            cout << "Non existing command use on of the following:" << endl;
            list_instructions();
          break;
        }
        cout << "------------------------------------" << endl;
        cout << "Enter new command: " ;
      } catch (ConnectionClosedException&) {
        cout << "No reply from server. The protocol was possbily violated. Exiting..." << endl;
        exit(1);
      }
    } else {
      check_for_eof();
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid command, please choose one from the list: " << endl;
      list_instructions();
      cout << "------------------------------------" << endl;
      cout << "Enter new command: " ;
    }
  }
}

#include "databaseinterface.h"
#include "database.h"
#include "diskdatabase.h"
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
using std::pair;
using std::vector;

void handle_list_newsgroups(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_LIST_NG);
  vector<pair<int, string>> v = db.list_news_groups();
  int size = v.size();
  mh.send_int_parameter(size);
  for (const auto& p: v) {
    mh.send_int_parameter(p.first);
    mh.send_string_parameter(p.second);
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_create_news_group(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_CREATE_NG);
  if (db.create_news_group(mh.recv_string_parameter())) {
    mh.send_code(Protocol::ANS_ACK);
  } else {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_NG_ALREADY_EXISTS);
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_delete_news_group(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_DELETE_NG);
  int grpID = mh.recv_int_parameter();
  if (db.delete_news_group(grpID)) {
    mh.send_code(Protocol::ANS_ACK);
  } else {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_list_articles(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_LIST_ART);
  int nbr = mh.recv_int_parameter();
  pair<vector<pair<int, string>>, bool> articles = db.list_articles(nbr);
  if (!articles.second) {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
  } else {
    mh.send_code(Protocol::ANS_ACK);
    vector<pair<int, string>> v = articles.first;
    int size = v.size();
    mh.send_int_parameter(size);
    for (const auto& p: v) {
      mh.send_int_parameter(p.first);
      mh.send_string_parameter(p.second);
    }
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_create_article(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_CREATE_ART);
  int grpID = mh.recv_int_parameter();
  string title = mh.recv_string_parameter();
  string author = mh.recv_string_parameter();
  string text = mh.recv_string_parameter();
  if (db.create_article(grpID, title, author, text)) {
    mh.send_code(Protocol::ANS_ACK);
  } else {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_delete_article(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_DELETE_ART);
  int grpID = mh.recv_int_parameter();
  int artID = mh.recv_int_parameter();
  int delete_int = db.delete_article(grpID, artID);
  if (delete_int == OK) { // OK = constant from database
    mh.send_code(Protocol::ANS_ACK);
  } else if (delete_int == NO_ARTICLE) {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_ART_DOES_NOT_EXIST);
  } else {
    mh.send_code(Protocol::ANS_NAK);
    mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
  }
  mh.send_code(Protocol::ANS_END);
}

void handle_get_article(MessageHandler& mh, DatabaseInterface& db) {
  mh.send_code(Protocol::ANS_GET_ART);
  int grpID = mh.recv_int_parameter();
  int artID = mh.recv_int_parameter();
  pair<Article, int> p =  db.get_article(grpID, artID);
  if (p.second == OK) {
    mh.send_code(Protocol::ANS_ACK);
    Article a = p.first;
    mh.send_string_parameter(a.get_title());
    mh.send_string_parameter(a.get_author());
    mh.send_string_parameter(a.get_text());
  } else {
    mh.send_code(Protocol::ANS_NAK);
    if (p.second == NO_ARTICLE) {
      mh.send_code(Protocol::ERR_ART_DOES_NOT_EXIST);
    } else {
      mh.send_code(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
  }
  mh.send_code(Protocol::ANS_END);
}

int main(int argc, char* argv[]) {
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
  // Database db;
  DiskDatabase db;
  while (true) {
    auto conn = server.waitForActivity();
    if (conn != nullptr) {
      try {
        MessageHandler mh(conn);
        Protocol nbr = mh.recv_code();
        switch (nbr) {
          case Protocol::COM_LIST_NG: handle_list_newsgroups(mh, db); break;
          case Protocol::COM_CREATE_NG: handle_create_news_group(mh, db); break;
          case Protocol::COM_DELETE_NG: handle_delete_news_group(mh, db); break;
          case Protocol::COM_LIST_ART: handle_list_articles(mh, db); break;
          case Protocol::COM_CREATE_ART: handle_create_article(mh, db); break;
          case Protocol::COM_DELETE_ART: handle_delete_article(mh, db); break;
          case Protocol::COM_GET_ART: handle_get_article(mh, db); break;
          default: throw ConnectionClosedException(); break; // Client didn't follow protocol
        }
        if(mh.recv_code() != Protocol::COM_END) {
          throw ConnectionClosedException();
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

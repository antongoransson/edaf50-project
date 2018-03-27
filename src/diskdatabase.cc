#include "diskdatabase.h"

#include <iostream>
#include <fstream>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::pair;
using std::make_pair;

vector<pair<int, string>> DiskDatabase::list_news_groups() const {
  sqlite3_stmt *statement;
  vector<pair<int, string>> pairs;
  const char* query = "SELECT * FROM newsgroups";
  if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK) {
    while (1) {
      int res = sqlite3_step(statement);
      if (res == SQLITE_ROW) {
        int grpID = sqlite3_column_int(statement, 0);
        string ng_name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
        pairs.push_back(make_pair(grpID, ng_name));
      }
      if (res == SQLITE_DONE || res == SQLITE_ERROR) {
        break;
      }
    }
  }
  return pairs;
}

bool DiskDatabase::create_news_group(const string& title) {
  sqlite3_stmt *statement;
  const char* query = "INSERT INTO newsgroups (ng_name) values (?)";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_text(statement, 1, title.c_str(), title.size(), NULL);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (res == SQLITE_DONE) {
      return true;
    }
  }
  return false;
}

bool DiskDatabase::delete_news_group(int grpID) {
  if (!news_group_exists(grpID)) {
    return false;
  }
  sqlite3_stmt *statement;
  const char *query = "DELETE FROM newsgroups WHERE grpID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_step(statement);
    sqlite3_finalize(statement);
  }
  return true;
}

pair<vector<pair<int, string>>, bool> DiskDatabase::list_articles(int grpID) const {
  if (!news_group_exists(grpID)) {
    return make_pair(get_articles(grpID), false);
  }
  return make_pair(get_articles(grpID), true);
}

bool DiskDatabase::create_article(int grpID, const string& title, const string& author, const string& text) {
  sqlite3_stmt* statement;
  const char* query = "INSERT INTO articles (title, author, txt, grpID) values (?,?,?,?)";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_text(statement, 1,title.c_str(), title.size(), NULL);
    sqlite3_bind_text(statement, 2, author.c_str(), author.size(), NULL);
    sqlite3_bind_text(statement, 3, text.c_str(), text.size(), NULL);
    sqlite3_bind_int(statement, 4, grpID);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (res == SQLITE_DONE) {
      return true;
    }
  }
  return false;
}


int DiskDatabase::delete_article(int grpID, int artID) {
  if (!news_group_exists(grpID)) {
    return 3;
  }
  if (!article_exists(grpID, artID)) {
    return 2;
  }
  sqlite3_stmt *statement;

  const char *query = "DELETE FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    sqlite3_step(statement);
    sqlite3_finalize(statement);
  }
  return 1;
}

pair<Article, int> DiskDatabase::get_article(int grpID, int artID) const {
  Article art;
  if (!news_group_exists(grpID)) {
    return make_pair(art, 3);
  }
  if (!article_exists(grpID, artID)) {
    return make_pair(art, 2);
  }
  sqlite3_stmt *statement;
  const char *query = "SELECT * FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    sqlite3_step(statement);
    int art_i = sqlite3_column_int(statement, 0);
    string title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
    string author = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
    string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
    int grp_i = sqlite3_column_int(statement, 4);
    art = Article(art_i, grp_i, title, author, text);
  }
  return make_pair(art, 1);
}

bool file_exist(const string& fileName) {
    ifstream infile(fileName);
    return infile.good();
}

bool DiskDatabase::connect(const char* db_file) {
  const char* init = "PRAGMA foreign_keys = ON";
  bool res = sqlite3_open(db_file, &db) == SQLITE_OK;
  sqlite3_exec(db, init, NULL, 0, NULL);
  return res;
}

vector<pair<int, string>> DiskDatabase::get_articles(int grpID) const {
  sqlite3_stmt *statement;
  vector<pair<int, string>> pairs;
  const char* query = "SELECT * FROM articles WHERE grpID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    int res = 0;
    while (1) {
      res = sqlite3_step(statement);
      if (res == SQLITE_ROW) {
        int artID = sqlite3_column_int(statement, 0);
        string title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
        pairs.push_back(make_pair(artID, title));
      }
      if (res == SQLITE_DONE || res == SQLITE_ERROR) {
        break;
      }
    }
  }
  return pairs;
}

void DiskDatabase::initializeDB(const char* db_file) {
  const int STATEMENTS = 2;
	char* zErrMsg = 0;
	const char* pSQL[STATEMENTS];
  pSQL[0] = "CREATE TABLE newsgroups (\
              grpID INTEGER, \
              ng_name TEXT NOT NULL UNIQUE, \
              PRIMARY KEY(grpID)\
            );";
  pSQL[1] = "CREATE TABLE articles (\
              artID INTEGER, \
              title TEXT, \
              author TEXT,\
              txt TEXT, \
              grpID INTEGER, \
              PRIMARY KEY(artID),\
              FOREIGN KEY (grpID) REFERENCES newsgroups(grpID) ON DELETE CASCADE\
            );";

  if (!connect(db_file)) {
  	cout << "Can't open database: "<< sqlite3_errmsg(db) << "\n";
  }	else {
    for(int i = 0; i < STATEMENTS; i++) {
      if (sqlite3_exec(db, pSQL[i], NULL, 0, &zErrMsg) != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << "\n";
        sqlite3_free(zErrMsg);
        break;
      }
    }
  }
}

bool DiskDatabase::news_group_exists(int grpID) const {
  sqlite3_stmt *statement;
  const char *query = "SELECT * FROM newsgroups WHERE grpID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (res == SQLITE_DONE) {
      return false;
    }
  }
  return true;
}

bool DiskDatabase::article_exists(int grpID, int artID) const  {
  sqlite3_stmt* statement;

  const char* query = "SELECT * FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (res == SQLITE_DONE) {
      return false;
    }
  }
  return true;
}

DiskDatabase::DiskDatabase() {
  const char* db_file("news.db");
  if (!file_exist(db_file)) {
    initializeDB(db_file);
  } else {
    if (!connect(db_file)) {
      cout << "Can't open database: " << sqlite3_errmsg(db) << "\n";
    }
  }
}

DiskDatabase::~DiskDatabase() {
  sqlite3_close(db);
}

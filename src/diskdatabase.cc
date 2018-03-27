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

vector<pair<int, string>> DiskDatabase::get_table_data(int grpID) const {
  sqlite3_stmt *statement;
  vector<pair<int, string>> pairs;
  const char* query = "SELECT * FROM articles WHERE grpID = ?";
  if (grpID == -1) {
    query = "SELECT * FROM newsgroups";
  }

  if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    int res = 0;
    while (1) {
      res = sqlite3_step(statement);
      if (res == SQLITE_ROW) {
        pairs.push_back(make_pair(
          sqlite3_column_int(statement, 0),
          reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)))
        );
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
    cout << res << endl;
    if (res == SQLITE_DONE) {
      return true;
    }
  }
  return false;
}

bool DiskDatabase::create_article(int grpID, const string& title, const string& author, const string& text) {
  sqlite3_stmt *statement;
  const char *query = "INSERT INTO articles (title, author, txt, grpID) values (?,?,?,?)";
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

void DiskDatabase::delete_ng(unsigned int grpID) {
  sqlite3_stmt *statement;

  const char *query = "DELETE FROM newsgroups WHERE grpID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
  }
}

void DiskDatabase::delete_art(unsigned int grpID, unsigned int artID) {
  sqlite3_stmt *statement;

  const char *query = "DELETE FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    sqlite3_step(statement);
    sqlite3_finalize(statement);
  }
}

bool DiskDatabase::has_ng(unsigned int grpID) const {
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

bool DiskDatabase::has_art(unsigned int grpID, unsigned int artID) const  {
  sqlite3_stmt *statement;

  const char *query = "SELECT * FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    int res = sqlite3_step(statement);
    sqlite3_finalize(statement);
    cout << res << endl;
    if (res == SQLITE_DONE) {
      return false;
    }
  }
  return true;
}
Article DiskDatabase::get_art(unsigned int grpID, unsigned int artID) const {
  sqlite3_stmt *statement;
  int art_i, grp_i;
  string title, author, text;
  const char *query = "SELECT * FROM articles WHERE grpID = ? AND artID = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    sqlite3_bind_int(statement, 2, artID);
    int ctotal = sqlite3_column_count(statement);
    int res = 0;
    res = sqlite3_step(statement);
    art_i = sqlite3_column_int(statement, 0);
    title = (char*)sqlite3_column_text(statement, 1);
    author = (char*)sqlite3_column_text(statement, 2);
    text = (char*)sqlite3_column_text(statement, 3);
    grp_i = sqlite3_column_int(statement, 4);
  }

  return Article(art_i,grp_i,title,author,text);
}


void DiskDatabase::createDB(const char* db_file) {
  const int STATEMENTS = 2;
	char *zErrMsg = 0;
	const char *pSQL[STATEMENTS];
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
  	cout << "Open database successfully\n\n";
    for(int i = 0; i < STATEMENTS; i++) {
      if (sqlite3_exec(db, pSQL[i], NULL, 0, &zErrMsg) != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << "\n";
        sqlite3_free(zErrMsg);
        break;
      }
    }
  }
}

DiskDatabase::DiskDatabase() {
  const char* db_file("news.db");
  if (!file_exist(db_file)) {
    createDB(db_file);
  } else {
    if (!connect(db_file)) {
      cout<<"Can't open database: " << sqlite3_errmsg(db) << "\n";
    }	else {
      cout<<"Open database successfully\n\n";
    }
  }
}

DiskDatabase::~DiskDatabase() {
  sqlite3_close(db);
}

vector<pair<int, string>> DiskDatabase::list_news_groups() const {
  return get_table_data(-1);

}

bool DiskDatabase::delete_news_group(int grpID) {
  if (!has_ng(grpID)) {
    return false;
  }
  delete_ng(grpID);
  return true;
}

pair<vector<pair<int, string>>, bool> DiskDatabase::list_articles(int grpID) const {
  if (!has_ng(grpID)) {
    return make_pair(get_table_data(grpID), false);
  }
  return make_pair(get_table_data(grpID), true);
}



int DiskDatabase::delete_article(int grpID, int artID) {
  if (!has_ng(grpID)) {
    return 3;
  }
  if (!has_art(grpID,artID)) {
    return 2;
  }
  delete_art(grpID,artID);
  return 1;
}

pair<Article, int> DiskDatabase::get_article(int grpID, int artID) const {
  Article art;
  if (!has_ng(grpID)) {
    return make_pair(art, 3);
  }
  if (!has_art(grpID, artID)) {
    return make_pair(art, 2);
  }
  art = get_art(grpID, artID);
  return make_pair(art, 1);
}

#include "diskdatabase.h"

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;

bool file_exist(const string fileName) {
    ifstream infile(fileName);
    return infile.good();
}
bool DiskDatabase::connect(const char* db_file) {
  return sqlite3_open(db_file, &db) == SQLITE_OK;
}

void DiskDatabase::getArticles(unsigned int grpID) {
  sqlite3_stmt *statement;
  const char *query = "SELECT * FROM articles WHERE ng_id = ?";
  if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK) {
    sqlite3_bind_int(statement, 1, grpID);
    int ctotal = sqlite3_column_count(statement);
    int res = 0;

    while (1) {
      res = sqlite3_step(statement);
      if (res == SQLITE_ROW) {
        for (int i = 0; i < ctotal; i++) {
            string s = (char*)sqlite3_column_text(statement, i);
            // print or format the output as you want
            cout << s << " " ;
        }
        cout << endl;
      }
      if (res == SQLITE_DONE || res == SQLITE_ERROR) {
        cout << "done " << endl;
        break;
      }
    }
  }
}

void DiskDatabase::getTableData() {
  sqlite3_stmt *statement;
  const char *query = "SELECT * FROM articles";
  if (sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK) {
    int ctotal = sqlite3_column_count(statement);
    int res = 0;

    while (1) {
      res = sqlite3_step(statement);
      if (res == SQLITE_ROW) {
        for (int i = 0; i < ctotal; ++i) {
            string s = (char*)sqlite3_column_text(statement, i);
            // print or format the output as you want
            cout << s << " " ;
        }
        cout << endl;
      }
      if (res == SQLITE_DONE || res == SQLITE_ERROR) {
        cout << "done " << endl;
        break;
      }
    }
  }
}

void DiskDatabase::createDB(const char* db_file) {
  cout << "Creating DB" << endl;
  const int STATEMENTS = 4;
	char *zErrMsg = 0;
	const char *pSQL[STATEMENTS];
  pSQL[0] = "CREATE TABLE newsgroups (\
              ng_id INTEGER, \
              ng_name TEXT NOT NULL, \
              PRIMARY KEY(ng_id)\
            );";
  pSQL[1] = "CREATE TABLE articles (\
              art_id INTEGER, \
              title TEXT, \
              author TEXT,\
              txt TEXT, \
              ng_id INTEGER, \
              PRIMARY KEY(art_id),\
              FOREIGN KEY (ng_id) REFERENCES newsgroups(ng_id)\
            );";
	pSQL[2] = "INSERT \
            INTO newsgroups (ng_name) \
            values ('Sports'),\
                    ('International'),\
                    ('Jokes');";

	pSQL[3] = "INSERT \
            INTO articles (title, author, txt, ng_id) \
            values ('SWEDEN GOGO', 'Calle', 'Sweden once again takes a victor', 1), \
                    ('DANMARK NONO', 'Cölle', 'Danmark once again loses', 1), \
                    ('KAOZ I USA', 'Trump', 'Oh no, here we go again', 2), \
                    ('Norskt skämt', 'Björne', 'Varför öppnar norrmännen mjölkpaketen i affären? Det står öppna här', 3);";
  int rc = sqlite3_open(db_file, &db);
  if(!connect(db_file)) {
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
  getArticles(1); // Just to test functions
  getArticles(3);
}

DiskDatabase::~DiskDatabase() {
  sqlite3_close(db);
}

std::vector<std::pair<int, string>> DiskDatabase::list_news_groups() {

}
std::pair<std::vector<std::pair<int, string>>, bool> DiskDatabase::list_articles(int grpID) {

}
bool DiskDatabase::create_news_group(string name) {

}
bool DiskDatabase::create_article(int grpID, string name, string author, string text) {

}
bool DiskDatabase::delete_news_group(int grpID) {

}
int DiskDatabase::delete_article(int grpID, int artID) {

}
std::pair<Article, int> DiskDatabase::get_article(int grpID, int artID) {

}

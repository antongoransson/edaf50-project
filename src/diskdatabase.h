#ifndef DISK_DATABASE_H
#define DISK_DATABASE_H

#include "databaseinterface.h"
#include "article.h"
#include "newsgroup.h"
#include "sqlite3.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

class DiskDatabase: public DatabaseInterface {
public:
  DiskDatabase();
  ~DiskDatabase();
  std::vector<std::pair<int, std::string>> list_news_groups() override;
  std::pair<std::vector<std::pair<int, std::string>>, bool> list_articles(int grpID) override;
  bool create_news_group(std::string name) override;
  bool create_article(int grpID, std::string name, std::string author, std::string text) override;
  bool delete_news_group(int grpID) override;
  int delete_article(int grpID, int artID) override;
  std::pair<Article, int> get_article(int grpID, int artID) override;
private:
  bool connect(const char*);
  void createDB(const char* db_file);
  void getTableData();
  void getArticles(unsigned int grpID);
  sqlite3* db;
};

#endif

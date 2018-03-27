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
  std::vector<std::pair<int, std::string>> list_news_groups() const override;
  bool create_news_group(const std::string& name) override;
  bool delete_news_group(int grpID) override;
  std::pair<std::vector<std::pair<int, std::string>>, bool> list_articles(int grpID) const override;
  bool create_article(int grpID, const std::string& name, const std::string& author, const std::string& text) override;
  int delete_article(int grpID, int artID) override;
  std::pair<Article, int> get_article(int grpID, int artID) const override;
private:
  bool connect(const char*);
  void createDB(const char* db_file);
  void delete_ng(unsigned int grpID);
  void delete_art(unsigned int grpID, unsigned int artID);
  bool has_ng(unsigned int grpID) const;
  bool has_art(unsigned int grpID, unsigned int artID) const;
  Article get_art(unsigned int grpID, unsigned int artID) const;
  std::vector<std::pair<int, std::string>> get_table_data(int grpID) const;
  void OLD_get_table_data();
  sqlite3* db;
};

#endif

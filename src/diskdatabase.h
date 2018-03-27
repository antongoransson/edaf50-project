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
  bool create_article(int grpID, const std::string& title, const std::string& author, const std::string& text) override;
  int delete_article(int grpID, int artID) override;
  std::pair<Article, int> get_article(int grpID, int artID) const override;
private:
  /*
    Opens a connection to the database
    @param db_file a pointer to the name of the database file.
    @return true if successfully connected, false otherwise
  */
  bool connect(const char* db_file);
  /*
    Creates the news group and article table, this is only done once for each db_file
    @param db_file a pointer to the name of the database file.
  */
  void initializeDB(const char* db_file);
  /*
    Checks if the news group exists in the database
    @param grpID the ID of the news group
    @return true if the news group exists, false otherwise
  */
  bool news_group_exists(int grpID) const;
  /*
    Checks if the article exists in the database
    @param article the ID of the article
    @return true if the article exists, false otherwise
  */
  bool article_exists(int grpID, int artID) const;
  /*
    Returns a vector containing pairs with the articles ID and title
    @param grpID the ID of the news group that the article belongs to
    @return a vector with pairs containg ID and title of each article
  */
  std::vector<std::pair<int, std::string>> get_articles(int grpID) const;
  sqlite3* db;
};

#endif

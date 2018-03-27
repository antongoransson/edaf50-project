#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include <string>
#include <vector>
#include <utility>
#include "article.h"
#include "newsgroup.h"

class DatabaseInterface {
public:
  virtual ~DatabaseInterface() = default;
  /*
    Gets all the news groups in the database
    @return a vector of pairs containing the ID and name of each news group
  */
  virtual std::vector<std::pair<int, std::string>> list_news_groups() const = 0;
  /*
    Creates a new news group and saves it in the database
    @param name the name of the news group to be added
    @return true if the news group was created, false if the name already exists
  */
  virtual bool create_news_group(const std::string& name) = 0;
  /*
    Deletes a news group from the database
    @param grpID the id of the group to be deleted
    @return true if the news group was deleted, false if no news group has the id grpID
  */
  virtual bool delete_news_group(int grpID) = 0;
  /*
    Gets all the articles in the database beloning to a news group
    @param grpID the id of the group to list articles in
    @return a pair with a vector containing a pair with each articles id and title and a bool that is
    true if a group with id grpID exists, false otherwise
  */
  virtual std::pair<std::vector<std::pair<int,std::string>>, bool> list_articles(int grpID) const = 0;
  /*
    Creates a new article and saves it in the database
    @param grpID the id of the group that the article shall belong to
    @param title the title of the article
    @param author the author of the article
    @param text the text of the article
    @return true if the article was created, false if the no news group with id grpID exists
  */
  virtual bool create_article(int grpID, const std::string& title, const std::string& author, const std::string& text) = 0;
  /*
    Deletes an article from the database
    @param grpID the id of the group that the article belongs to
    @param artID the id of the article
    @return 3 if no group with grpID exists,
      2 if no article with id artID exists
      and 1 if it was successful
  */
  virtual int delete_article(int grpID, int artID) = 0;
  /*
    Gets an article in the database
    @param grpID the id of the group that the article belongs to
    @param artID the id of the article
    @return a pair containg the article and 1 if it was successful,
      a pair with a default constructed article and 3 if no group with grpID exists,
      a pair with a default constructed article and 2 if no article with artID exists,
  */
  virtual std::pair<Article, int> get_article(int grpID, int artID) const = 0;
};
#endif

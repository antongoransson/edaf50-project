#include <string>
#include <vector>
#include <map>
#include <utility>
#include "article.h"
#include "newsgroup.h"

using std::map;
using std::string;
using std::vector;
using std::pair;

class DatabaseInterface {
public:
  virtual vector<pair<int,string> list_news_groups();
  virtual vector<pair<int,string> list_articles(int grpID);
  virtual bool create_news_group(string name);
  virtual bool create_article(string name, string author, string text);
  virtual bool delete_news_group(int grpID);
  virtual bool delete_article(int grpID, int artID);
  virtual Article get_article(int grpID, int artID);

};

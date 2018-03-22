#include <string>
#include <vector>
#include <map>

using std::map;
using std::string;
using std::vector;

class Interface {
public:


  virtual vector<Pair<int,string> list_news_groups();
  virtual vector<Pair<int,string> list_articles(int grpID);
  virtual void create_news_group(string name);
  virtual void create_article(string name, string author, string text );
  virtual bool delete_news_group(int grpID);
  virtual bool delete_article(int grpID, int artID );
  virtual string read_article(int grpID, int artID);

private:

}

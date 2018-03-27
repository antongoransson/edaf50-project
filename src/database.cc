#include "database.h"
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::make_pair;

Database::Database(): ngCount(0), artCount(0) {}

vector<pair<int,string>> Database::list_news_groups() const {
  vector<pair<int,string>> output;
  for(auto it = news_groups.begin(); it != news_groups.end(); ++it) {
    output.push_back(make_pair(it->second.get_id(), it->second.get_name()));
  }
  return output;
}

bool Database::create_news_group(const string& name) {
  for(auto it = news_groups.begin(); it != news_groups.end(); ++it) {
    if(it->second.get_name() == name) {
      return false;
    }
  }
  NewsGroup ng(name, ngCount);
  news_groups.emplace(ngCount, ng);
  ++ngCount;
  return true;
}

bool Database::delete_news_group(int grpID) {
  if(news_groups.find(grpID) == news_groups.end()) {
    return false;
  }
  news_groups.erase(grpID);
  return true;
}

pair<vector<pair<int,string>>,bool> Database::list_articles(int grpID) const {
  vector<pair<int,string>> output;
  if(news_groups.find(grpID) == news_groups.end()) {
    return make_pair(output, false);
  }
  if (articles.count(grpID) == 1 && articles.at(grpID).size() > 0) {
    for(auto it = articles.at(grpID).begin(); it != articles.at(grpID).end(); ++it) {
      output.push_back(make_pair(it->second.get_id(), it->second.get_title()));
    }
  }
  return make_pair(output, true);
}

bool Database::create_article(int grpID, const string& name, const string& author, const string& text) {
  if(news_groups.find(grpID) == news_groups.end()) {
    return false;
  }
  Article art(artCount, grpID, name, author, text);
  if (articles.count(grpID) == 0) {
    articles[grpID] = map<int, Article>();
  }
  articles.at(grpID).emplace(artCount, art);
  ++artCount;
  return true;
}

int Database::delete_article(int grpID, int artID) {
  if(articles.find(grpID) == articles.end()) {
    return 3;
  }
  if(articles.at(grpID).find(artID) == articles.at(grpID).end()) {
    return 2;
  }
  articles.at(grpID).erase(artID);
  return 1;
}

pair<Article, int> Database::get_article(int grpID, int artID) const {
  Article art;
  if(articles.find(grpID) == articles.end()) {
    return make_pair(art, 3);
  }
  if(articles.at(grpID).find(artID) == articles.at(grpID).end()) {
    return make_pair(art, 2);
  }

  art = articles.at(grpID).at(artID);
  return make_pair(art, 1);
}

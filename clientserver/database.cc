#include <string>
#include <vector>
#include <utility>
#include <iterator>
#include <map>
#include "database.h"
#include "databaseinterface.h"
#include "newsgroup.h"
#include "article.h"

using std::string;
using std::vector;
using std::pair;
using std::map;

Database::Database(){}

vector<pair<int,string> Database::list_news_groups(){
  vector<pair<int,string> output;

  for(ForwardIterator it = news_groups.begin(); it != news_groups.end();it++){
    vector.add(make_pair(*it.get_id(), *it.get_name()));
  }
  return output;
}

pair<vector<pair<int,string>,bool> Database::list_articles(int grpID){
  vector<pair<int,string> output;
  if(articles.find(grpID) == articles.end()){
    return make_pair(output,false;);
  }

  for(ForwardIterator it = news_groups.at(grpID).begin(); it != news_groups.at(grpID).end();it++){
    vector.add(make_pair(*it.get_id(), *it.get_title()));
  }
  return make_pair(output,true);
}

bool Database::create_news_group(string name){
  for(ForwardIterator it = news_groups.begin(); it != news_groups.end();it++){
    if(*it.get_name() == name){
      return false;
    }
  }

  NewsGroup ng(name, ngCount);
  news_groups.emplace(ngCount, ng);
  ++ngCount;
  return true;
}
bool Database::create_article(int grpID, string name, string author, string text){
  if(articles.find(grpID) == articles.end()){
    return false;
  }

  Article art(artCount, grpID, name, author, text);
  articles.at(grpID).emplace(artCount, art);
  ++artCount;
  return true;
}

bool Database::delete_news_group(int grpID){
  if(news_groups.find(grpID) == news_groups.end()){
    return false;
  }

  news_groups.erase(grpID);
  return true;
}

bool Database::delete_article(int grpID, int artID){
  if(articles.find(grpID) == articles.end()){
    return false;
  }
  if(articles.at(grpID).find(grpID) == articles.at(grpID).end()){
    return false;
  }

  articles.at(grpID).erase(artID);
  return true;
}

pair<Article, bool> Database::get_article(int grpID, int artID){
  Article art;
  if(articles.find(grpID) == articles.end()){
    return make_pair(art, false);
  }
  if(articles.at(grpID).find(grpID) == articles.at(grpID).end()){
    return make_pair(art,false);
  }

  art = articles.at(grpID).at(artID);
  return make_pair(art,true);
}

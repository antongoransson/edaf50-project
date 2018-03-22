#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
using std::string;

class Article
  public:
    Article(int, int, string, string, string);
    ~Article();
    int get_id();
    int get_group_id();
    string get_author();
    string get_title();
    string get_text();
  private:
    int id;
    int group_id;
    string author;
    string title;
    string text;
};


#endif

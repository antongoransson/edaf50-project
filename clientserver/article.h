#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>
class Article {
  public:
    Article(int, int, std::string, std::string, std::string);
    ~Article();
    int get_id();
    int get_group_id();
    std::string get_author();
    std::string get_title();
    std::string get_text();
  private:
    int id;
    int group_id;
    std::string author;
    std::string title;
    std::string text;
};


#endif

#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
  public:
    Article(int id_i = -1, int group_id_i = -1, std::string author_i = "", std::string title_i = "", std::string text_i = "");
    ~Article();
    int get_id() const;
    int get_group_id() const;
    std::string get_title() const;
    std::string get_author() const;
    std::string get_text() const;
  private:
    int id;
    int group_id;
    std::string title;
    std::string author;
    std::string text;
};


#endif

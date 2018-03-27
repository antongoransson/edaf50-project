#include "article.h"

using std::string;

Article::Article(int id_i, int group_id_i, string title_i, string author_i, string text_i):
      id(id_i), group_id(group_id_i), title(title_i), author(author_i),  text(text_i) {}

Article::~Article(){}

int Article::get_id() const { return id; }

int Article::get_group_id() const { return group_id; }

string Article::get_author() const { return author; }

string Article::get_title() const { return title; }

string Article::get_text() const { return text; }

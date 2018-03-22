#include "newsgroup.h"
using std::string;

NewsGroup::NewsGroup(string namei, int idi): name(namei), id(idi) {}

NewsGroup::~NewsGroup(){}

int NewsGroup::get_id(){ return id; }

string NewsGroup::get_name() { return name; }

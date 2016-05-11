#ifndef REUSABLE_H
#define REUSABLE_H

#include <string>

using std::string ;

bool file_exists(const string &filename);
string slurp(const char *filename);

#endif

#ifndef REUSABLE_H
#define REUSABLE_H

#include <string>
#include <vector>

using std::string ;

bool file_exists(const string &filename);
string slurp(const char *filename);

string nchars(char c, int num);
string pad_right(string s, int n);
string pad_left(string s, int n);
string format_num(double n, int dp);
std::string intercalate(std::string inter, std::vector<std::string> strings);



#endif

#pragma once
//#ifndef REUSABLE_H
//#define REUSABLE_H

#include <string>
#include <vector>

//using std::string ;

bool file_exists(const std::string &filename);
std::string slurp(const char *filename);
std::string slurp(const std::string filename);
void spit(const char *filename, const char *content);
void spit(const std::string& filename, const std::string& content);

std::string nchars(char c, int num);
std::string pad_right(std::string s, int n);
std::string pad_left(std::string s, int n);
std::string format_num(double n, int dp);
std::string intercalate(std::string inter, std::vector<std::string> strings);



//#endif

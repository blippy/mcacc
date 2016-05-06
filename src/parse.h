#ifndef PARSE_H
#define PARSE_H

//#include <iostream>
#include <string>
#include <vector>

using namespace std;


typedef vector<string>  vs_t;
typedef vector<vector<string> > vecvec_t;

extern const char white[];

void get_period(string &start, string &end);
string trim(string &str);
vector<string> tokenize_line(string &input);

vecvec_t vecvec(istream  &istr);
vecvec_t vecvec(const char *fname);
vecvec_t vecvec(string  &filename);
void prin_vecvec1(vecvec_t &vv);

void prin_vecvec(vecvec_t & vvs, const char *sep = "\t", const char *recsep= "\n", const char *filename = "");



double bround(double x);
double round2(double x);
#endif

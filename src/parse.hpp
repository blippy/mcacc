#pragma once
//#ifndef PARSE_H
//#define PARSE_H

//#include <iostream>
#include <string>
#include <vector>

#include "types.hpp"
//using namespace std;


typedef std::vector<std::string>  vs_t;
typedef std::vector<std::vector<std::string> > vecvec_t;

extern const char white[];

period get_period();

std::string trim(std::string &str);
vs_t tokenize_line(std::string &input);

vecvec_t vecvec(std::istream  &istr);
vecvec_t vecvec(const char *fname);
vecvec_t vecvec(std::string  &filename);
void prin_vecvec1(vecvec_t &vv);

void prin_vecvec(vecvec_t & vvs, const char *sep = "\t", const char *recsep= "\n", const char *filename = "");



double bround(double x);
double round2(double x);
//#endif

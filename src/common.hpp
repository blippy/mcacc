#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

//#include "etran.hpp"
#include "parse.hpp"
//#include "nacc.hpp"
#include "inputs.hpp"
#include "posts.hpp"

std::string rootdir();
std::string workdir();
std::string sndir(int n);
// create a filename that is based in the stage N directory
void sn(int n, const char *name, std::string &outname);

void s1(const char *name, std::string &outname);
void s2(const char *name, std::string &outname);
void s3(const char *name, std::string &outname);

typedef std::map<std::string, std::vector<std::string> > msvs_t;


int epics_main(const etran_ts& es);
int etb_main(nacc_ts& the_naccs);
void eaug_main(inputs_t& inputs);
void etb_main(nacc_ts& the_naccs, const post_ts& posts);
int stend_main(const period& p);

void conv(double& d, std::string s);
void conv(std::string& dest, std::string src);

template<typename T>
void print(const std::vector<T>& xs)
{
	for(auto& x: xs) std::cout << x << " ";
	std::cout << std::endl;
}


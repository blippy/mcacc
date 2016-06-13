#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "parse.h"
#include "autotypes.h"
#include "nacc.hpp"

std::string rootdir();
std::string workdir();
// create a filename that is based in the stage N directory
void sn(int n, const char *name, std::string &outname);

void s1(const char *name, std::string &outname);
void s2(const char *name, std::string &outname);
void s3(const char *name, std::string &outname);

typedef std::map<std::string, std::vector<std::string> > msvs_t;

enum dsv_e { etransa };

vecvec_t read_registered_dsv(dsv_e dsv);


int eaug_main(const period& p);
int epics_main(const etranas_t& es);
//int etb_main();
int etb_main(const nacc_ts& the_naccs);
//int posts_main(const etranas_t& es);
int posts_main(const etranas_t& es, const period& per, 
	const nacc_ts& the_naccs);
int stend_main(const period& p);
int yproc_main();

void conv(double& d, std::string s);
void conv(std::string& dest, std::string src);

template<typename T>
void print(const std::vector<T>& xs)
{
	for(auto& x: xs) std::cout << x << " ";
	std::cout << std::endl;
}

#endif

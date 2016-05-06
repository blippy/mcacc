#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <string>
#include <vector>

#include "parse.h"

// create a filename that is based in the stage N directory
void sn(int n, const char *name, std::string &outname);

void s1(const char *name, std::string &outname);
void s2(const char *name, std::string &outname);
void s3(const char *name, std::string &outname);

typedef std::map<std::string, std::vector<std::string> > msvs_t;

enum dsv_e { etransa };

vecvec_t read_registered_dsv(dsv_e dsv);

string interspersing(string inter, vector<string> strings);

int eaug_main();
//int dsv_main();
int epics_main();
int etb_main();
int posts_main();
int stend_main();
int yproc_main();

#endif

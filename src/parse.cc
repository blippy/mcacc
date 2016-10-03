/* separate out inputs into dsv files
created 19-Feb-2016
*/

// TODO eliminate this module
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>

#include "common.hpp"
#include "parse.hpp"

using namespace std;


const char white[] = " \t\r";

string trim(string& str)
{
    if(str.length() ==0) { return str;}
    size_t first = str.find_first_not_of(white);
    if(first == string::npos) return "";
    size_t last = str.find_last_not_of(white);
    return str.substr(first, (last-first+1));
}

vector<string> tokenize_line_XXX(string &input) //, const char sep= ' ')
{
	vector<string> output;
	string trimmed = input;
	string token;
	size_t first;
	int i;
loop:
	trimmed = trim(trimmed);
	if(trimmed.length() == 0) goto fin;
	//cout << "len = " << trimmed.length() << endl;
	if(trimmed[0] == '#') goto fin;
	if(trimmed[0] == '"') {
		token = "";
		for(i=1; i<trimmed.length(); i ++) {
			switch (trimmed[i]) {
			case '"':
				output.push_back(token);
				trimmed = trimmed.substr(i+1);
				goto loop;
			case ' ':
				token += '_' ; //167 ; // 26; // so that fields work
				break;
			default :
				token += trimmed[i];
			}
		}
		// hit eol without enclosing "
		output.push_back(token);
		goto fin;
	} else { // normal case
		first = trimmed.find_first_of(white);
		if(first == string::npos) {
			output.push_back(trimmed);
			goto fin;
		}
		token = trimmed.substr(0, first);
		output.push_back(token);
		trimmed = trimmed.substr(first+1);
		goto loop;
	}
	assert(false); // we should never get here


fin:
	return output;
}

/*
period get_period()
{
	period p;
	string fname;
	s1("period.dsv", fname);
	string line;

	//start = "0000-01-01";
	//end   = "3000-12-31";
	ifstream fin;
	fin.open(fname, ifstream::in);
	while(getline(fin, line)) {
		vector<string> fields = tokenize_line(line);
		p.start_date = fields[0];
		p.end_date = fields[1];
	}
	fin.close();

	return p;
}
*/





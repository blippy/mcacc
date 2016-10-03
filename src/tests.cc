#include <iostream>
#include <string>

//using namespace std;
#include "reusable.hpp"

// TODO NOW this should be a new version

typedef struct lexer_t {
	//basic_string<iterator> cursor;
	//iterator<basic_string> cursor;
	std::string::iterator cursor, end, tok_start, tok_end;
	//string *cursor;
	std::string token() { 
		//return cursor == end? '\0' : *curso
		std::string s ; // = "";
		for(auto& it= tok_start; it != tok_end; it++) s+= *it;
		return s;
	}
	bool is_white() {
		char c = *cursor;
		return c == ' ';
	}
	//bool not_white() { return is_white(); }

	bool more() { 
		if(cursor == end) return false;

		// eat white
		while(cursor != end) {
			if(! is_white()) break;
			cursor++;
		}
		if(*cursor == '#') return false;
		if(cursor == end) return false;

		tok_start = cursor;
		bool is_string  = *cursor == '"';
		if(is_string) {
			tok_start++;
			cursor++;
			while(cursor !=end && *cursor != '"') cursor++;
			tok_end = cursor;
			if(cursor !=end) cursor++;
		} else {
			while(!is_white() && cursor !=end) cursor++;
			tok_end = cursor;
		}

		//while(!is_white() && cursor !=end) cursor++;
		/*
		if(is_string) cursor++;
		while(cursor !=end) {
			if(!is_string && is_white()) break;
			cursor++;
			if(is_string && *cursor == '"') break;
		}
		if(is_string) cursor++;
		*/

		return true;
	}
	lexer_t(std::string& s) {cursor = s.begin(); end = s.end(); }
} lexer_t;

std::vector<std::string> tokenise_line_1(std::string& s)
{
	std::vector<std::string> result;
	lexer_t lexer(s);

	while(lexer.more()) {
		std::string token = lexer.token();
		std::cout << '<' << token << '>' << std::endl;
		result.push_back(token);
	}
	std::cout << std::endl;
	return result;
}

void check(bool ok, std::string msg)
{
	std::string s = ok? "PASS" : "FAIL";
	std::cout << s << " " << msg << std::endl;
}


void check_erase_all(std::string src, char c, std::string targ)
{
	std::string s = src;
	erase_all(s, c);
	std::string cstr;
       	cstr	= c;
	std::string msg = "erase_all() " + cstr + " in " + src;
	//msg +=	c + " in " + src;
	check(s == targ, msg);
}

void check_tokeniser(std::string str, std::vector<std::string> strs)
{
	std::vector<std::string> result = tokenise_line_1(str);
	std::string msg = "tokenise_line() <" + str + ">";

	bool ok = strs.size() == result.size();
	for(int i = 0; i< strs.size() && ok; i++) {
		if(strs[i] != result[i]) { 
			//std::cout << strs[i] <<result[i] << strs[i].size() << result[i].size() ; 
			//std::cout << strs[i].size() << result[i].size();
			ok = false; 
		}
	}
	check(ok, msg);
}

void run_all_tests()
{
	check_erase_all("", 'a', "");
	check_erase_all("aaa", 'a', "");
	check_erase_all("alibaba", 'a', "libb");
	check_erase_all("baad", 'a', "bd");

	check_tokeniser("hello world",  { "hello", "world" });
	check_tokeniser("foo  \"baz shazam\"  bar", {"foo", "baz shazam", "bar"});
       	check_tokeniser("    hello  \"joe blogg\"  world   # just a comment   ", {"hello", "joe blogg", "world"});
	check_tokeniser("this is \"tricky disco", {"this", "is", "tricky disco"});
	//std::string s;
	//tokenise_line_1(s);
}

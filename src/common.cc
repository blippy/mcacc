#include <assert.h>
#include <string>

#include <boost/filesystem.hpp>
namespace fsys = boost::filesystem;

#include "common.hpp"
#include "reusable.hpp"

using namespace std;


string rootdir()
{
	string root = expand_user("~/.mcacc");
	//fsys::path p(s);
	static bool created = false;
	if(!created) {
		fsys::create_directory(root);
		string s = root + "/work";
		fsys::create_directory(s);
		fsys::create_directory(s + "/s1");
		fsys::create_directory(s + "/s2");
		fsys::create_directory(s + "/s3");
		fsys::create_directory(root + "/yahoo");
		created = true;
	}
	return root;
}

std::string workdir() { return rootdir() + "/work" ; }


std::string sndir(int n)
{
	return workdir() + "/s" + to_string(n);
}

// create a filename that is based in the stage N directory
void sn(int n, const char *name, string &outname) 
{
	//outname = workdir();
	//outname +=  "/s" +  to_string(n)  + "/" + name;
	outname = sndir(n) + "/" + name;
}

void s1(const char *name, string &outname) { sn(1, name, outname);}
void s2(const char *name, string &outname) { sn(2, name, outname);}
void s3(const char *name, string &outname) { sn(3, name, outname);}

typedef map<string, vector<string> > msvs_t;



void conv(double& d, std::string s) { d = stod(s); }
void conv(std::string& dest, std::string src) { dest = src; }

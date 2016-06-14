#include <assert.h>
#include <string>

#include <boost/filesystem.hpp>
namespace fsys = boost::filesystem;

#include "common.h"


// TODO reusuable
// http://stackoverflow.com/questions/4891006/how-to-create-a-folder-in-the-home-directory
std::string expand_user(std::string path) 
{
	if (not path.empty() and path[0] == '~') {
		assert(path.size() == 1 or path[1] == '/');  // or other error handling
		char const* home = getenv("HOME");
		if (home or ((home = getenv("USERPROFILE")))) {
			path.replace(0, 1, home);
		}
		else {
			char const *hdrive = getenv("HOMEDRIVE"),
			     *hpath = getenv("HOMEPATH");
			assert(hdrive);  // or other error handling
			assert(hpath);
			path.replace(0, 1, std::string(hdrive) + hpath);
		}
	}
	return path;
}

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


vecvec_t read_registered_dsv(dsv_e dsv)
{
	assert(dsv == etransa); // enhance via some kind of lookup table
	std::string filename;
        s3("etrans-aug.dsv", filename);

	vecvec_t rows = vecvec(filename);
	return rows;
}

void conv(double& d, std::string s) { d = stod(s); }
void conv(std::string& dest, std::string src) { dest = src; }

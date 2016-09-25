#include <assert.h>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

#include <sys/stat.h>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "assets.hpp"
#include "cgt.hpp"
#include "common.hpp"
#include "etb.hpp"
#include "parse.hpp"
#include "reusable.hpp"
#include "gaap.hpp"
#include "inputs.hpp"
#include "posts.hpp"
#include "stend.hpp"
#include "etrans-aug.hpp"
#include "epics.hpp"
#include "yproc.hpp"
#include "wiegley.hpp"


namespace fsys = boost::filesystem;
using namespace std;
namespace po = boost::program_options;



bool operator<(const etran_t& lhs, const etran_t& rhs)
{
	return std::tie(lhs.ticker, lhs.dstamp) < std::tie(rhs.ticker, rhs.dstamp);
}

bool operator>(const etran_t &a, const etran_t &b)
{
	return std::tie(a.ticker, a.dstamp) > std::tie(b.ticker, b.dstamp);
}




// remove directory contents
// http://stackoverflow.com/questions/14610185/how-to-avoid-removing-directory-on-remove-all-with-boost-libraries
// NB C++ has a filesystem::remove_all() function which will do this for us,
// but is only experimental at 22-Sep-2016
void rmfiles(std::string fname)
{
	fsys::path path_to_remove(fname.c_str());
	for (fsys::directory_iterator end_dir_it, it(path_to_remove); it!=end_dir_it; ++it) {
		fsys::remove_all(it->path());
	}

}

void init()
{
	string htm = get_html();
	string fname = rootdir() + "/mcacc.htm";
	spit(fname, htm.c_str());
}

void clean()
{
	init();
	rmfiles(sndir(1));
	rmfiles(sndir(2));
	rmfiles(sndir(3));
}


void main_processing(po::variables_map vm)
{

	if(vm.count("init")) init();
	if(vm.count("clean")) clean();

	if(vm.count("pre")>0) {
		string pre = vm["pre"].as<string>();
		system(pre.c_str()); // TODO LOW check return status
		//cout << "Pre cmd is " << pre << "\n";
	}

	inputs_t inps = read_inputs();
	process_yahoos(inps, vm.count("snap")>0);
	period p = inps.p;

	stend_ts stends = stend_main(inps, p);
	eaug_main(inps, stends);

	post_ts posts = posts_main(inps);
	etb_main(inps.naccs, posts);
	gaap_main(inps.naccs, p);
	epics_main(inps.etrans, stends);
	cgt(inps.etrans, p);
	
	string wiegley_str = vm["wiegley"].as<string>();
	if(wiegley_str == "on") {
		wiegley(inps);
	} else if(wiegley_str != "off") {
		cerr << "ERR: Option wiegley error. Must be on|off, but given`"
			<< wiegley_str 
			<< "'. Continuing anyway." << endl;
	}

	system("mcacc-reports.sh");
}

void print_version()
{
	cout << "mcacc " << VERSION << '\n';
}

void print_rootdir()
{
	cout << rootdir() << "\n";
}


po::variables_map process_options(int argc, char *argv[])
{
	//options res;
	po::options_description desc{"Options"};
	desc.add_options()
		("clean", "Clean up the working folders")
		("help,h", "Help")
		("init", "Initialise working folders if necessary, and without cleaning")
		("pre", po::value<string>(), "Preprocess command")
		("root", "Print root directory")
		("snap,s", "Snapshot")
		("version,v", "Version")
		("wiegley", po::value<string>()->default_value("on"), 
		 "Produce (on) or surpress (off) wiegley output")
	;
	po::variables_map vm;

	po::store(po::parse_command_line(argc, argv, desc), vm);

	string cfg_name = "/home/mcarter/.mcacc/mcacc.ini"; // TODO generalise
	ifstream ifs;
	ifs.open(cfg_name.c_str());
	po::store(po::parse_config_file(ifs, desc), vm); 
	ifs.close();

	po::notify(vm);


	if(vm.count("help")) cout << desc << "\n";

	return vm;
}

int main(int argc, char *argv[])
{

	po::variables_map vm;

	try {
		vm = process_options(argc, argv);

	} catch (const exception &ex) {
		cerr << ex.what() << "\n";
	}

	if(vm.count("help")) return EXIT_SUCCESS;

	if(vm.count("root")) {
		print_rootdir();
		return EXIT_SUCCESS;
	}

	if(vm.count("version")) {
		print_version();
		return EXIT_SUCCESS;
	}




	main_processing(vm);

	return EXIT_SUCCESS;
}

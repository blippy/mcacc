#include <algorithm>
#include <assert.h>
#include <cfenv>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

#include <sys/stat.h>
#include <unistd.h>

//#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "common.hpp"
#include "oven.hpp"
#include "tests.hpp"
#include "reusable.hpp"
#include "show.hpp"


//namespace fsys = boost::filesystem;
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
/*
void rmfiles(std::string fname)
{
	fsys::path path_to_remove(fname.c_str());
	for (fsys::directory_iterator end_dir_it, it(path_to_remove); it!=end_dir_it; ++it) {
		fsys::remove_all(it->path());
	}

}
*/

// http://www.linuxquestions.org/questions/programming-9/deleting-a-directory-using-c-in-linux-248696/
// remove directory recursively
// include dirent.h sys/types.h
int rmdir(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                rmdir(path);
            }

            /*
             * Here, the actual deletion must be done.  Beacuse this is
             * quite a dangerous thing to do, and this program is not very
             * well tested, we are just printing as if we are deleting.
             */
            //printf("(not really) Deleting: %s\n", path);
	    remove(path);
            /*
             * When you are finished testing this and feel you are ready to do the real
             * deleting, use this: remove*STUB*(path);
             * (see "man 3 remove")
             * Please note that I DONT TAKE RESPONSIBILITY for data you delete with this!
             */
        }

    }
    closedir(dir);

    /*
     * Now the directory is emtpy, finally delete the directory itself. (Just
     * printing here, see above) 
     */
    //printf("(not really) Deleting: %s\n", dirname);
    remove(dirname);

    return 1;
}

int rmdir(const string& dirname)
{
	return rmdir(dirname.c_str());
}

void clean()
{
	rmdir(sndir(1));
	rmdir(sndir(2));
	rmdir(sndir(3));
}


/* Perform user preprocessing
 *
 * It doesn't do much, but it has been separated out to aid profiling
 */
void preprocess(const char* command)
{
	system(command); // TODO LOW check return status
	
}

void main_processing(po::variables_map vm)
{

	//if(vm.count("init")) init();
	if(vm.count("clean")) clean();

	if(vm.count("pre")>0) {
		string pre = vm["pre"].as<string>();
		preprocess(pre.c_str());
	}

	string wiegley_str = vm["wiegley"].as<string>();
	bool do_wiegley = wiegley_str == "on";
	if(! do_wiegley && wiegley_str != "off") {
		cerr << "ERR: Option wiegley error. Must be on|off, but given`"
			<< wiegley_str 
			<< "'. Continuing anyway." << endl;
	}

	bool do_snap = vm.count("snap") > 0;
	oven ove;
	ove.load_inputs();
	if(do_snap) ove.fetch();
	ove.process(do_wiegley);

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
		//("init", "Initialise working folders if necessary, and without cleaning")
		("pre", po::value<string>(), "Preprocess command")
		("root", "Print root directory")
		("show", po::value<string>(), "Show a report")
		("snap,s", "Snapshot")
		("tests", "Run tests only")
		("version,v", "Version")
		("wiegley", po::value<string>()->default_value("on"), 
		 "Produce (on) or surpress (off) wiegley output")
	;
	po::variables_map vm;

	po::store(po::parse_command_line(argc, argv, desc), vm);

	string cfg_name = rootdir() + "/mcacc.ini";
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

	feenableexcept(FE_OVERFLOW);

	po::variables_map vm;

	try {
		vm = process_options(argc, argv);

	} catch (const exception &ex) {
		cerr << ex.what() << "\n";
	}

	if(vm.count("help")) return EXIT_SUCCESS;

	if(vm.count("tests")) { run_all_tests(); return EXIT_SUCCESS; }
	
	if(vm.count("root")) { print_rootdir(); return EXIT_SUCCESS; }

	if(vm.count("version")) { print_version(); return EXIT_SUCCESS; }

	main_processing(vm);

	if(vm.count("show")) { 
		const string str = vm["show"].as<string>();
		show(str);
	}

	return EXIT_SUCCESS;
}

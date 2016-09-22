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
#include "parse.hpp"
#include "reusable.hpp"
#include "gaap.hpp"
#include "inputs.hpp"
#include "posts.hpp"
#include "stend.hpp"
#include "etrans-aug.hpp"
#include "epics.hpp"
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



strings commasepstr(const string& line)
{
	string line1 = line;
	for(auto it = line1.begin(); it != line1.end(); ++it) {
		if(*it == ',') *it = ' '; }
	return tokenize_line(line1);
}	
vecvec_t commasep(string  &filename)
{
        vecvec_t res;
        ifstream fin;
        string line;
        fin.open(filename.c_str(), ifstream::in);
        while(getline(fin, line)) {
		strings fields = commasepstr(line);
                if(fields.size() >0) res.push_back(fields);
        }
        return res;
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

void download(inputs_t& inputs)
{
	const comm_ts& the_comms = inputs.comms;
	//load(the_comms);

	strings tickers;
	for(auto& cm:the_comms) {
		auto &c = cm.second;
		if(c.down == "W") tickers.push_back(c.ticker);
	}

	string fname;
	string usd;
	strings retrs = fetch_tickers(tickers, usd);
	s2("usd.csv", fname);
	spit(fname, usd);
	s2("yfetch.csv", fname);
	spit(fname, intercalate("", retrs));



	std::time_t t = std::time(nullptr);
	char dstamp[80], tstamp[80];
	strftime(dstamp, 80, "%Y-%m-%d", std::localtime(&t));
	s2("dstamp", fname);
	spit(fname, dstamp);
	strftime(tstamp, 80, "%H:%M:%S", std::localtime(&t));
	s2("tstamp", fname);
	spit(fname, tstamp);

	// TODO LOW This is incorrect for commodities not downloaded in pennies
	for(auto& line: retrs) {
		strings fields;
		boost::split(fields, line, boost::is_any_of(","), boost::token_compress_on);
		yahoo_t y;
		y.dstamp = dstamp;
		y.tstamp = tstamp;
		y.ticker = fields[0]; 
		boost::erase_all(y.ticker, "\"");
		//cout << "tocker = " << y.ticker << endl;
		y.price = stod(fields[1]); //enpennies(fields[1]); //stod(fields[1])/100;
		y.chg = stod(fields[2]); // enpennies(fields[2]); //stod(fields[2])/100;
		y.chgpc = y.chg / (y.price - y.chg) * 100;
		insert_yahoo(y, inputs);

	}
}

bool yorder (vector<string> a, vector<string> b) { return a[0] < b[0];}



void yproc_main(const comm_ts & the_comms)
{	

/*
IN: s1/comm.dsv s2/dstamp s2/ystamp s2/yfetch.csv s2/usd.csv
OUT: s3/yrpoc.dsv ~/.mca/yahoo/<YYYY-MM-DD>.txt

NB Further processing will pick up the generated yahoo file from the cache
(in ~/.mca), but it is still useful to have s3/yproc.dsv for creating
a snapshot of gains
*/


	set<string> usds; // those comms which are in UDB
	for(auto &cm: the_comms) {
		auto & c = cm.second;
		if(c.unit == "USD") 
			usds.insert(c.ticker);
	}

	string fname;
	s2("usd.csv", fname); 
	if(! file_exists(fname)) 
		throw runtime_error("ERR-102 (fatal): File s2/usd.csv does not exist.");

	string file_contents = slurp(fname);
	strings usdv = commasepstr(file_contents);
	string rox_str;
	try { rox_str = usdv.at(1); } // TODO needs checking to see if it's pulling out right value
	catch (const out_of_range& ex) { throw runtime_error("Couldn't decompose <"s + file_contents + ">"); }
	double rox = 100.0 * stod(rox_str);
	//cout << rox;


	s2("dstamp", fname);
	string dstamp = slurp(fname);
	s2("tstamp", fname);
	string tstamp = slurp(fname);
	string dtstamp = dstamp + "\t" + tstamp;
	
	s2("yfetch.csv", fname);
	vecvec_t yfetch = commasep(fname);

	ofstream yout;
	s3("yproc.dsv", fname);
	yout.open(fname);
	sort(yfetch.begin(), yfetch.end(), yorder);
	for(auto y: yfetch) {
		yout << "yahoo " << dtstamp << "\t" ;
		string ticker = y[0];
		yout << ticker << "\t" ;
		string rox_str = "1.0000";
		double price = stod(y[1]);
		double chg   = stod(y[2]);
		if(usds.find(ticker) != usds.end()) {
			rox_str = to_string(rox);
			price *= rox;
			chg *= rox;
		}
		yout << rox_str << "\t" << to_string(price) << "\t" ;;
		yout << chg << "\t" ;

		// elminate the + and % from chgpc
		string chgpc ; // =y[3];
		char chars[] = {'+', '%' };
		for(auto c : y[3]) if(c != '+' && c!= '%') chgpc += c;
		yout << chgpc << "\tP" ;
	
		yout << endl;
	}
	yout.close();

	string yout_name = rootdir() + "/yahoo/" + dstamp + ".txt";
	s3("yproc.dsv", fname);
	string content = slurp(fname);
	spit(yout_name, content);

}

int yproc_main_nox(const comm_ts & the_comms) 
{
	try {
		yproc_main(the_comms);
	} catch (const runtime_error& ex) {
		cerr << "ERR-101 (Non-fatal): USD decoding: " << ex.what() << endl;
	}
	return EXIT_SUCCESS;
}

int dsv_extract()
{
	string input;
	while(getline(cin, input)) {
		vector<string> output = tokenize_line(input);
		if(output.size() == 0) continue;
		string fname = output[0];
		fname  += ".dsv";
		fstream fs;
		ios_base::openmode mode =  fstream::out | fstream::app;
		fs.open(fname.c_str(), mode); // user has to pre-delete the file
		for(int i = 1; i < output.size() -1; i++) { fs << output[i] << "\t"; }
		fs << output.back() << endl ;
		fs.close();
		//cout << "In: " << trimmed << endl;
	}
	return EXIT_SUCCESS;
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

	if(vm.count("snap")) {
		download(inps);
	}

	yproc_main_nox(inps.comms);

	period p = inps.p;

	stend_ts stends = stend_main(inps, p);
	eaug_main(inps, stends);

	post_ts posts = posts_main(inps);
	etb_main(inps.naccs, posts);
	gaap_main(inps.naccs, p);
	epics_main(inps.etrans, stends);
	cgt(inps.etrans, p);
	wiegley(inps);

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

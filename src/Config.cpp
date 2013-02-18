
#include "sbe/Config.hpp"

#include "sbe/Engine.hpp"
#include "sbe/io/IO.hpp"

#include <boost/property_tree/info_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

/*const*/ std::string Config::_fileName = "config.conf";

Config::Config() {
	load();
}


void Config::load() {
	fs::ifstream fin;

	fs::path p ( Engine::GetIO()->topPath() ); // use to path in IO-stack to save config to
	p /= _fileName;

	if(!(fs::status(p).type() == fs::file_type::file_not_found) && fs::is_regular_file(p)){

		fin.open(p);

	} else {
		Engine::out(Engine::ERROR) << "[config::load] Cant load from '" << p << "'!" << std::endl;
		return;
	}

	try{

		pt::info_parser::read_info( fin, _settings );



	} catch (fs::filesystem_error& e)	{
		Engine::out(Engine::ERROR) << "[config::load] boost::fs exception! '" << e.what() << "'" << std::endl;
	}

	_fileName = "debug.conf";
	save(true);
	_fileName = "config.conf";

	fin.close();

	Engine::out(Engine::INFO) << "[config::load] Loading done!" << std::endl;
}

void Config::save(bool overwrite) {
	fs::ofstream fout;

	fs::path p ( Engine::GetIO()->topPath() ); // use to path in IO-stack to save config to
	p /= _fileName;

	if ( fs::status(p).type() == fs::file_type::file_not_found || (fs::is_regular_file(p) && overwrite) ){

		fout.open(p);

	}	else {

		Engine::out(Engine::WARNING) << "[config::save] Saving " << " ( " << p << " -- " << ") would overwrite existing file!" << std::endl;
	}

	try {

		if (!fout.is_open()) return;

		// write the complete ptree
		pt::info_parser::write_info( fout, _settings );

	} catch (fs::filesystem_error& e)	{
		Engine::out(Engine::ERROR) << "[config::save] boost::fs exception! '" << e.what() << "'" << std::endl;

	}

	Engine::out(Engine::INFO) << "[config::save] Saving done! '" << p << "'" << std::endl;

	fout.close();
}

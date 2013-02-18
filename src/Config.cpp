
#include "sbe/Config.hpp"

#include "sbe/Engine.hpp"
#include "sbe/io/IO.hpp"

const std::string Config::_fileName = "config.conf";

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

		pt::ini_parser::read_ini( fin, _settings );

	} catch (fs::filesystem_error& e)	{
		Engine::out(Engine::ERROR) << "[config::load] boost::fs exception! '" << e.what() << "'" << std::endl;
	}

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
		pt::ini_parser::write_ini( fout, _settings );

	} catch (fs::filesystem_error& e)	{
		Engine::out(Engine::ERROR) << "[config::save] boost::fs exception! '" << e.what() << "'" << std::endl;

	}

	fout.close();
}

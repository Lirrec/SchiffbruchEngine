#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "util/NamedList.hpp"

#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

class Config {

private:
	/// stores settings
	boost::property_tree::ptree _settings;

	// settings cache
	//NamedList<boost::any> _cache;

	static const std::string _fileName;

#ifdef __GCC_4_6__
	Config(const Config&) {} // no copying allowed
	Config& operator=(const Config&) {}
#else
public:
	Config(const Config&) = delete; // no copy allowed
	Config& operator=(const Config&) = delete;
#endif

public:

	Config();

	/**
	 * \brief set a key to value
	 * \warning not threadsave
	 */
	template<typename T>
	void set(const std::string &key, const T &value);

	/**
	 * \brief get value of key
	 * \return the value key is set to
	 * \warning not threadsave
	 */
	template<typename T>
	T get(const std::string &key) const;

	/**
	 * \brief get value of key
	 * \param default default value
	 * \return the value key is set to or, if key not set, the the value given as default
	 * \warning not threadsave
	 */
	template<typename T>
	T get(const std::string &key, const T &dfault) const;

	/**
	 * \brief load settings into the internal cache from the settings file.
	 * Searchpath for the settings file is the current top of the IO-stack.
	 * \warning not threadsave
	 * \warning Make sure there's a valid IO-stack in IO bevore calling this
	 * \see IO::topPath()
	 */
	void load();

	/**
	 * \brief save settings from the internal cache to the settings file.
	 * Searchpath for the settings file is the current top of the IO-stack.
	 * \warning not threadsave
	 * \warning Make sure there's a valid IO-stack in IO bevore calling this
	 * \see IO::topPath()
	 */
	void save(bool overwrite = true);


};

template<typename T>
void Config::set(const std::string &key, const T &value){
	_settings.put<T>(key, value);
}

template<typename T>
T Config::get(const std::string &key) const {
	return _settings.get<T>(key);
}

template<typename T>
T Config::get(const std::string &key, const T &dfault) const {
	try{

		return _settings.get<T>(key);

	} catch(pt::ptree_error &e){ // if key doesn't exist or translation fails, return default

		return dfault;
	}
}

#endif // CONFIG_HPP

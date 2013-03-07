#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#include "sbe/Engine.hpp"

class Config {

private:
	/// stores settings
	boost::property_tree::ptree _settings;

	// settings cache
	//NamedList<boost::any> _cache;

	static /*const*/ std::string _fileName;

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
	 * \brief get a ptree node by path
	 * \return the requested node or empty optional
	 * \warning not threadsave
	 */
	boost::optional<const boost::property_tree::ptree&> getPath(const std::string &path) const;

	/**
	 * \brief get value of key
	 * \param key key to get value from
	 * \param dfault default value
	 * \return the value key is set to or, if key not set, the the value given as default
	 * \warning not threadsave
	 */
	template<typename T>
	T get(const std::string &key, const T &dfault) const;

	/**
	 * \brief loads content of the default conf file into the "system" subtree.
	 * Searchpath for the settings file is the current top of the IO-stack.
	 * \warning not threadsave
	 * \warning Make sure there's a valid IO-stack in IO bevore calling this
	 * \see IO::topPath(), loadInto()
	 */
	void load();

	/**
	 * \brief load a ptree from a file into our config ptree
	 * Reads a ptree from given .info file and inserts it
	 * as subtree with the given name into the config ptree.
	 * Wehn calling save(), those will be written to their respective files.
	 * \warning not threadsave
	 * \warning Make sure there's a valid IO-stack in IO bevore calling this
	 */
	void loadInto(const std::string &dest, const std::string &file);

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
	try {
	return _settings.get<T>(key);
	} catch(boost::property_tree::ptree_error &e){ // if key doesn't exist or translation fails, return default
		Engine::out(Engine::ERROR) << "[Config] " << e.what() << std::endl;
		throw;
	}
}

template<typename T>
T Config::get(const std::string &key, const T &dfault) const {
	try{

		return _settings.get<T>(key);

	} catch(boost::property_tree::ptree_error &e){ // if key doesn't exist or translation fails, return default

		return dfault;
	}
}

#endif // CONFIG_HPP

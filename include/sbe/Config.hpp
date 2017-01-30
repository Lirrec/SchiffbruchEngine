#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#include <SFML/System/NonCopyable.hpp>

#include <sbe/Engine.hpp>

namespace sbe
{
	/**
		Manager class for loading and accessing various configuration options.
		For storing the values a boost::ptree is used, all paths used should be valid ptree paths.

		The templated get/set methods allow saving/loading of all basic types supported in boost::property_tree.
		By default the system/default configuration file is loaded into the "system." subtree.
		Other configfiles will be loaded into seperate subtrees defined by their name ( keys.cfg will be loaded into "keys." ).

		When saving the config each subtree is saved to its individual file.

		@warning not threadsafe!
	*/
	class Config : sf::NonCopyable
	{

	private:
		/// stores settings
		boost::property_tree::ptree _settings;

		// settings cache
		//NamedList<boost::any> _cache;

		/*const*/
		std::string _fileName;
		const std::string _defaultPath;

	public:

		Config();

		Config(const std::string& fileName, const std::string& defaultPath);

		/**
		 * \brief set a key to value
		 */
		template<typename T>
		void set(const std::string& key, const T& value);

		/**
		 * \brief get value of key
		 * \return the value key is set to
		 */
		template<typename T>
		T get(const std::string& key) const;

		/**
		 * \brief get a ptree node by path
		 * \return the requested node or empty optional
		 */
		boost::optional<const boost::property_tree::ptree&> getPath(const std::string& path) const;

		/**
		 * \brief get value of key
		 * \param key key to get value from
		 * \param dfault default value
		 * \return the value key is set to or, if key not set, the the value given as default
		 */
		template<typename T>
		T get(const std::string& key, const T& dfault) const;

		/**
		 * \brief loads content of the default conf file into the "system" subtree.
		 * Searchpath for the settings file is the current top of the IO-stack.
		 * \warning Make sure there's a valid IO-stack in IO before calling this
		 * \see IO::topPath(), loadInto()
		 */
		void load();

		/**
		 * \brief load a ptree from a file into our config ptree
		 * Reads a ptree from given .info file and inserts it
		 * as subtree with the given name into the config ptree.
		 * Wehn calling save(), those will be written to their respective files.
		 * \warning Make sure there's a valid IO-stack in IO before calling this
		 */
		void loadInto(const std::string& dest, const std::string& file);

		/**
		 * \brief save settings from the internal cache to the settings file.
		 * Searchpath for the settings file is the current top of the IO-stack.
		 * \warning Make sure there's a valid IO-stack in IO before calling this
		 * \see IO::topPath()
		 */
		void save(bool overwrite = true);


	};

	template<typename T>
	void Config::set(const std::string& key, const T& value) {
		_settings.put<T>(key, value);
	}

	template<typename T>
	T Config::get(const std::string& key) const {
		try
		{
			return _settings.get<T>(key);
		}
		catch (boost::property_tree::ptree_error& e)
		{ // if key doesn't exist or translation fails, return default
			Engine::out(Engine::ERROR) << "[Config] " << key << ":" << e.what() << std::endl;
			throw;
		}
	}

	template<typename T>
	T Config::get(const std::string& key, const T& dfault) const {
		try
		{
			return _settings.get<T>(key);
		}
		catch (boost::property_tree::ptree_error& e)
		{ // if key doesn't exist or translation fails, return default
			return dfault;
		}
	}
} // namespace sbe
#endif // CONFIG_HPP

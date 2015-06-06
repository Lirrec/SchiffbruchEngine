#include "sbe/Config.hpp"

#include "sbe/io/IO.hpp"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace sbe
{


	Config::Config() : _fileName("config.conf"), _defaultPath("system") {
		load();
	}

	Config::Config(const std::string& fileName, const std::string& defaultPath)
			: _fileName(fileName), _defaultPath(defaultPath) {
		load();
	}

	boost::optional<const boost::property_tree::ptree&> Config::getPath(const std::string& path) const {
		try
		{
			const pt::ptree& p = _settings.get_child(path);
			return boost::optional<const boost::property_tree::ptree&>(p);
		}
		catch (pt::ptree_bad_path& e)
		{
			Engine::out(Engine::ERROR) << "[Config::getNode] Path '" << path << "' doesn't exist!" << std::endl;
			return boost::optional<const pt::ptree&>();
		}
	}

	void Config::load() {
		loadInto(_defaultPath, _fileName); // load default config file and add it to system
	}

	void Config::loadInto(const std::string& dest, const std::string& filename) {
		pt::ptree tree;
		fs::ifstream fin;

		fs::path p(Engine::GetIO()->topPath()); // use to path in IO-stack to save config to
		p /= filename;

		if (!(fs::status(p).type() == fs::file_type::file_not_found) && fs::is_regular_file(p))
		{

			fin.open(p);

		} else
		{
			Engine::out(Engine::ERROR) << "[config::load] Cant load from '" << p << "'!" << std::endl;
		}

		if (fin.is_open())
		{
			try
			{

				pt::info_parser::read_info(fin, tree);

			}
			catch (fs::filesystem_error& e)
			{
				Engine::out(Engine::ERROR) << "[config::load] boost::fs exception! '" << e.what() << "'" << std::endl;
			}

			fin.close();

			_settings.add_child(dest, tree);

			Engine::out(Engine::INFO) << "[config::load] Loading " << p
			<< " into " << dest << " successful!" << std::endl;
		}
		_settings.put<std::string>(dest, p.string()); // or better add filename alone?
	}

	void Config::save(bool overwrite) {
		fs::ofstream fout;

		for (const pt::ptree::value_type& n : _settings)
		{

			//~ fs::path p ( Engine::GetIO()->topPath() ); // use to path in IO-stack to save config to
			//~ p /= n.second.get_value<std::string>(); //_settings.get<std::string>(n.first);
			fs::path p(n.second.get_value<std::string>());

			Engine::out(Engine::SPAM) << p << std::endl;

			if (fs::status(p).type() == fs::file_type::file_not_found || (fs::is_regular_file(p) && overwrite))
			{

				fout.open(p);

			} else
			{

				Engine::out(Engine::WARNING) << "[config::save] Saving " << " ( " << p << " -- " << ") would overwrite existing file!" << std::endl;
			}

			try
			{

				if (!fout.is_open()) return;

				// write the complete ptree
				pt::info_parser::write_info(fout, n.second);

			}
			catch (fs::filesystem_error& e)
			{
				Engine::out(Engine::ERROR) << "[config::save] boost::fs exception! '" << e.what() << "'" << std::endl;

			}

			Engine::out(Engine::INFO) << "[config::save] Saved " << n.first
			<< " to '" << p << "'" << std::endl;

			fout.close();
		}
	}
} // namespace sbe

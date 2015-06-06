#include "sbe/ScriptingEngine.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace sbe
{
	ScriptingEngine::ScriptingEngine() {
		RegisterForEvent("RUN_SCRIPT");

		RegisterForEvent("SCRIPT_HELLO", [this](Event&) {
			RunString("print ( \"hello, world\" )\0");
		});

		RegisterForEvent("RESTART_SCRIPTING", [this](Event&) {
			deinit();
			init();
		});
	}

	void ScriptingEngine::HandleEvent(Event& e) {
		if (e.Is("RUN_SCRIPT", typeid(std::string)))
		{
			std::string filename = boost::any_cast<std::string>(e.Data());

			if (!fs::exists(fs::path(filename)))
			{
				Engine::out(Engine::ERROR) << "[SCRIPTING] Unable to load path! '" << filename << "' not found!" << std::endl;
				return;
			}
		}
	}


} // namespace sbe

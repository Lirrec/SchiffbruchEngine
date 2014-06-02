#ifndef SCRIPTING_HPP
#define SCRIPTING_HPP

#include "sbe/event/EventUser.hpp"
#include <string>

namespace sbe
{

	/**
		This class provides Access to a scripting language runtime.
	*/
	class ScriptingEngine : public EventUser
	{

		public:

			ScriptingEngine();
			virtual ~ScriptingEngine() {}

			/**
				Initialize the scripting language, has to be called once before using other functions
			*/
			virtual void init() = 0;

			/** Execute a given script
				@return false on error
			*/
			virtual bool RunString(const std::string& code) = 0;

			/** Execute a given file
				@return false on error
			 */
			virtual bool RunFile(const std::string& path) = 0;



			/**
				Shuts the scripting runtime down and cleans up
			*/
			virtual void deinit() = 0;


		protected:
			void HandleEvent(Event& e) override;
	};

} // namespace sbe

#endif // SCRIPTING_HPP

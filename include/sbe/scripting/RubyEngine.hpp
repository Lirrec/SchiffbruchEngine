#ifndef RUBYSCRIPTING_HPP
#define RUBYSCRIPTING_HPP

#include <sbe/ScriptingEngine.hpp>

namespace sbe
{

	/**
		Ruby Scripting Engine.
	*/
	class RubyEngine : public ScriptingEngine
	{
	public:

		RubyEngine();

		~RubyEngine();

		/**
			Initialize the scripting language, has to be called once before using other functions
		*/
		virtual void init();

		/** Execute a given script
			@return false on error
		*/
		virtual bool RunString(const std::string& code);

		/** Execute a given file
			@return false on error
		 */
		virtual bool RunFile(const std::string& path);

		/**
			Shuts the scripting runtime down and cleans up
		*/
		virtual void deinit();

	private:
		void backtrace();

		typedef long unsigned int VALUE;

		bool protectedCall(VALUE(* fn)(VALUE), const std::string& msg = "", VALUE arg = 0, VALUE* out = nullptr);

	};

} //namespace sbe

#endif // RUBYSCRIPTING_HPP

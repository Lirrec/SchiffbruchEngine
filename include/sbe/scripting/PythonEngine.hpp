#ifndef PYSCRIPTING_HPP
#define PYSCRIPTING_HPP

#include "sbe/ScriptingEngine.hpp"

namespace sbe
{

	/**
		Python Scripting Engine.
	*/
	class PythonEngine : public ScriptingEngine
	{
	public:

		PythonEngine();

		~PythonEngine();

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


	};

} //namespace sbe

#endif // PYSCRIPTING_HPP

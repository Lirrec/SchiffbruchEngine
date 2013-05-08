#include "sbe/scripting/RubyEngine.hpp"

#include <ruby.h>

#include <cstdio>

namespace sbe
{

	RubyEngine::RubyEngine()
	{
	}

	RubyEngine::~RubyEngine()
	{
	}

	void RubyEngine::init()
	{
		Engine::out(Engine::ERROR) << "[rbScript] init:" << std::endl;

		ruby_init();
		ruby_init_loadpath();
		ruby_script( "sbeRuby" );

		RunString("$:.unshift(\"./res/scripting\")");

		if (!protectedCall(
							[](VALUE)->VALUE { rb_require("SBE.so"); },
							 "Error loading SBE module!" ))
			return;

		RunString("require '__init__.rb'");

		Engine::out(Engine::ERROR) << "[rbScript] initialized." << std::endl;
	}


	void RubyEngine::deinit()
	{
		ruby_finalize();
		Engine::out(Engine::ERROR) << "[rbScript] finalized." << std::endl;
	}

	bool RubyEngine::RunString(const std::string& code)
	{
		return protectedCall( [](VALUE arg)->VALUE { rb_eval_string( ((std::string*)arg)->c_str()); }, "Error executing string: "+code, (VALUE)&code);
	}

	bool RubyEngine::RunFile(const std::string& path)
	{
		return protectedCall( [](VALUE arg)->VALUE { rb_load_file( ((std::string*)arg)->c_str() ); }, "Error executing file: "+path, (VALUE)&path );
	}

	bool RubyEngine::protectedCall( VALUE(*fn)(VALUE), const std::string& msg, VALUE arg, VALUE* out)
	{
		int err;
		VALUE re = rb_protect( fn, arg, &err);
		if ( err )
		{
			backtrace();
			if ( msg != "" ) Engine::out(Engine::ERROR) << "[RbScript] " << msg << std::endl;
		}
		else
		{
			if ( out != nullptr ) *out = re;
		}

		return err == 0;
	}

	void RubyEngine::backtrace()
	{
		rb_eval_string_protect("if (e = $!) then STDERR.puts(\"#{e.class}:\n#{e.message}\", e.backtrace.map {|s| \"\tfrom #{s}\" }) end", nullptr);
	}

} // namespace sbe

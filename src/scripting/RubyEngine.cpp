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
		first = false;

		//PySys_SetPath( L".:res/scripting:/usr/lib64/python3.3" );
		int err;
		auto lambda = [](VALUE)->VALUE { rb_require("sbe"); };
		rb_protect( lambda, (VALUE)0, &err);
		if ( err )
		{
			backtrace();
			Engine::out(Engine::ERROR) << "[rbScript] Error loading SBE module!" << std::endl;
			return;
		}

		auto lambda2 = [](VALUE)->VALUE { rb_load_file("res/scripting/__init__.rb"); };
		rb_protect( lambda2, (VALUE)0, &err);
		if ( err )
		{
			backtrace();
			Engine::out(Engine::ERROR) << "[rbScript] Error loading __init__.rb!" << std::endl;
			return;
		}
		Engine::out(Engine::ERROR) << "[rbScript] initialized." << std::endl;
	}


	void RubyEngine::deinit()
	{
		ruby_finalize();
		Engine::out(Engine::ERROR) << "[rbScript] finalized." << std::endl;
	}

	bool RubyEngine::RunString(const std::string& code)
	{
		int re;
		rb_eval_string_protect( code.c_str(), &re );
		if ( re ) backtrace();
		return (re == 0);
	}

	bool RubyEngine::RunFile(const std::string& path)
	{
		int err = 0;

		rb_load_file( path.c_str() );

		//VALUE v;
		//auto l = [path](VALUE)->VALUE { return rb_load_file( path.c_str() ); };
		//rb_protect( l, v, &err);

		if ( err )
		{
			backtrace();
			Engine::out(Engine::ERROR) << "[RbScript] Error opening file: " << path << std::endl;
			return false;
		}

		return true;
	}

	void RubyEngine::backtrace()
	{
		int error;
		rb_eval_string_protect("if (e = $!) then STDERR.puts(\"#{e.class}:\n#{e.message}\", e.backtrace.map {|s| \"\tfrom #{s}\" }) end", &error);
	}

} // namespace sbe

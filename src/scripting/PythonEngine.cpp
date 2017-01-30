#include <sbe/scripting/PythonEngine.hpp>

#include <Python.h>

#include <cstdio>

namespace sbe
{

	PythonEngine::PythonEngine()
	{
	}

	PythonEngine::~PythonEngine()
	{
	}

	void PythonEngine::init()
	{
		Engine::out(Engine::ERROR) << "[PyScript] init:" << std::endl;
		//Py_SetProgramName(argv[0]);
		Py_Initialize();

		PySys_SetPath( L".:res/scripting:/usr/lib64/python3.3" );
		PyObject *module = PyImport_Import(PyUnicode_FromString("SBE"));

		RunFile("res/scripting/__init__.py");
		Engine::out(Engine::ERROR) << "[PyScript] initialized." << std::endl;
	}


	void PythonEngine::deinit()
	{
		Py_Finalize();
		Engine::out(Engine::ERROR) << "[PyScript] finalized." << std::endl;
	}

	bool PythonEngine::RunString(const std::string& code)
	{
		int re = PyRun_SimpleString( code.c_str() );
		return (re == 0);
	}

	bool PythonEngine::RunFile(const std::string& path)
	{
		FILE* fp = fopen( path.c_str(), "r" );
		if ( fp == NULL )
		{
			Engine::out(Engine::ERROR) << "[PyScript] Error opening file: " << path << std::endl;
			return false;
		}
		int re = PyRun_SimpleFile( fp, path.c_str() );
		fclose( fp );

		return (re == 0);
	}

} // namespace sbe

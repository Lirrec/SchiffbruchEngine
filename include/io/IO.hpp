#ifndef IO_H
#define IO_H

class IO
{

	public:
	void addBinaryPlugin(std::shared_ptr<iBinaryLoaderPlugin> LP);
	void addTreePlugin(std::shared_ptr<iTreeLoaderPlugin> LP);

	template<class T>
	std::shared_ptr<T> loadObjectFile<T>( const std::string& filename );

	template<class T>
	bool saveObject<T>( std::shared_ptr<T> pObj, const std::string& path );

}

///TODO: move into ResourceManager
class ResMgr
{

	plugin:
	// ebenfalls über Events erreichbar
	boost::ptree::node getSetting()
	void setSetting()

	void addPath( const std::string& path );

	void loadDirectory( const std::string& path );

	bool loadFile( const std::string& path );
}

#endif IO_H

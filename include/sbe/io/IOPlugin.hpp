#ifndef IOPLUGIN_H
#define IOPLUGIN_H

template<class T>
class iBinaryIOPlugin<T> : IOPlugin
{

	public:
        typedef std::vector<std::shared_ptr<T>> ObjectList;


        virtual ObjectList decodeStream(std::istream& in);


        /**
        * @return true on successfull encoding
        */
        virtual bool encodeStream( const T& object, std::ostream& out);

}

template<class T>
class iTreeIOPlugin<T> : IOPlugin
{

	public:
        typedef std::vector<std::shared_ptr<T>> ObjectList;

        virtual ObjectList loadObjects(boost::ptree::node& root);

        /**
        * @return true on successfull encoding
        */
        virtual bool saveObject( const T& object, boost::ptree::node& root);
}

#endif

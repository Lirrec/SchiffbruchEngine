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


#ifndef     HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_
# define    HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_

/**
A simple functor to allow using boost::uuids with std hash
@file taken from the hypodermic project:
http://code.google.com/p/hypodermic/source/browse/Hypodermic/BoostUuidHashFunctor.h

*/

# include <functional>
# include <boost/uuid/uuid.hpp>


namespace std
{

    template <>
    class hash< boost::uuids::uuid > : public std::unary_function< boost::uuids::uuid, size_t >
    {   // hash functor
    public:
        std::size_t operator()(const boost::uuids::uuid& _Keyval) const
        {       // hash _Keyval to size_t value by pseudorandomizing transform
            return boost::uuids::hash_value(_Keyval);
        }
    };

} // namespace std


#endif /* !HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_ */


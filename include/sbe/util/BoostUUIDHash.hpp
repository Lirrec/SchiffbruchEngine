#ifndef     HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_
#define    HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_

/**
	@file BoostUUIDHash.hpp
	A simple functor to allow using boost::uuids with std hash

	taken from the hypodermic project:
	http://code.google.com/p/hypodermic/source/browse/Hypodermic/BoostUuidHashFunctor.h
*/

# include <functional>
# include <boost/uuid/uuid.hpp>


// std::hash<boost::uuids::uuid> is now provided by Boost >= 1.66 natively.

#endif /* !HYPODERMIC_BOOST_UUID_HASH_FUNCTOR_H_ */

#ifndef STRINGHASH_H
#define STRINGHASH_H

#include <string>
#include <cstring>

#include <sbe/util/Hash.hpp>

namespace sbe
{
	/**
		@file HashedString.hpp
		This Code was taken from GameCode3, which accompanies the great book "Game Coding Complete"
		--
		GameCode3 is the sample application that encapsulates much of the source code
		discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
		Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5

		A hashed string.  It retains the initial (ANSI) string in addition to the hash value for easy reference.

		class HashedString				- Chapter 10, page 274
	*/
	class HashedString
	{
	public:

		explicit HashedString(char const* const pIdentString)
				: m_ident(hash_name(pIdentString)), m_identStr(pIdentString) {
		}

		HashType getHashValue(void) const {
			return m_ident;
		}

		const std::string& getStr() const {
			return m_identStr;
		}

		bool operator<(HashedString const& o) const {
			bool r = (getHashValue() < o.getHashValue());
			return r;
		}

		bool operator==(HashedString const& o) const {
			bool r = (getHashValue() == o.getHashValue());
			return r;
		}

	private:

		HashType m_ident;
		std::string m_identStr;
	};

} // namespace sbe

#endif // STRINGHASH_H

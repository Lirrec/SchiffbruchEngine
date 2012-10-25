#ifndef HASHMGR_H
#define HASHMGR_H

/** This class manages a set of mappings from string to hashedstrings.
This is required for the eventsystem, as internal stringcompares are costly.
*/

#include "HashedString.hpp"

#include <map>
#include <string>
#include <memory>

class HashManager
{
	public:

		HashManager() :invalid_name("##Invalid Hash!##") {}

		/// Register a new Element. Returns false on Collision
		bool RegisterElement ( const std::string& Name );

		/// UnRegister an Element. returns false if the given Element was not found
		bool UnRegisterElement ( const std::string& Name );
		/// UnRegister an Element. returns false if the given Element was not found
		bool UnRegisterElement ( const HashedString::HashType& Hash);

		/// Look up the Hash to a given String.
		/// @return 0 if String was not registered before
		HashedString::HashType GetHash( const std::string& Name) const;
		/// Look up the String to a given Hash.
		/// @return empty String if Hash was not registered before
		const std::string& GetString( const HashedString::HashType& Hash) const;

		/// Returns true if the given String is registered
		bool ElementExists( const std::string& Name ) const;
		/// Returns true if the given Hash is registerd
		bool ElementExists( const HashedString::HashType& Hash ) const;

		typedef std::map< std::string, std::shared_ptr<HashedString>> 					StringLookupList;
		typedef std::map< HashedString::HashType, std::shared_ptr<HashedString>>		HashLookupList;

	private:
		std::string 				invalid_name;
		StringLookupList			ElementsByString;
		HashLookupList				ElementsByHash;
};

#endif // HASHMGR_H


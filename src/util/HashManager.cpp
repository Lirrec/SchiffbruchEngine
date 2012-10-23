#include "util/HashManager.h"

#include "Engine.h"

/** @brief ElementExists
  *
  * @todo: document this function
  */
bool HashManager::ElementExists(const HashedString::HashType & Hash) const
{
	return ElementsByHash.find ( Hash ) != ElementsByHash.end();
}

/** @brief ElementExists
  *
  * @todo: document this function
  */
bool HashManager::ElementExists(const std::string& Name) const
{
	return ElementsByString.find ( Name ) != ElementsByString.end();
}

/** @brief GetString
  *
  * @todo: document this function
  */
const std::string& HashManager::GetString(const HashedString::HashType& Hash) const
{
	auto it = ElementsByHash.find ( Hash );
	if (it != ElementsByHash.end() )
	{
		return it->second->getStr();
	}

	Engine::out() << " Couldnt find String for Hash " << Hash << std::endl;
	return invalid_name;
}

/** @brief GetHash
  *
  * @todo: document this function
  */
HashedString::HashType HashManager::GetHash(const std::string& Name) const
{
	auto it = ElementsByString.find ( Name );
	if (it != ElementsByString.end() )
	{
		return it->second->getHashValue();
	}

	Engine::out() << " Couldnt find hash for string " << Name << std::endl;
	return HashedString::InvalidHash;
}

/** @brief UnRegisterElement
  *
  * @todo: document this function
  */
bool HashManager::UnRegisterElement(const HashedString::HashType& Hash)
{
	if (ElementExists( Hash ))
	{
		ElementsByString.erase( GetString( Hash ));
		ElementsByHash.erase ( Hash );
		return true;
	}
	return false;
}

/** @brief UnRegisterElement
  *
  * @todo: document this function
  */
bool HashManager::UnRegisterElement(const std::string& Name)
{
	if (ElementExists( Name ))
	{
		ElementsByHash.erase( GetHash( Name ));
		ElementsByString.erase ( Name );
		return true;
	}

	return false;
}

/** @brief RegisterElement
  *
  * @todo: document this function
  */
bool HashManager::RegisterElement(const std::string& Name)
{
	if ( !ElementExists( Name ) )
	{
		std::shared_ptr<HashedString> HS( new HashedString(Name.c_str()  ) );

		if (HS->getHashValue() == 0)
		{
			Engine::out() << "HashManager: Element '" << Name << "' gives 0 (zero) as Hash, which is reserved!" << std::endl;
			return false;
		}

		ElementsByString[ Name ] = HS;
		ElementsByHash[ HS->getHashValue() ] = HS;

		return true;
	}
	else
	{
		Engine::out() << "HashManager: Element '" << Name << "' already exists" << std::endl;
		return false;
	}
}


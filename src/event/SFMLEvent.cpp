#include "event/SFMLEvent.h"



/** @brief SFMLEventConverter
  *
  * @todo: document this function
  */
 SFMLEventConverter::SFMLEventConverter()
{

}

/** @brief CreateConversion
  *
  * @todo: document this function
  */
SFMLEventConverter::ConvEvt SFMLEventConverter::CreateConversion(const std::string& EvtName, bool sendglobal, boost::any Data)
{
	return std::make_tuple( EvtName, sendglobal, Data );
}

/** @brief AddEventConversion
  *
  * @todo: document this function
  */
void SFMLEventConverter::AddEventConversion(sf::Event::EventType SFMLEvtType, const std::string& EvtName, bool sendglobal, boost::any Data)
{
	EvtConversions.insert( std::make_pair( SFMLEvtType, CreateConversion( EvtName, sendglobal, Data )) );
}

/** @brief AddKeyConversion
  *
  * @todo: document this function
  */
void SFMLEventConverter::AddKeyConversion(sf::Keyboard::Key Key, const std::string& EvtName, bool sendglobal, boost::any Data)
{
	KeyConversions.insert( std::make_pair( Key, CreateConversion( EvtName, sendglobal, Data )) );
}

/** @brief HandleEvent
  *
  * @todo: document this function
  */
void SFMLEventConverter::HandleEvent(Event& e)
{
	if (e.Is("SFML_KEY_CONV"))
	{
		if (e.Data().type() == typeid(KeyConvData))
		{
			KeyConvData KCData = boost::any_cast<KeyConvData>(e.Data());
			KeyConversions.insert( KCData );
		}
	}
	else if (e.Is("SFML_EVT_CONV"))
	{
		if (e.Data().type() == typeid(EvtConvData))
		{
			EvtConvData ECData = boost::any_cast<EvtConvData>(e.Data());
			EvtConversions.insert( ECData );
		}
	}
}

/** @brief HandleEvent
  *
  * @todo: document this function
  */
bool SFMLEventConverter::HandleEvent(sf::Event& e)
{
	for ( auto it = EvtConversions.lower_bound( e.type ); it != EvtConversions.upper_bound( e.type ); ++ it )
	{
		Engine::out() << "Converting sf::event to event: " << std::get<0>(it->second) << std::endl;

		Event TmpEvt( std::get<0>(it->second) );
		TmpEvt.Data() = std::get<2>(it->second);
		Module::Get()->GetEventQueue()->QueueEvent( TmpEvt, std::get<1>(it->second) );
	}

	if (e.type == sf::Event::KeyReleased )
	{
		if ( KeyConversions.find( e.key.code ) != KeyConversions.end() )
		{
			for ( auto it = KeyConversions.lower_bound( e.key.code ); it != KeyConversions.upper_bound( e.key.code ); ++ it )
			{
				Engine::out() << "Converting key to event: " << std::get<0>(it->second) << std::endl;

				Event TmpEvt( std::get<0>(it->second) );
				TmpEvt.Data() = std::get<2>(it->second);
				Module::Get()->GetEventQueue()->QueueEvent( TmpEvt, std::get<1>(it->second) );
			}
		}

	}

	return false;
}



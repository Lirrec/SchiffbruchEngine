#include "sbe/event/SFMLEvent.hpp"
#include "sbe/event/SFMLKeys.hpp"

#include "sbe/Config.hpp"

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

 SFMLEventConverter::SFMLEventConverter()
{

}

SFMLEventConverter::ConvEvt SFMLEventConverter::CreateConversion(const std::string& EvtName, bool sendglobal, boost::any Data)
{
	return std::make_tuple( EvtName, sendglobal, Data );
}

void SFMLEventConverter::AddEventConversion(sf::Event::EventType SFMLEvtType, const std::string& EvtName, bool sendglobal, boost::any Data)
{
	EvtConversions.insert( std::make_pair( SFMLEvtType, CreateConversion( EvtName, sendglobal, Data )) );
}

void SFMLEventConverter::AddKeyConversion(sf::Keyboard::Key Key, const std::string& EvtName, bool sendglobal, boost::any Data)
{
	KeyConversions.insert( std::make_pair( Key, CreateConversion( EvtName, sendglobal, Data )) );
}

void SFMLEventConverter::AddKeyConversion(const std::string& Key, const std::string& EvtName, bool sendglobal, boost::any Data)
{
	auto keycode = SFMLKeyNames.find( Key );
	if ( keycode == SFMLKeyNames.end() )
	{
		Engine::out(Engine::ERROR) << "[SFMLEventConverter] Tried to register unknown key string: " << Key << std::endl;
		return;
	}

	KeyConversions.insert( std::make_pair( keycode->second, CreateConversion( EvtName, sendglobal, Data )) );
}

void SFMLEventConverter::HandleEvent(Event& e)
{
	if (e.Is("SFML_KEY_CONV", typeid(KeyConvData)))
	{
		KeyConvData KCData = boost::any_cast<KeyConvData>(e.Data());
		KeyConversions.insert( KCData );
	}
	else if (e.Is("SFML_EVT_CONV", typeid(EvtConvData )))
	{
		EvtConvData ECData = boost::any_cast<EvtConvData>(e.Data());
		EvtConversions.insert( ECData );
	}
}

void SFMLEventConverter::HandleSfmlEvent ( const sf::Event& e)
{
	for ( auto it = EvtConversions.lower_bound( e.type ); it != EvtConversions.upper_bound( e.type ); ++ it )
	{
		Engine::out(Engine::SPAM) << "[EvtConv] SFML: " << std::get<0>(it->second) << std::endl;

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
				Engine::out(Engine::SPAM) << "[EvtConv] KEY: " << std::get<0>(it->second) << std::endl;

				Event TmpEvt( std::get<0>(it->second) );
				TmpEvt.Data() = std::get<2>(it->second);
				Module::Get()->GetEventQueue()->QueueEvent( TmpEvt, std::get<1>(it->second) );
			}
		}

	}
}

int SFMLEventConverter::LoadKeyBindingsFromConfig(const std::string& root)
{
	auto rootnode = Engine::getCfg()->getPath(root);
	if (!rootnode) return 0;
	int count = 0;

	const boost::property_tree::ptree& pt = *rootnode;

	for ( const boost::property_tree::ptree::value_type& node : pt)
	{
		auto keycode = SFMLKeyNames.find(node.first);
		if( keycode != SFMLKeyNames.end())
		{
			try {
				AddKeyConversion(keycode->second, node.second.get<std::string>("event"),  node.second.get<bool>("global", false));
				count++;
			} catch ( boost::property_tree::ptree_error& e) {
				Engine::out(Engine::ERROR) << "[EvtConv::LoadKeyBindingsFromConfig] Error: " << e.what() << std::endl;
			}
		}
	}

	return count;
}

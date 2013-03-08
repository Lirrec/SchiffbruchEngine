#ifndef SFML_EVT_H
#define SFML_EVT_H


#include <SFML/Window/Event.hpp>
#include <boost/any.hpp>

#include "sbe/event/EventUser.hpp"
#include "sbe/event/SFMLEventUser.hpp"

#include <tuple>
#include <map>



/**
A Class for easily listening to global SFMLEvents
by letting them be converted to normalEvents.

Conversion can also be added by Events:

Name          | DataType
--------------|-------------------
SFML_KEY_CONV | SFMLEventConverter::KeyConvData
SFML_EVT_CONV | SFMLEventConverter::EvtConvData

*/
class SFMLEventConverter : public EventUser, public SFMLEventUser
{
	public:
		SFMLEventConverter();

		void HandleEvent( Event& e );

		void HandleSfmlEvent ( const sf::Event& e);

		/// This defines the event which is sent, once a conversion matches.
		/// The string is the Name of the event to be sent, the bool defines
		/// if it will be sent global, and the boost.any is optional Data for the event
		typedef std::tuple< std::string, bool, boost::any> ConvEvt;

		/// Data required for setting a key-conversion by event
		typedef std::pair< sf::Keyboard::Key, SFMLEventConverter::ConvEvt > KeyConvData;
		/// Data required for setting a event-conversion by event
		typedef std::pair< sf::Event::EventType, SFMLEventConverter::ConvEvt > EvtConvData;

		void AddKeyConversion( sf::Keyboard::Key Key, const std::string& EvtName, bool sendglobal = false, boost::any Data = boost::any() );
		void AddKeyConversion( const std::string& Key , const std::string& EvtName, bool sendglobal = false, boost::any Data = boost::any() );
		void AddEventConversion( sf::Event::EventType SFMLEvtType, const std::string& EvtName, bool sendglobal = false, boost::any Data = boost::any() );

		/**
			Loads key bindings from a node in the config file ( default: "KeyEventBindings"). Right now this only works for key bindings, no eventconversions (sfml->sbe)
			The config should look like this:

			@code
			KeyEventBindings
			{
				Escape	; //<- the name of the key, see sf::Keyboard::Key for possible values
				{
					Event "EVT_QUIT"	; //<- the name of the event to send
					global true			; //<- this is optional, default is false
				}

			}
			@endcode

			@return the number of bindings read from the config file
		*/
		int LoadKeyBindingsFromConfig(const std::string& root = "system.KeyEventBindings");

	private:
		ConvEvt CreateConversion( const std::string& EvtName, bool sendglobal, boost::any Data );

		std::multimap<sf::Keyboard::Key, ConvEvt> KeyConversions;
		std::multimap<sf::Event::EventType, ConvEvt> EvtConversions;
};

#endif // SFML_EVT_H

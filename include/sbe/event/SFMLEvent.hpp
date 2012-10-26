#ifndef SFML_EVT_H
#define SFML_EVT_H


#include <SFML/Window/Event.hpp>
#include <boost/any.hpp>

#include "event/EventUser.hpp"

#include <tuple>
#include <map>

/**
This Class defines a Listener for an sfml::Event
*/
class SFMLEventListener
{
	public:
	/// Handle an event
	/// @return returns true if the event was consumed, false it it should
	/// continue bubbling
	virtual bool HandleEvent( sf::Event& e ) = 0;
};


/** A Class for easily listening to global SFMLEvents
by letting them be converted to normalEvents.

Conversion can also be added by Events:

	Name		| 		DataType
----------------------------------
SFML_KEY_CONV	|  SFMLEventConverter::KeyConvData
SFML_EVT_CONV	|  SFMLEventConverter::EvtConvData

*/
class SFMLEventConverter : public EventUser, public SFMLEventListener
{
	public:
		SFMLEventConverter();

		virtual void HandleEvent( Event& e );

		virtual bool HandleEvent(sf::Event& e);

		/// This defines the event which is sent, once a conversion matches.
		/// The string is the Name of the event to be sent, the bool defines
		/// if it will be sent global, and the boost.any is optional Data for the event
		typedef std::tuple< std::string, bool, boost::any> ConvEvt;

		/// Data required for setting a key-conversion by event
		typedef std::pair< sf::Keyboard::Key, SFMLEventConverter::ConvEvt > KeyConvData;
		/// Data required for setting a event-conversion by event
		typedef std::pair< sf::Event::EventType, SFMLEventConverter::ConvEvt > EvtConvData;

		void AddKeyConversion( sf::Keyboard::Key Key, const std::string& EvtName, bool sendglobal = false, boost::any Data = boost::any() );
		void AddEventConversion( sf::Event::EventType SFMLEvtType, const std::string& EvtName, bool sendglobal = false, boost::any Data = boost::any() );

	private:
		ConvEvt CreateConversion( const std::string& EvtName, bool sendglobal, boost::any Data );

		std::multimap<sf::Keyboard::Key, ConvEvt> KeyConversions;
		std::multimap<sf::Event::EventType, ConvEvt> EvtConversions;
};

#endif // SFML_EVT_H

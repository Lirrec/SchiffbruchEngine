#ifndef SFMLKEYS_HPP
#define SFMLKEYS_HPP

#include <map>
#include <string>
#include <SFML/Window/Keyboard.hpp>

/**
	This global Map contains a std::string to sf::Keyboard::Key mapping.
	All values available in sf::Keyboard::Key can be looked up by their string names.
	Usefull for storing Key bindings in config files
*/
extern const std::map<std::string, sf::Keyboard::Key> SFMLKeyNames;

#endif // SFMLKEYS_HPP


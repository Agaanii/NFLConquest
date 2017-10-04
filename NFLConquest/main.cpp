#include "SFML\Graphics.hpp"

sf::RenderWindow s_window(sf::VideoMode(1600, 900), "NFL Conquest");

int main() 
{
	sf::Event event;
	while (true)
	{
		while (s_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}
	}
	return 0; 
}
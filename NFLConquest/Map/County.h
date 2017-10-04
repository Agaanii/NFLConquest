#include "SFML\Graphics.hpp"

#include <memory>

using CountyId = long;
struct County
{
	County() { }
	County(std::unique_ptr<sf::Sprite>&& sprite, const sf::Vector2f& position, const CountyId& county)
		: m_sprite(std::move(sprite))
		, m_globalPosition(position)
		, m_countyId(county)
	{ }
	County(County&& moveOther)
		: m_sprite(std::move(moveOther.m_sprite))
		, m_globalPosition(moveOther.m_globalPosition)
		, m_countyId(moveOther.m_countyId)
	{ }
	County(const County& other) = delete;

	std::unique_ptr<sf::Sprite> m_sprite;
	sf::Vector2f m_globalPosition;
	CountyId m_countyId{ 0 };
};

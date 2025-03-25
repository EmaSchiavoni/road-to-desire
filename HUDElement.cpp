#include "HUDElement.h"

using std::abs;
using rtd::HUDElement;
using namespace sf;

HUDElement::HUDElement()
{
	m_backgroundTexture.loadFromFile("Textures/Statics/HUD_background.png");
	m_borderTexture.loadFromFile("Textures/Statics/HUD_border.png");
	m_background.setTexture(m_backgroundTexture);
	m_border.setTexture(m_borderTexture);
	setPosition(0, 0);
}

void HUDElement::invert(bool value)
{
	Vector2f currentBorderScale = m_border.getScale();
	Vector2f currentBgScale = m_background.getScale();
	Vector2f position = m_background.getPosition();

	if (value && !m_is_inverted) {
		m_is_inverted = true;
		m_border.setScale(-currentBorderScale.x, currentBorderScale.y);
		m_background.setScale(-currentBgScale.x, currentBgScale.y);
		m_background.setOrigin(m_background.getGlobalBounds().width, 0);
	}

	if (!value && m_is_inverted) {
		m_is_inverted = false;
		m_border.setScale(abs(currentBorderScale.x), currentBorderScale.y);
		m_border.setOrigin(0, 0);
		m_background.setScale(abs(currentBgScale.x), currentBgScale.y);
		m_background.setOrigin(0, 0);
	}

	setPosition(position.x, position.y);
}

void HUDElement::setWidth(float width)
{
	float new_x_scale = width / m_backgroundTexture.getSize().x;
	m_background.setScale(new_x_scale, m_background.getScale().y);

	auto bgPos = m_background.getPosition();
	m_border.setPosition(bgPos.x + m_background.getGlobalBounds().width, bgPos.y);
}

void HUDElement::setHeight(float height)
{
	float new_y_scale = height / m_backgroundTexture.getSize().y;
	m_background.setScale(m_background.getScale().x, new_y_scale);
	m_border.setScale(
		(m_is_inverted)? -new_y_scale : new_y_scale,
		new_y_scale
	);
}

void HUDElement::setPosition(float x, float y)
{
	m_background.setPosition(x, y);
	float border_new_x_pos = (m_is_inverted)? m_background.getGlobalBounds().left : x + m_background.getGlobalBounds().width;
	m_border.setPosition(border_new_x_pos, y);
}

FloatRect HUDElement::getGlobalBounds() const
{
	return m_background.getGlobalBounds();
}

void HUDElement::draw(sf::RenderWindow& window)
{
	window.draw(m_background);
	window.draw(m_border);
}
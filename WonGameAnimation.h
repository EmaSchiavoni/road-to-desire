#pragma once
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Animation.h"

namespace rtd {

	class WonGameAnimation
	{
		sf::SoundBuffer m_ballSoundBuffer;
		sf::Sound m_ballSound;
		sf::Texture m_ballTextures[7];
		sf::Sprite m_ballSprites[7];
		rtd::Animation m_ballAnimations[7];
		rtd::Animation m_rectsAnimation, m_textAnimation;
		sf::Text m_text, m_shadow;
		sf::Color m_pureMagenta = { 255, 0, 42 };
		sf::Color m_pureYellow = { 255, 200, 0 };
		sf::RectangleShape m_topBgRect, m_botBgRect;
		float m_elapsed_time = 0;
		float m_bg_color_time = 0;
		bool m_bg_color = true;
		float m_ball_animation_duration = .5;
		int m_current_ball_animation = 0;
		bool m_music_was_set = false;

		void prepareSound();
		void prepareBalls();
		void prepareRects();
		void prepareText();
		void prepareAnimations();
		void updateBallsAnimation();
		void updateRectsAnimation();
		void updateTextAnimation();
		void updateBackground(const float& delta_time);

	public:
		WonGameAnimation();
		bool animationsAreCompleted() const;
		void update(const float& delta_time);
		void draw(sf::RenderWindow& window);
	};

}
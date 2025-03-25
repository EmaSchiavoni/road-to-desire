#pragma once
#include "Menu.h"
#include "TextInput.h"

namespace rtd {
	
	class SettingsMenu : public rtd::Menu
	{
		rtd::TextInput m_musicVolumeInput;
		rtd::TextInput m_soundsVolumeInput;
		rtd::TextInput* m_inputs[2];
		const int m_inputs_count = 2;

		int getInputValue(rtd::TextInput& input) const;
		void increaseCurrentInput();
		void reduceCurrentInput();
		void sanitizeInput(rtd::TextInput& input);
		void sanitizeAllInputs();

	public:
		SettingsMenu();
		void update(const float& delta_time, rtd::Game& game) override;
		void select(rtd::Game& game);
		void processEvents(sf::Event event) override;
		void draw(sf::RenderWindow& window) override;
		void save();
		~SettingsMenu();
	};

}
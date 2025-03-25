#pragma once
#include "Collectable.h"

namespace rtd {

	class Coin : public rtd::Collectable
	{
	public:
		Coin();
		Coin(sf::Vector2u positionAsTile);
		~Coin();
		void setTexture() override;
	};

}
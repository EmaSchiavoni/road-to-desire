#include "Coin.h"
#include "ResourceHandler.h"

using rtd::Coin;
using rtd::ResourceHandler;
using rtd::resourceHandler;
using sf::Vector2u;

extern ResourceHandler resourceHandler;

Coin::Coin(Vector2u positionAsTile) : Collectable(positionAsTile)
{
	this->setType(Collectable::Type::Coin);
	setTexture();
	centerOrigin();
}

Coin::~Coin()
{
	resourceHandler.unloadAnimatedCoin();
}

void Coin::setTexture()
{
	auto coinAnimationData = resourceHandler.getAnimatedCoin();
	coinAnimationData.first->setSmooth(true);
	makeItAnimated(*coinAnimationData.first, coinAnimationData.second);
}
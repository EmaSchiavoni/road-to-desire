#pragma once
#include <SFML/System/Vector2.hpp>
#include "Entity.h"

namespace rtd {

	class Collectable : public rtd::Entity
	{
	public:
		enum class Type {
			NOT_INITIALIZED = -1,
			Coin = 0,
			DragonBall1 = 1,
			DragonBall2 = 2,
			DragonBall3 = 3,
			DragonBall4 = 4,
			DragonBall5 = 5,
			DragonBall6 = 6,
			DragonBall7 = 7
		};

		Collectable(sf::Vector2u positionAsTile);
		Collectable(const Collectable*& collectable);
		virtual ~Collectable();
		virtual bool canBeDeleted();
		void setGlobalPosition(float x, float y);
		sf::Vector2u getPositionAsTile() const;
		sf::Vector2f getGlobalPosition() const;
		sf::Vector2f getSize() const;
		void collect();
		bool wasCollected() const;
		virtual void setTexture() = 0;
		virtual int getTypeId() const;
		virtual Collectable::Type getType() const;

	protected:
		void setType(Collectable::Type type);

	private:
		Collectable::Type m_type = Collectable::Type::NOT_INITIALIZED;
		sf::Vector2u m_posAsTile;
		sf::Vector2f m_globalPosition;
		bool m_collected = false;
	};

	std::string toString(Collectable::Type type);
	int getCollectableId(Collectable::Type type);

}
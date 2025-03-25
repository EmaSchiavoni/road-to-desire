#include "Map.h"
#include "MapFile.h"
#include "Level.h"
#include "MainMenu.h"
#include "SoundsHandler.h"
#include "SoundPaths.h"
#include "Settings.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern SoundsHandler globalSoundsHandler;
extern Profile currentProfile;

Map::Map(Map& map)
{
    extern Settings settings;
    globalSoundsHandler.setMusic(string(MUSIC_PATH) + "map.ogg");
    m_moveSoundBuffer.loadFromFile(string(MAP_SOUNDS_PATH) + "move.ogg");
    m_moveSound.setBuffer(m_moveSoundBuffer);
    m_moveSound.setVolume(settings.getSoundsVolume());
    m_backSprite.setTexture(*resourceHandler.getBackTexture());
    m_backSprite.setScale(.2f, .2f);
    m_backSprite.setPosition(0, WIN_HEIGHT - 40);
    m_backSprite.setColor({ 255,255,255,155 });

    this->m_map_name = map.m_map_name;
    this->setPartsQuantity( map.getPartsQuantity() );
    this->m_levels = map.m_levels;
    this->init();
}

Map::Map()
{
    m_map_name = currentProfile.getMapName();
}

Map::~Map()
{
    if (m_initialized)
        resourceHandler.unloadBackTexture();
}

void Map::init()
{
    if (m_initialized || m_parts_quantity <= 0)
        return;

    loadTextures();
    generateMapSprites();
    generateLockSprite();
    generateSelector();

    setFocus(*m_levels.begin());
    m_selector.setPosition(m_level_focus->getPosition());

    unsigned int lastUnlockedLevel = min( currentProfile.getLevelScores().size(), m_levels.size() - 1 );
    unlockUpToLevel(lastUnlockedLevel);
    m_HUD.setPoints();
    m_HUD.setDragonBalls();

    setFocus( *next(m_levels.begin(), lastUnlockedLevel) );

    if (!currentProfile.wonGame()) {
        if (gameIsComplete()) {
            m_wonGameAnimation = new WonGameAnimation();
            currentProfile.setWonGame(true);
            currentProfile.save();
        }
    } else
        globalSoundsHandler.setMusic(string(MUSIC_PATH) + "won.ogg");

    m_initialized = true;
}

bool Map::gameIsComplete() const
{
    return currentProfile.getDragonBalls().size() == 7;
}

int Map::getCurrentPart() const
{
    return m_level_focus->getMapPart();
}

void Map::loadTextures()
{
    m_lockTexture.loadFromFile("Textures/Maps/lock.png");
    m_lockTexture.setSmooth(true);

    m_selectorTexture.loadFromFile("Textures/Maps/selector.png");
    m_selectorTexture.setSmooth(true);

    for (int i = 0; i < m_parts_quantity; ++i) {
        m_mapTextures.push_back(Texture());
        (--m_mapTextures.end())->setSmooth(true);
        (--m_mapTextures.end())->loadFromFile("Textures/Maps/" + m_map_name + "/" + std::to_string(i) + ".jpg");
    }
}

void Map::generateSelector()
{
    m_selector.setTexture(m_selectorTexture);
    m_selector.setScale(.25, .25);

    FloatRect rect = m_selector.getLocalBounds();
    m_selector.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
}

void Map::generateMapSprites()
{
    if (m_mapSprites.size() == 0)
        for (auto texture_it = m_mapTextures.begin(); texture_it < m_mapTextures.end(); ++texture_it) {
            m_mapSprites.push_back(Sprite());
            Sprite* s = &*(m_mapSprites.end() - 1);
            s->setTexture(*texture_it);
            s->setScale(.5, .5);
        }
}

void Map::generateLockSprite()
{
    m_lockSprite.setTexture(m_lockTexture);
    m_lockSprite.setScale(.5, .5);
    FloatRect lockRect = m_lockSprite.getLocalBounds();
    m_lockSprite.setOrigin(lockRect.left + lockRect.width / 2.0f, lockRect.top + lockRect.height / 2.0f);
}

void Map::unlockLevel(unsigned int level)
{
    (m_levels.begin() + level)->unlock();
}

void Map::unlockUpToLevel(unsigned int last_unlocked_index)
{
    for (unsigned int i = 0; i <= last_unlocked_index; ++i)
        next(m_levels.begin(), i)->unlock();
}

void Map::addLevel(MapLevel level)
{
    m_levels.push_back(level);
}

MapLevel& Map::getLevelStructure(int index)
{
    return m_levels[index];
}

int Map::getPartsQuantity()
{
    return m_parts_quantity;
}

void Map::draw(RenderWindow& window)
{
    window.clear({ 0,0,0 });
    window.draw( *(m_mapSprites.begin() + getCurrentPart()) );

    for (auto lvl_it = m_levels.begin(); lvl_it < m_levels.end(); ++lvl_it)
        if (lvl_it->getMapPart() == getCurrentPart() && lvl_it->isLocked()) {
            m_lockSprite.setPosition(lvl_it->getPosition());
            window.draw(m_lockSprite);
        }
        else if (lvl_it->getMapPart() > getCurrentPart())
            break;
    window.draw(m_selector);
    m_HUD.draw(window);
    window.draw(m_backSprite);

    if (m_wonGameAnimation != nullptr) {
        m_wonGameAnimation->draw(window);
        if (m_wonGameAnimation->animationsAreCompleted())
            window.draw(m_backSprite);
    }

    window.display();
}

void Map::update(const float& delta_time, Game& game)
{
    if (m_wonGameAnimation != nullptr) {
        m_wonGameAnimation->update(delta_time);
        return;
    }

    updateSelector(delta_time);

    if (m_level_selected != -1) {
        game.setScene(new Level(m_map_name, m_level_selected));
        return;
    }

    if (m_back_to_menu)
        game.setScene(new MainMenu());
}

void Map::processEvents(Event event)
{
    if (m_wonGameAnimation != nullptr) {
        if (m_wonGameAnimation->animationsAreCompleted() && event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            delete m_wonGameAnimation;
            m_wonGameAnimation = nullptr;
        }
        return;
    }

    switch (event.type) {

    case Event::KeyPressed:
        switch (event.key.code) {

        case Keyboard::Up:
            moveFocus(Ubication::top);
            break;

        case Keyboard::Right:
            moveFocus(Ubication::right);
            break;

        case Keyboard::Down:
            moveFocus(Ubication::bottom);
            break;

        case Keyboard::Left:
            moveFocus(Ubication::left);
            break;

        case Keyboard::Enter: {
            auto lvl_focus_it = find(m_levels.begin(), m_levels.end(), *m_level_focus);
            m_level_selected = lvl_focus_it - m_levels.begin();
            globalSoundsHandler.setSound(string(MAP_SOUNDS_PATH) + "select.ogg");
            break;
        }

        case Keyboard::Escape: {
            m_back_to_menu = true;
            break;
        }
        }
        break;

    }
}

void Map::moveFocus(Ubication direction)
{
    int index = m_level_focus->getNeighborIndex(direction);
    bool is_null = index == -1;

    if (index >= 0) {
        MapLevel& target = m_levels[index];

        if (!target.isLocked() && !is_null)
            setFocus(target);
    }
}

void Map::setFocus(MapLevel& target)
{
    m_level_focus = &target;
    m_moveSound.play();
}

void Map::updateSelector(float delta_time)
{
    m_selector.rotate(60 * delta_time);

    if (m_selector.getPosition() != m_level_focus->getPosition()) {
        Vector2f A = m_selector.getPosition(); //Punto A
        Vector2f B = m_level_focus->getPosition(); //Punto B
        
        float x_distance = abs(B.x - A.x);
        int x_direction = (B.x > A.x)? 1 : -1;
        float x = A.x + x_direction * x_distance * delta_time * 5;

        float y_distance = abs(B.y - A.y);
        int y_direction = (B.y > A.y) ? 1 : -1;
        float y = A.y + y_direction * y_distance * delta_time * 5;

        m_selector.setPosition(x, y);
    }
}

void Map::setPartsQuantity(int parts_quantity)
{
    m_parts_quantity = parts_quantity;
}
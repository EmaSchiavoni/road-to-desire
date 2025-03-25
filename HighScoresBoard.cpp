#include "HighScoresBoard.h"
#include "MainMenu.h"
#include "SoundsHandler.h"
#include "SoundPaths.h"
#include "ResourceHandler.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern SoundsHandler globalSoundsHandler;
extern ResourceHandler resourceHandler;
extern Profile currentProfile;

HighScoresBoard::HighScoresBoard()
{
    globalSoundsHandler.setMusic(string(MUSIC_PATH) + "menu.ogg");
	m_backgroundTexture = resourceHandler.getMenuBackground();
	m_font = resourceHandler.getMenuFont();
	m_mapMenu = new MapSelectMenu();

    m_backSprite.setTexture(*resourceHandler.getBackTexture());
    m_backSprite.setScale(.2f, .2f);
    m_backSprite.setPosition(0, WIN_HEIGHT - 40);
    m_backSprite.setColor({ 255,255,255,125 });

    m_backgroundSprite.setTexture(*m_backgroundTexture);
    m_backgroundSprite.setScale(WIN_WIDTH / m_backgroundTexture->getSize().x, WIN_WIDTH / m_backgroundTexture->getSize().x);
    setTitle();
}

HighScoresBoard::~HighScoresBoard()
{
	if (m_mapMenu != nullptr)
		delete m_mapMenu;
	resourceHandler.unloadMenuBackground();
	resourceHandler.unloadMenuFont();
    resourceHandler.unloadBackTexture();
}

void HighScoresBoard::setTitle()
{
    m_title.setFont(*m_font);
    m_title.setString("Puntajes altos");
    m_title.setFillColor({ 255, 220, 45 });
    m_title.setLetterSpacing(1);
    m_title.setCharacterSize(m_font_size*3);
    m_title.setScale(m_font_scale, m_font_scale);
    m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getLocalBounds().height);
    m_title.setPosition(WIN_WIDTH/2, 75);
}

void HighScoresBoard::setBoardText()
{
    string board_text = getHighScoresString();
    if (board_text.empty())
        board_text = "Aun no hay puntajes";
    
    for (int i = 0; i < 2; ++i) {
        Text* text = (i)? &m_text : &m_shadow;
        text->setFont(*m_font);
        text->setString(board_text);
        text->setFillColor(m_textColor);
        text->setLetterSpacing(2);
        text->setLineSpacing(1.5);
        text->setCharacterSize(m_font_size);
        text->setScale(m_font_scale, m_font_scale);
        text->setOrigin(text->getLocalBounds().width / 2, 0);
    }
    m_shadow.setFillColor({0,0,0,180});
    m_shadow.setPosition(WIN_WIDTH/2+1.5f, m_title.getPosition().y + 90 + 1.5f);
    m_text.setPosition(WIN_WIDTH/2, m_title.getPosition().y + 90);
}

string HighScoresBoard::getHighScoresString() const
{
    auto highScores = currentProfile.getHighScores();
    string scores_string;
    int position = 1;
    for (auto score = highScores.begin(); score != highScores.end(); ++score)
    {
        string position_str = (position < 10)? "0" + to_string(position++) : "10";
        string points_str = to_string( (int)round(score->second) ) + " Pts.\n";
        string score_string = position_str + "    " + score->first;
        while (score_string.size() < 40 - points_str.size())
            score_string += " ";
        score_string += points_str;
        scores_string += score_string;
    }

    return scores_string;
}

void HighScoresBoard::processEvents(Event event)
{
    switch (event.type) {

    case Event::KeyReleased:
        switch (event.key.code) {

        case Keyboard::Up:
        case Keyboard::Down:
            if(m_mapMenu != nullptr)
                m_mapMenu->processEvents(event);
            break;
        case Keyboard::Enter:
            if (!m_map_selected && m_mapMenu != nullptr)
                m_map_selected = true;
            break;

        case Keyboard::Escape:
            m_escape_was_pressed = true;
            break;
        }
        break;
    }
}

void HighScoresBoard::update(const float& delta_time, Game& game)
{
    if (m_map_selected)
        currentProfile.setMap(m_mapMenu->getCurrentButtonText());

    if (m_map_selected && !string(currentProfile.getMapName()).empty())
    {
        globalSoundsHandler.setSound( string(MENU_SOUNDS_PATH) + "select.ogg" );
        setBoardText();
        if (m_mapMenu != nullptr) {
            delete m_mapMenu;
            m_mapMenu = nullptr;
            m_map_selected = false;
        }
    }
    if (m_escape_was_pressed)
        game.setScene(new MainMenu());
}

void HighScoresBoard::draw(sf::RenderWindow& window)
{
    if (m_mapMenu != nullptr) {
        m_mapMenu->draw(window);
        return;
    }
    window.clear();
    window.draw(m_backgroundSprite);
    window.draw(m_backSprite);
    auto exTitleColor = m_title.getFillColor();
    auto exTitlePos = m_title.getPosition();
    m_title.setFillColor({ 0,0,0 });
    m_title.setPosition(exTitlePos.x + 3, exTitlePos.y + 4);
    window.draw(m_title);
    m_title.setFillColor(exTitleColor);
    m_title.setPosition(exTitlePos);
    window.draw(m_title);
    window.draw(m_shadow);
    window.draw(m_text);
    window.display();
}
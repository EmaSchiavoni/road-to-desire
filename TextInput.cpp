#include "TextInput.h"

using rtd::TextInput;
using namespace std;
using namespace sf;

TextInput::TextInput(string initialText, Vector2f size, const Font& font, unsigned int font_size, Color background_color) :
    m_inputBox(size),
    m_outline{RectangleShape(size), RectangleShape(size)},
    m_sides{CircleShape(size.y/2, 40), CircleShape(size.y/2, 40)}
{
    m_inputBox.setFillColor(background_color);

    m_text.setFont(font);
    m_text.setCharacterSize(font_size);
    m_text.setScale(.25, .25);
    m_text.setFillColor({ 255,255,255 });
    m_text.setString(initialText);
    m_text.setLetterSpacing(.5);

    alignCenter();

    for (size_t i = 0; i < 2; ++i) {
        m_sides[i].setFillColor(background_color);
        m_sides[i].setOutlineColor({ 255, 255, 255 });
        m_sides[i].setOutlineThickness(1);
    }

    for (size_t i = 0; i < 2; ++i) 
        m_outline[i].setFillColor({ 255, 255, 255 });
}

TextInput::~TextInput()
{

}

void TextInput::setAlignCenter()
{
    FloatRect inputBox = m_inputBox.getGlobalBounds();

    FloatRect textBox = m_text.getLocalBounds();
    m_text.setOrigin(textBox.width / 2.0f, textBox.height / 2.0f);
    m_text.setPosition({ inputBox.left + inputBox.width / 2.0f, inputBox.top + inputBox.height / 2.0f - 4 });
}

void TextInput::setAlignLeft()
{
    FloatRect inputBox = m_inputBox.getGlobalBounds();

    FloatRect textBox = m_text.getLocalBounds();
    m_text.setOrigin(textBox.left, textBox.height / 2.0f);
    m_text.setPosition({ inputBox.left, inputBox.top + inputBox.height / 2.0f - 6});
}


void TextInput::input(Uint32 unicode_key_pressed)
{
    String str = m_text.getString();
    if (unicode_key_pressed == '\b')//es backspace
    {
        if(str.getSize() > 0)
            str.erase(str.getSize() - 1, 1);
    }
    else if (str.getSize() < max_characters && (
        (unicode_key_pressed > 47 && unicode_key_pressed < 58) ||
        (unicode_key_pressed > 64 && unicode_key_pressed < 91) ||
        (unicode_key_pressed > 96 && unicode_key_pressed < 123)))
        str += static_cast<char>(unicode_key_pressed);

    m_text.setString(str);

    if (m_align_center)
        setAlignCenter();
    else
        setAlignLeft();
}

void TextInput::alignCenter()
{
    m_align_center = true;
    setAlignCenter();
}

void TextInput::alignLeft()
{
    m_align_center = false;
    setAlignLeft();
}

void TextInput::setOutline(Color color)
{
    m_outline[0].setFillColor(color);
    m_outline[1].setFillColor(color);
}

void TextInput::setColor(Color color)
{
    m_text.setFillColor(color);
}

void TextInput::setBackground(Color color)
{
    m_inputBox.setFillColor(color);
}

void TextInput::setOrigin(float x, float y)
{
    m_inputBox.setOrigin(x, y);
    m_outline[0].setOrigin(x, y);
    m_outline[1].setOrigin(x, y);
}

void TextInput::setPosition(Vector2f pos)
{
    m_inputBox.setPosition(pos);

    m_outline[0].setPosition({pos.x, pos.y-1});
    m_outline[1].setPosition({ pos.x, pos.y + 1 });

    FloatRect inputBounds = m_inputBox.getGlobalBounds();
    float circleWidth = m_sides[0].getLocalBounds().width;

    m_sides[0].setPosition({ inputBounds.left - circleWidth/2, inputBounds.top });
    m_sides[1].setPosition({ inputBounds.left + inputBounds.width - circleWidth/2, inputBounds.top});

    if (m_align_center)
        setAlignCenter();
    else
        setAlignLeft();
}

void TextInput::setString(string string)
{
    m_text.setString(string);

    if (m_align_center)
        setAlignCenter();
    else
        setAlignLeft();
}

Vector2f TextInput::getPosition() const
{
    return m_inputBox.getPosition();
}

FloatRect TextInput::getLocalBounds() const
{
    return m_inputBox.getLocalBounds();
}

String TextInput::getString() const
{
    return m_text.getString();
}

size_t TextInput::getStringSize() const
{
    return m_text.getString().getSize();
}

void TextInput::draw(RenderWindow& window)
{
    window.draw(m_outline[0]);
    window.draw(m_outline[1]);
    window.draw(m_sides[0]);
    window.draw(m_sides[1]);
    window.draw(m_inputBox);
    window.draw(m_text);
}
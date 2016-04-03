#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#pragma once

#endif // BUTTON_H_INCLUDED

class Button {

    bool pressed;
    int weight, height, posx, posy;
    std::ostringstream label;

    public:
    sf::Text text;
    sf::Font font;

    sf::RectangleShape button;

    void newButton(int wh, int hh, int px, int py)
    {

        weight=wh; height=hh; posx=px; posy=py;
        button.setSize(sf::Vector2f(weight, height));
        button.setPosition(posx,posy);
        //button.setFillColor(sf::Color(200, 200, 200));
        button.setFillColor(sf::Color(200, 20, 70));
    };

    bool isCursor (float corx, float cory)
    {
        if ((corx>posx) && (corx<weight+posx) && (cory>posy) && (cory<height+posy)) {
        button.setFillColor(sf::Color(100, 100, 100));
        return true; }
        else { button.setFillColor(sf::Color(200, 200, 200));
        return false;
        }
    }

    void setLabel (std::string lb, std::string fnt)
    {
        label <<lb;

        font.loadFromFile(fnt);
        text.setString(label.str());
        text.setFont(font);

        text.setCharacterSize(24);
        text.setColor(sf::Color::Black);
        text.setPosition(posx+0,posy);

    }

    void upPosText()
    {
    text.setPosition(posx+20,posy);
    }


};

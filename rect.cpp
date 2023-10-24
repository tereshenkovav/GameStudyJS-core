#include "rect.h"
#include "game.h"

Rect::Rect(Game * game, QObject *parent) : QObject(parent)
{
    this->game = game ;

    rect.setFillColor(sf::Color::White) ;
    rect.setOutlineColor(sf::Color::White) ;
}

void Rect::drawTo(int x, int y, int w, int h)
{
    rect.setPosition(x,y) ;
    rect.setSize(sf::Vector2f(w,h)) ;
    game->RenderDrawable(rect) ;
}

void Rect::setFillColor(int r, int g, int b, int a)
{
    rect.setFillColor(sf::Color(r,g,b,a)) ;
}

void Rect::setBorderColor(int r, int g, int b, int a)
{
    rect.setOutlineColor(sf::Color(r,g,b,a)) ;
}

void Rect::setLineWidth(int w)
{
    rect.setOutlineThickness(w) ;
}

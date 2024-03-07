#include "text.h"
#include "game.h"
#include <QDebug>

Text::Text(QString fontname, Game *game, QObject *parent) : QObject(parent)
{
    this->game = game ;

    m_font.loadFromFile(("fonts/"+fontname).toStdString()) ;

    m_text.setFillColor(sf::Color::White) ;
    m_text.setFont(m_font);
    m_text.setCharacterSize(14) ;
    setText("") ;    
    iscentered = false ;
}

void Text::setXY(int x, int y)
{
    m_text.setPosition(x-(iscentered?m_text.getLocalBounds().width/2:0),y) ;
}

void Text::printTo(int x, int y)
{
    setXY(x,y) ;
    print() ;
}

void Text::print()
{
    game->RenderDrawable(m_text) ;
}

void Text::setAlignCenter()
{
    iscentered=true ;
}

void Text::setText(QString str)
{
    str = str.replace("\\n","\n\r") ;
    QByteArray bb = str.toUtf8();
    m_text.setString(sf::String::fromUtf8(bb.constBegin(),bb.constEnd())) ;
}

void Text::setSize(int size)
{
    m_text.setCharacterSize(size);
}

void Text::setColor(int r, int g, int b)
{
    m_text.setFillColor(sf::Color(r,g,b)) ;
}

int Text::getTextWidth()
{
    return (int)m_text.getLocalBounds().width ;
}

bool Text::isPointIn(int x, int y) const
{
    return ((m_text.getPosition().x<x)&&(x<m_text.getPosition().x+m_text.getLocalBounds().width)&&
            (m_text.getPosition().y<y)&&(y<m_text.getPosition().y+m_text.getLocalBounds().height)) ;
}

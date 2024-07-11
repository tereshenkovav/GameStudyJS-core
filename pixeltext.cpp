#include "pixeltext.h"
#include "game.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>

const int SYMW = 8 ;
const int SYMH = 8 ;
const int charsizeinmap = SYMW*SYMH ;
const int SPACEW = 4 ;
const int LETTERDIST = 1 ;
const int SYM_ENDLINE = 10 ;
const int VERTDIST = 2 ;

PixelText::PixelText(QString fontfile, Game *game, QObject *parent) : QObject(parent)
{
    this->game = game ;
    this->usemonowidth = false ;

    QFile file("fonts/"+fontfile);
    if (file.exists())
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray chars = file.readAll() ;
            pixelmap = QBitArray::fromBits(chars.data(),chars.length()*8) ;
            for (int i=0; i<256; i++) {
                QPair<int,int> sf(0,0) ;
                if (i==32) {
                    mapsf.push_back(sf) ;
                    continue ;
                }
                for (int x=0; x<SYMW; x++) {
                    bool isspace = true ;
                    for (int y=0; y<SYMH; y++)
                        if (pixelmap[charsizeinmap*i+y*SYMW+x]) isspace=false ;
                    if (!isspace) {
                        sf.first = x ;
                        break ;
                    }
                }
                for (int x=SYMW-1; x>=0; x--) {
                    bool isspace = true ;
                    for (int y=0; y<SYMH; y++)
                        if (pixelmap[charsizeinmap*i+y*SYMW+x]) isspace=false ;
                    if (!isspace) {
                        sf.second = x ;
                        break ;
                    }
                }
                mapsf.push_back(sf) ;
            }
        }

    spr.setColor(sf::Color::White) ;
    spr.setScale(1.0,1.0) ;
    setText("") ;
    iscentered = false ;
}

void PixelText::setXY(int x, int y)
{
    spr.setPosition(x-(iscentered?getTextWidth()/2:0),y) ;
}

void PixelText::printTo(int x, int y)
{
    setXY(x,y) ;
    print() ;
}

void PixelText::print()
{
    game->RenderDrawable(spr) ;
}

void PixelText::setAlignCenter()
{
    iscentered=true ;
}

void PixelText::setText(QString str)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    QByteArray bb  = codec->fromUnicode(str) ;

    if (bb.length()==0) return ;

    sf::Image img ;

    int w = 0 ;
    int h = SYMH ;
    int tekw = 0 ;
    if (usemonowidth) {
        for (int i=0; i<bb.length(); i++) {
            int c = (unsigned char)bb[i] ;
            if (c==SYM_ENDLINE) {
                h+=(SYMH+VERTDIST) ;
                if (w<tekw) w=tekw ;
                tekw=0 ;
            }
            else
                tekw+=SYMW ;
        }
        if (w<tekw) w=tekw ;

        img.create(w,h,sf::Color(0,0,0,0)) ;
        int px = 0 ;
        int py = 0 ;
        for (int i=0; i<bb.length(); i++) {
            int c = (unsigned char)bb[i] ;
            if (c==SYM_ENDLINE) {
                py+=(SYMH+VERTDIST) ;
                px=0 ;
            }
            else {
                for (int x=0; x<SYMW; x++)
                    for (int y=0; y<SYMH; y++)
                        if (pixelmap[charsizeinmap*c+y*SYMW+x]) img.setPixel(px+x,py+y,sf::Color::White) ;
                px+=SYMW ;
            }
        }

    }
    else {
        for (int i=0; i<bb.length(); i++) {
            int c = (unsigned char)bb[i] ;
            if (c==SYM_ENDLINE) {
                h+=(SYMH+VERTDIST) ;
                if (w<tekw) w=tekw ;
                tekw=0 ;
            }
            else {
                if (c==32) tekw+=SPACEW ; else tekw+=(mapsf[c].second-mapsf[c].first+1+LETTERDIST) ;
            }
        }
        if (w<tekw) w=tekw ;

        img.create(w,h,sf::Color(0,0,0,0)) ;
        int px = 0 ;
        int py = 0 ;
        for (int i=0; i<bb.length(); i++) {
            int c = (unsigned char)bb[i] ;
            if (c==SYM_ENDLINE) {
                py+=(SYMH+VERTDIST) ;
                px=0 ;
            }
            else {
                if (c==32)
                    px+=SPACEW ;
                else {
                    QPair<int,int> sf = mapsf[c] ;
                    for (int x=sf.first; x<=sf.second; x++)
                        for (int y=0; y<SYMH; y++)
                            if (pixelmap[charsizeinmap*c+y*SYMW+x]) img.setPixel(px+x-sf.first,py+y,sf::Color::White) ;
                    px+=(sf.second-sf.first+1+LETTERDIST) ;
                }
            }
        }
    }

    tex.loadFromImage(img) ;
    spr.setTexture(tex,true) ;
}

void PixelText::setScale(float scale)
{
    spr.setScale(scale,scale) ;
}

void PixelText::setColor(int r, int g, int b)
{
    spr.setColor(sf::Color(r,g,b)) ;
}

void PixelText::setMonoWidth(bool v)
{
    usemonowidth = v ;
}

int PixelText::getTextWidth()
{
    return (int)spr.getGlobalBounds().width ;
}

bool PixelText::isPointIn(int x, int y) const
{
    return ((spr.getPosition().x<x)&&(x<spr.getPosition().x+spr.getGlobalBounds().width)&&
            (spr.getPosition().y<y)&&(y<spr.getPosition().y+spr.getGlobalBounds().height)) ;
}

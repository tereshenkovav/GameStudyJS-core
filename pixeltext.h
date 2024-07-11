#ifndef PIXELTEXT_H
#define PIXELTEXT_H

#include <QObject>
#include <QString>
#include <SFML/Graphics.hpp>
#include <QBitArray>

class Game ;

class PixelText : public QObject
{
    Q_OBJECT
private:
   Game * game ;
   bool iscentered ;
   sf::Texture tex ;
   sf::Sprite spr ;
   QBitArray pixelmap ;
   QList<QPair<int,int>> mapsf ;
   bool usemonowidth ;

public:
    explicit PixelText(QString fontfile, Game * game, QObject *parent = 0);

signals:

public slots:
   void setXY(int x, int y) ;
   void printTo(int x, int y) ;
   void print() ;
   void setAlignCenter() ;
   void setText(QString str) ;
   void setScale(float scale) ;
   void setColor(int r, int g, int b) ;
   void setMonoWidth(bool v) ;
   bool isPointIn(int x, int y) const ;
   int getTextWidth() ;
};

#endif // PIXELTEXT_H

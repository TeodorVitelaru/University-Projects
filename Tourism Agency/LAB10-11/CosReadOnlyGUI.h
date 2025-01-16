#pragma once

#include <qwidget.h>
#include <qpainter.h>
#include "cosOferte.h"
#include "qdebug.h"

class HistogramaGUI : public QWidget, public Observer
{
private:
    cosOferte& cos;
public:
    HistogramaGUI(cosOferte& cos) :cos{ cos }
    {
        cos.addObserver(this);
    }
    void update() override
    {
        repaint();
    }

    void paintEvent(QPaintEvent* ev) override
    {
        QPainter p{ this };
       srand(time(NULL));
       for(const auto& u : cos.get_all_cos())
       {
           int x = rand() % 300;
           int y = rand() % 300;
           qDebug() << x << " " << y;
           p.drawRect(x, y, 20, 60);
           p.drawLine(x, y, width(), height());
           QImage image("imagine1.jpg");
           if (image.isNull()) {
               qDebug() << "Failed to load image";
           } else {
               p.drawImage(x, 0, image);
           }

       }
    }

};
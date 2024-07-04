#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QPainter>
#include <iostream>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

constexpr double PI = 3.141592653589793;

static int line_counter = 0;

static int widthCounter = 0;

class line : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_PROPERTY(QPointF end READ end WRITE setEnd)
public:
    line(QGraphicsItem *parent, QPointF start, QPointF end) : start(start), finalEnd(end), end_m(start), QGraphicsItem(parent)
    {
        setPos(start);
        pen.setWidth(100);
        pen.setColor(QColor::fromRgb(20,20,200,10));

        /* For Trees and Flowers*/
        pen.setWidth(++widthCounter % 300);
        if(id%30>10)
            pen.setColor(QColor::fromRgb(200,0,10,2*widthCounter%20));
        else if(id%30>20)
            pen.setColor(QColor::fromRgb(255,widthCounter%150,220,2*widthCounter%20));
        else
            pen.setColor(QColor::fromRgb(20,0,200,2*widthCounter%20));

        //For Tree. Makes Leaves green
        if(widthCounter > 200)
            pen.setColor(QColor::fromRgb(20,200,20,10));

    }

    void startAnimation() {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "end", this);
        animation->setStartValue(start);
       // std::cerr << "start = (" << start.x() << ", " << start.y() << ")" << std::endl;
       // std::cerr << "finalEnd = (" << finalEnd.x() << ", " << finalEnd.y() << ")" << std::endl;
        animation->setEndValue(finalEnd);
        animation->setDuration(100);
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QPointF end()
    {
        return end_m;
    }

    void setEnd(QPointF end)
    {
        end_m = end;
        update();
    }

protected:
    QRectF boundingRect() const override
    {
        return QRectF();//this->mapFromScene(start),this->mapFromScene(end_m));
        //return QRectF(-100,-100,1000,1000);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(pen);
        painter->drawLine(this->mapFromScene(start),this->mapFromScene(end_m));
    }

private:
    QPointF start;
    QPointF end_m;
    QPointF finalEnd;
    int id = ++line_counter;
    QPen pen;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int frame READ frame WRITE setFrame)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawTriangleHexagonPattern();

    void drawTree();

    void makeRing();

    void applyRules();

    void makeLines();

    void draw();

    void drawFast();

    int frame() const
    {
        return frame_m;
    }

    void setFrame(int frame);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    std::vector<QPointF> cursors;
    std::vector<qreal> thetas;
    std::vector<std::pair<QString,QString>> rules;

    std::vector<line *> lineBank;
    std::vector<line *>::const_iterator lineCursor;
    std::vector<line *> lines;

    int frame_m = 0;
    int lastFrameDrawn = 0;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "frame", this);

    QString string;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setPalette(Qt::white);
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scale(.125,.125);
  //  scene->addLine(-250,-250,1000,1000)->setOpacity(0);

    drawTriangleHexagonPattern();

    showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawTriangleHexagonPattern()
{
    string = "FR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFBBBR240FR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFBBBR240FR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFFR015FR015FR015FR015FFFBBBR240";
    rules.push_back(std::make_pair(string,string+"FFFFFFFFFFFFR240FFFFFFFFFFFF"+string+"FFFFFFFFFFFFR240FFFFFFFFFFFF"+string+"FFFFFFFFFFFFR240FFFFFFFFFFFF"));
    applyRules();

    cursors.push_back(QPointF(0,0));
    thetas.push_back(PI/2);


    makeLines();
    drawFast();
}

void MainWindow::drawTree()
{

      string = "SSSSS";
      rules.push_back(std::make_pair("S","SS"));
      rules.push_back(std::make_pair("SSSSSSSS","FSSSSSSSS"));
      applyRules();

      cursors.push_back(QPointF(0,0));
      thetas.push_back(PI/2);

      makeLines();
      draw();
}

void MainWindow::makeRing()
{

    string = "FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020";
    rules.push_back(std::make_pair("FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020","FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FFFFR020R020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020FR020"));
    applyRules(); applyRules(); applyRules(); applyRules(); applyRules();
    applyRules(); applyRules(); applyRules(); applyRules(); applyRules();
    applyRules(); applyRules(); applyRules(); applyRules(); applyRules();
    applyRules(); applyRules(); applyRules(); applyRules(); applyRules();
    // Apply Rules More

    cursors.push_back(QPointF(0,100));
    thetas.push_back(PI/2);

    makeLines();
    draw();

}

void MainWindow::applyRules()
{
    for(auto it = rules.cbegin(); it != rules.cend(); ++it)
        string.replace(it->first, it->second);
}

constexpr double len = 350;
constexpr double split_rotation = PI/10;

void MainWindow::makeLines()
{

    std::vector<QPointF> newCursors;
    std::vector<double> newThetas;
    for(auto symbol = string.cbegin(); symbol != string.cend(); ++symbol) {
        auto theta = thetas.cbegin();
     //   std::cerr << "symbol = " << symbol->toLatin1() << std::endl;
        for(auto cursor = cursors.cbegin(); cursor != cursors.cend() && theta != thetas.cend(); ++cursor, ++theta) {
     //       std::cerr << "cursor = (" << cursor->x() << ", " << cursor->y() << ")" << std::endl;
     //       std::cerr << "theta = " << std::to_string(*theta) << std::endl;
            switch(symbol->toLatin1()) {
            case 'F':
            {
                line *line0 = new line(nullptr, *cursor, QPointF(cursor->x()+len*cos(*theta), cursor->y()+len*qSin(*theta)));
                newCursors.push_back(QPointF(cursor->x()+len*qCos(*theta), cursor->y()+len*qSin(*theta)));
                newThetas.push_back(std::fmod(*theta, 2*PI));
                lineBank.push_back(line0);
            }
                break;
            case 'B':
            {
                line *line0 = new line(nullptr, *cursor, QPointF(cursor->x()-len*qCos(*theta), cursor->y()-len*qSin(*theta)));
                newCursors.push_back(QPointF(cursor->x()-len*qCos(*theta), cursor->y()-len*qSin(*theta)));
                newThetas.push_back(std::fmod(*theta, 2*PI));
                lineBank.push_back(line0);
            }
                break;
            case 'S':
            {
                line *line0 = new line(nullptr, *cursor, QPointF(cursor->x()+len*qCos(*theta+split_rotation), cursor->y()+len*qSin(*theta+split_rotation)));
                newCursors.push_back(QPointF(cursor->x()+len*qCos(*theta+split_rotation), cursor->y()+len*qSin(*theta+split_rotation)));
                newThetas.push_back(std::fmod(*theta+split_rotation, 2*PI));
                lineBank.push_back(line0);
                line *line1 = new line(nullptr, *cursor, QPointF(cursor->x()+len*qCos(*theta-split_rotation), cursor->y()+len*qSin(*theta-split_rotation)));;
                newCursors.push_back(QPointF(cursor->x()+len*qCos(*theta-split_rotation), cursor->y()+len*qSin(*theta-split_rotation)));
                newThetas.push_back(std::fmod(*theta-split_rotation, 2*PI));
                lineBank.push_back(line1);
            }
                break;
            case 'R':
            {
                std::string arg;
                for(int i = 0; i < 3; ++i) {
                    ++symbol;
                    arg.push_back(symbol->toLatin1());
                }
                newThetas.push_back(std::fmod(*theta+2*PI*std::stod(arg)/360, 2*PI));
                newCursors.push_back(*cursor);
            }
                break;
            }
        }
        cursors = newCursors;
        thetas = newThetas;
        newThetas.clear();
        newCursors.clear();
    }
    lineCursor = lineBank.cbegin();
}

void MainWindow::draw()
{
    animation->setStartValue(0);
    animation->setEndValue(100000);
    animation->setDuration(10000000);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::drawFast()
{
    for(; lineCursor != lineBank.cend(); ++lineCursor) {
        scene->addItem(*lineCursor);
        (*lineCursor)->startAnimation();
    }
}

void MainWindow::setFrame(int frame)
{
    std::cerr << "frame = " << frame_m << std::endl;
    frame_m = frame;
    if(lastFrameDrawn == frame) return;
    if(lineCursor == lineBank.cend())
    {
        animation->stop();
        return;
    }
    scene->addItem(*lineCursor);
    (*lineCursor)->startAnimation();
    ++lineCursor;
    lastFrameDrawn = frame;
}


#ifndef BRIGHTNCONTRAST_H
#define BRIGHTNCONTRAST_H
#include <QtNetwork>
#include <QMainWindow>
#include <QGraphicsScene>

class QUdpSocket;
namespace Ui {
class BrightNContrast;
}

class BrightNContrast : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrightNContrast(QWidget *parent = nullptr);
    ~BrightNContrast();
    // Broadcast slider values over UDP
    void broadcastSliders(QByteArray data);

private slots:
    // Broadcast slider values over UDP
    //void broadcastSliders(QByteArray data);
    // load original image
    void on_LoadOriginal_clicked();
    // generate new image
    void on_Generate_clicked();
    void on_pushButton_clicked();
    // listens for brightness slider value
    void on_brightnessSlider_listener(int value);
    // listens for contrast slider value
    void on_contrastSlider_listener(int value);

private:
    Ui::BrightNContrast *ui;
    QPixmap image;
    QPixmap imageGen;
    QImage *imageObject;
    QImage *imageObjectGen;
    QUdpSocket *udpSocket=nullptr;
    QGraphicsScene *scene;
    QGraphicsScene *sceneGen;
};

#endif // BRIGHTNCONTRAST_H

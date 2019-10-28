#include "brightncontrast.h"
#include "ui_brightncontrast.h"
#include <QtGui>
#include <QtCore>
#include <QtNetwork>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPoint>

BrightNContrast::BrightNContrast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrightNContrast)
{
    ui->setupUi(this);
}

BrightNContrast::~BrightNContrast()
{
    delete ui;
}

void BrightNContrast::on_LoadOriginal_clicked()
{
    QString imagePath=QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("BMP (*.bmp)"));
    imageObject = new QImage();
    imageObject->load(imagePath);
    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->OriginalView->setScene(scene);
}


void BrightNContrast::on_Generate_clicked()
{
    int contrast = ui->ContrastSlider->value();
    int contrastScale = 255/99;
    int contrastIncrease = contrast * contrastScale;
    int contrastDecrease = (0-contrast) * contrastScale;
    //make sure implment contrast before brightness, will lose data otherwise
    int brightness = ui->BrightSlider->value();
    //on_brightnessSlider_listener(brightness);
    QByteArray Contrast = (ui->ContrastSlider->value() < 10)?'0'+QByteArray::number(ui->ContrastSlider->value()): QByteArray::number(ui->ContrastSlider->value());
    QByteArray Brightness = (ui->BrightSlider->value() < 10)?'0'+QByteArray::number(ui->BrightSlider->value()): QByteArray::number(ui->BrightSlider->value());
    broadcastSliders(Contrast+Brightness);
    int brightScale = 255/99;
    int brightIncrease = brightness * brightScale;
    int brightDecrease = (0-brightness) * brightScale;

    QRgb brightValue;
    uint gray;
    bool brightCheckBox = ui->brightCheck->checkState();
    bool contrCheckBox = ui->contrCheck->checkState();
    imageObjectGen = imageObject;

    for(int i = 0; i < imageObject->width(); i++)
    {
        for(int j = 0; j< imageObject->height(); j++)
        {
            brightValue = imageObject->pixel(i,j);
            if(brightCheckBox == 0)
            {
                gray = static_cast<uint>(qGray(brightValue) + brightIncrease);
            }
            else
            {
                gray = static_cast<uint>(qGray(brightValue) + brightDecrease);
            }
            if(gray > 255) // ceiling
            {
                gray = 255;
            }
            if(gray < 0) // floor
            {
                gray = 0;
            }
            imageObjectGen->setPixel(i,j,gray);
        }
    }
    imageGen = QPixmap::fromImage(*imageObjectGen);
    sceneGen = new QGraphicsScene(this);
    sceneGen->addPixmap(imageGen);
    sceneGen->setSceneRect(imageGen.rect());

    ui->GenerateView->setScene(sceneGen);


}

void BrightNContrast::on_pushButton_clicked()
{
    QFile f(QFileDialog::getSaveFileName() + ".bmp");
    f.open(QIODevice::WriteOnly);
    image.save(&f);
    f.close();
}
void BrightNContrast::on_brightnessSlider_listener(int pos)
{
    // convert Contrast slider value to byte array for UDP
    // ternary operator used to check if slider value is less then 10
    // if so concatenate a '0'
    QByteArray contrast = (ui->ContrastSlider->value() < 10)?'0'+QByteArray::number(ui->ContrastSlider->value()):QByteArray::number(ui->ContrastSlider->value());
    // convert Bright slider value to byte array for UDP
    // ternary operator used to check if slider value is less then 10
    // if so concatenate a '0'
    QByteArray brightness = (ui->BrightSlider->value() < 10)?'0'+QByteArray::number(ui->BrightSlider->value()):QByteArray::number(ui->BrightSlider->value());
    broadcastSliders(contrast+brightness);
}
void BrightNContrast::on_contrastSlider_listener(int value)
{
    // convert Contrast slider value to byte array for UDP
    // ternary operator used to check if slider value is less then 10
    // if so concatenate a '0'
    QByteArray contrast = (ui->ContrastSlider->value() < 10)?'0'+QByteArray::number(ui->ContrastSlider->value()):QByteArray::number(ui->ContrastSlider->value());
    // convert Bright slider value to byte array for UDP
    // ternary operator used to check if slider value is less then 10
    // if so concatenate a '0'
    QByteArray brightness = (ui->BrightSlider->value() < 10)?'0'+QByteArray::number(ui->BrightSlider->value()):QByteArray::number(ui->BrightSlider->value());
    broadcastSliders(contrast+brightness);
}
void BrightNContrast::broadcastSliders(QByteArray data)
{
    //create UDP socket
    udpSocket = new QUdpSocket(this);
    //pointer connects to socket,
    udpSocket->bind(QHostAddress("192.168.1.136"), 80);
    //send data to device using above IP and port num
    udpSocket->writeDatagram(data,QHostAddress("192.168.1.136"), 80);

}

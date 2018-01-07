#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lightControl->setMinimum(0);
    ui->lightControl->setMaximum(100);

    ui->compareControl->setMinimum(0);
    ui->compareControl->setMaximum(100);
    ui->rotateControl->setMinimum(0);
    ui->rotateControl->setMaximum(180);

    mainScene = new QGraphicsScene(QRect(0,0,800,600));
    ui->mainGraphic->setScene(mainScene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(! ( currentImg.save(filename) ) ) //保存图像
        {
            QMessageBox::information(this,
                tr("Failed to save the image"),
                tr("Failed to save the image!"));
            return;
        }
    }
}

void MainWindow::on_selectImg_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
        tr("选择图像"),
        "",
        tr("Images (*.png *.bmp *.jpg *.tif *.GIF)")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        if(! ( currentImg.load(filename) ) ) //加载图像
        {
            QMessageBox::information(this,
                tr("打开图像失败"),
                tr("打开图像失败!"));
            return;
        }
        currentImg.scaled(300,300,Qt::KeepAspectRatio);
        ui->mainlabel->setPixmap(QPixmap::fromImage(currentImg)); //显示图像
         mainItem =  mainScene->addPixmap(QPixmap::fromImage(currentImg));
    }

}

void MainWindow::on_moveUp_clicked()
{
    mainItem->setPos(mainItem->pos()+ QPointF(0,2));
}

void MainWindow::on_moveLeft_clicked()
{
    mainItem->setPos(mainItem->pos()+ QPointF(-2,0));
}

void MainWindow::on_moveRight_clicked()
{

     mainItem->setPos(mainItem->pos()+ QPointF(+2,0));
}

void MainWindow::on_moveDown_clicked()
{
    mainItem->setPos(mainItem->pos()+ QPointF(0,-2));
}

void MainWindow::on_zoomify_clicked()
{
      currentImg.scaled(currentImg.size() * 2,Qt::KeepAspectRatio);
    //mainScene->addPixmap(QPixmap::fromImage(currentImg));
    mainItem->setPixmap(QPixmap::fromImage(currentImg));
    mainItem->setScale(2);
     //mainItem->resize(2* ui->mainlabel->pixmap()->size());
}

void MainWindow::on_mirror_clicked()
{
    //QImage tempImg = currentImg;
    currentImg = currentImg.mirrored(true,false);
    mainItem->setPixmap(QPixmap::fromImage(currentImg));
}

void MainWindow::on_saveImg_clicked()
{
    saveImage();
}



QImage MainWindow::changeLight(QImage &image,int brightness)
{
    uchar *line =image.scanLine(0);
    uchar *pixel = line;

    for (int y = 0; y < image.height(); ++y)
    {
        pixel = line;
        for (int x = 0; x < image.width(); ++x)
        {
            *pixel = qBound(0, *pixel + brightness, 255);
            *(pixel + 1) = qBound(0, *(pixel + 1) + brightness, 255);
            *(pixel + 2) = qBound(0, *(pixel + 2) + brightness, 255);
            pixel += 4;
        }

        line += image.bytesPerLine();
    }
    return image;
}

QImage MainWindow::changeContrast(QImage &image,int contrast)
{
    uchar *line =image.scanLine(0);
    uchar *pixel = line;

    for (int y = 0; y < image.height(); ++y)
    {
        pixel = line;
        for (int x = 0; x < image.width(); ++x)
        {
            *pixel = 127+ (double((double)*pixel - 127.0)/(1.0 - (double)contrast / 100));
            *(pixel + 1) =127+ (double((double)*(pixel + 1)  - 127.0)/(1.0 - (double)contrast / 100));
            *(pixel + 2) = 127+ (double((double)*(pixel + 2) - 127.0)/(1.0 - (double)contrast / 100));
            pixel += 4;
        }

        line += image.bytesPerLine();
    }
    return image;
}


void MainWindow::on_rotateControl_sliderMoved(int position)
{
    mainItem->setRotation((double)position/ 3.14);
    mainItem->setPixmap(QPixmap::fromImage(currentImg));

}

void MainWindow::on_lightControl_sliderMoved(int position)
{
    QImage temp = currentImg;
    temp= changeLight(temp,position);
     mainItem->setPixmap(QPixmap::fromImage(temp));

   // ui->mainAera->horizontalScrollBar()->setValue(position*10);
}

void MainWindow::on_compareControl_sliderMoved(int position)
{
    QImage temp = currentImg;
    temp= changeContrast(temp,position);
     mainItem->setPixmap(QPixmap::fromImage(temp));
}

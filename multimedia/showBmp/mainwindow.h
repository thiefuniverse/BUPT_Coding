#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectImg_clicked();

    void on_moveUp_clicked();

    void on_moveLeft_clicked();

    void on_moveRight_clicked();

    void on_moveDown_clicked();

    void on_zoomify_clicked();

    void on_mirror_clicked();

    void on_saveImg_clicked();

    void on_rotateControl_sliderMoved(int position);

    void on_lightControl_sliderMoved(int position);

    void on_compareControl_sliderMoved(int position);

private:
    QImage changeLight(QImage &image,int brightness);
     QImage changeContrast(QImage &image,int contrast);
    void saveImage();
    Ui::MainWindow *ui;
    QGraphicsScene * mainScene;
    QImage currentImg;
       QGraphicsPixmapItem * mainItem;
};

#endif // MAINWINDOW_H

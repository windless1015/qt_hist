#ifndef HISTWIDGET_H
#define HISTWIDGET_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;

struct CordinateSysParam{
    QPoint originalPoint;  //原点的坐标,相对于需要绘制的具体widget
    QPoint xEndPoint;  //横坐标结束点
    QPoint yEndPoint;  //纵坐标结束点
    uint xScaleNum;   //横轴的刻度个数
    uint yScaleNum;   //纵轴刻度个数
};

namespace Ui {
class histWidget;
}

class histWidget : public QWidget
{
    Q_OBJECT

public:
    explicit histWidget(CordinateSysParam *c, QWidget *parent = 0);
    ~histWidget();
    void GetHistArrData(double*, uint);

protected:
    void paintEvent(QPaintEvent*);

private:
    Ui::histWidget *ui;
    CordinateSysParam *csparam;
    double hist_arr_norm[256];
};

#endif // HISTWIDGET_H

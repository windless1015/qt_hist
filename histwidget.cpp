#include "histwidget.h"
#include "ui_histwidget.h"
#include <QtGui/qpainter.h>
#include <QtGui/QFontMetrics>
#include <QString>
#include <QDebug>

histWidget::histWidget(CordinateSysParam *c ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histWidget)
{
    csparam = c;
    for(uint i=0; i< 256; i++)
    {
      hist_arr_norm[i] = 0;
    }
    ui->setupUi(this);
}

histWidget::~histWidget()
{
    delete ui;
}

//重写paintEvent事件, 一般来说Qpainter只能在paintEvetn使用,也可以用在外面,可以去搜一下使用方法
void histWidget::paintEvent(QPaintEvent* e){
    Q_UNUSED(e);
    QPainter painter(this);

    int xAxis_ry = csparam->originalPoint.ry(); //横轴的纵坐标
    //int yAxis_rx = csparam->originalPoint.rx(); //纵轴的横坐标
    int xAxis_effe_dis = csparam->xEndPoint.rx() - csparam->originalPoint.rx() - 50; //横轴的有效距离
    int yAxis_effc_dix = csparam->originalPoint.ry() - csparam->yEndPoint.ry() - 50;
    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);
    painter.drawLine(csparam->originalPoint, csparam->xEndPoint);

    //绘制箭头
    int arrow_off = 10;
    QPainterPath _path;
    //绘制x轴箭头
    _path.moveTo(csparam->xEndPoint.rx() - arrow_off, csparam->xEndPoint.ry() + 5); //移动到箭头下端的端点
    _path.lineTo(csparam->xEndPoint.rx(),csparam->xEndPoint.ry());  //移动到箭头点
    _path.lineTo(csparam->xEndPoint.rx() - arrow_off, csparam->xEndPoint.ry() - 5); //移动到箭头上端的端点处
    //绘制y轴箭头
    _path.moveTo(csparam->yEndPoint.rx() - 5, csparam->yEndPoint.ry() + arrow_off);
    _path.lineTo(csparam->yEndPoint.rx(), csparam->yEndPoint.ry());
    _path.lineTo(csparam->yEndPoint.rx() + 5, csparam->yEndPoint.ry() + arrow_off);
    painter.drawPath(_path);

    //绘制x轴的刻度,根据xScaleNum,找到每个x轴上的坐标,然后向下绘制一个坐标
    for(uint i=1; i<=csparam->xScaleNum; i++){
        int _x = csparam->originalPoint.rx() + i * xAxis_effe_dis/8.0f;
        painter.drawLine(_x, xAxis_ry, _x, xAxis_ry+3);  //绘制x轴刻度
        if(i*32 < 100)  //因为如果是两位数和三位数偏移的数量不一样,所以如果是两位数则左移量较小
          painter.drawText(QPoint(_x - 5, xAxis_ry+18), QString::number(i*32));
        else
          painter.drawText(QPoint(_x - 8, xAxis_ry+18), QString::number(i*32));
    }

    painter.drawLine(csparam->originalPoint, csparam->yEndPoint);
    //以为本直方图是做了归一化处理,所以纵轴没有刻度,只有一个1
    painter.drawLine(csparam->originalPoint.rx(),csparam->originalPoint.ry()- yAxis_effc_dix,
                     csparam->originalPoint.rx() - 3, csparam->originalPoint.ry()- yAxis_effc_dix); //绘制y轴唯一的刻度
    painter.drawText(QPoint(csparam->originalPoint.rx() - 15, csparam->originalPoint.ry()- yAxis_effc_dix + 3), "1");

    //绘制hist_arr_norm中的坐标
    for(uint i=0;i<256;i++){
        if(hist_arr_norm[i] <= 0.00f) //如果这个值为0,就不绘制
            continue;
        int height_temp = hist_arr_norm[i] * yAxis_effc_dix;
        int _x = csparam->originalPoint.rx() + xAxis_effe_dis/256.00f*i;
        painter.drawLine(_x,csparam->originalPoint.ry(),_x, csparam->originalPoint.ry()- height_temp);
    }

}

void histWidget::GetHistArrData(double* data, uint total_num){
    for(uint i=0; i< 256; i++)
    {
        hist_arr_norm[i] = data[i] / total_num * 50.0f;  //扩大50倍是为了便于观看,否则全部挤压在x轴上,没有纵坐标的高度
    }

}



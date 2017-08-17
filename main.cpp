
#include "histwidget.h"
#include <QApplication>
#include "displayimgdialog.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

//    double hist_arr[256] = {0,0,0,0,0,0,0,0,0,100};
//    CordinateSysParam csp;
//    csp.originalPoint = QPoint(70,300); //坐标原点
//    csp.xEndPoint = QPoint(500,300);  //横坐标结束点
//    csp.yEndPoint = QPoint(70, 30);  //纵坐标结束点

//    csp.xScaleNum = 8;  //因为要现实直方图,所以从0-255 分成8段
//    csp.yScaleNum = 0;

//    histWidget w = histWidget(&csp);
//    w.resize(530, 350);
//    int count =0;
//    for(uint i=0;i<256;i++)
//        count +=hist_arr[i];
//    w.GetHistArrData(hist_arr, count);
//    w.show();

    displayImgDialog disImgDialog;
    disImgDialog.show();

    return a.exec();
}

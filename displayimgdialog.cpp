#include "displayimgdialog.h"
#include "ui_displayimgdialog.h"
#include <QFileDialog>
#include <QMessageBox>


displayImgDialog::displayImgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displayImgDialog)
{
    fileName = "";
    scaleGrayImg = new QImage();
    scaleImg = new QImage();
    isGray = false; //默认不是灰色的
    csp = new CordinateSysParam();
    csp->originalPoint = QPoint(70,300); //坐标原点
    csp->xEndPoint = QPoint(500,300);  //横坐标结束点
    csp->yEndPoint = QPoint(70, 30);  //纵坐标结束点

    csp->xScaleNum = 8;  //因为要现实直方图,所以从0-255 分成8段
    csp->yScaleNum = 0;

    w = NULL;
    hist_arr = new double[256];
    for(int i=0;i<256;i++)
        hist_arr[i] = 0;
    ui->setupUi(this);
}

displayImgDialog::~displayImgDialog()
{
    delete scaleGrayImg;
    delete scaleImg;
    delete ui;
    delete csp;
    delete hist_arr;
}

QImage displayImgDialog::ScaleImage2Label(QImage& qImage, QLabel* qLabel){
    QImage qScaledImage;
    QSize qImageSize = qImage.size();
    QSize qLabelSize = qLabel->size();
    double dWidthRatio = 1.0*qImageSize.width() / qLabelSize.width();
    double dHeightRatio = 1.0*qImageSize.height() / qLabelSize.height();
    if (dWidthRatio>dHeightRatio)
        qScaledImage = qImage.scaledToWidth(qLabelSize.width());
    else
        qScaledImage = qImage.scaledToHeight(qLabelSize.height());
    return qScaledImage;
}

void displayImgDialog::GetSrcImagePtr(QImage *return_ptr) const{
    return_ptr = scaleGrayImg;
}

void displayImgDialog::on_openImgBtn_clicked()
{

    fileName = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开图像"),"c:/User/st/Desktop/",
                                                   tr("Image (*.jpg *.jpeg *.bmp *.png)"));
    if(fileName.isEmpty())
        return;
    else{
        QImage sourceImg;
        if(!(sourceImg.load(fileName))){
            QMessageBox::information(this, QString::fromLocal8Bit("打开图像失败"), QString::fromLocal8Bit("打开图像失败!"));
            return;
        }
        //判断图像格式
        if(sourceImg.format() == QImage::Format_Indexed8)
            isGray = true;
        *scaleImg = ScaleImage2Label(sourceImg, ui->label_display);
        ui->label_display->setPixmap(QPixmap::fromImage(*scaleImg));
        ui->label_display->setAlignment(Qt::AlignCenter);
    }
}

//显示图像的灰度直方图
void displayImgDialog::on_displayHistBtn_clicked()
{
    if(scaleGrayImg->depth() <= 0)
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先打开图像!"));
    else if(!scaleGrayImg->isGrayscale())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先做灰度变换!"));
    }else{


//        double hist_arr[256] = {0,0,0,0,0,0,0,0,0,100};
//        qDebug("%d", scaleGrayImg->depth());

//        w =  new histWidget(csp);
//        w->resize(530, 350);
//        int count =0;
//        for(uint i=0;i<256;i++)
//            count += hist_arr[i];
//        w->GetHistArrData(hist_arr, count);
//        w->show();
        int64_t count = 0;

        GetEveryLevelPixelCount(*scaleGrayImg, hist_arr, &count);
        w =  new histWidget(csp);
        w->resize(530, 350);
        w->GetHistArrData(hist_arr, count);
        w->show();
        for(int i=0;i<256;i++)  //如果不对数组置0则,数据一直保存,导致每次都要*50
            hist_arr[i] = 0;
    }
}

void displayImgDialog::GetEveryLevelPixelCount(QImage& src, double* arr, int64_t *total_count){
    if(src.isNull()) return;
    if(!src.isGrayscale())  return;
    *total_count = src.width() * src.height();
    for(int i=0; i<src.width(); i++)
        for(int j=0; j<src.height(); j++){
            QRgb pixel = src.pixel(i,j);
            arr[qGray(pixel)] += 1;
        }

}

 QImage displayImgDialog::toGray( QImage& src_image ){
     int height = src_image.height();
     int width = src_image.width();
     QImage ret(width, height, QImage::Format_Indexed8);
     ret.setColorCount(256);
     for(int i = 0; i < 256; i++)
     {
         ret.setColor(i, qRgb(i, i, i));
     }
     switch(src_image.format())
     {
         case QImage::Format_Indexed8:
             for(int i = 0; i < height; i ++)
             {
                 const uchar *pSrc = (uchar *)src_image.constScanLine(i);
                 uchar *pDest = (uchar *)ret.scanLine(i);
                 memcpy(pDest, pSrc, width);
             }
             break;
         case QImage::Format_RGB32:
         case QImage::Format_ARGB32:
         case QImage::Format_ARGB32_Premultiplied:
             for(int i = 0; i < height; i ++)
             {
                 const QRgb *pSrc = (QRgb *)src_image.constScanLine(i);
                 uchar *pDest = (uchar *)ret.scanLine(i);
     //QRgb *pDest = (QRgb*)image.scanLine(i);
                 for( int j = 0; j < width; j ++)
                 {
                      pDest[j] = qGray(pSrc[j]);
     //((unsigned char*)&pDest[j])[3] = 56;
                 }
             }
             break;
         }
         return ret;
 }

//灰度变换
void displayImgDialog::on_convertGreyBtn_clicked()
{
    if(scaleImg->depth() <= 0){
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先打开图像文件!"));
        return;
    }
    if(scaleImg->isGrayscale()){
        QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("已经完成灰度变换!"));
        return;
    }
    //进行灰度转换
    *scaleGrayImg = toGray(*scaleImg);
    ui->label_display->setPixmap(QPixmap::fromImage(*scaleGrayImg));

}

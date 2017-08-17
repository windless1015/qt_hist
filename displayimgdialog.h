#ifndef DISPLAYIMGDIALOG_H
#define DISPLAYIMGDIALOG_H

#include <QDialog>
#include <QImage>
#include <QLabel>
#include "histwidget.h"
#include <stdint.h>

namespace Ui {
class displayImgDialog;
}

class displayImgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit displayImgDialog(QWidget *parent = 0);
    ~displayImgDialog();
    void GetSrcImagePtr(QImage *return_ptr) const;

private slots:
    void on_openImgBtn_clicked();
    void on_displayHistBtn_clicked();
    void on_convertGreyBtn_clicked();


private:
    Ui::displayImgDialog *ui;
    QImage ScaleImage2Label(QImage& qImage, QLabel* qLabel);
    QImage toGray( QImage& image );
    void GetEveryLevelPixelCount(QImage& src, double* arr, int64_t * total_count);
    QString fileName;
    QImage *scaleGrayImg;
    QImage *scaleImg;
    CordinateSysParam *csp;
    histWidget *w;
    double *hist_arr;
    bool isGray;
};

#endif // DISPLAYIMGDIALOG_H

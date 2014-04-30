#ifndef OpenCVwidget_H
#define OpenCVwidget_H
#include <QtOpenGL>
#include <QTime>
#include <iostream>
#include <QGLWidget>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class OpenCVwidget : public QGLWidget
{
    Q_OBJECT
public:
    void        lock();      /// Zablokuj/odblokuj możliwość zmiany rozmiaru obrazu
    void        unlock();    /// Odblokuj możliwość zmiany rozmiaru obrazu
    void        resizeImage(int x, int y);
    bool        locked;
    explicit OpenCVwidget(QWidget *parent = 0);

signals:
    void        imageSizeChanged( int outW, int outH ); /// sygnał do zmiany rozmiaru
    void        imageSingleClicked(OpenCVwidget* sender);
    void        imageDoubleClicked(OpenCVwidget* sender);


public slots:
    bool        showImage( cv::Mat image ); /// pokaż obraz
    void        reset();

protected:
    void        mousePressEvent( QMouseEvent * event );
    void 	initializeGL(); /// inicjalizacja OpenGL
    void 	paintGL(); /// OpenGL - renderowanie obrazu
    void 	resizeGL(int width, int height);        /// event- zmiana rozmiaru
    void        updateScene();
    void        renderImage();


private:

    QTime       time;
    bool        clicked;
    bool        mSceneChanged;
    QImage      mRenderQtImg;
    cv::Mat     mOrigImage;
    QColor 	mBgColor;
    int         mOutH;
    int         mOutW;
    float       mImgRatio;

    int         mPosX;
    int         mPosY;

};

#endif // OpenCVwidget_H

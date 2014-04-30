#include "camera/WidgetOpenCV.h"

OpenCVwidget::OpenCVwidget(QWidget *parent) :
    QGLWidget(parent)
{
mSceneChanged = false;
mBgColor = QColor::fromRgb(237,237,237);
clicked=false;
locked = false;
mOutH = 0;
mOutW = 0;
mImgRatio = 4.0f/3.0f;
mPosX = 0;
mPosY = 0;
reset();
}

void OpenCVwidget::lock(){
       locked = true;
}

void OpenCVwidget::unlock(){
       locked = false;
}

void OpenCVwidget::initializeGL()
{
    makeCurrent();
    qglClearColor(mBgColor);
}

void OpenCVwidget::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, 0, height, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    mOutH = width/mImgRatio;
    mOutW = width;
    if(mOutH>height)
    {
        mOutW = height*mImgRatio;
        mOutH = height;
    }
    emit imageSizeChanged( mOutW, mOutH );

    mPosX = (width-mOutW)/2;
    mPosY = (height-mOutH)/2;

    mSceneChanged = true;

    updateScene();
}

void OpenCVwidget::updateScene()
{
    if( mSceneChanged && this->isVisible() )
        updateGL();
}

void OpenCVwidget::paintGL()
{
    makeCurrent();

    if( !mSceneChanged )
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;
}

void OpenCVwidget::renderImage()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        QImage image;

        glPushMatrix();
        {
            int imW = mRenderQtImg.width();
            int imH = mRenderQtImg.height();

            image = mRenderQtImg.scaled( QSize(mOutW,mOutH),Qt::KeepAspectRatio, Qt::SmoothTransformation);
            glRasterPos2i( mPosX, mPosY );
            imW = image.width();
            imH = image.height();

            glDrawPixels( imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }
        glPopMatrix();

        // end
        glFlush();
    }
}

void OpenCVwidget::resizeImage(int x,int y)
{
    if (!locked)
        resize(x,y);

}



void OpenCVwidget::mousePressEvent (QMouseEvent * event)
{
    int t = time.elapsed();
    time.restart();
    // Jeżeli odstęp między kliknięciami jest mniejszy niż 550ms
    if (t!=0 && t < 550 ){
        emit imageDoubleClicked(this);
        clicked=true;
    }

    // Jeżeli odstęp między kliknięciami jest większy niż 550ms
    if (t==0 || t > 550){
        if (!clicked)
            emit imageSingleClicked(this);
        else
            clicked = false;
    }
 }



bool OpenCVwidget::showImage( cv::Mat image )
{
    image.copyTo(mOrigImage);

    mImgRatio = (float)image.cols/(float)image.rows;

    if( mOrigImage.channels() == 3)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);
    else
        return false;

    mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

    mSceneChanged = true;

    updateScene();

    return true;
}

void OpenCVwidget::reset(){

  cv::Mat im(480,640,CV_8SC3,cv::Scalar(50,50,50));
  showImage(im);
}

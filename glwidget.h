#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
 #include <QMouseEvent>

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:

public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);

    int x, y, changeNum = -1;

public slots:
    void lodChanged(int change);
    void changeWeight1(int w);
    void changeWeight2(int w);
    void changeWeight3(int w);
    void changeWeight4(int w);
    void changeWeight5(int w);
    void changeWeight6(int w);

};

#endif // GLWIDGET_H

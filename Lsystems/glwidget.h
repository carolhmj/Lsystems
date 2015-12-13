#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QGLWidget>
#include "lsystem.h"

class GLWidget : public QGLWidget
{
private:
    Lsystem sys = Lsystem("../teste.txt");
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // GLWIDGET_H

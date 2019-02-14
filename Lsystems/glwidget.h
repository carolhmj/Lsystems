#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QGLWidget>
#include <QTimer>
#include <GL/glu.h>
#include <GL/gl.h>
#include <list>
#include <QListIterator>
#include <QMouseEvent>

#include "lsystem.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
class GLWidget : public QGLWidget
{
private:
    Lsystem sys = Lsystem("../teste.txt");
    void drawGrid();
    void drawAxis(GLfloat *mat);

    QTimer timer;

    float curquat[4];
    float lastquat[4];
    int beginx, beginy;
    float zoom = 0.10;
    float verticalIncrease, horizontalIncrease;
    glm::mat4 mvMatrix;

public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // GLWIDGET_H

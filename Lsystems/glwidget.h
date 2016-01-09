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

using namespace std;
class GLWidget : public QGLWidget
{
private:
    Lsystem sys = Lsystem("../teste.txt");
    void drawGrid();
    void drawAxis(GLfloat *mat);
    void drawBackground();
    void loadProjection();

    QTimer timer;

    bool show_grid;
    bool perspective;

    float zoomDepth;
    float curquat[4];
    float lastquat[4];
    int beginx, beginy;
    float clipClose;
    float fovy;

    glm::vec4 eyePosition = glm::vec4(0.0, 0.0, 1.0, 1.0);
    glm::vec4 currentRotation = glm::mat4(1.0f);
    int lastx, lasty;
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // GLWIDGET_H

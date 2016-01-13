#include "glwidget.h"
#include "trackball.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    sys.printRules();
    sys.setAxiom("S");
    for (int i = 0; i < 6; i++){
        cout << "i : " << i << endl;
        sys.evolveState();
    }
    //cout << sys.getState() << endl;
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(1);

    zoomDepth = 200.0;

    trackball(curquat, 0.0, 0.0, 0.0, 0.0);
    clipClose = 0.001;
    fovy = 60.0;
    show_grid = true;
    perspective = true;
}

void GLWidget::initializeGL(){
    glEnable(GL_LINE_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);
    sys.drawState(mvMatrix);
}

void GLWidget::resizeGL(int w, int h){
    if(h<w) {
        glViewport((w-h)/2,0,h,h);
    }
    else {
        glViewport(0,(h-w)/2,w,w);
    }
}

void GLWidget::paintGL(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float m[4][4];

   build_rotmatrix(m, curquat);

   glm::mat4 mm = glm::make_mat4(m[0]);
   mvMatrix = glm::ortho(-10.f,10.f,-10.f, 10.f, -10.f, 10.f) * mm;

   sys.drawState(mvMatrix);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastx = event->x();
        lasty = event->y();
    }
    //cout << "lastx " << event->x() << " lasty " << event->y() << endl;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){


}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    float width = size().width();
    float height = size().height();

    trackball(lastquat,
      (2.0 * beginx - width) / width,
      (height - 2.0 * beginy) / height,
      (2.0 * event->x() - width) / width,
      (height - 2.0 * event->y()) / height
    );
    beginx = event->x();
    beginy = event->y();
    add_quats(lastquat, curquat, curquat);
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
//    float ds = event->delta()*0.1;
//    zoomDepth -= ds;
//    zoomDepth = zoomDepth < 0.0 ? 0.0 : zoomDepth;
//    updateGL();
}

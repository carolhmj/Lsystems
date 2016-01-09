#include "glwidget.h"
#include "trackball.h"
#include <iostream>
#include <cmath>

using namespace std;
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    sys.setAxiom("X");
    for (int i = 0; i < 5; i++){
        cout << "i : " << i << endl;
        sys.evolveState();
    }

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
    sys.drawState();
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
   sys.drawState();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event){
    lastx = event->x();
    lasty = event->y();

    cout << "last x: " << lastx << "last y: " << lasty << endl;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    double diffx = abs(event->x() - lastx) / this->width();
    double diffy = abs(event->y() - lasty) / this->height();

    double rotationYAng = diffx * M_PI;
    double rotationXAng = diffy * M_PI;

    glm::mat4 rotationX = glm::rotate(currentRotation, rotationXAng, glm::vec3(1.0,0.0,0.0));
    glm::mat4 rotationY = glm::rotate(currentRotation, rotationYAng, glm::vec3(1.0,0.0,0.0));
}

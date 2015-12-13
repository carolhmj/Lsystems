#include "glwidget.h"
#include <iostream>

using namespace std;
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    sys.setAxiom("AXA");
    for (int i = 0; i < 5; i++){
        cout << "i : " << i << endl;
        sys.evolveState();
    }
}

void GLWidget::initializeGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);
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

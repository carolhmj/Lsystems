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
    for (int i = 0; i < 6; i++){
        cout << "i : " << i << endl;
        sys.evolveState();
    }
    sys.generateModel();
    sys.writeModel("vertices.txt", "arestas.txt");
    //cout << sys.getState() << endl;
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(1);

    trackball(curquat, 0.0, 0.0, 0.0, 0.0);

    float w = zoom*this->width() + horizontalIncrease*this->width();
    float h = zoom*this->height() + verticalIncrease*this->height();
    mvMatrix = glm::ortho(-w/2,w/2, -h/2, h/2, -10.f, 10.f);
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
   float w = zoom*this->width() + horizontalIncrease*this->width();
   float h = zoom*this->height() + verticalIncrease*this->height();
   mvMatrix = glm::ortho(-w/2,w/2, -h/2, h/2, -20000.f, 20000.f) * mm;
   sys.drawState(mvMatrix);
   //sys.drawState(glm::mat4());
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        beginx = event->x();
        beginy = event->y();
    }
    //cout << "lastx " << event->x() << " lasty " << event->y() << endl;
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
    float scale = 0.01;
    float ds = scale*(event->delta()/120);
    this->zoom -= ds;
    zoom = zoom < scale ? scale : zoom;
    //zoom = zoom > 1.0 ? 1.0 : zoom;
    cout << "zoom: " << zoom << endl;
    flush(cout);
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    cout << "key press on key: " << event->key() << "\n";
    string s = event->key() == Qt::Key_Up ? "sim" : "nao";
    cout << "up? " << s << endl;
    switch (event->key()){
    case Qt::Key_Up:
        verticalIncrease += 0.01;
        break;
    case Qt::Key_Down:
        verticalIncrease -= 0.01;
        break;
    case Qt::Key_Right:
        horizontalIncrease += 0.01;
        break;
    case Qt::Key_Left:
        horizontalIncrease -= 0.01;
    }
}

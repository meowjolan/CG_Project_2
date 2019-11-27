#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <math.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *e);

    // 辅助函数
    void DrawCube();
    void DrawBodyPart(float length, float width, float height, float x_shift, float y_shift, \
        float z_shift, float color_1, float color_2, float color_3);
    void DrawBody();
    void DrawCircle(float radius);
    void DrawLeg(float angle);
    
private:
    QTimer *timer;
    float scene_angle_h;
    float scene_angle_v;
};
#endif // MYGLWIDGET_H

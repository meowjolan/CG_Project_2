#include "myglwidget.h"

/*###################################################
##  定义一些常量，如速度等
#####################################################*/
// 机器人圆周运动的角速度
#define ROBOT_ANG_VEL 0.5f
// 手臂运动的角度速
#define ARM_ANG_VEL 5.0f
// 大腿运动的角速度
#define LEG_ANG_VEL 5.0f
// 机器人运动半径
#define MOVE_RADIUS 80.0f


/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	scene_angle_h = scene_angle_v = 0.0f; // 初始化场景旋转角度
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());  
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glEnable(GL_DEPTH_TEST);       
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
	// 机器人做圆周运动时离原始位置的角度
	static float angle = 0;
	// 更新角度
	angle = (angle + ROBOT_ANG_VEL) >= 360.0f ? 0.0f : (angle + ROBOT_ANG_VEL);

	// 设置清理颜色为黑色，使背景为黑色
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	// 清空缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//压入当前矩阵
	glPushMatrix();

	// 设置为投影矩阵堆栈
	glMatrixMode(GL_PROJECTION);
	// 复位矩阵
	glLoadIdentity();
	// 透视投影
	glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1001.0f);
	//glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 1001.0f);

	// 设置为建观察矩阵
	glMatrixMode(GL_MODELVIEW);
	// 复位矩阵
	glLoadIdentity();
	// 全体远离摄像机一定距离
	glTranslatef(0.0f, 0.0f, -130.0f);
	// 倾斜圆周运动平面，方便观察
	glRotatef(scene_angle_h, 1.0f, 0.0f, 0.0f);
	glRotatef(scene_angle_v, 0.0f, 1.0f, 0.0f);

	// 绘制运动轨迹
	DrawCircle(MOVE_RADIUS);

	// 绘制运动过程中的整个机器人
	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(MOVE_RADIUS, 0.0f, 0.0f);
	DrawBody();
	glPopMatrix();

	// 弹出当前矩阵
	glPopMatrix();
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}


/*###################################################
##  函数: DrawCube
##  函数描述： 在xOy平面上绘制一个单位立方体
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawCube()
{
	// 开始绘制四边形
	glBegin(GL_QUADS);

	// 绘制位于x轴负半轴的面
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 1.0f);

	// 绘制位于x轴正半轴的面
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 1.0f);

	// 绘制位于y轴负半轴的面
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 1.0f);

	// 绘制位于y轴正半轴的面
	glVertex3f(-0.5f, 0.5f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 1.0f);

	// 绘制位于z轴零点的面
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	// 绘制位于z轴正半轴的面
	glVertex3f(-0.5f, 0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, 0.5f, 1.0f);

	//绘制完成
	glEnd();
}


/*###################################################
##  函数: DrawBodyPart
##  函数描述： 绘制某个身体部位
##  参数描述： 
##	length, width, height: 长宽高
##	x_shift, y_shift, z_shift: 偏移量
##	color_v: 颜色数组 
#####################################################*/
void MyGLWidget::DrawBodyPart(float length, float width, float height, float x_shift, float y_shift, \
        float z_shift, float color_1, float color_2, float color_3)
{
	// 压入当前矩阵
	glPushMatrix();

	// 设置颜色
	glColor4f(color_1/255.0f, color_2/255.0f, color_3/255.0f, 1.0f);

	// 设置为建模观察矩阵堆栈
	glMatrixMode(GL_MODELVIEW);
	// 平移
	glTranslatef(x_shift, y_shift, z_shift);
	// 缩放
	glScalef(length, width, height);

	// 绘制立方体
	DrawCube();

	// 恢复当前矩阵
	glPopMatrix();
}


/*###################################################
##  函数: DrawBody
##  函数描述： 绘制整个身体
##  参数描述： 无
#####################################################*/
void MyGLWidget::DrawBody()
{
	// 大腿运动的角度
	static float leg_angle = 0.0f;
	leg_angle = (leg_angle + LEG_ANG_VEL) >= 180.0f? 0.0f : (leg_angle + LEG_ANG_VEL);
	leg_angle = leg_angle >= 90? -leg_angle : leg_angle;

	// 要抬哪一条腿
	static float which_leg = 0.0f;
	which_leg = (which_leg + LEG_ANG_VEL) >= 360.0f? 0.0f : (which_leg + LEG_ANG_VEL);

	// 绘制头部
	DrawBodyPart(4.0f, 4.0f, 4.0f, 0.0f, 0.0f, 38.5f, 25.0f, 202.0f, 173.0f);

	//两只眼睛
	DrawBodyPart(0.5f, 0.5f, 0.5f, -1.0f, 2.5f, 41.5f, 0.0f, 0.0f, 0.0f);
	DrawBodyPart(0.5f, 0.5f, 0.5f, 1.0f, 2.5f, 41.5f, 0.0f, 0.0f, 0.0f);

	// 绘制脖子
	DrawBodyPart(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 38.0f, 255.0f, 255.0f, 255.0f);

	// 绘制躯干
	DrawBodyPart(12.0f, 8.0f, 16.0f, 0.0f, 0.0f, 22.0f, 209.0f, 186.0f, 116.0f);

	// 绘制两条腿
	float leg_shift = which_leg < 180.0f? -3.0f : 3.0f;

	// 悬浮的那条腿
	glPushMatrix();
	glTranslatef(leg_shift, 0.0f, 22.0f);
	glRotatef(abs(leg_angle), 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -22.0f);
	DrawLeg(-abs(leg_angle));
	glPopMatrix();

	// 触地的那条腿
	glPushMatrix();
	glTranslatef(-leg_shift, 0.0f, 0.0f);
	DrawLeg(0.0f);
	glPopMatrix();

	// 绘制两只手
	// 腿与手的顺序刚好相反，旋转角度相同
	float hand_shift = which_leg < 180.0f? 7.0f : -7.0f;

	// 旋转的那只手
	glPushMatrix();
	glTranslatef(hand_shift, 0.0f, 38.0f);
	glRotatef(abs(leg_angle), 1.0f, 0.0f, 0.0f);
	DrawBodyPart(2.0f, 2.0f, 18.0f, 0.0f, 0.0f, -18.0f, 224.0f, 96.0f, 108.0f);
	glPopMatrix();

	// 竖直的那只手
	glPushMatrix();
	glTranslatef(-hand_shift, 0.0f, 0.0f);
	DrawBodyPart(2.0f, 2.0f, 18.0f, 0.0f, 0.0f, 20.0f, 224.0f, 96.0f, 108.0f);
	glPopMatrix();
}


/*###################################################
##  函数: DrawCircle
##  函数描述： 在xOy平面上绘制一个以原点为圆心的有色圆形
##  参数描述： 
##	radius： 半径
##	x, y： 圆心坐标
#####################################################*/
void MyGLWidget::DrawCircle(float radius)
{
	// 绘制精度
	static const int N = 100;
	static const double PI = acos(-1.0);

	// 保存当前矩阵
	glPushMatrix();
	//设置颜色
	glColor4f(214.0f/255.0f, 213.0f/255.0f, 183.0f/255.0f, 1.0f);

	// 开始绘制
	glBegin(GL_POLYGON);
	for (int i = 0; i < N; i++)
	{
		glVertex3f(radius*cos(2*PI*i/N), radius*sin(2*PI*i/N), 0.0f);
	}
	glEnd();

	//恢复当前矩阵
	glPopMatrix();
}

/*###################################################
##  函数: DrawLeg
##  函数描述： 绘制一条腿
##  参数描述： 
## 	angle：小腿旋转的角度
#####################################################*/
void MyGLWidget::DrawLeg(float angle)
{
	// 绘制大腿
	DrawBodyPart(2.0f, 2.0f, 10.0f, 0.0f, 0.0f, 12.0f, 255.0f, 0.0f, 0.0f);
	// 绘制膝关节
	DrawBodyPart(3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 11.0f, 255.0f, 165.0f, 79.0f);
	// 绘制小腿和脚板
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 11.0f);
	glRotatef(angle, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -11.0f);
	DrawBodyPart(2.0f, 2.0f, 10.0f, 0.0f, 0.0f, 1.0f, 190.0f, 237.0f, 199.0f);
	DrawBodyPart(3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 255.0f, 165.0f, 79.0f);
	glPopMatrix();
}


/*###################################################
##  函数: keyPressEvent
##  函数描述： 实现通过按方向键来旋转场景
##  参数描述： 无
#####################################################*/
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Left) {
		scene_angle_v -= 2.0f;
		scene_angle_v = scene_angle_v < 0.0f? 365.0f : scene_angle_v;
	}
	else if (e->key() == Qt::Key_Down) {
		scene_angle_h += 2.0f;
		scene_angle_h = scene_angle_h >= 360.0f? 0.0f : scene_angle_h;
	}
	else if (e->key() == Qt::Key_Up) {
		scene_angle_h -= 2.0f;
		scene_angle_h = scene_angle_h < 0.0f? 365.0f : scene_angle_h;
	}
	else if (e->key() == Qt::Key_Right) {
		scene_angle_v += 2.0f;
		scene_angle_v = scene_angle_v >= 360.0f? 0.0f : scene_angle_v;
	}
}
/*
* 存在的问题：控制点的初始化只有4个,存储形成的点的数组分配内存大小问题
* 在绘画线段时候 glBegin()和glend()放不放入for循环中的问题
*/
/*
思路框架：bezier曲线，绘画，展示
*/
#include<glut.h>
#include<GL/GL.h>
#include<cmath>
int mask = -1;//鼠标响应控制
int n = 4;//4个控制点
GLfloat cp[4][2] = {//控制点的x,y坐标
	{-0.0f,0.2f},
	{-0.4f,0.7f},
	{0.1f,-0.2f},
	{0.3f,0.5f}
};
//找点
void de_Cateljau(GLfloat bezier[][2]) {
	//存放x,y
	float x[4] = {};
	float y[4] = {};
	int count=0;//记录bezier轨迹点
	//t在[0，1]内
	for (double t = 0.0; t <= 1.0; t += 0.1) {
		//从第一次开始迭代
		for (int i = 1; i <= n; i++) {
			x[i] = cp[i][0];//把控制点的x,y坐标存放
			y[i] = cp[i][1];
		}
		for(int i=1;i<n;i++){
			for (int j = 0; j < n - i; j++) {//n-i是去除控制点的意思，每次迭代都比前一次少一个点
				//if (i == 1) {//直接使用最初的控制点迭代，找出相邻两个控制点之间的点
				//	x[i] = (1 - t) * cp[i][0] + t * cp[i + 1][0];
				//	y[i] = (1 - t) * cp[i][1] + t * cp[i + 1][1];
				//}
				x[i] = (1 - t) * x[i] + t * x[i + 1];
				y[i] = (1 - t) * y[i] + t * y[i + 1];
			}
		}
		//经过历次的迭代，最后可以获得一个点 这个点在[0,1]内的运动轨迹就是bezier曲线
		bezier[count][0] = x[0];
		bezier[count][1] = y[0];	
		count++;
	}
}
//绘制
void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制控制点
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		glBegin(GL_POINTS);
		//glVertex2f和glVertex2fv 两者的参数类型不同，一个GLfloat，一个指针
		glVertex2fv(&cp[i][0]);//v表示传递的几个参数将使用指针的方式
		glEnd();
	}

	//绘制第一个控制点到最后一个控制的点的线段，
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++) {
		glVertex2fv(&cp[i][0]);
	}
	glEnd();

	//绘制Bezier曲线
	GLfloat be[11][2]={};
	de_Cateljau(be);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 11; i++) {
		glVertex2fv(be[i]);
	}
	glEnd();

	glFlush();

}


//鼠标响应  变化
void mouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			if (mask == -1) {
				for (int i = 0; i < 4; i++)
				{
					float mouse_x = (x - 200) / 200.0f, mouse_y = (200 - y) / 200.0f;
					float dis = sqrt((mouse_x - cp[i][0]) * (mouse_x - cp[i][0]) + (mouse_y - cp[i][1]) * (mouse_y - cp[i][1]));
					if (dis < 0.1f) mask = i;
				}
			}
		}
		else if (state == GLUT_UP)
			mask = -1;
	}
}
// 鼠标移动
void mouseMove(int x, int y)
{
	if (mask != -1) {
		float mouse_x = (x - 200) / 200.0f, mouse_y = (200 - y) / 200.0f;
		cp[mask][0] = mouse_x;
		cp[mask][1] = mouse_y;
		glutPostRedisplay();
	}
}

int main(int argc,char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
	glutCreateWindow("bezier");
	glutDisplayFunc(&display);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);

	glutMainLoop();
	return 0;
}
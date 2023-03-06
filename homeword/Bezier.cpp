/*******************************************************************
* @brief : Bezier基本绘画
* @author : wj
* @date : 2023/2/27 21:46
*******************************************************************/

/*
* 1.存在的问题：控制点的初始化只有4个,存储形成的点的数组分配内存大小问题
* 2.在绘画线段时候 glBegin()和glend()放不放入for循环中的问题
* 3.t 的精度的问题，不一样的话，Bezier的曲线头不在起始控制点
* 4.鼠标引动这块还要继续研究，特别是在屏幕上坐标的变换，曾出现过点击控制点移动时候鼠标不在点上，产生偏移 
* 5.!!!!存在走样的问题！！！  这个还不知道怎么解决！！！
*/
/*
思路框架：bezier曲线，绘画，展示
*/
#include<glut.h>
#include<cmath>
int mask = -1;//鼠标响应控制
int n = 4;//4个控制点
GLfloat Cp[4][2] = {//控制点的x,y坐标
	{ -0.1f,  0.9f },
	{ -0.5f,  0.2f },
	{  0.5f, -0.3f },
	{  0.1f, 0.4f },
};
//找点
void deCateljau(GLfloat Bezier[][2])
{
	//存放x,y
	float x[4]={};
	float y[4]={};
	int count = 0;//记录bezier轨迹点
	
	for (double t = 0.0; t <= 1; t += 0.1)//t在[0，1]内  
	{
		for (int i = 0; i < n; i++) {
			x[i] = Cp[i][0];  //必须要把控制点全部存储，若不是出错，曾做过多次使用下面的传入导致出错
			y[i] = Cp[i][1];
		}

		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j < n - i; j++)//n-i是去除控制点的意思，每次迭代都比前一次少一个点
			{
				//if (i == 1) {//直接使用最初的控制点迭代，找出相邻两个控制点之间的点
				//	x[i] = (1 - t) * cp[i][0] + t * cp[i + 1][0];
				//	y[i] = (1 - t) * cp[i][1] + t * cp[i + 1][1];
				//}
				x[j] = (1 - t) * x[j] + t * x[j + 1];
				y[j] = (1 - t) * y[j] + t * y[j + 1];
			}

		}
		//经过历次的迭代，最后可以获得一个点 这个点在[0,1]内的运动轨迹就是bezier曲线
		Bezier[count][0] = x[0];
		Bezier[count][1] = y[0];
		count++;
	}
}

//绘制
void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);//当前可写的颜色缓冲
	
	//绘制控制点
	glPointSize(10);
	glColor3f(0.5f, 1.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		glBegin(GL_POINTS);
		//glVertex2f和glVertex2fv 两者的参数类型不同，一个GLfloat，一个指针
		glVertex2fv(&Cp[i][0]);//v表示传递的几个参数将使用指针的方式
		glEnd();

	}

	//绘制第一个控制点到最后一个控制的点的线段，
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINE_STRIP);//绘制从第一个顶点到最后一个顶点依次相连的一组线段
	for (int i = 0; i < 4; i++) {
		glVertex2fv(&Cp[i][0]);
	}
	glEnd();

	//绘制Bezier曲线
	GLfloat Bezier[11][2] = { 0 };
	deCateljau(Bezier);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 11; i++) {
		glVertex2fv(Bezier[i]);//指定顶点

	}
	glEnd();

	glFlush();//强制刷新缓冲，保证绘图命令将被执行

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
					float dis = sqrt((mouse_x - Cp[i][0]) * (mouse_x - Cp[i][0]) + (mouse_y - Cp[i][1]) * (mouse_y - Cp[i][1]));
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
		Cp[mask][0] = mouse_x;
		Cp[mask][1] = mouse_y;
		glutPostRedisplay();
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Bezier曲线绘画");
	glutDisplayFunc(&display);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutMainLoop();  
	return 0;
}

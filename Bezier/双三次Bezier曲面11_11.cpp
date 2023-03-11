/*
* @Name:双三次Bezier曲面
* @Date:2023/3/11
* @author:twj
*/
/*
* 难点：
		①定义存放曲面的点的数组 或 结构体
		②选取u v方向 如从Bezier曲线 到 Bezier曲面 由一维到二维是怎么变化的
		③用什么来存储曲面上的点？？？那么又如何知道曲面上的点的个数，然后才能定义出数组存储面上的点

* 总结：
		①设置16个控制点的数组 一开始想到一维数组+结构体  不方便，转三维数组
*		②存放Bezier曲面的点的问题：
			先是思考如何在计算出Bezier曲线上的点的问题，主要运用到n次Beizer曲线
*			的定义式，当有四个控制点时候，为三次Bezier曲线，双三次则为二维的。
*			数组定义时候要明白一个道理，（u,v）  （u,v） （u,v） 的变化形式，重要的说三遍
*			**从0到1.0，每次+0.1 那么在一条Bezier曲线上就有11个点 在一个面上 两个gor循环就有11*11个点**
*		③关于精度的问题：
*			不知道是不是计算机的问题，当我的（u,v）选取float时候绘画的图形出错，而使用double时候才成功，float u=o.double u=0.0???
*			其实，这里设计到计算机硬件的知识：（x86 ）
*				浮点运算都是以双精度进行的，即使只有float的运算，也要先转换成double型再算的，所以doublex型比float型要快一点。
*		④关于OpenGL的API的调用：
*			glFlush()搭配glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
*			glutSwapBuffers()搭配glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
*			
*			因为API的问题，搞得我画面呈现全白的问题，一度认为Bezier曲面上的点出错，数值有误，仔细检查过方程式但有没什么问题呀， API。。。。
*		 ⑤给定16个控制点 ，存放在三维数组中
*/

#include <glut.h>
//存放16个控制点4*4
//双三次Bezier曲面指的是由两个二维Bezier曲线组成的二维曲面，
//每个Bezier曲线包含四个控制点，因此曲面上总共有16个控制点。 
float R[4][4][3] = {
	{
		{ -0.8f, -0.7f, 0.2f }, { -0.3f, -0.6f, 0.2f }, { 0.2f, -0.65f, 0.3f }, { 0.7f, -0.7f, 0.2f }
	},
	{
		{ -0.9f, -0.2f, 0.3f }, { -0.4f, -0.2f, 0.2f }, { 0.3f, -0.2f, 0.4f },	{ 0.75f, -0.2f, 0.3f },
	},
	{
		{ -0.9f, 0.3f, 0.3f }, { -0.3f, 0.2f, 0.5f }, { 0.25f, 0.25f, 0.6f },	{ 0.8f, 0.3f, 0.4f },
	},
	{
		{ -0.8f, 0.8f, 0.1f }, { -0.3f, 0.8f, 0.2f }, { 0.2f, 0.85f, 0.1f },	{ 0.7f, 0.8f, 0.1f },
	}
};
//float uv[][3] = {0};//存放面上的点  [][][3]
/* glBegin(GL_LINE_STRIP);

	 for (float v = 0; v <= 1; v += 0.01) {
		 float B0v = (1 - v) * (1 - v) * (1 - v);
		 float B1v = 3 * (1 - v) * (1 - v) * v;
		 float B2v = 3 * (1 - v) * v * v;
		 float B3v = v * v * v;
		 for (float u = 0; u <= 1; u += 0.01) {
			 float B0u = (1 - u) * (1 - u) * (1 - u);
			 float B1u = 3 * (1 - u) * (1 - u) * u;
			 float B2u = 3 * (1 - u) * u * u;
			 float B3u = u * u * u;
			 for (int j = 0; j < 4; j++) {
				 uv[j][0] = R[0][j][0] * B0u + R[1][j][0] * B1u + R[2][j][0] * B2u + R[3][j][0] * B3u;
				 uv[j][1] = R[0][j][1] * B0u + R[1][j][1] * B1u + R[2][j][1] * B2u + R[3][j][1] * B3u;
				 uv[j][2] = R[0][j][2] * B0u + R[1][j][2] * B1u + R[2][j][2] * B2u + R[3][j][2] * B3u;
			 }
		 }
		 float vx = uv[0][0] * B0v + uv[1][0] * B1v + uv[2][0] * B2v + uv[3][0] * B3v;
		 float vy = uv[0][1] * B0v + uv[1][1] * B1v + uv[2][1] * B2v + uv[3][1] * B3v;
		 float vz = uv[0][2] * B0v + uv[1][2] * B1v + uv[2][2] * B2v + uv[3][2] * B3v;
		 glVertex3f(vx/100, vy/100, vz/100);
	 }*/
	 //曲面分为u v方向 看作  错误理解为x-y轴
	 //先计算一个u方向上的Bezier曲线上的点，存储在一个数组中  
	 // 目标先求出有n行的曲线点数组
	 // 曲线上的点有 xyz坐标值 有0-t行4列 ---> 三维数组 points_u[t][4][3]
	 //该遍历有4列，每行四个控制点
	 // 
	 //t相当于 在u从0到1.0变化时候存在的点数 u每次+0.01
	 //上面被注释的代码没有 考虑到 二维怎么存放三维的问题 除非使用结构体？？？    
	/*for (float u = 0; u <= 1; u += 0.1) {
		float B0u = (1 - u) * (1 - u) * (1 - u);
		float B1u = 3 * (1 - u) * (1 - u) * u;
		float B2u = 3 * (1 - u) * u * u;
		float B3u = u * u * u;
		int t = 0;
		for (int j = 0; j < 4; j++) {
			points_u[t][j][0] = R[0][j][0] * B0u + R[1][j][0] * B1u + R[2][j][0] * B2u + R[3][j][0] * B3u;
			points_u[t][j][1] = R[0][j][1] * B0u + R[1][j][1] * B1u + R[2][j][1] * B2u + R[3][j][1] * B3u;
			points_u[t][j][2] = R[0][j][2] * B0u + R[1][j][2] * B1u + R[2][j][2] * B2u + R[3][j][2] * B3u;
		}
		t += 1;
	}*/
	//此步同上面的步骤
	//已经计算出4行Bezier曲线上的点 ，接下来在这基础上在才求出（列）v方向的点
	//结合v方向（列）的控制点 即从控制点出发 慢慢地利用第一步计算出的Bezier曲线的点再得出Bezier曲线
	//
	//设置一个存储最终Bezier曲面的点 
	//float points_uv[11][11][3] = { 0 };
	/*for (float v = 0; v <= 1; v += 0.1) {
		float B0v = (1 - v) * (1 - v) * (1 - v);
		float B1v = 3 * (1 - v) * (1 - v) * v;
		float B2v = 3 * (1 - v) * v * v;
		float B3v = v * v * v;
		int t = 0;
		for (int j = 0; j < 11; j++) {
			points_uv[j][t][0] = points_u[j][0][0] * B0v + points_u[j][1][0] * B1v + points_u[j][2][0] * B2v + points_u[j][3][0] * B3v;
			points_uv[j][t][0] = points_u[j][0][1] * B0v + points_u[j][1][1] * B1v + points_u[j][2][1] * B2v + points_u[j][3][1] * B3v;
			points_uv[j][t][0] = points_u[j][0][2] * B0v + points_u[j][1][2] * B1v + points_u[j][2][2] * B2v + points_u[j][3][2] * B3v;
		}
		t += 1;
	}*/



void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	//旋转 
	glRotatef(-65.0, 1.0, 0.3, 0.2);

	
	//设置11是因为  t每次增加0.1  到1.0就有11个 
	//数组定义为 point_u[11][4][3]  选取垂直方向 每4个垂直的（看作近似在一条线）控制点组成Bezier曲线 可以算出行
	//也可以定义为point_u[4][11][3] 但后面要改变方向为R[j][0][0]   
	//这里直接跟着这条式子思考 ux[j] = RX[0][j]*B0u + RX[1][j]*B1u + RX[2][j]*B2u + RX[3][j]*B3u;  也是选取竖直方向  
	//也可选水平方向

	float points_u[11][4][3] = { 0.0 };
	for (int j = 0; j < 4; j++) {
		int t = 0;
		for (double u = 0.0; u <= 1.0; u += 0.1)
		{
			float B0u = (1 - u) * (1 - u) * (1 - u);
			float B1u = 3 * (1 - u) * (1 - u) * u;
			float B2u = 3 * (1 - u) * u * u;
			float B3u = u * u * u;
			points_u[t][j][0] = R[0][j][0] * B0u + R[1][j][0] * B1u + R[2][j][0] * B2u + R[3][j][0] * B3u;
			points_u[t][j][1] = R[0][j][1] * B0u + R[1][j][1] * B1u + R[2][j][1] * B2u + R[3][j][1] * B3u;
			points_u[t][j][2] = R[0][j][2] * B0u + R[1][j][2] * B1u + R[2][j][2] * B2u + R[3][j][2] * B3u;
			t = t + 1;
		}
	}
	// 行列 11*11
	float points_uv[11][11][3] = { 0.0 };
	for (int j = 0; j < 11; j++) {
		int i = 0;
		for (double t = 0.0; t <= 1.0; t += 0.1)
		{
			float a1 = (1 - t) * (1 - t) * (1 - t);
			float a2 = 3 * (1 - t) * (1 - t) * t;
			float a3 = 3 * t * t * (1 - t);
			float a4 = t * t * t;
			points_uv[j][i][0] = a1 * points_u[j][0][0] + a2 * points_u[j][1][0] + a3 * points_u[j][2][0] + a4 * points_u[j][3][0];
			points_uv[j][i][1] = a1 * points_u[j][0][1] + a2 * points_u[j][1][1] + a3 * points_u[j][2][1] + a4 * points_u[j][3][1];
			points_uv[j][i][2] = a1 * points_u[j][0][2] + a2 * points_u[j][1][2] + a3 * points_u[j][2][2] + a4 * points_u[j][3][2];
			i = i + 1;
		}
	}

	//Bezier曲面上的点连线
	glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < 11; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 11; j++)
			glVertex3fv(&points_uv[i][j][0]);
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 11; j++)
			glVertex3fv(&points_uv[j][i][0]);
		glEnd();
	}


	//绘画控制点	
	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);

	for (int i = 0; i < 4; i++) {
		glBegin(GL_POINTS);
		for (int j = 0; j < 4; j++)
			glVertex3fv(&R[i][j][0]);
		glEnd();
	}

	//绘画连接控制点的连线
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < 4; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 4; j++)
			glVertex3fv(&R[i][j][0]);
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 4; j++)
			glVertex3fv(&R[j][i][0]);
		glEnd();
	}

	//glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Bicubic Bezier surface");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
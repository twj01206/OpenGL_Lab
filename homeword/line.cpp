#include<glut.h>

void display() {
	//绘画直线
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glColor3f(1, 0.5, 0.2);
	glLineWidth(5);
	glVertex2f(-0.6, 0.4);
	glVertex2f(0.3, 0.4);
	glEnd();
	//绘画虚线
	glColor3f(1, 1.0, 0.8);
	glLineStipple(2, 0X00FF);//建立画线的模式
	glEnable(GL_LINE_STIPPLE);//打开模式
	glBegin(GL_LINES);//  GL_LINE_STRIP 不闭合折线
	glVertex2f(0.0, 0.5);
	glVertex2f(0.5, 0.5);
	glEnd();

	glFlush();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("绘画直线");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

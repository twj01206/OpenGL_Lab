#include<glut.h>

void display() {
	//�滭ֱ��
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glColor3f(1, 0.5, 0.2);
	glLineWidth(5);
	glVertex2f(-0.6, 0.4);
	glVertex2f(0.3, 0.4);
	glEnd();
	//�滭����
	glColor3f(1, 1.0, 0.8);
	glLineStipple(2, 0X00FF);//�������ߵ�ģʽ
	glEnable(GL_LINE_STIPPLE);//��ģʽ
	glBegin(GL_LINES);//  GL_LINE_STRIP ���պ�����
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
	glutCreateWindow("�滭ֱ��");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
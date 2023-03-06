/*******************************************************************
* @brief : Bezier�����滭
* @author : wj
* @date : 2023/2/27 21:46
*******************************************************************/

/*
* 1.���ڵ����⣺���Ƶ�ĳ�ʼ��ֻ��4��,�洢�γɵĵ����������ڴ��С����
* 2.�ڻ滭�߶�ʱ�� glBegin()��glend()�Ų�����forѭ���е�����
* 3.t �ľ��ȵ����⣬��һ���Ļ���Bezier������ͷ������ʼ���Ƶ�
* 4.���������黹Ҫ�����о����ر�������Ļ������ı任�������ֹ�������Ƶ��ƶ�ʱ����겻�ڵ��ϣ�����ƫ�� 
* 5.!!!!�������������⣡����  �������֪����ô���������
*/
/*
˼·��ܣ�bezier���ߣ��滭��չʾ
*/
#include<glut.h>
#include<cmath>
int mask = -1;//�����Ӧ����
int n = 4;//4�����Ƶ�
GLfloat Cp[4][2] = {//���Ƶ��x,y����
	{ -0.1f,  0.9f },
	{ -0.5f,  0.2f },
	{  0.5f, -0.3f },
	{  0.1f, 0.4f },
};
//�ҵ�
void deCateljau(GLfloat Bezier[][2])
{
	//���x,y
	float x[4]={};
	float y[4]={};
	int count = 0;//��¼bezier�켣��
	
	for (double t = 0.0; t <= 1; t += 0.1)//t��[0��1]��  
	{
		for (int i = 0; i < n; i++) {
			x[i] = Cp[i][0];  //����Ҫ�ѿ��Ƶ�ȫ���洢�������ǳ������������ʹ������Ĵ��뵼�³���
			y[i] = Cp[i][1];
		}

		for (int i = 1; i < n; i++)
		{
			for (int j = 0; j < n - i; j++)//n-i��ȥ�����Ƶ����˼��ÿ�ε�������ǰһ����һ����
			{
				//if (i == 1) {//ֱ��ʹ������Ŀ��Ƶ�������ҳ������������Ƶ�֮��ĵ�
				//	x[i] = (1 - t) * cp[i][0] + t * cp[i + 1][0];
				//	y[i] = (1 - t) * cp[i][1] + t * cp[i + 1][1];
				//}
				x[j] = (1 - t) * x[j] + t * x[j + 1];
				y[j] = (1 - t) * y[j] + t * y[j + 1];
			}

		}
		//�������εĵ����������Ի��һ���� �������[0,1]�ڵ��˶��켣����bezier����
		Bezier[count][0] = x[0];
		Bezier[count][1] = y[0];
		count++;
	}
}

//����
void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);//��ǰ��д����ɫ����
	
	//���ƿ��Ƶ�
	glPointSize(10);
	glColor3f(0.5f, 1.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		glBegin(GL_POINTS);
		//glVertex2f��glVertex2fv ���ߵĲ������Ͳ�ͬ��һ��GLfloat��һ��ָ��
		glVertex2fv(&Cp[i][0]);//v��ʾ���ݵļ���������ʹ��ָ��ķ�ʽ
		glEnd();

	}

	//���Ƶ�һ�����Ƶ㵽���һ�����Ƶĵ���߶Σ�
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINE_STRIP);//���ƴӵ�һ�����㵽���һ����������������һ���߶�
	for (int i = 0; i < 4; i++) {
		glVertex2fv(&Cp[i][0]);
	}
	glEnd();

	//����Bezier����
	GLfloat Bezier[11][2] = { 0 };
	deCateljau(Bezier);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 11; i++) {
		glVertex2fv(Bezier[i]);//ָ������

	}
	glEnd();

	glFlush();//ǿ��ˢ�»��壬��֤��ͼ�����ִ��

}

//�����Ӧ  �仯
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

// ����ƶ�
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
	glutCreateWindow("Bezier���߻滭");
	glutDisplayFunc(&display);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutMainLoop();  
	return 0;
}

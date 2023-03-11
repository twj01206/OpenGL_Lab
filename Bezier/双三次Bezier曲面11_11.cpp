/*
* @Name:˫����Bezier����
* @Date:2023/3/11
* @author:twj
*/
/*
* �ѵ㣺
		�ٶ���������ĵ������ �� �ṹ��
		��ѡȡu v���� ���Bezier���� �� Bezier���� ��һά����ά����ô�仯��
		����ʲô���洢�����ϵĵ㣿������ô�����֪�������ϵĵ�ĸ�����Ȼ����ܶ��������洢���ϵĵ�

* �ܽ᣺
		������16�����Ƶ������ һ��ʼ�뵽һά����+�ṹ��  �����㣬ת��ά����
*		�ڴ��Bezier����ĵ�����⣺
			����˼������ڼ����Bezier�����ϵĵ�����⣬��Ҫ���õ�n��Beizer����
*			�Ķ���ʽ�������ĸ����Ƶ�ʱ��Ϊ����Bezier���ߣ�˫������Ϊ��ά�ġ�
*			���鶨��ʱ��Ҫ����һ��������u,v��  ��u,v�� ��u,v�� �ı仯��ʽ����Ҫ��˵����
*			**��0��1.0��ÿ��+0.1 ��ô��һ��Bezier�����Ͼ���11���� ��һ������ ����gorѭ������11*11����**
*		�۹��ھ��ȵ����⣺
*			��֪���ǲ��Ǽ���������⣬���ҵģ�u,v��ѡȡfloatʱ��滭��ͼ�γ�����ʹ��doubleʱ��ųɹ���float u=o.double u=0.0???
*			��ʵ��������Ƶ������Ӳ����֪ʶ����x86 ��
*				�������㶼����˫���Ƚ��еģ���ʹֻ��float�����㣬ҲҪ��ת����double������ģ�����doublex�ͱ�float��Ҫ��һ�㡣
*		�ܹ���OpenGL��API�ĵ��ã�
*			glFlush()����glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
*			glutSwapBuffers()����glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
*			
*			��ΪAPI�����⣬����һ������ȫ�׵����⣬һ����ΪBezier�����ϵĵ������ֵ������ϸ��������ʽ����ûʲô����ѽ�� API��������
*		 �ݸ���16�����Ƶ� ���������ά������
*/

#include <glut.h>
//���16�����Ƶ�4*4
//˫����Bezier����ָ������������άBezier������ɵĶ�ά���棬
//ÿ��Bezier���߰����ĸ����Ƶ㣬����������ܹ���16�����Ƶ㡣 
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
//float uv[][3] = {0};//������ϵĵ�  [][][3]
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
	 //�����Ϊu v���� ����  �������Ϊx-y��
	 //�ȼ���һ��u�����ϵ�Bezier�����ϵĵ㣬�洢��һ��������  
	 // Ŀ���������n�е����ߵ�����
	 // �����ϵĵ��� xyz����ֵ ��0-t��4�� ---> ��ά���� points_u[t][4][3]
	 //�ñ�����4�У�ÿ���ĸ����Ƶ�
	 // 
	 //t�൱�� ��u��0��1.0�仯ʱ����ڵĵ��� uÿ��+0.01
	 //���汻ע�͵Ĵ���û�� ���ǵ� ��ά��ô�����ά������ ����ʹ�ýṹ�壿����    
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
	//�˲�ͬ����Ĳ���
	//�Ѿ������4��Bezier�����ϵĵ� ������������������ڲ�������У�v����ĵ�
	//���v�����У��Ŀ��Ƶ� ���ӿ��Ƶ���� ���������õ�һ���������Bezier���ߵĵ��ٵó�Bezier����
	//
	//����һ���洢����Bezier����ĵ� 
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
	//��ת 
	glRotatef(-65.0, 1.0, 0.3, 0.2);

	
	//����11����Ϊ  tÿ������0.1  ��1.0����11�� 
	//���鶨��Ϊ point_u[11][4][3]  ѡȡ��ֱ���� ÿ4����ֱ�ģ�����������һ���ߣ����Ƶ����Bezier���� ���������
	//Ҳ���Զ���Ϊpoint_u[4][11][3] ������Ҫ�ı䷽��ΪR[j][0][0]   
	//����ֱ�Ӹ�������ʽ��˼�� ux[j] = RX[0][j]*B0u + RX[1][j]*B1u + RX[2][j]*B2u + RX[3][j]*B3u;  Ҳ��ѡȡ��ֱ����  
	//Ҳ��ѡˮƽ����

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
	// ���� 11*11
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

	//Bezier�����ϵĵ�����
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


	//�滭���Ƶ�	
	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);

	for (int i = 0; i < 4; i++) {
		glBegin(GL_POINTS);
		for (int j = 0; j < 4; j++)
			glVertex3fv(&R[i][j][0]);
		glEnd();
	}

	//�滭���ӿ��Ƶ������
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
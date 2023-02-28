/*
*基于Bernstein基函数 的公式转化为代码必须熟悉熟练  
*重点掌握Bezier曲线的绘画 
*/
#include <glut.h>
#include <cmath>
#define CON_POINTS 4//控制点书  当要绘画n次Bezier则修改为n+1（n+1个控制点）
int mask = -1;
struct Point_xy {
    double x, y;
};
Point_xy vec[CON_POINTS];//定义点数组


double B(int n, int i, float t) {
    int sum = 1;
    if (i == n || i == 0)
        return pow(t, i) * pow(1 - t, n - i);
    int j;
    for (j = n - i + 1; j <= n; j++)
        sum *= j;
    for (j = 1; j <= i; j++)
        sum /= j;
    return sum * pow(t, i) * pow(1 - t, n - i);
}
Point_xy Bezier(float t) {
    Point_xy p;
    p.x = 0;
    p.y = 0;
    for (int k = 0; k < CON_POINTS; k++) {
        p.x += vec[k].x * B(CON_POINTS - 1, k, t);
        p.y += vec[k].y * B(CON_POINTS - 1, k, t);
    }
    return p;
}
// 画线
void DrawLine(Point_xy p1, Point_xy p2) {
    glColor3f(1.0f, 0.0f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //画点
    glPointSize(10.0f);
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int b = 0; b < CON_POINTS; b++)
        glVertex2f(vec[b].x, vec[b].y);
    glEnd();

    for (int a = 0; a < CON_POINTS - 1; a++) {
        glColor3f(0.0, 1.0, 1.0);
        DrawLine(vec[a], vec[a + 1]);
    }

    Point_xy p = vec[0];
    for (float t = 0; t <= 1; t += 0.05) {
        Point_xy pB = Bezier(t);
        DrawLine(p, pB);
        p = pB;
    }
    glFlush();
    glutSwapBuffers();
}

void init() {//初始化
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    for (int d = 0; d < CON_POINTS; d++) {
        vec[d].x = 10 + 100 * d;
        vec[d].y = 500 - 10 *d * d;//生成不在同一直线的若干点
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLsizei)w, (GLsizei)h, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        mask = true;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        mask = false;

}

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void motion(int x, int y) {
    if (mask) {
        for (int c = 0; c < CON_POINTS; c++) {
            if (distance(vec[c].x, vec[c].y, x, y) < 20) {
                vec[c].x = x;
                vec[c].y = y;
            }
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bezier");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}


/*
参考地址：https://blog.csdn.net/qq_42276781/article/details/89301281
*/

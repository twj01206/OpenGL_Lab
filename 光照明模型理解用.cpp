#include <glut.h>
#include <cmath>

// ���ڴ�С
int image_width = 800;
int image_height = 600;
    // ���峡���е�����
struct Sphere {
     float radius;                   // ����뾶
     float position[3];              // ����λ��
     float ambient[3];               // ��������ɫ
     float diffuse[3];               // ��������ɫ
     float specular[3];              // ���淴����ɫ
     float shininess;                // ����� �������Խ�⻬����⼯�У����йܹ���
    };
    // ����һ���������
    Sphere sphere = { 1.0f, { 0.0f, 0.0f, 0.0f },
                      { 0.2f, 0.2f, 0.2f }, { 0.7f, 0.7f, 0.7f },
                      { 1.0f, 1.0f, 1.0f }, 50.0f };    
// ��ת�Ƕ�
GLfloat angle = 0.0f;               

void init() {
    glEnable(GL_DEPTH_TEST);       // ������Ȳ���
    glEnable(GL_LIGHTING);         // ���ù���
    glEnable(GL_LIGHT0);           // ����0�Ź�Դ

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);   // ���ñ�����ɫ

    // ���ù�Դ����
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };   // ƽ�й�Դλ��
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };    // ƽ�й�Դ��������ɫ
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // ƽ�й�Դ��������ɫ
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // ƽ�й�Դ���淴����ɫ

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // ���ò�������
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // ���廷������ɫ
    GLfloat material_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };  // ������������ɫ
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ���徵�淴����ɫ
    GLfloat material_shininess[] = { 50.0f };                // ��������

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void drawSphere() {
    glTranslatef(sphere.position[0], sphere.position[1], sphere.position[2]);
    glutSolidSphere(sphere.radius, 50, 50);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);   // �����ӵ�λ�úͷ���

    glRotatef(angle, 0.0f, 1.0f, 0.0f);    // ��Y����ת������

    // ��������
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    drawSphere();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);   // ����ͶӰ͸��
}

//update ������������ʵ�ֳ����Ķ���Ч��
void update(int value) {
    angle += 2.0f;
    if (angle > 360.0f)
        angle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

// ����������ߺ�����Ľ���  at*t + 2bt +c  = 0 һ��ʽ   b*b-4ac�жϽ���Ĵ������
bool intersectSphere(const float rayOrigin[3], const float rayDirection[3], float& t) {
    float a = pow(rayDirection[0], 2) + pow(rayDirection[1], 2) + pow(rayDirection[2], 2);
    float b = 2 * (rayDirection[0] * rayOrigin[0] + rayDirection[1] * rayOrigin[1] + rayDirection[2] * rayOrigin[2]);
    float c = pow(rayOrigin[0], 2) + pow(rayOrigin[1], 2) + pow(rayOrigin[2], 2) - pow(sphere.radius, 2);

    float discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return false;  // ����������û�н���
    }

    float t0 = (-b - sqrt(discriminant)) / (2 * a);
    float t1 = (-b + sqrt(discriminant)) / (2 * a);

    if (t0 > 0) {
        t = t0;
        return true;
    }

    if (t1 > 0) {
        t = t1;
        return true;
    }

    return false;  // ����������û�н���
}

// ��������ķ������
void computeReflectionRay(const float incidentRayDirection[3], const float surfaceNormal[3], float reflectedRayDirection[3]) {
    float dotProduct = -2 * (incidentRayDirection[0] * surfaceNormal[0] + incidentRayDirection[1] * surfaceNormal[1] + incidentRayDirection[2] * surfaceNormal[2]);
    reflectedRayDirection[0] = incidentRayDirection[0] + dotProduct * surfaceNormal[0];
    reflectedRayDirection[1] = incidentRayDirection[1] + dotProduct * surfaceNormal[1];
    reflectedRayDirection[2] = incidentRayDirection[2] + dotProduct * surfaceNormal[2];
}

// �����������ӰЧ��
float computeShadow(const float intersectionPoint[3], const float lightDirection[3]) {
    float shadowRayOrigin[3] = { intersectionPoint[0], intersectionPoint[1], intersectionPoint[2] };
    float shadowRayDirection[3] = { lightDirection[0], lightDirection[1], lightDirection[2] };
    float t;

    if (intersectSphere(shadowRayOrigin, shadowRayDirection, t)) {
        return 0.0f;  // �������Դ֮�����ڵ���������Ӱ
    }

    return 1.0f;  // ����͹�Դ֮�����ڵ�����������Ӱ
}

// ����Whitted�������ģ�͵���ɫ
void computeColor(const float rayOrigin[3], const float rayDirection[3], float& red, float& green, float& blue, int recursionDepth) {
    float t;
    if (intersectSphere(rayOrigin, rayDirection, t)) {
        // ���㽻��λ��
        float intersectionPoint[3] = { rayOrigin[0] + t * rayDirection[0], rayOrigin[1] + t * rayDirection[1], rayOrigin[2] + t * rayDirection[2] };

        // ������淨��
        float surfaceNormal[3] = { intersectionPoint[0] - sphere.position[0], intersectionPoint[1] - sphere.position[1], intersectionPoint[2] - sphere.position[2] };
        // ������淨�������ĳ���
        float length = sqrt(pow(surfaceNormal[0], 2) + pow(surfaceNormal[1], 2) + pow(surfaceNormal[2], 2));
        // ��һ�����淨������  ��λ����
        surfaceNormal[0] /= length;
        surfaceNormal[1] /= length;
        surfaceNormal[2] /= length;

        // �������߷���
        float viewDirection[3] = { -rayDirection[0], -rayDirection[1], -rayDirection[2] };

        // ���㻷������   ��ɫֵ��˱�Ȼ��ʹ��ɫֵ��С���������������ӱ�Ȼ��ʹ��ɫֵ���Ӹ��ӽӽ�1�����������
        //Shader�������������ɫ=������+�Է���+������+�߹⡣
        //��������������������� ��ǿx�ⷴ����߹ⷴ��x��ǿ ���߹ⷴ��x�ⷴ�䡣����ⷢ����˥�����߷������������˱仯
        float ambientRed = sphere.ambient[0] * sphere.diffuse[0];
        float ambientGreen = sphere.ambient[1] * sphere.diffuse[1];
        float ambientBlue = sphere.ambient[2] * sphere.diffuse[2];

        // �������������
        float diffuseRed = 0.0f;
        float diffuseGreen = 0.0f;
        float diffuseBlue = 0.0f;

        float lightDirection[3] = { 1.0f, 1.0f, 1.0f };//������ߵķ��򣬽������Ϊ {1.0f, 1.0f, 1.0f}�������������������ϵķ�����Ϊ1.0
        length = sqrt(pow(lightDirection[0], 2) + pow(lightDirection[1], 2) + pow(lightDirection[2], 2));
        lightDirection[0] /= length;
        lightDirection[1] /= length;
        lightDirection[2] /= length;

        float shadowFactor = computeShadow(intersectionPoint, lightDirection);//��ʾ��Ӱ���ӵı���

        if (shadowFactor > 0.0f) {
            //����Ľ�������ڼ�����������ǿ��
            float dotProduct = lightDirection[0] * surfaceNormal[0] + lightDirection[1] * surfaceNormal[1] + lightDirection[2] * surfaceNormal[2];
            if (dotProduct > 0.0f) {
                //���������������ɫϵ������Ӱ���ӣ��õ����յ��������ǿ�ȡ�
                diffuseRed = dotProduct * sphere.diffuse[0] * shadowFactor;
                diffuseGreen = dotProduct * sphere.diffuse[1] * shadowFactor;
                diffuseBlue = dotProduct * sphere.diffuse[2] * shadowFactor;
            }
        }

        // ���㾵�淴�����
        float specularRed = 0.0f;
        float specularGreen = 0.0f;
        float specularBlue = 0.0f;

        float reflectedRayDirection[3];//������ߵķ�������
        computeReflectionRay(rayDirection, surfaceNormal, reflectedRayDirection);

        if (recursionDepth > 0) {
            float reflectedRed = 0.0f;
            float reflectedGreen = 0.0f;
            float reflectedBlue = 0.0f;

            computeColor(intersectionPoint, reflectedRayDirection, reflectedRed, reflectedGreen, reflectedBlue, recursionDepth - 1);
            //�������ɫ������Ӧ��ϵ��
            specularRed = sphere.specular[0] * reflectedRed;
            specularGreen = sphere.specular[1] * reflectedGreen;
            specularBlue = sphere.specular[2] * reflectedBlue;
        }

        // ����������ɫ  ������+�����+ ���淴��
        red = ambientRed + diffuseRed + specularRed;
        green = ambientGreen + diffuseGreen + specularGreen;
        blue = ambientBlue + diffuseBlue + specularBlue;
    }
    else {
        red = 0.0f;
        green = 0.0f;
        blue = 0.0f;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(image_width, image_height);
    glutCreateWindow("Whitted����ģ��");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();

    return 0;
}
/*
float a = pow(rayDirection[0], 2) + pow(rayDirection[1], 2) + pow(rayDirection[2], 2);
���д�����������ߵķ��������ĳ��ȵ�ƽ������a��ֵ��

float b = 2 * (rayDirection[0] * rayOrigin[0] + rayDirection[1] * rayOrigin[1] + rayDirection[2] * rayOrigin[2]);
���д�����������ߵķ����������������ĳ˻�����������b��ֵ������ͨ�������߷����������������ĵ������õ��ġ�

float c = pow(rayOrigin[0], 2) + pow(rayOrigin[1], 2) + pow(rayOrigin[2], 2) - pow(sphere.radius, 2);
���д��������������㵽���ĵľ����ƽ����ȥ����뾶��ƽ������c��ֵ��

float discriminant = pow(b, 2) - 4 * a * c;
���д���������б�ʽ��ֵ����b��ƽ����ȥ4ac���б�ʽ����ȷ�����̵Ľ�����͡�����б�ʽС��0����ʾ����������û�н��㡣

if (discriminant < 0) { return false; }
���д����ж��б�ʽ��ֵ�Ƿ�С��0�����С��0�����ʾ����������û�н��㣬��������false��
*/
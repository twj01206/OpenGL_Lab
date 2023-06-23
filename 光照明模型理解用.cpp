#include <glut.h>
#include <cmath>

// 窗口大小
int image_width = 800;
int image_height = 600;
    // 定义场景中的球体
struct Sphere {
     float radius;                   // 球体半径
     float position[3];              // 球体位置
     float ambient[3];               // 环境光颜色
     float diffuse[3];               // 漫反射颜色
     float specular[3];              // 镜面反射颜色
     float shininess;                // 光泽度 物体表面越光滑反射光集中，具有管光泽
    };
    // 创建一个球体对象
    Sphere sphere = { 1.0f, { 0.0f, 0.0f, 0.0f },
                      { 0.2f, 0.2f, 0.2f }, { 0.7f, 0.7f, 0.7f },
                      { 1.0f, 1.0f, 1.0f }, 50.0f };    
// 旋转角度
GLfloat angle = 0.0f;               

void init() {
    glEnable(GL_DEPTH_TEST);       // 启用深度测试
    glEnable(GL_LIGHTING);         // 启用光照
    glEnable(GL_LIGHT0);           // 启用0号光源

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);   // 设置背景颜色

    // 设置光源属性
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };   // 平行光源位置
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };    // 平行光源环境光颜色
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // 平行光源漫反射颜色
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // 平行光源镜面反射颜色

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 设置材质属性
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // 物体环境光颜色
    GLfloat material_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };  // 物体漫反射颜色
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 物体镜面反射颜色
    GLfloat material_shininess[] = { 50.0f };                // 物体光泽度

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
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);   // 设置视点位置和方向

    glRotatef(angle, 0.0f, 1.0f, 0.0f);    // 绕Y轴旋转立方体

    // 绘制球体
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
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);   // 设置投影透视
}

//update 函数的作用是实现场景的动画效果
void update(int value) {
    angle += 2.0f;
    if (angle > 360.0f)
        angle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

// 计算入射光线和球体的交点  at*t + 2bt +c  = 0 一般式   b*b-4ac判断交点的存在与否
bool intersectSphere(const float rayOrigin[3], const float rayDirection[3], float& t) {
    float a = pow(rayDirection[0], 2) + pow(rayDirection[1], 2) + pow(rayDirection[2], 2);
    float b = 2 * (rayDirection[0] * rayOrigin[0] + rayDirection[1] * rayOrigin[1] + rayDirection[2] * rayOrigin[2]);
    float c = pow(rayOrigin[0], 2) + pow(rayOrigin[1], 2) + pow(rayOrigin[2], 2) - pow(sphere.radius, 2);

    float discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return false;  // 光线与球体没有交点
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

    return false;  // 光线与球体没有交点
}

// 计算球体的反射光线
void computeReflectionRay(const float incidentRayDirection[3], const float surfaceNormal[3], float reflectedRayDirection[3]) {
    float dotProduct = -2 * (incidentRayDirection[0] * surfaceNormal[0] + incidentRayDirection[1] * surfaceNormal[1] + incidentRayDirection[2] * surfaceNormal[2]);
    reflectedRayDirection[0] = incidentRayDirection[0] + dotProduct * surfaceNormal[0];
    reflectedRayDirection[1] = incidentRayDirection[1] + dotProduct * surfaceNormal[1];
    reflectedRayDirection[2] = incidentRayDirection[2] + dotProduct * surfaceNormal[2];
}

// 计算球体的阴影效果
float computeShadow(const float intersectionPoint[3], const float lightDirection[3]) {
    float shadowRayOrigin[3] = { intersectionPoint[0], intersectionPoint[1], intersectionPoint[2] };
    float shadowRayDirection[3] = { lightDirection[0], lightDirection[1], lightDirection[2] };
    float t;

    if (intersectSphere(shadowRayOrigin, shadowRayDirection, t)) {
        return 0.0f;  // 球体与光源之间有遮挡，产生阴影
    }

    return 1.0f;  // 球体和光源之间无遮挡，不产生阴影
}

// 计算Whitted整体光照模型的颜色
void computeColor(const float rayOrigin[3], const float rayDirection[3], float& red, float& green, float& blue, int recursionDepth) {
    float t;
    if (intersectSphere(rayOrigin, rayDirection, t)) {
        // 计算交点位置
        float intersectionPoint[3] = { rayOrigin[0] + t * rayDirection[0], rayOrigin[1] + t * rayDirection[1], rayOrigin[2] + t * rayDirection[2] };

        // 计算表面法线
        float surfaceNormal[3] = { intersectionPoint[0] - sphere.position[0], intersectionPoint[1] - sphere.position[1], intersectionPoint[2] - sphere.position[2] };
        // 计算表面法线向量的长度
        float length = sqrt(pow(surfaceNormal[0], 2) + pow(surfaceNormal[1], 2) + pow(surfaceNormal[2], 2));
        // 归一化表面法线向量  求单位向量
        surfaceNormal[0] /= length;
        surfaceNormal[1] /= length;
        surfaceNormal[2] /= length;

        // 计算视线方向
        float viewDirection[3] = { -rayDirection[0], -rayDirection[1], -rayDirection[2] };

        // 计算环境光照   颜色值相乘必然会使颜色值减小（物体更暗），相加必然会使颜色值增加更加接近1（物体更亮）
        //Shader中物体的最终颜色=环境光+自发光+漫反射+高光。
        //两个光的相乘往往代表的是 光强x光反射或者光反射x光强 或者光反射x光反射。结果光发生了衰减或者反射能力发生了变化
        float ambientRed = sphere.ambient[0] * sphere.diffuse[0];
        float ambientGreen = sphere.ambient[1] * sphere.diffuse[1];
        float ambientBlue = sphere.ambient[2] * sphere.diffuse[2];

        // 计算漫反射光照
        float diffuseRed = 0.0f;
        float diffuseGreen = 0.0f;
        float diffuseBlue = 0.0f;

        float lightDirection[3] = { 1.0f, 1.0f, 1.0f };//定义光线的方向，将其假设为 {1.0f, 1.0f, 1.0f}，即光线在三个方向上的分量都为1.0
        length = sqrt(pow(lightDirection[0], 2) + pow(lightDirection[1], 2) + pow(lightDirection[2], 2));
        lightDirection[0] /= length;
        lightDirection[1] /= length;
        lightDirection[2] /= length;

        float shadowFactor = computeShadow(intersectionPoint, lightDirection);//表示阴影因子的变量

        if (shadowFactor > 0.0f) {
            //点积的结果将用于计算漫反射光的强度
            float dotProduct = lightDirection[0] * surfaceNormal[0] + lightDirection[1] * surfaceNormal[1] + lightDirection[2] * surfaceNormal[2];
            if (dotProduct > 0.0f) {
                //点积乘以漫反射颜色系数和阴影因子，得到最终的漫反射光强度。
                diffuseRed = dotProduct * sphere.diffuse[0] * shadowFactor;
                diffuseGreen = dotProduct * sphere.diffuse[1] * shadowFactor;
                diffuseBlue = dotProduct * sphere.diffuse[2] * shadowFactor;
            }
        }

        // 计算镜面反射光照
        float specularRed = 0.0f;
        float specularGreen = 0.0f;
        float specularBlue = 0.0f;

        float reflectedRayDirection[3];//反射光线的方向向量
        computeReflectionRay(rayDirection, surfaceNormal, reflectedRayDirection);

        if (recursionDepth > 0) {
            float reflectedRed = 0.0f;
            float reflectedGreen = 0.0f;
            float reflectedBlue = 0.0f;

            computeColor(intersectionPoint, reflectedRayDirection, reflectedRed, reflectedGreen, reflectedBlue, recursionDepth - 1);
            //反射光颜色乘以相应的系数
            specularRed = sphere.specular[0] * reflectedRed;
            specularGreen = sphere.specular[1] * reflectedGreen;
            specularBlue = sphere.specular[2] * reflectedBlue;
        }

        // 计算最终颜色  环境光+反射光+ 镜面反光
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
    glutCreateWindow("Whitted光照模型");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    glutMainLoop();

    return 0;
}
/*
float a = pow(rayDirection[0], 2) + pow(rayDirection[1], 2) + pow(rayDirection[2], 2);
这行代码计算了射线的方向向量的长度的平方，即a的值。

float b = 2 * (rayDirection[0] * rayOrigin[0] + rayDirection[1] * rayOrigin[1] + rayDirection[2] * rayOrigin[2]);
这行代码计算了射线的方向向量与射线起点的乘积的两倍，即b的值。它是通过对射线方向向量和起点坐标的点积计算得到的。

float c = pow(rayOrigin[0], 2) + pow(rayOrigin[1], 2) + pow(rayOrigin[2], 2) - pow(sphere.radius, 2);
这行代码计算了射线起点到球心的距离的平方减去球体半径的平方，即c的值。

float discriminant = pow(b, 2) - 4 * a * c;
这行代码计算了判别式的值，即b的平方减去4ac。判别式用于确定方程的解的类型。如果判别式小于0，表示射线与球体没有交点。

if (discriminant < 0) { return false; }
这行代码判断判别式的值是否小于0，如果小于0，则表示射线与球体没有交点，函数返回false。
*/
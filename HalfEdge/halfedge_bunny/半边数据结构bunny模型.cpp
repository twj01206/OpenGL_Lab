/*
 * @Author: twj
 * @Date: 2023-03-22 至 2023-03-31
 * @Name:半边数据结构实现对bunny模型OFF文件的读取
 * @question：建立数据结构不会，然后谷歌慢慢入门  结合了之前使用数组的做法，不断尝试  批判思考
 * @Summy:
		  ①建立数据结构的困难，由懵懂到逐渐了解  画图我就理解
		  ②遇到了面的存储问题，其实也不难，先设置一个面，然后面里面的边再按照边具有的属性，一个个设置，画图慢慢理解
		  ③最难的是对称半边的查询和设置，目前为此，我好像只实现了对称半边的设置而已，查询未使用上，也还未测试过，这里日后继续终点测试
			目前只是完成了由数组到半边数据结构的绘画，对于自己设置的边的一些信息还没有使用上
			----》未实现增、删、查、改 的功能函数  这就是有待改进的程序
		  ④完成本次的读取模型，我还要继续思考一下思路，（实现的过程，最好形成markdown文档，）然后继续实现出一些功能
		  ⑤逐渐开始批判，网上的帖子也未必正确，学会用自己的思路跟它杠，事实证明，我的思路才是正确的，我写的也对！
		  ⑥算上本次，本周内一共有过3此交流，频率还行，但问问题不突出，不明确，
		  （突然看到的一句话：
			----》好比员工汇报给领导，凡事有交代，件件有着落，事事有回音，这就是程序员职场（有问题及时跟领导反馈））
	总之：感觉有了些进步，还需要继续加油！！！

	勤学如春起之苗，不见其增，日有所长；辍学如磨刀之石，不见其损，日有所亏；
	目标：凡事有交代，件件有着落，事事有回音，这就是程序员职场（有问题及时跟领导反馈）
 * 
 */
#include<glut.h>
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <windows.h>
using namespace std;
string filepath = "bunny.off";
class Vertex;//顶点类
class HalfEdge;//半边类
class Face;//面类
class HeMesh;//网格类
class Vertex {
public:
	int index;
	float x, y, z;
	HalfEdge* halfedge;// 任意一条以此顶点为起点的半边
	Vertex(int index, float x, float y, float z) :index(index), x(x), y(y), z(z), halfedge(NULL) {}
};
class HalfEdge {
public:
	Vertex* start;
	HalfEdge* pre, * next;
	HalfEdge* pair;
	Face* face;
	HalfEdge() :start(nullptr), pre(nullptr), next(nullptr), pair(nullptr), face(nullptr) {}
};
class Face {
public:
	HalfEdge* edge;// 任意一条边
	Face() :edge(nullptr) {}
};
class HeMesh {
public:
	vector<Vertex*> vertexs;
	vector<HalfEdge*> halfedges;
	vector<Face*> faces;
	void readOFF(string filepath);
};
void HeMesh::readOFF(string filepath) {
	ifstream fin(filepath);
	if (!fin.is_open()) {
		cout << "文件" << filepath << "打开失败" << endl;
	}
	//设置读取头部变量
	string format;
	int vn, fn, en;//顶点的个数 面的个数  边的个数
	float x, y, z;

	fin >> format;
	fin >> vn >> fn >> en;
	// 读取顶点
	for (int i = 0; i < vn; i++) {
		fin >> x >> y >> z;
		Vertex* v = new Vertex(i, x, y, z);
		vertexs.emplace_back(v);// 将点添加到点集
	}
	cout << "vertexs.size()=" << vertexs.size() << endl;
	// 读取面
	for (int i = 0; i < fn; i++) {
		int n, index1, index2, index3;
		fin >> n >> index1 >> index2 >> index3;

		Face* f = new Face();
		//没有指向f->edge？？？
		HalfEdge* he1 = new HalfEdge();
		HalfEdge* he2 = new HalfEdge();
		HalfEdge* he3 = new HalfEdge();

		he1->start = vertexs[index1];
		he2->start = vertexs[index2];
		he3->start = vertexs[index3];
		//顺时针
		he1->next = he2;
		he2->next = he3;
		he3->next = he1;

		he1->face = f;
		he2->face = f;
		he3->face = f;

		he1->pair = nullptr;
		he2->pair = nullptr;
		he3->pair = nullptr;

		halfedges.push_back(he1);
		halfedges.push_back(he2);
		halfedges.push_back(he3);

		f->edge = he1;//面的起始边
		faces.emplace_back(f);
	}
	cout << "faces.size()=" << faces.size() << endl;

	// 建立半边之间的关联关系  面中的边与边之间的关系
	for (int i = 0; i < faces.size(); i++) {
		Face* f = faces[i];
		HalfEdge* edge = f->edge;
		//面的3条半边
		HalfEdge* he1 = edge;
		HalfEdge* he2 = edge->next;
		HalfEdge* he3 = edge->next->next;
		//设置半边对应的面
		he1->face = f;
		he2->face = f;
		he3->face = f;
		//设置半边的起始点 
		he1->start = edge->start;
		he2->start = edge->next->start;
		he3->start = edge->next->next->start;
		//设置半边的前驱和后继 顺时针
		he1->pre = he3;
		he3->next = he1;
		he2->pre = he1;
		he1->next = he2;
		he3->pre = he2;
		he2->next = he3;
		//以此为起始点的任意一条半边
		he1->start->halfedge = he1;
		he2->start->halfedge = he2;
		he3->start->halfedge = he3;
		//每条半边的对称半边  外界顺时针 内逆时针
		//设置对称半边 he->pair 为空不存在  自己寻找
		if (he1->pair == nullptr) {
			for (int j = 0; j < halfedges.size(); j++) {
				HalfEdge* he = halfedges[j];   //找起始点是否相等
				if (he->start == he2->start && he->next->start == he1->start) {
					he->pair = he1;
					he1->pair = he;
				}
			}
		}
		if (he2->pair == nullptr) {
			for (int j = 0; j < halfedges.size(); j++) {
				HalfEdge* he = halfedges[j];
				if (he->start == he3->start && he->next->start == he2->start) {
					he->pair = he2;
					he2->pair = he;
				}
			}
		}
		if (he3->pair == nullptr) {
			for (int j = 0; j < halfedges.size(); j++) {
				HalfEdge* he = halfedges[j];
				if (he->start == he1->start && he->next->start == he3->start) {
					he->pair = he3;
					he3->pair = he;
				}
			}
		}
	}
	fin.close();
}
HeMesh mesh;// 全局变量
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	mesh.readOFF(filepath);//读取OFF文件
	glEnable(GL_DEPTH_TEST);
}
void display() {
	// 示要清除颜色缓冲以及深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	// 绘制每个面
	//
	
	for (int i = 0; i < mesh.faces.size(); i++) {
		//glBegin(GL_TRIANGLES); //将一组顶点表示为三角形
		glBegin(GL_LINE_LOOP);//将一组顶点表示为一个封闭的线段环
		HalfEdge* edge = mesh.faces[i]->edge;
		Vertex* v1 = edge->start;
		Vertex* v2 = edge->next->start;
		Vertex* v3 = edge->next->next->start;
		/*glVertex3f(v1->x / 20, v1->y / 20, v1->z / 20);
		glVertex3f(v2->x / 20, v2->y / 20, v2->z / 20);
		glVertex3f(v3->x / 20, v3->y / 20, v3->z / 20);*/
		Sleep(0.5);//表示休息
		glVertex3f(v1->x / 10, v1->y / 10 - 1, v1->z / 10);
		glVertex3f(v2->x / 10, v2->y / 10 - 1, v2->z / 10);
		glVertex3f(v3->x / 10, v3->y / 10 - 1, v3->z / 10);
		glEnd();
		//glutSwapBuffers();
		glFlush();
	}
	
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Bunny");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}

/*
for (int j = 0; j < halfedges.size(); j++) {
				HalfEdge* he = halfedges[j];
				if (he->start == he2->start && he->next->start == he1->start) {}
				if (he->start == he3->start && he->next->start == he2->start) {}
			}
*/
/*for (int i = 0; i < fn; i++) {
	int n, index1, index2, index3;
	fin >> n >> index1 >> index2 >> index3;
	Face* f = new Face();
	cout << "n=" << n << " " << index1 << " " << index2 << " " << index3 << endl;
	HalfEdge* pre=nullptr;
	if (pre == nullptr) {
		exit(-1);
	}

//	for (int j = 0; j < n; j++) { //3条边的起始点
//		HalfEdge* he = new HalfEdge();//创建半边实列  可以创建3次
//		he->start = vertexs[index1];//设置起始点
//		halfedges.push_back(he);
//		if(pre==NULL){
//			f->edge = he;
//		}
//		else {
//			pre->next = he;
//			he->pre = pre;
//		}
//		pre = he;
//	}
//
//	pre->next = f->edge;//3--->1  第3条半边 pre 第1条半边 f->edge = he;
//	f->edge->pre = pre;
//	faces.push_back(f);
//	cout << "faces.size()=" << faces.size() << endl;
//}
*/
/*for (int i = 0; i < halfedges.size(); i++) {
	HalfEdge* he = halfedges[i];
	he->next = halfedges[++i];
}*/

/*
class Vertex {
public:
	int index;
	float x, y, z;
	HalfEdge* halfedge;// 任意一条以此顶点为起点的半边
	Vertex(int index, float x, float y, float z) :index(index), x(x), y(y), z(z), halfedge(NULL) {}
	void sethalfedge(HalfEdge* halfEdge) { halfEdge = halfEdge; }//设置以此顶点为起点的边
	HalfEdge* getHalfEdge() { return halfedge; }
};*/

/*
			//设置前后边的关系 1 2 3 条半边的顺序  先设置一条为起始边 只有一条边时候怎么指向
			pre = he;
			f->edge = he;//作为一条边   //初始化设置一条边
			f->edge->pre = pre;//未明确这条边是那条边  pre=he
			f->edge->next = next;
			*/
			/*
				for (int i = 0; i < faceNum; i++) {
					int n, index1, index2, index3;
					fin >> n >> index1 >> index2 >> index3;
					vector<HalfEdge*> he_;//一个面的三条边的存储
					Face* f = new Face();
					faces.push_back(f);
					//指向的问题
					//创建三条半边的类指针
					HalfEdge* halfedge1 = new HalfEdge();
					HalfEdge* halfedge2 = new HalfEdge();
					HalfEdge* halfedge3 = new HalfEdge();
					halfedges.push_back(halfedge1);
					halfedges.push_back(halfedge2);
					halfedges.push_back(halfedge3);
					//三条半边的起点
					halfedge1->vertex = vertexs[index1];
					halfedge2->vertex = vertexs[index2];
					halfedge3->vertex = vertexs[index3];
					//三条半边的pre和next指针  顺时针
					halfedge1->pre =halfedge3 ;
					halfedge2->pre = halfedge1;
					halfedge3->pre = halfedge2;
					//三条半边的pair
					halfedge1->next = halfedge2;
					halfedge2->next = halfedge3;
					halfedge3->next = halfedge1;
					//三条半边的face
					halfedge1->face=f;
					halfedge2->face=f;
					halfedge3->face=f;

					he_.push_back(halfedge1);
					he_.push_back(halfedge2);
					he_.push_back(halfedge3);
				}*/



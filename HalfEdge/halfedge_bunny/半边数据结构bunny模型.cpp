/*
 * @Author: twj
 * @Date: 2023-03-22 �� 2023-03-31
 * @Name:������ݽṹʵ�ֶ�bunnyģ��OFF�ļ��Ķ�ȡ
 * @question���������ݽṹ���ᣬȻ��ȸ���������  �����֮ǰʹ����������������ϳ���  ����˼��
 * @Summy:
		  �ٽ������ݽṹ�����ѣ����¶������˽�  ��ͼ�Ҿ����
		  ����������Ĵ洢���⣬��ʵҲ���ѣ�������һ���棬Ȼ��������ı��ٰ��ձ߾��е����ԣ�һ�������ã���ͼ�������
		  �����ѵ��ǶԳư�ߵĲ�ѯ�����ã�ĿǰΪ�ˣ��Һ���ֻʵ���˶Գư�ߵ����ö��ѣ���ѯδʹ���ϣ�Ҳ��δ���Թ��������պ�����յ����
			Ŀǰֻ������������鵽������ݽṹ�Ļ滭�������Լ����õıߵ�һЩ��Ϣ��û��ʹ����
			----��δʵ������ɾ���顢�� �Ĺ��ܺ���  ������д��Ľ��ĳ���
		  ����ɱ��εĶ�ȡģ�ͣ��һ�Ҫ����˼��һ��˼·����ʵ�ֵĹ��̣�����γ�markdown�ĵ�����Ȼ�����ʵ�ֳ�һЩ����
		  ���𽥿�ʼ���У����ϵ�����Ҳδ����ȷ��ѧ�����Լ���˼·�����ܣ���ʵ֤�����ҵ�˼·������ȷ�ģ���д��Ҳ�ԣ�
		  �����ϱ��Σ�������һ���й�3�˽�����Ƶ�ʻ��У��������ⲻͻ��������ȷ��
		  ��ͻȻ������һ�仰��
			----���ñ�Ա���㱨���쵼�������н��������������䣬�����л���������ǳ���Աְ���������⼰ʱ���쵼��������
	��֮���о�����Щ����������Ҫ�������ͣ�����

	��ѧ�紺��֮�磬���������������������ѧ��ĥ��֮ʯ��������������������
	Ŀ�꣺�����н��������������䣬�����л���������ǳ���Աְ���������⼰ʱ���쵼������
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
class Vertex;//������
class HalfEdge;//�����
class Face;//����
class HeMesh;//������
class Vertex {
public:
	int index;
	float x, y, z;
	HalfEdge* halfedge;// ����һ���Դ˶���Ϊ���İ��
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
	HalfEdge* edge;// ����һ����
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
		cout << "�ļ�" << filepath << "��ʧ��" << endl;
	}
	//���ö�ȡͷ������
	string format;
	int vn, fn, en;//����ĸ��� ��ĸ���  �ߵĸ���
	float x, y, z;

	fin >> format;
	fin >> vn >> fn >> en;
	// ��ȡ����
	for (int i = 0; i < vn; i++) {
		fin >> x >> y >> z;
		Vertex* v = new Vertex(i, x, y, z);
		vertexs.emplace_back(v);// ������ӵ��㼯
	}
	cout << "vertexs.size()=" << vertexs.size() << endl;
	// ��ȡ��
	for (int i = 0; i < fn; i++) {
		int n, index1, index2, index3;
		fin >> n >> index1 >> index2 >> index3;

		Face* f = new Face();
		//û��ָ��f->edge������
		HalfEdge* he1 = new HalfEdge();
		HalfEdge* he2 = new HalfEdge();
		HalfEdge* he3 = new HalfEdge();

		he1->start = vertexs[index1];
		he2->start = vertexs[index2];
		he3->start = vertexs[index3];
		//˳ʱ��
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

		f->edge = he1;//�����ʼ��
		faces.emplace_back(f);
	}
	cout << "faces.size()=" << faces.size() << endl;

	// �������֮��Ĺ�����ϵ  ���еı����֮��Ĺ�ϵ
	for (int i = 0; i < faces.size(); i++) {
		Face* f = faces[i];
		HalfEdge* edge = f->edge;
		//���3�����
		HalfEdge* he1 = edge;
		HalfEdge* he2 = edge->next;
		HalfEdge* he3 = edge->next->next;
		//���ð�߶�Ӧ����
		he1->face = f;
		he2->face = f;
		he3->face = f;
		//���ð�ߵ���ʼ�� 
		he1->start = edge->start;
		he2->start = edge->next->start;
		he3->start = edge->next->next->start;
		//���ð�ߵ�ǰ���ͺ�� ˳ʱ��
		he1->pre = he3;
		he3->next = he1;
		he2->pre = he1;
		he1->next = he2;
		he3->pre = he2;
		he2->next = he3;
		//�Դ�Ϊ��ʼ�������һ�����
		he1->start->halfedge = he1;
		he2->start->halfedge = he2;
		he3->start->halfedge = he3;
		//ÿ����ߵĶԳư��  ���˳ʱ�� ����ʱ��
		//���öԳư�� he->pair Ϊ�ղ�����  �Լ�Ѱ��
		if (he1->pair == nullptr) {
			for (int j = 0; j < halfedges.size(); j++) {
				HalfEdge* he = halfedges[j];   //����ʼ���Ƿ����
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
HeMesh mesh;// ȫ�ֱ���
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	mesh.readOFF(filepath);//��ȡOFF�ļ�
	glEnable(GL_DEPTH_TEST);
}
void display() {
	// ʾҪ�����ɫ�����Լ���Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	// ����ÿ����
	//
	
	for (int i = 0; i < mesh.faces.size(); i++) {
		//glBegin(GL_TRIANGLES); //��һ�鶥���ʾΪ������
		glBegin(GL_LINE_LOOP);//��һ�鶥���ʾΪһ����յ��߶λ�
		HalfEdge* edge = mesh.faces[i]->edge;
		Vertex* v1 = edge->start;
		Vertex* v2 = edge->next->start;
		Vertex* v3 = edge->next->next->start;
		/*glVertex3f(v1->x / 20, v1->y / 20, v1->z / 20);
		glVertex3f(v2->x / 20, v2->y / 20, v2->z / 20);
		glVertex3f(v3->x / 20, v3->y / 20, v3->z / 20);*/
		Sleep(0.5);//��ʾ��Ϣ
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

//	for (int j = 0; j < n; j++) { //3���ߵ���ʼ��
//		HalfEdge* he = new HalfEdge();//�������ʵ��  ���Դ���3��
//		he->start = vertexs[index1];//������ʼ��
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
//	pre->next = f->edge;//3--->1  ��3����� pre ��1����� f->edge = he;
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
	HalfEdge* halfedge;// ����һ���Դ˶���Ϊ���İ��
	Vertex(int index, float x, float y, float z) :index(index), x(x), y(y), z(z), halfedge(NULL) {}
	void sethalfedge(HalfEdge* halfEdge) { halfEdge = halfEdge; }//�����Դ˶���Ϊ���ı�
	HalfEdge* getHalfEdge() { return halfedge; }
};*/

/*
			//����ǰ��ߵĹ�ϵ 1 2 3 ����ߵ�˳��  ������һ��Ϊ��ʼ�� ֻ��һ����ʱ����ôָ��
			pre = he;
			f->edge = he;//��Ϊһ����   //��ʼ������һ����
			f->edge->pre = pre;//δ��ȷ��������������  pre=he
			f->edge->next = next;
			*/
			/*
				for (int i = 0; i < faceNum; i++) {
					int n, index1, index2, index3;
					fin >> n >> index1 >> index2 >> index3;
					vector<HalfEdge*> he_;//һ����������ߵĴ洢
					Face* f = new Face();
					faces.push_back(f);
					//ָ�������
					//����������ߵ���ָ��
					HalfEdge* halfedge1 = new HalfEdge();
					HalfEdge* halfedge2 = new HalfEdge();
					HalfEdge* halfedge3 = new HalfEdge();
					halfedges.push_back(halfedge1);
					halfedges.push_back(halfedge2);
					halfedges.push_back(halfedge3);
					//������ߵ����
					halfedge1->vertex = vertexs[index1];
					halfedge2->vertex = vertexs[index2];
					halfedge3->vertex = vertexs[index3];
					//������ߵ�pre��nextָ��  ˳ʱ��
					halfedge1->pre =halfedge3 ;
					halfedge2->pre = halfedge1;
					halfedge3->pre = halfedge2;
					//������ߵ�pair
					halfedge1->next = halfedge2;
					halfedge2->next = halfedge3;
					halfedge3->next = halfedge1;
					//������ߵ�face
					halfedge1->face=f;
					halfedge2->face=f;
					halfedge3->face=f;

					he_.push_back(halfedge1);
					he_.push_back(halfedge2);
					he_.push_back(halfedge3);
				}*/



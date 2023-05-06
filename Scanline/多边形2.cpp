#include<glut.h>
#include<iostream>
#include<vector>
#include<list>
#include<cmath>
#include<deque>
#include<algorithm>
#include<cfloat>
using namespace std;

class Point;
class Edge;
class ET;//边表
class AET;//活化边表
class Ploygon;

class Point {
public:
	float x, y;
	Point(float _x, float _y) :x(_x), y(_y) {}
};
class Edge {
public:
	Point start;
	Point end;
	Edge() = default;
	Edge(Point start_, Point end_) :start(start_), end(end_) {}
};
class ET {
public:
	float ymax, ymin;
	float x;
	float dx;
	ET* next;//指向下一条边的指针
	ET(float ymax, float x, float dx) :ymax(ymax), x(x), dx(dx), ymin(0), next(nullptr) {}
};
class AEL {
public:
	float x;//交点x坐标
	float dx;//斜率
	float ymax;
	AEL* next;// 指向下一个节点的指针
	AEL(float x, float dx, float ymax) :x(x), dx(dx), ymax(ymax), next(nullptr) {}
};
class Polygon {
public:
	vector<Point> points;    // 顶点
	vector<Edge> edges;      // 边表
	list<ET*> et;            // 边表项
	vector<list<ET*>> vl;	 //分类边表
	AEL* ael;                // 活化边表
public:
	Polygon() = default;
	Polygon(const vector<Point>& points) :points(points), ael(nullptr) {}
	void BuildEdges();                // 将顶点转换成边 Edge 并存入 edges 数组中
	void BuildET();                  // 边表项  ---将多边形转换为边表项
	void SortET();                   // 分类边表---对边表项排序  //多边形的分类边表（建立和排序） 
	void AddETtoAEL(float scanline);   //建立活化边表AEL并且排序
	void Scanline();                 //多边形扫描线算法
	void Scanline2();

	void DeleteAELNodes(float scanline);
	void UpdateAELNodes();

	void PrintSortAEL(AEL* ael);     //打印信息 排序后的AEL
	void setPixel(int x, int y);    //填充像素点
	void DrawBoard();
public:
	//归并排序
	AEL* merge(AEL* left, AEL* right);
	AEL* merge_sort(AEL* head);
	void sort_ael(AEL* ael);
};

AEL* Polygon::merge(AEL* left, AEL* right) {
	AEL* dummy = new AEL(0, 0, 0);// 创建一个新的链表，用于存储合并后的结果
	AEL* cur = dummy;
	while (left != nullptr && right != nullptr) {
		if ((left->x < right->x) || (left->x == right->x && left->dx < right->dx)) {
			cur->next = left;
			left = left->next;
		}
		else {
			cur->next = right;
			right = right->next;
		}
		cur = cur->next;
	}

	if (left != nullptr) cur->next = left;
	if (right != nullptr) cur->next = right;

	return dummy->next;
}
AEL* Polygon::merge_sort(AEL* head) {
	if (head == nullptr || head->next == nullptr) return head;

	AEL* slow = head;
	AEL* fast = head->next->next;
	while (fast != nullptr && fast->next != nullptr) {
		slow = slow->next;
		fast = fast->next;
	}

	AEL* right = slow->next;
	slow->next = nullptr;

	AEL* left_sort = merge_sort(head);// 分别对子链表进行排序
	AEL* right_sort = merge_sort(right);

	return merge(left_sort, right_sort); // 合并两个已排序的子链表
}
void Polygon::sort_ael(AEL* ael) {
	AEL* sorted = merge_sort(ael->next);
	ael->next = sorted;
}
void Polygon::setPixel(int x, int y) {
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void Polygon::DrawBoard() {
	//画多边形
	glColor3f(1.0f, .0f, 1.0f);
	for (int i = 0; i < edges.size(); i++) {
		glBegin(GL_LINES);
		glVertex2f(edges[i].start.x, edges[i].start.y);
		glVertex2f(edges[i].end.x, edges[i].end.y);
		glEnd();
	}
}

void Polygon::UpdateAELNodes() {
	AEL* cur = ael;
	while (cur != nullptr) {
		if (cur->dx != 0) {
			cur->x += cur->dx;
		}
		cur = cur->next;
	}
	cout << "完成Updating AEL Nodes" << std::endl;
}
void Polygon::DeleteAELNodes(float scanline) {
	AEL* pre = nullptr;
	AEL* cur = ael;
	while (cur != nullptr) {
		if (cur->ymax == scanline) {
			AEL* tmp = cur;
			if (pre == nullptr) {
				ael = cur->next;
				cur = ael;
			}
			else {
				pre->next = cur->next;//指向当前的下一个节点，删除当前的
				cur = pre->next;
			}
			delete tmp;
		}
		else {
			pre = cur;
			cur = cur->next;
		}
	}
}
void Polygon::PrintSortAEL(AEL* ael) {
	cout << endl << "AEL sorted" << endl;
	AEL* cur = ael->next;
	while (cur != nullptr) {
		//cout << "自己的  " << &cur->next << "\t";
		cout << "sorted AEL node:x =" << cur->x << ", dx = " << cur->dx << ", ymax = " << cur->ymax << endl;
		cur = cur->next;
	}
}

void Polygon::BuildEdges() {
	int n = points.size();
	for (int i = 0; i < n; i++) {
		Edge e(points[i], points[(i + 1) % n]);//构造多边形的边
		edges.emplace_back(e);//边存储到edges数组中
		cout << "Edge " << i << " start: (" << e.start.x << ", " << e.start.y
			<< "), end: (" << e.end.x << ", " << e.end.y << ")" << endl;
	}
	cout << endl;
}
void Polygon::BuildET() {
	int n = edges.size();//如果边的起始点y坐标小于等于终止点y坐标，则起始点为下端点，终止点为上端点；否则，起始点为上端点，终止点为下端点。
	for (int i = 0; i < n; i++) {
		Edge e = edges[i];
		Point start = e.start, end = e.end;
		if (start.y > e.end.y) {//ymin为下端点
			start = e.end;
			end = e.start;
		}
		if (start.y == end.y) continue;
		float ymax = max(e.start.y, e.end.y);
		float ymin = min(e.start.y, e.end.y);
		float x = start.x;
		float dx = (e.end.x - e.start.x) / (e.end.y - e.start.y);

		ET* _et = new ET(ymax, x, dx);
		_et->ymin = ymin;
		et.emplace_back(_et);

		cout << "et.ymin:" << ymin << "  et.ymax:" << _et->ymax << "  et.x:" << _et->x << "  et.dx:" << _et->dx << "  et.next:" << _et->next << std::endl;
	}
	cout << endl;
}
void Polygon::SortET() {

	for (auto it = et.begin(); it != et.end(); it++) {
		ET* et1 = *it;
		float y = et1->ymin;
		int index = floor(y);
		if (vl.size() <= index) {
			vl.resize(index + 1);
		}
		//cout << "index(ymin):" << et1->ymin << endl;
		vl[index].push_back(et1);
	}
	for (int i = 0; i < vl.size(); i++) {
		cout << "Elements at index " << i << ": ";
		for (auto it = vl[i].begin(); it != vl[i].end(); it++) {
			cout << "min" << (*it)->ymin << " ymax " << (*it)->ymax << " x" << (*it)->x << " dx" << (*it)->dx << "; ";
		}
		cout << endl;
	}
	vector<int>idx(vl.size());
	for (int i = 0; i < vl.size(); i++) idx[i] = i;  //idx，存储 vl 中每个子 vector 的下标
	for (int i = 1; i < idx.size(); i++) {          //插入排序 idx
		int tmp = idx[i];
		int j = i - 1;
		while (j >= 0 && idx[j] > tmp) {
			idx[j + 1] = idx[j];
			j--;
		}
		idx[j + 1] = tmp;
	}
	//我之前没考虑到j==lst.begin()的情况，弄了半天时间，第二天突然领悟,debug打印信息成功
	for (auto& lst : vl) {
		/*for (int i = 0; i < idx.size(); i++) {
			int index = idx[i];
			auto& lst = vl[index];*/
		for (auto it = next(lst.begin()); it != lst.end(); it++) {
			auto tmp = *it;
			auto j = prev(it);
			while (j != lst.begin() && ((*j)->ymax > tmp->ymax ||
				(*j)->ymax == tmp->ymax && (*j)->x > tmp->x ||
				(*j)->ymax == tmp->ymax && (*j)->x == tmp->x && (*j)->dx > tmp->dx)) {
				iter_swap(next(j), j);
				j--;
			}
			if (j == lst.begin() && ((*j)->ymax > tmp->ymax ||
				((*j)->ymax == tmp->ymax && (*j)->x > tmp->x) ||
				((*j)->ymax == tmp->ymax && (*j)->x == tmp->x && (*j)->dx > tmp->dx))) {
				iter_swap(j, it);
			}
			else {
				iter_swap(next(j), it);
			}
		}
	}

	for (auto& lst : vl) {
		if (lst.empty()) continue;    //空
		else if (lst.size() == 1) {   //只有一个边表项
			auto it = lst.begin();
			(*it)->next = nullptr;
		}
		else {						  //2个或以上的边表项	
			auto it = lst.begin();
			auto last_it = prev(lst.end());//lst.end() 实际上是返回指向 list 容器中最后一个元素之后的末尾元素的迭代器
			while (it != last_it) {
				auto next_it = next(it);
				(*it)->next = *next_it;
				++it;
			}
			(*last_it)->next = nullptr;
		}
	}
	cout << endl << "分类边表排序(按下端点y分类排序) sort index:" << endl;
	for (int i = 0; i < idx.size(); i++) {
		int index = idx[i];
		cout << "Elements at index " << index << ": ";
		for (auto it = vl[index].begin(); it != vl[index].end(); it++) {
			//cout << "(*it).&" << &(*it)->next<<" ";//查看地址
			cout << "min" << (*it)->ymin << " ymax " << (*it)->ymax << " x" << (*it)->x << " dx" << (*it)->dx << " next" << (*it)->next << "; ";
		}
		cout << endl;
	}
	cout << endl;
}

void Polygon::AddETtoAEL(float scanline) {
	ael = new AEL(0, 0, 0);
	AEL* cur = ael;
	cout << "Building AEL" << endl;
	for (auto& et_list : vl) {  //从边桶中找出相交的边
		for (auto& p : et_list) {
			bool intersects = (p->ymin <= scanline && scanline <= p->ymax);//相交
			if (intersects) {
				if (p->ymax > scanline) {
					float x = p->x + p->dx * (scanline - p->ymin);
					AEL* tmp = new AEL(x, p->dx, p->ymax);
					cur->next = tmp;
					tmp->next = nullptr;
					cur = tmp; // 更新cur指针
				}
			}
		}
	}
	cout << "完成Finished building AEL" << endl;
	sort_ael(ael);//快排
	PrintSortAEL(ael);//打印
}
void Polygon::Scanline() {
	float y_min = 0, y_max = 0;
	bool is_updated = true;//判断是否第一次更新y_min
	for (int i = 0; i < edges.size(); i++) {
		if (is_updated) {
			y_min = min(edges[i].start.y, edges[i].end.y);
			is_updated = false;
		}
		else {
			y_min = min(y_min, min(edges[i].start.y, edges[i].end.y));
		}
		y_max = max(y_max, max(edges[i].start.y, edges[i].end.y));
	}
	cout << "y的范围为最小y_min" << y_min << "  最大y_max：" << y_max << endl << endl;

	for (float scanline = y_min; scanline <= y_max; scanline += 1) {
		cout << endl << "设置扫描线 " << scanline << endl;
		AddETtoAEL(scanline);
		DeleteAELNodes(scanline);
		UpdateAELNodes();
		// 填充像素
		AEL* cur = ael;
		while (cur != nullptr && cur->next != nullptr) {
			int x1 = static_cast<int>(cur->x);
			int x2 = static_cast<int>(cur->next->x);
			for (int x = x1; x <= x2; x++) {
				setPixel(x, scanline);//填充
			}
			cur = cur->next;
		}
	}
}
void Polygon::Scanline2() {
	float y_min = 0, y_max = 0;
	bool is_updated = true;//判断是否第一次更新y_min
	for (int i = 0; i < edges.size(); i++) {
		if (is_updated) {
			y_min = min(edges[i].start.y, edges[i].end.y);
			is_updated = false;
		}
		else {
			y_min = min(y_min, min(edges[i].start.y, edges[i].end.y));
		}
		y_max = max(y_max, max(edges[i].start.y, edges[i].end.y));
	}
	cout << "y的范围为最小y_min" << y_min << "  最大y_max：" << y_max << endl << endl;

	if (ael == nullptr) {
		cout << "ael is nullptr" << endl;
	}

}
void display() {
	glClearColor(1.0, 1.0, 1.0, 0.0); // 将背景颜色设置为白色
	glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区
	gluOrtho2D(0.0, 20.0, 0.0, 20.0);
	vector<Point> points = { Point(3, 4), Point(2, 10), Point(5, 7), Point(12, 11), Point(15, 5), Point(12, 2), Point(7, 2) };
	Polygon p(points);
	//BuildEdges来进行绘画多边形轮廓
	p.BuildEdges();
	p.DrawBoard();
	p.BuildET();//开始建立边表ET
	p.SortET();//开始创建排序分类边表

	p.Scanline2();

	glFlush();
}
void initOpenGL(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Scanline Polygon");
	glutDisplayFunc(display);
	glutMainLoop();
}
int main(int argc, char* argv[]) {
	/*vector<Point> points = { Point(3, 4), Point(2, 10), Point(5, 7), Point(12, 11), Point(15, 5), Point(12, 2), Point(7, 2) };
	Polygon p(points);
	p.BuildEdges();
	p.BuildET();
	p.SortET();*/
	//p.BuildAEL(5);
	initOpenGL(argc, argv);
	return 0;
}
/*
void Polygon::BuildAEL(float scanline) {
	ael = new AEL(0, 0, 0);
	AEL* cur = ael;
	cout << "Building AEL" << endl;
	for (auto& et_list : vl) {
		for (auto& p : et_list) {
			bool intersects = (p->ymin <= scanline && scanline <= p->ymax);
			if (intersects) {
				if (p->ymax > scanline) {
					AEL* tmp = new AEL(p->x, p->dx, p->ymax);
					cur->next = tmp;
					tmp->next = nullptr;
					cur = tmp; // 更新cur指针
				}
			}
		}
	}
	cout << "完成Finished building AEL" << endl;
	sort_ael(ael);//快排
	PrintSortAEL(ael);
	//测试 节点是否相连
	//cout << "ael.next.next.x" << ael->next->next->dx;
}
*/
/*
void Polygon::BuildAEL(float scanline) {
	ael = new AEL(0, 0, 0);
	AEL* cur = ael;
	cout << "Building AEL" << endl;
	for (auto& et_list : vl) {
		for (auto& p : et_list) {
			// 检查当前边是否与扫描线 相交、重合
			bool intersects = (p->ymin <= scanline && scanline <= p->ymax);
			bool overlaps = (p->ymax == scanline && cur->next != nullptr && cur->next->x == p->x);

			if (intersects || overlaps) {// 判断当前边是否与扫描线相交
				//p->ymax = scanline; // 更新边的yMax值为交点的纵坐标

				//if (p->ymax >= cur->ymax && p->ymin != cur->ymax) {
				if (p->ymax!=scanline) {
					AEL* tmp = new AEL(p->x, p->dx, p->ymax);
					cur->next = tmp;
					tmp->next = nullptr;
					cur = tmp; // 更新cur指针

					cout << "Added edge to AEL: x = " << p->x << ", dx = " << p->dx << ", ymax = " << p->ymax << std::endl;
				}
				else {
					cout << "Skipped edge: x = " << p->x << ", dx = " << p->dx << ", ymax = " << p->ymax << std::endl;
				}
			}
			else {//不相交跳过
				cout << "Skipped edge: x = " << p->x << ", dx = " << p->dx << ", ymin = " << p->ymin << ", ymax = " << p->ymax << std::endl;
			}
		}
	}
	cout << "完成Finished building AEL" << endl;

	sort_ael(ael);//快排  AEL

	PrintSortAEL(ael);
}
*/

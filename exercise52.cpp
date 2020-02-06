//============================================================================
// Name        : Ex5.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;


class Point2D{
public:
	float x;
	float y;

	Point2D(){
	}

	Point2D(float a,float b){
		x=a;
		y=b;
	}
};

class h_edge;

class vertex {
public:
	Point2D p;
	h_edge* e=nullptr;
};

class face {
public:
	h_edge* outer;
	vector<h_edge*> inner;
};

class h_edge {
public:
	vertex* v=nullptr;
	h_edge* twin=nullptr;
	face* f=nullptr;
	h_edge* next=nullptr;
	h_edge* prev=nullptr;

	h_edge(){

	}
};

class Triangle{
public:
	vector<int> indexList;
	Triangle(int a,int b,int c){
		this->indexList.push_back(a);
		this->indexList.push_back(b);
		this->indexList.push_back(c);

	}
};

int ORI(Point2D p1, Point2D p2,Point2D p3){
	float dx21 = p2.x - p1.x;
	float dy21 = p2.y - p1.y;
	float dx31 = p3.x - p1.x;
	float dy31 = p3.y - p1.y;
	if (dx21*dy31 < dy21*dx31) return -1; // ccw

	if (dx21*dy31 > dy21*dx31) return +1; // cw

	if (dx21*dx31 < 0 || dy21*dy31<0) return -1; // ccw

	if (dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21) return +1; //cw

	return 0; // p3 is inside p1,p2
}

void orderCCW(vector<int> &indexlist, vector<Point2D> &pointList){
	if(ORI(pointList[indexlist[0]],pointList[indexlist[1]],pointList[indexlist[2]])!=-1){
		return;
	}else{
		int saveIndex=indexlist[1];
		int saveIndex2=indexlist[2];
		//indexlist[1]=saveIndex2;
		//indexlist[2]=saveIndex;
	}

}

void GetPointList(vector<Triangle> &triangleList,vector<Point2D> &pointList){
	string str;
	int num1,num2;
	int count=0;
	getline(cin,str);
	istringstream ss(str);
	//Number of points
	ss >> num1 >> num2;
	while(getline(cin,str)){
		count++;
		if(count<=num1){
			istringstream ss(str);
			Point2D point;
			ss >>  point.x >> point.y;
			pointList.push_back(point);
		}else if(count>num1 && count<=(num2+num1)){
			istringstream ss(str);
			int a,b,c;
			ss >> a >> b >> c;
			Triangle triangle(a,b,c);
			orderCCW(triangle.indexList,pointList);
			triangleList.push_back(triangle);
		}
		if(count==(num2+num1)){
			break;
		}

	}
}

///https://stackoverflow.com/questions/39984709/how-can-i-check-wether-a-point-is-inside-the-circumcircle-of-3-points
bool CheckByDeterminandTriangle(vector<Point2D> &pointList,Triangle triangle,vector<vertex*> vertexList){

	if(pointList.size()==0){
		return true;
	}

	for(int k=0;k<triangle.indexList.size();k++){
		cout << "- " << triangle.indexList[k] << " <-> ";

	}
	cout << endl;
	Point2D a = vertexList[triangle.indexList[0]]->p;
	Point2D b = vertexList[triangle.indexList[1]]->p;
	Point2D c = vertexList[triangle.indexList[2]]->p;
	cout << "A:"<< a.x << " - " << a.y << " | ";
	cout << "B:"<< b.x << " - " << b.y << " | ";
	cout << "C:"<< c.x << " - " << c.y << endl;

	for(int ii=0;ii<pointList.size();ii++){

	Point2D d=pointList[ii];
		auto ax_ = a.x-d.x;
		auto ay_ = a.y-d.y;
		auto bx_ = b.x-d.x;
		auto by_ = b.y-d.y;
		auto cx_ = c.x-d.x;
		auto cy_ = c.y-d.y;
		auto num=(
				(ax_*ax_ + ay_*ay_) * (bx_*cy_-cx_*by_) -
				(bx_*bx_ + by_*by_) * (ax_*cy_-cx_*ay_) +
				(cx_*cx_ + cy_*cy_) * (ax_*by_-bx_*ay_)
			);
		cout << "DET:" << num << endl;
		if ((
			(ax_*ax_ + ay_*ay_) * (bx_*cy_-cx_*by_) -
			(bx_*bx_ + by_*by_) * (ax_*cy_-cx_*ay_) +
			(cx_*cx_ + cy_*cy_) * (ax_*by_-bx_*ay_)
		) > 0){
			return false;
		}
	}

	return true;
}


class DCEL{
private:
void printEdgeList(){
	//Print edge and twin list
	for(int i=0;i<h_edgeList.size();i++){
		cout << "h_edge " << i << " | ";
		cout << "Point: " << h_edgeList[i]->v->p.x << " " << h_edgeList[i]->v->p.y << " - "<< h_edgeList[i]->next->v->p.x << " " << h_edgeList[i]->next->v->p.y << " - " << (h_edgeList[i]->f!=nullptr) << " | ";
		cout << " Twin: " << h_edgeList[i]->twin->v->p.x << " " << h_edgeList[i]->twin->v->p.y << " - " << h_edgeList[i]->twin->next->v->p.x << " " << h_edgeList[i]->twin->next->v->p.y   << " - " << (h_edgeList[i]->twin->f!=nullptr) << endl;
	}
}

public:
	vector<vertex*> vertexList;
	vector<h_edge*> h_edgeList;
	vector<face*> faceList;
	void createDCEL(vector<Triangle> &triangleList,vector<Point2D> &pointList){

		for(int i=0;i<pointList.size();i++){
			vertex* newVertex=new vertex;
			newVertex->p=pointList[i];
			vertexList.push_back(newVertex);
		}

		//create elements reading the segments,
		for(int i=0;i<triangleList.size();i++){
			face* new_face= new face;

			h_edge* new_h_edge1=new h_edge;
			new_h_edge1->v=vertexList[triangleList[i].indexList[0]];
			h_edge* new_h_edge_twin1=new h_edge;
			new_h_edge_twin1->v=vertexList[triangleList[i].indexList[1]];

			new_h_edge1->twin=new_h_edge_twin1;
			new_h_edge_twin1->twin=new_h_edge1;

			h_edge* new_h_edge2=new h_edge;
			new_h_edge2->v=vertexList[triangleList[i].indexList[1]];
			h_edge* new_h_edge_twin2=new h_edge;
			new_h_edge_twin2->v=vertexList[triangleList[i].indexList[2]];

			new_h_edge2->twin=new_h_edge_twin2;
			new_h_edge_twin2->twin=new_h_edge2;

			h_edge* new_h_edge3=new h_edge;
			new_h_edge3->v=vertexList[triangleList[i].indexList[2]];
			h_edge* new_h_edge_twin3=new h_edge;
			new_h_edge_twin3->v=vertexList[triangleList[i].indexList[0]];

			new_h_edge3->twin=new_h_edge_twin3;
			new_h_edge_twin3->twin=new_h_edge3;

			new_h_edge1->next=new_h_edge2;
			new_h_edge2->next=new_h_edge3;
			new_h_edge3->next=new_h_edge1;

			new_h_edge3->prev=new_h_edge2;
			new_h_edge2->prev=new_h_edge1;
			new_h_edge1->prev=new_h_edge3;

			new_h_edge1->f=new_face;
			new_h_edge2->f=new_face;
			new_h_edge3->f=new_face;

			new_h_edge_twin1->next=new_h_edge_twin3;
			new_h_edge_twin2->next=new_h_edge_twin1;
			new_h_edge_twin3->next=new_h_edge_twin2;

			new_h_edge_twin1->prev=new_h_edge_twin2;
			new_h_edge_twin2->prev=new_h_edge_twin3;
			new_h_edge_twin3->prev=new_h_edge_twin1;

			new_face->outer=new_h_edge1;


			h_edgeList.push_back(new_h_edge1);
			//h_edgeList.push_back(*new_h_edge_twin1);
			h_edgeList.push_back(new_h_edge2);
			//h_edgeList.push_back(*new_h_edge_twin2);
			h_edgeList.push_back(new_h_edge3);
			//h_edgeList.push_back(*new_h_edge_twin3);

			faceList.push_back(new_face);

		}
		printEdgeList();
		//Check for edges and merge mirrored ones
		for(int i=0;i<h_edgeList.size();i++){

			for(int ii=0;ii<h_edgeList.size();ii++){

				if(i==ii){
					continue;
				}

				//Check same edges and mirror
				if(h_edgeList[i]->v->p.x==h_edgeList[ii]->twin->v->p.x &&
				   h_edgeList[i]->v->p.y==h_edgeList[ii]->twin->v->p.y &&
				   h_edgeList[i]->twin->v->p.x==h_edgeList[ii]->v->p.x &&
				   h_edgeList[i]->twin->v->p.y==h_edgeList[ii]->v->p.y &&
				   h_edgeList[i]->twin->f==nullptr && h_edgeList[ii]->twin->f==nullptr){
					cout << "Merge" << endl;
				   h_edgeList[i]->twin=h_edgeList[ii]->next->prev;
				   h_edgeList[ii]->twin=h_edgeList[i]->next->prev;
				}
			}

		}
		printEdgeList();
	}


	bool Check(vector<Triangle> &triangleList){
		//Will implement a strategy to check vertices adjacent to the previous edge, origin point,

		for(int i=0;i<faceList.size();i++){
			vector<Point2D> pointList;
			cout << (faceList[i]->outer->twin->f != nullptr) << " | ";
			cout << (faceList[i]->outer->next->twin->f != nullptr) << " | ";
			cout << (faceList[i]->outer->next->next->twin->f != nullptr) << endl;

			if(faceList[i]->outer->twin->f!=nullptr){
				pointList.push_back(faceList[i]->outer->twin->prev->v->p);
			}

			if(faceList[i]->outer->next->twin->f!=nullptr){
				pointList.push_back(faceList[i]->outer->next->twin->prev->v->p);
			}

			if(faceList[i]->outer->next->next->twin->f!=nullptr){
				pointList.push_back(faceList[i]->outer->next->next->twin->prev->v->p);
			}

			for(int k=0;k<pointList.size();k++){
				cout <<"Point "<< i << ":" << pointList[k].x<< " - " <<pointList[k].y << endl;
			}

			for(int k=0;k<triangleList[i].indexList.size();k++){
				cout << triangleList[i].indexList[k] << " <-> ";

			}
			cout << endl;

			if(!CheckByDeterminandTriangle(pointList,triangleList[i],vertexList)){

				return false;

			}
		}

		return true;
	}
};



int main() {
	//https://cs.stackexchange.com/questions/2450/how-do-i-construct-a-doubly-connected-edge-list-given-a-set-of-line-segments
	vector<Point2D> pointList;
	vector<Triangle> triangleList;
	GetPointList(triangleList,pointList);
	DCEL dcrl;
	dcrl.createDCEL(triangleList,pointList);

	//bool result=CheckByDeterminand(pointList,triangleList);
	bool result=dcrl.Check(triangleList);
	if(result){
		cout << "YES";
	}else{
		cout << "NO";
	}
	return 0;
}



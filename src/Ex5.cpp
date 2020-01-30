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
		cout << "Change order" << endl;
		int saveIndex=indexlist[1];
		indexlist[1]=indexlist[indexlist[2]];
		indexlist[2]=saveIndex;
	}

}

void GetPointList(vector<Point2D> &pointList,vector<Triangle> &triangleList){
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

float getDistance(Point2D &a,Point2D &b){
	float minx,miny,maxx,maxy;
	if(a.x>b.x){
		maxx=a.x;
		minx=b.x;
	}else{
		maxx=b.x;
		minx=a.x;
	}
	if(a.y>b.y){
		maxy=a.y;
		miny=b.y;
	}else{
		maxy=b.y;
		miny=a.y;
	}

	return sqrt(pow(maxx-minx,2)+pow(maxy+miny,2));

}

///https://stackoverflow.com/questions/39984709/how-can-i-check-wether-a-point-is-inside-the-circumcircle-of-3-points
bool CheckByDeterminand(vector<Point2D> &pointList,vector<Triangle> &triangleList){
	for(int i=0;i<triangleList.size();i++){
		Point2D a = pointList[triangleList[i].indexList[0]];
		Point2D b = pointList[triangleList[i].indexList[1]];
		Point2D c = pointList[triangleList[i].indexList[2]];

		for(int ii=0;ii<pointList.size();ii++){
			if(	triangleList[i].indexList[0]!=ii ||
				triangleList[i].indexList[1]!=ii ||
				triangleList[i].indexList[2]!=ii){

			Point2D d=pointList[ii];
			float ax_ = a.x-d.x;
			float ay_ = a.y-d.y;
			float bx_ = b.x-d.x;
			float by_ = b.y-d.y;
			float cx_ = c.x-d.x;
			float cy_ = c.y-d.y;
			if ((
				(ax_*ax_ + ay_*ay_) * (bx_*cy_-cx_*by_) -
				(bx_*bx_ + by_*by_) * (ax_*cy_-cx_*ay_) +
				(cx_*cx_ + cy_*cy_) * (ax_*by_-bx_*ay_)
			) > 0){
				return false;
			}
			}
		}
	}
	return true;
}

int main() {
	vector<Point2D> pointList;
	vector<Triangle> triangleList;
	GetPointList(pointList,triangleList);
	bool result=CheckByDeterminand(pointList,triangleList);
	if(result){
		cout << "YES";
	}else{
		cout << "NO";
	}
	return 0;
}



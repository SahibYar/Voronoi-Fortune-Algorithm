#include"Utility.h"
#include<vector>
#include<iostream>
#include"VoronoiGraph.h"
using namespace std;

int main()
{
	vector<Point> DataPoints;
	DataPoints.push_back(Point(150, 104));
	DataPoints.push_back(Point(244, 246));
	DataPoints.push_back(Point(494, 196));
	DataPoints.push_back(Point(605, 36));
	DataPoints.push_back(Point(312, 503));
	DataPoints.push_back(Point(31, 596));

	VoronoiGraph Result = ComputeVoronoiGraph(DataPoints);

	cout << "\t\tVertices";
	for (list<Point>::iterator i = Result.Vertices.begin(); i != Result.Vertices.end(); i++)
		cout << "(" << i->data[0] << ", " << i->data[1] << ")\n";
	system("pause");
}
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <GraphicsProc.h>
#include <Polygon.h>
#include <Shape.h>
#include <Line.h>

using namespace std;

float *PixelBuffer;
int bufferSize;
int height;
int width;

void display();
void keyboard(unsigned char key, int x, int y);
void update();
vector<myPolygon*> listOfPolygons;
vector<Shape*> listOfShapes;
Point winMin, winMax;

void main(int argc, char* argv[])
{
	height = 200;
	width = 200;

	cout << "Please set the height of the window: ";
	cin >> height;
	while (cin.fail()) {
		cout << "Error: Not an integer." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please set the height of the window: ";
		cin >> height;
	}
	cout << "Please set the width of the window: ";
	cin >> width;
	while (cin.fail()) {
		cout << "Error: Not an integer." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please set the width of the window: ";
		cin >> width;
	}

	winMin.x = 1; winMin.y = 1;
	winMax.x = width-1; winMax.y = height-1;

	//Need to multiply height and width by three because each point is represented by a red green and blue value
	PixelBuffer = new float[height*width * 3];
	bufferSize = height*width * 3 * sizeof(float);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);

	int MainWindow = glutCreateWindow("Project 1");
	glClearColor(0, 0, 0, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return;
}


void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	string command;
	if (key == 27/*ESC*/) {
		while (command != "continue") {
			cout << "Please enter a command: ";
			cin >> command;
			cout << "You entered: " << command << endl;
			
			if(command == "continue"){
				cout << "Continuing Program" << endl;
				break;
			}
			if (command == "dda") {
				int x0, y0, xEnd, yEnd;
				cout << "Please specify starting coordinates (formated as X Y no parenthesis): ";
				cin >> x0;
				cin >> y0;
				cout << "You specified " << x0 << " and " << y0 << endl;
				cout << "Please specify ending coordinates (formated as X Y no parenthesis): ";
				cin >> xEnd;
				cin >> yEnd;
				cout << "You specified " << xEnd << " and " << yEnd << endl;
				if (!DDADraw(x0, y0, xEnd, yEnd, width, height, PixelBuffer))
					cout << "Invalid Coordinates" << endl;
				else
				{
					Line *l = new Line(x0, y0, xEnd, yEnd, width, height, PixelBuffer, 0);
					listOfShapes.push_back(l);
				}
				break;
			}	
			if (command == "bresenham") {
				int x0, y0, xEnd, yEnd;
				cout << "Please specify starting coordinates (formated as X Y no parenthesis): ";
				cin >> x0;
				cin >> y0;
				cout << "You specified " << x0 << " and " << y0 << endl;
				cout << "Please specify ending coordinates (formated as X Y no parenthesis): ";
				cin >> xEnd;
				cin >> yEnd;
				cout << "You specified " << xEnd << " and " << yEnd << endl;
				if (!BresenhamDraw(x0, y0, xEnd, yEnd, width, height, PixelBuffer))
					cout << "Invalid Coordinates" << endl;
				else
				{
					Line *l = new Line(x0, y0, xEnd, yEnd, width, height, PixelBuffer, 1);
					listOfShapes.push_back(l);
				}
				break;
			}
			if (command == "polygon") {
				int numofpoints;
				Point p;
				vector<Point> listofPoints;
				cout << "Please specify number of points: ";
				cin >> numofpoints;
				for (int i = 0; i < numofpoints; i++) {
					cout << "Please specify coordinates of point " << i << " (formated as X Y no parenthesis): ";
					cin >> p.x;
					cin >> p.y;
					listofPoints.push_back(p);
				}
				myPolygon* poly = new myPolygon(listofPoints,PixelBuffer,width,height);
				poly->draw();
				listOfPolygons.push_back(poly);
				listOfShapes.push_back(poly);
				break;
			}
			if (command == "translate")
			{
				int maxPoly = listOfPolygons.size();
				int pId = 100;
				float tx, ty;
				cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
				cin >> pId;
				while (pId >= maxPoly || pId < 0)
				{
					cout << "Incorrect ID" << endl;
					cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
					cin >> pId;
				}
				cout << "Please enter TX and TY (formated as TX TY no parenthesis): ";
				cin >> tx;
				cin >> ty;
				listOfPolygons[pId]->translate(tx, ty);
				update();
				break;
			}
			if (command == "rotate")
			{
				int maxPoly = listOfPolygons.size();
				int pId = 100;
				double theta;
				cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
				cin >> pId;
				while (pId >= maxPoly || pId < 0)
				{
					cout << "Incorrect ID" << endl;
					cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
					cin >> pId;
				}
				cout << "Please enter theta: ";
				cin >> theta;
				listOfPolygons[pId]->rotate(theta);
				update();
				break;
			}
			if (command == "scale")
			{
				int maxPoly = listOfPolygons.size();
				int pId = 100;
				float sx, sy;
				cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
				cin >> pId;
				while (pId >= maxPoly || pId < 0)
				{
					cout << "Incorrect ID" << endl;
					cout << "Please specify polygon ID (range 0 - " << maxPoly - 1 << "): ";
					cin >> pId;
				}
				cout << "Please enter SX and SY (formated as SX SY no parenthesis): ";
				cin >> sx;
				cin >> sy;
				listOfPolygons[pId]->scale(sx, sy);
				update();
				break;
			}
			if (command == "clip")
			{
				cout << "Please specify winMin (formated as X Y no parenthesis): ";
				cin >> winMin.x;
				cin >> winMin.y;
				cout << "Please specify winMax (formated as X Y no parenthesis): ";
				cin >> winMax.x;
				cin >> winMax.y;
				update();
				break;
			}
			else {
				cout << "Invalid Command: " << command << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}

	display();
}

void update()
{
	memset(PixelBuffer, 0, bufferSize);
	for (int k = 0; k < listOfShapes.size(); k++)
	{
		listOfShapes[k]->clip(winMin, winMax);
		listOfShapes[k]->draw();
	}
}
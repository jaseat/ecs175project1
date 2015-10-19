#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include<GraphicsProc.h>
class Shape
{
public:
	virtual void draw(){}
	virtual void clip(Point winMin, Point winMax){}
};

#endif
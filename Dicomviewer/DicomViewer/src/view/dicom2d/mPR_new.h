#pragma once
#ifndef MPR_H
#define MPR_H
#include <QWidget>
#include "vtkResliceCursorCallback.h"


class MPR :public QWidget
{
	Q_OBJECT
public:
	MPR(QWidget* parent = 0);
	~MPR();
public:
	void MPRRendering();
};


#endif
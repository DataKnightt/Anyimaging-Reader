#pragma once
#ifndef MPR_lmy_H
#define MPR_lmy_H
#include <QWidget>
#include "vtkResliceCursorCallback.h"


class MPR_lmy :public QWidget
{
	Q_OBJECT
public:
	MPR_lmy(QWidget* parent = 0);
	~MPR_lmy();
public:
	void MPRRendering();
};


#endif
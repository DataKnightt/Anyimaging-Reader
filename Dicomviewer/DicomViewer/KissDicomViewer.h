#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_KissDicomViewer.h"

class KissDicomViewer : public QMainWindow
{
    Q_OBJECT

public:
    KissDicomViewer(QWidget *parent = Q_NULLPTR);

private:
    Ui::KissDicomViewerClass ui;
};

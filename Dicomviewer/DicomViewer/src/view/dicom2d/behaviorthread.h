#pragma once
#include <qthread.h>
#include <time.h>
#include "viewcontainerwidget.h"


class SeriesInstance;
class behaviorthread :public QThread
{
    Q_OBJECT
public:
    explicit behaviorthread(QObject* parent = nullptr);
    ~behaviorthread()
    {
        stop();
    }
    int** record;
    SeriesInstance* cu_series;
    int monitor_var_keep;
    void stop()
    {
        requestInterruption();
        quit();
        wait();
    }
 
    void Edge();
    void Emhance();
    void Blur();
    void Open();
    void Close();
    void Light();
    void HFlip();
    void VFlip();// 垂直翻转
    void HMirror();   //水平镜像
    void VMirror();
    void HVMirror();
    void RoateCCW(); // 逆时针90
    void RoateCW(); // 顺时针90

    void DrawLine();
    void DrawAngle();
    void DrawCobbAngle();
    void DrawRect();
    void DrawEllipse();
    void DrawTextMark();
    void DrawArrow();
    void Magnifier();
    
private:
    atomic_bool stopFlag;
    QWaitCondition condition;
    void run();
};


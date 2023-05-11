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
    void VFlip();// ��ֱ��ת
    void HMirror();   //ˮƽ����
    void VMirror();
    void HVMirror();
    void RoateCCW(); // ��ʱ��90
    void RoateCW(); // ˳ʱ��90

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


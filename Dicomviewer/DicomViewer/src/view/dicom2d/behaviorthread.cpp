
///****************************************************************************
/// @author  : 程嵩森                                                        
/// @date    : 2022.5.1                                                               
/// @file    :                                                                 
/// @brief   : 用户行为检测线程                                                                
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

#include "behaviorthread.h"
#include <iostream>
#include "String"
behaviorthread::behaviorthread(QObject* parent)
    : QThread(parent)   
{
   
}


void behaviorthread::run()
{
    monitor_var_keep = 0;
    int count = cu_series->GetFrameCount();
    record = new int*[count];
    for (int i = 0; i < count; ++i) {
        record[i] = new int[22]();
    }
    qint32 monitor;
    int i=0;
    int start, finish;
    int total_time=0;

    while (!isInterruptionRequested())
    {
       
            start = time(NULL);//记录本次监测变量开始的时间
            while (!isInterruptionRequested())
            {
        
                monitor = cu_series->GetCurIndex(VT_XYPlane);
                if ((monitor != monitor_var_keep)&&monitor<count&&monitor>=0)
                {
                    finish = time(NULL);//记录本次监测变量结束时间
                    total_time += (int)(finish - start);//记录监测变量总时间
                   /* printf("%d picture time is %d second\n", monitor_var_keep, total_time);*/
                    record[monitor_var_keep][0] += total_time;
                    total_time = 0;
                    monitor_var_keep = monitor;
                    break;
                }
            }
        
    }
    
    ofstream csvfile;
    QString series_uid_ = cu_series->series_uid_;
    QString dst = QString("./%1.csv").arg(series_uid_);
    csvfile.open(dst.toStdString(), ios::out);
    csvfile << "index,"<<"time,"<<"Edge,"<<"Emhance," << "Blur," << "Open," << "Close," 
        << "Light," <<"HFlip,"<< "VFlip," << "HMirror," << "VMirror," << "HVMirror," << "RoateCCW," << "RoateCW,"
        << "DrawLine," << "DrawAngle," << "DrawCobbAngle," << "DrawRect," << "DrawEllipse," << "DrawTextMark," 
        << "DrawArrow," << "Magnifier"
        << "\n";
    for (int i = 0; i < count; i++)
    {
        csvfile << i << ",";
        for (int j = 0; j <22; j++)
        {
            csvfile<< record[i][j] <<",";           
        }
        csvfile << "\n";
    }
    csvfile.close();

    //释放空间
    for (int i = 0; i < count; ++i)
        delete[]record[i];
    delete[]record;
    printf("stop");
}

void behaviorthread::Edge()
{

     record[monitor_var_keep][1]+=1;
}

void behaviorthread::Emhance()
{
    record[monitor_var_keep][2] += 1;
}

void behaviorthread::Blur()
{
    record[monitor_var_keep][3] += 1;
}

void behaviorthread::Open()
{
    record[monitor_var_keep][4] += 1;
}

void behaviorthread::Close()
{
    record[monitor_var_keep][5] += 1;
}

void behaviorthread::Light()
{
    record[monitor_var_keep][6] += 1;
}

void behaviorthread::HFlip()
{
    record[monitor_var_keep][7] += 1;
}

void behaviorthread::VFlip()
{
    record[monitor_var_keep][8] += 1;
}


void behaviorthread::HMirror()
{
    record[monitor_var_keep][9] += 1;
}

void behaviorthread::VMirror()
{
    record[monitor_var_keep][10] += 1;
}

void behaviorthread::HVMirror()
{
    record[monitor_var_keep][11] += 1;
}

void behaviorthread::RoateCCW()
{
    record[monitor_var_keep][12] += 1;
}

void behaviorthread::RoateCW()
{
    record[monitor_var_keep][13] += 1;
}

void behaviorthread::DrawLine()
{
    record[monitor_var_keep][14] += 1;
}

void behaviorthread::DrawAngle()
{
    record[monitor_var_keep][15] += 1;
}

void behaviorthread::DrawCobbAngle()
{
    record[monitor_var_keep][16] += 1;
}

void behaviorthread::DrawRect()
{
    record[monitor_var_keep][17] += 1;
}

void behaviorthread::DrawEllipse()
{
    record[monitor_var_keep][18] += 1;
}

void behaviorthread::DrawTextMark()
{
    record[monitor_var_keep][19] += 1;
}

void behaviorthread::DrawArrow()
{
    record[monitor_var_keep][20] += 1;
}

void behaviorthread::Magnifier()
{
    record[monitor_var_keep][21] += 1;
}

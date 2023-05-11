#include <window/dicomviewer.h>
#include <QtWidgets/QApplication>
#include "global/KissDicomViewConfig.h"
#include <window/KissWindow>
#include "global/global.h"
#include "global/stable.h"

///****************************************************************************
/// @author  : ³ÌáÔÉ­¡¢ËÕÒ»ÖÛ¡¢ÀîÃÏæ«¡¢ºîè÷Ñô                                                        
/// @date    :  2022.3.1                                                               
/// @file    :                                                                 
/// @brief   :  Ö÷º¯Êý                                                               
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("ZZUCSweb");
    a.setOrganizationDomain("www.ih.ha.edu.cn");
    a.setApplicationName("·ºÓ°ÔÄÆ¬");
    using namespace Kiss;
    QUIHelper::SetStyle(":/Style/style.qss");

    InitialDir();
    DicomViewer w(DicomViewer::SingleInstance);
    w.setWindowTitle(Project_NAME);
    w.setWindowIcon(QIcon(":/Multi.ico"));
    w.showMaximized();
    return a.exec();
}

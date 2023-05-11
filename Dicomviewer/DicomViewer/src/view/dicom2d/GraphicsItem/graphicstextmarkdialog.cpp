
///****************************************************************************
/// @author  : 苏一舟                                                         
/// @date    : 2022.4.1                                                                
/// @file    :                                                                 
/// @brief   : 输入文字标注窗口                                                                
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

#include "graphicstextmarkdialog.h"
#include "ui_graphicstextmarkdialog.h"

//----------------------------------------------------------------
GraphicsTextMarkDialog::GraphicsTextMarkDialog(QWidget * parent)
  : QDialog(parent)
  , ui(new Ui::GraphicsTextMarkDialog)
{
    ui->setupUi(this);
}

//----------------------------------------------------------------
GraphicsTextMarkDialog::~GraphicsTextMarkDialog()
{
    delete ui;
}

//----------------------------------------------------------------
QString GraphicsTextMarkDialog::getText()
{
    return ui->textEdit->text();
}

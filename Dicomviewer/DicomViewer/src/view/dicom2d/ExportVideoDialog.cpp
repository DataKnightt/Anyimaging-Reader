#include "ExportVideoDialog.h"
#include "global/stable.h"
#include "global/stdafx.h"
#include <script/KissScript>
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#define EXPORT_VIDEO_FOLDER "EXPORTVIDEOFOLDER"
using namespace cv;

///****************************************************************************
/// @author  : ����ɭ                                                        
/// @date    :  2022.4.1                                                               
/// @file    :                                                                 
/// @brief   :  ������Ƶ�Ի�����                                                               
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

ExportVideoDialog::ExportVideoDialog(QWidget *parent)
	: QDialog(parent)
    , ui(new Ui::ExportVideoDialog)
    , export_thread_(new ExportImageThread(this))
{
	ui->setupUi(this);
    Initialization();

}
QTemporaryDir *dir;
ExportVideoDialog::~ExportVideoDialog()
{
    delete ui;
    delete dir;
}


///****************************************************************************                 
/// @brief   :       ExportVideoDialog::Slot_SelectFolder
/// ѡ��Ŀ���ļ���                                                          
///****************************************************************************

void ExportVideoDialog::Slot_SelectFolder()
{
    QSettings settings;
    folder = settings.value(EXPORT_VIDEO_FOLDER, ".").toString();
    folder = QFileDialog::getExistingDirectory(this, tr("Ŀ���ļ���"), folder);
    if (!folder.isEmpty()) {
        settings.setValue(EXPORT_VIDEO_FOLDER, folder);
        ui->exportDestEdit->setText(folder);
    }
}

void ExportVideoDialog::Slot_Start(bool checked)
{
    dir = new QTemporaryDir();
    if (checked) {
        if (ui->exportDestEdit->text().isEmpty()) {
            Slot_SelectFolder();
            return;
        }      
        if (dir->isValid()) {
            //qDebug() << "temp dir = " << dir->path();
            png_path= dir->path();
            export_thread_->SetFileType(ExportImageThread::EFT_PNG);
            export_thread_->SetDestDir(png_path);
        }     
        ui->selectFolderButton->setDisabled(true);
        ui->closeBtn->setDisabled(true);
        ui->startBtn->setText(tr("��ֹ"));  
        ui->progressBar->setMaximum(image_count_);
        ui->progressBar->setValue(0);
        export_thread_->start();
    }

}

///****************************************************************************                 
/// @brief   :     ExportVideoDialog::Image_To_Video
/// ͼƬת��Ƶ                                                             
///****************************************************************************

void ExportVideoDialog::Image_To_Video(QString pngpath)
{
    QString image_name;
    VideoWriter writer;
    int frame_fps = 25;
    String exportpath = ui->exportDestEdit->text().toStdString() + "dicom.mp4";
    writer = VideoWriter(exportpath, VideoWriter::fourcc('m', 'p', '4', 'v'),
        frame_fps, Size(frame_width, frame_height), true);
    Mat img;
    for (int i=1; i <=num; i++)
    {  
        image_name= pngpath+ QDir::separator()+ QString("%1.png").arg(i);
        image_name.replace("/", QDir::separator()).replace("\\", QDir::separator());
        img = imread(image_name.toStdString());// imread picture
        if (img.empty())
        {
            break;
        }
        writer << img;
        Slot_VideoResultReady();
        if (waitKey(30) == 27 || i == num)
        {
            QMessageBox::information(nullptr, QString("��ʾ"), "�����ɹ�", QMessageBox::Ok);
            dir->remove();
            break;
        }

    }
    
}



///****************************************************************************                 
/// @brief   :     ExportVideoDialog::Slot_ExportFinished
///    ���������ۺ���                                           
///****************************************************************************

void ExportVideoDialog::Slot_ExportFinished()
{
    Image_To_Video(png_path);
    ui->selectFolderButton->setDisabled(false);
    ui->startBtn->setText(tr("��ʼ"));
    ui->startBtn->setChecked(false);
    ui->closeBtn->setDisabled(false);
}
//-------------------------------------------------------


void ExportVideoDialog::SetMultiplePng(const QStringList& images)
{
    export_thread_->SetMultiplePng(images);
    image_count_ = images.size()*2;
}

//-------------------------------------------------------
void ExportVideoDialog::Slot_VideoResultReady()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

///****************************************************************************                 
/// @brief   :  ExportVideoDialog::Initialization
/// ��ʼ���źŲۺ�������                                                               
///****************************************************************************

void ExportVideoDialog::Initialization()
{


    connect(ui->selectFolderButton, &QPushButton::clicked,
        this, &ExportVideoDialog::Slot_SelectFolder);
    connect(ui->startBtn, &QPushButton::clicked,
        this, &ExportVideoDialog::Slot_Start);
    connect(export_thread_, &ExportImageThread::finished,
        this, &ExportVideoDialog::Slot_ExportFinished);
    connect(export_thread_, &ExportImageThread::resultReady,
        this, &ExportVideoDialog::Slot_VideoResultReady);
    //
    ui->exportDestEdit->setText(QSettings().value(EXPORT_VIDEO_FOLDER).toString());
}

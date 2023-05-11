﻿#include "thumbnailbarwidget.h"
#include "dicomimagelabel.h"
#include "global/global.h"
#include "global/stable.h"
#include <script/KissScript>

#include "ImageData/imageinstance.h"
#include "ImageData/seriesinstance.h"
#include "JlCompress.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "filewatcherthread.h"
#include <QTimer>

///****************************************************************************
/// @author  : 程嵩森                                                        
/// @date    :      2022.4.1                                                           
/// @file    :                                                                 
/// @brief   :  文件列表                                                            
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//-------------------------------------------------------
ThumbnailBarWidget::ThumbnailBarWidget(QWidget * parent)
  : QWidget(parent)
  , currentImageLabel(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->addStretch();
    layout->setAlignment(Qt::AlignCenter);
}

//-------------------------------------------------------
ThumbnailBarWidget::~ThumbnailBarWidget()
{
    clear();
    emit SgnQuitFileWatcher();
}

//-------------------------------------------------------
void ThumbnailBarWidget::setCurrentImageLabel(const SeriesInstance * series)
{
    if (currentImageLabel && currentImageLabel->getSeriesInstance() == series) {
        currentImageLabel->setHighlight(true);
    } else {
        bool found = false;
        foreach (DicomImageLabel * label, imageLabelList) {
            if (series == label->getSeriesInstance()) {
                if (currentImageLabel) {
                    currentImageLabel->setHighlight(false);
                }
                currentImageLabel = label;
                currentImageLabel->setHighlight(true);
                found = true;
                break;
            }
        }
        if ((!found) && currentImageLabel) {
            currentImageLabel->setHighlight(false);
            currentImageLabel = nullptr;
        }
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::firstSeries()
{
    if (!imageLabelList.isEmpty()) {
        Slot_ImageDoubleClicked(imageLabelList.first());
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::lastSeries()
{
    if (!imageLabelList.isEmpty()) {
        if ((!currentImageLabel) || (currentImageLabel && currentImageLabel != imageLabelList.last())) {
            Slot_ImageDoubleClicked(imageLabelList.last());
        }
    }
}

//---------------------上一个----------------------------------
void ThumbnailBarWidget::prevSeries()
{
    if (currentImageLabel && (imageLabelList.size())) {
        int i = imageLabelList.indexOf(currentImageLabel);
        if (i > 0) {
            Slot_ImageDoubleClicked(imageLabelList.at(i - 1));
        }
    }
}

//--------------------下一个-----------------------------------
void ThumbnailBarWidget::nextSeries()
{
    if (currentImageLabel && (imageLabelList.size())) {
        int i = imageLabelList.indexOf(currentImageLabel);
        if (i < imageLabelList.size() - 1) {
            Slot_ImageDoubleClicked(imageLabelList.at(i + 1));
        }
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::currSeries()
{
    if (currentImageLabel) {
        emit SgnImageDoubleClicked(currentImageLabel->getSeriesInstance());
    }
}

void ThumbnailBarWidget::UpdataLabeSize()
{
    foreach (DicomImageLabel * label, imageLabelList) {
        label->setFixedSize(label->sizeHint());
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::setFileWatcher(const QString & dir)
{
    emit SgnQuitFileWatcher();
    FileWatcherThread * t = new FileWatcherThread(dir);
    connect(this, &ThumbnailBarWidget::SgnQuitFileWatcher,
            t, &FileWatcherThread::quit);
    connect(t, &FileWatcherThread::finished,
            t, &FileWatcherThread::deleteLater);
    connect(t, &FileWatcherThread::SgnFilesChanged,
            this, &ThumbnailBarWidget::Slot_FilesChanged);
    t->start();
}

//-------------------------------------------------------
void ThumbnailBarWidget::updateLabelImage(const SeriesInstance * series)
{
    if (currentImageLabel && currentImageLabel->getSeriesInstance() == series) {
        currentImageLabel->updateThumbnailImage();
        return;
    }
    foreach (DicomImageLabel * label, imageLabelList) {
        if (label->getSeriesInstance() == series) {
            label->updateThumbnailImage();
            break;
        }
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::setImagePaths(const QStringList & paths)
{
    appendImagePaths(paths, true);
}

//-------------------添加文件路径---------------------------------
void ThumbnailBarWidget::appendImagePaths(
  const QStringList & paths, bool clear_old)
{
    emit SgnImageLoadBegin();
    //判断是否是压缩包
    if (1 == paths.size() && paths.first().right(4) == ".zip") {
        QPointer<UnzipDicomFile> script_recoery_;
        script_recoery_ = new UnzipDicomFile();
        connect(script_recoery_, &UnzipDicomFile::finished,
                this, [&] {
                    appendImagePaths(QStringList() << "./ZipCache", true);
                });
        connect(script_recoery_, &UnzipDicomFile::finished,
                script_recoery_, &UnzipDicomFile::deleteLater);
        script_recoery_->SetPath(paths.first());
        script_recoery_->start();
        return;
    }
    QStringList path_list = paths;
    QStringList files;
    //未加载文件
    QStringList unloaded_files;
    while (!path_list.isEmpty()) {
        QString p = path_list.takeFirst();
        QDir dir(p);
        //准确判断文件夹是否存在
        if (dir.exists()) {
            QStringList subs = dir.entryList(QDir::Files);//遍历目录
            foreach (const QString & s, subs) {
                files += p + QDir::separator() + s;
            }
            subs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach (const QString & s, subs) {
                path_list += p + QDir::separator() + s;
            }
        } else {
            files += p;
        }
    }
    if (clear_old) {
        unloaded_files = files;
     /*   clear();*/
    } else {
        foreach (const QString & file, files) {
            bool found = false;
            foreach (DicomImageLabel * label, imageLabelList) {
                if (label->HasImage(file)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                unloaded_files << file;
            }
        }
    }
    //文件数量过多
    if (unloaded_files.size() > 3000) {
        qDebug() << unloaded_files.size()
                 << "files. That's too much";
        emit SgnImageLoadFinished();
        return;
    }
    OFLog::configure(OFLogger::WARN_LOG_LEVEL);
    foreach (const QString & p, unloaded_files) {
        //多线程处理
        QApplication::processEvents();
        Slot_ImagePathReady(p);
    }
    OFLog::configure(OFLogger::INFO_LOG_LEVEL);
    Kiss::FileUtil::DirRemove("./ZipCache");

    emit SgnImageLoadFinished();
}

//-------------------------------------------------------
//void ThumbnailBarWidget::Slot_ImageReady(ImageInstance * image)
//{
//    bool inserted = false;
//    foreach (DicomImageLabel * label, imageLabelList) {
//        if (label->insertImage(image)) {
//            inserted = true;
//            emit SgnSeriesAppend();
//            break;
//        }
//    }
//    if ((!inserted) && image) {
//        DicomImageLabel * imageLabel =
//          new DicomImageLabel(new SeriesInstance(image->GetSeriesUid()));
//        if (imageLabel->insertImage(image)) {
//            connect(imageLabel, &DicomImageLabel::SgnImageClicked,
//                    this, &ThumbnailBarWidget::SLot_ImageClicked);
//            connect(imageLabel, &DicomImageLabel::SgnImageDoubleClicked,
//                    this, &ThumbnailBarWidget::Slot_ImageDoubleClicked);
//            layout->insertWidget(imageLabelList.size(), imageLabel);
//            imageLabelList.append(imageLabel);
//            if (!currentImageLabel) {
//                currentImageLabel = imageLabel;
//            }
//            //DicomImageview  SetSeriesInstance()
//            emit SgnSeriesInserted(imageLabel->getSeriesInstance());
//        } else {
//            delete imageLabel;
//        }
//    }
//}

void ThumbnailBarWidget::Slot_ImagePathReady(const QString path)
{
    ImageInstance * image = new ImageInstance(path);
    bool inserted = false;
    foreach (DicomImageLabel * label, imageLabelList) {
        if (label->insertImage(image)) {
            inserted = true;
            emit SgnSeriesAppend();
            break;
        }
    }
    if ((!inserted) && image) {
        DicomImageLabel * imageLabel =
          new DicomImageLabel(
            new SeriesInstance(image->GetSeriesUid()));
        if (imageLabel->insertImage(image)) {
            connect(imageLabel, &DicomImageLabel::SgnImageClicked,
                    this, &ThumbnailBarWidget::SLot_ImageClicked);
            connect(imageLabel, &DicomImageLabel::SgnImageDoubleClicked,
                    this, &ThumbnailBarWidget::Slot_ImageDoubleClicked);
            layout->insertWidget(imageLabelList.size(), imageLabel);
            imageLabelList.append(imageLabel);
            if (!currentImageLabel) {
                currentImageLabel = imageLabel;
            }
            //发射信号 viewContainer插入序列
            //DicomImageview  SetSeriesInstance()
            emit SgnSeriesInserted(imageLabel->getSeriesInstance());
        } else {
            delete imageLabel;
        }
        Slot_ImageDoubleClicked(imageLabel);
    }
    QApplication::processEvents();
}

//-------------------------------------------------------
void ThumbnailBarWidget::Slot_FilesChanged(
  const QStringList & removed,
  const QStringList & added)
{
    foreach (const QString & f, removed) {
        foreach (DicomImageLabel * l, imageLabelList) {
            if (l->removeImage(f) && l->getSeriesInstance()->IsEmpty()) {
                layout->removeWidget(l);
                imageLabelList.removeOne(l);
                if (currentImageLabel == l) {
                    currentImageLabel = nullptr;
                }
                l->deleteLater();
                break;
            }
        }
    }
    if ((!currentImageLabel) && (!imageLabelList.isEmpty())) {
        SLot_ImageClicked(imageLabelList.first());
    }
    appendImagePaths(added);
}

//---------------------单击----------------------------------
void ThumbnailBarWidget::SLot_ImageClicked(
  DicomImageLabel * imageLabel)
{
    if (currentImageLabel != imageLabel) {
        if (currentImageLabel) {
            currentImageLabel->select_ = false;
            currentImageLabel->setHighlight(false);
        }
        currentImageLabel = imageLabel;
        if (currentImageLabel) {
            currentImageLabel->select_ = true;
            currentImageLabel->setHighlight(true);
        }
    }
}

//-------------------双击------------------------------------
void ThumbnailBarWidget::Slot_ImageDoubleClicked(
  DicomImageLabel * imageLabel)
{
    if (currentImageLabel) {
        currentImageLabel->select_ = false;
        currentImageLabel->setHighlight(false);
    }
    currentImageLabel = imageLabel;
    if (currentImageLabel) {
        currentImageLabel->select_ = true;
        currentImageLabel->setHighlight(true);
        emit SgnImageDoubleClicked(currentImageLabel->getSeriesInstance());
    }
}

//-------------------------------------------------------
QList<SeriesInstance *> ThumbnailBarWidget::getSeriesList() const
{
    QList<SeriesInstance *> seriesList;
    foreach (DicomImageLabel * label, imageLabelList) {
        seriesList << label->getSeriesInstance();
    }
    return seriesList;
}

//-------------------------------------------------------
QSize ThumbnailBarWidget::sizeHint() const
{
    QMargins margin = layout->contentsMargins();
    int width = DicomImageLabel::getImageLabelSize();
    int height = DicomImageLabel::getImageLabelSize() * imageLabelList.size();
    if (imageLabelList.size() > 1) {
        height += (imageLabelList.size() - 1) * layout->spacing();
    }
    switch (layout->direction()) {
    case QBoxLayout::TopToBottom:
    case QBoxLayout::BottomToTop:
        return QSize(width + margin.left() + margin.right(),
                     height + margin.top() + margin.bottom());
    case QBoxLayout::LeftToRight:
    case QBoxLayout::RightToLeft:
        return QSize(width + margin.top() + margin.bottom(),
                     height + margin.left() + margin.right());
        /*default:
                return QSize();*/
    }
}

//-------------------------------------------------------
void ThumbnailBarWidget::clear()
{
    qDeleteAll(imageLabelList);
    imageLabelList.clear();
    currentImageLabel = nullptr;

}

//-------------------------------------------------------
void UnzipDicomFile::run()
{
    Kiss::FileUtil::DirRemove("./ZipCache");
    Kiss::FileUtil::DirMake("./ZipCache/");
    JlCompress::extractDir(this->path_, "./ZipCache/");
}

//-------------------------------------------------------
void UnzipDicomFile::SetPath(
  const QString & path)
{
    this->path_ = path;
}

#include "dicomimagelabel.h"
#include "ImageData/seriesinstance.h"
#include "global/stable.h"

#include "dcmtk/dcmdata/dcdeftag.h"

qint32 DicomImageLabel::image_label_size_ = 120;


///****************************************************************************
/// @author  : 程嵩森                                                        
/// @date    : 2022.3.4                                                                
/// @file    :                                                                 
/// @brief   : 已打开的 dicom 文件标签                                                                
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//-------------------------------------------------------
DicomImageLabel::DicomImageLabel(SeriesInstance * seriesPtr, QWidget * parent)
  : QLabel(parent)
{
    m_series_ = seriesPtr;
    name_label_ = new QLabel(this);
    frame_label_ = new QLabel(this);
    setStyleSheet(QString::fromLatin1(""
                                      "background-color: rgb(0, 0, 0);"
                                      "font-size:12px;"
                                      "color:rgba(160,174,184,1);"));
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);
    setLineWidth(2);
    setAlignment(Qt::AlignCenter);
    setScaledContents(true);
    name_label_->setFixedSize(110, 13);
}

//-------------------------------------------------------
DicomImageLabel::~DicomImageLabel()
{
    delete m_series_;
}

//---------------判断是否有文件-----------------------------------
bool DicomImageLabel::HasImage(const QString & file) const
{
    return m_series_ && m_series_->HasImage(file);
}

//-----------------------插入文件--------------------------
bool DicomImageLabel::insertImage(ImageInstance * image)
{
    if (m_series_ && m_series_->InsertImage(image)) {
        if (m_series_->GetFrameCount(VT_XYPlane) > 0) {
            QPixmap pixmap;
            if (m_series_->GetPixmap(pixmap, VT_XYPlane)) {
                setPixmap(pixmap.scaled(
                  image_label_size_ - 2, image_label_size_ - 2,
                  Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            name_label_->setText(m_series_->GetTagKeyValue(DCM_PatientName));
#if HideNmae == 1
            name_label_->setText("hide name");
#endif
            name_label_->move(4, 4);
        }
        QString tag = QString("%1: %2-%3")
                        .arg(m_series_->GetTagKeyValue(DCM_Modality),
                             m_series_->GetTagKeyValue(DCM_SeriesNumber),
                             QString::number(m_series_->GetFrameCount(VT_XYPlane)));
        frame_label_->setText(tag);
        QSize tagSize = frame_label_->sizeHint();
        frame_label_->resize(tagSize);
        frame_label_->move(image_label_size_ - tagSize.width(),
                           image_label_size_ - tagSize.height());
        return true;
    }
    return false;
}

//---------------------------移除文件------------------------
bool DicomImageLabel::removeImage(const QString & imgFile)
{
    return m_series_ && m_series_->RemoveImage(imgFile);
}

//-----------------------------设置高亮----------------------
void DicomImageLabel::setHighlight(bool yes)
{
    QPalette p = palette();
    if (yes) {
        p.setColor(QPalette::Window, Qt::green);
    } else {
        p.setColor(QPalette::Window, Qt::black);
    }
    setPalette(p);
}

//-----------------更新-----------------------------------
void DicomImageLabel::updateThumbnailImage()
{
    if (m_series_) {
        QPixmap pixmap;
        if (m_series_->GetPixmap(pixmap, VT_XYPlane)) {
            setPixmap(pixmap.scaled(image_label_size_ - 2,
                                    image_label_size_ - 2,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
        }
    }
}

void DicomImageLabel::paintEvent(QPaintEvent * e)
{
    QLabel::paintEvent(e);
    if (select_) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.save();
        painter.setPen(QPen(QColor(32, 218, 208), 4));
        painter.drawRect(rect());
        painter.restore();
    }
}

//--------------------鼠标拖动-----------------------------------
void DicomImageLabel::mousePressEvent(QMouseEvent * e)
{
    emit SgnImageClicked(this);
    drag_org_ = e->pos();
    QLabel::mousePressEvent(e);
}

//------------------------鼠标移动-------------------------------
void DicomImageLabel::mouseMoveEvent(QMouseEvent * e)
{
    if ((e->buttons() & Qt::LeftButton) && ((e->pos() - drag_org_).manhattanLength() > QApplication::startDragDistance())) {
        QDrag * drag = new QDrag(this);
        QMimeData * data = new QMimeData;
        data->setText(QString::number((qulonglong)m_series_));
        drag->setMimeData(data);
        drag->exec(Qt::CopyAction);
    }
    QLabel::mouseMoveEvent(e);
}

qint32 DicomImageLabel::getImage_label_size()
{
    return image_label_size_;
}

void DicomImageLabel::setImage_label_size(const qint32 & value)
{
    image_label_size_ = value;
}

//-------------------双击------------------------------------
void DicomImageLabel::mouseDoubleClickEvent(QMouseEvent * e)
{
    emit SgnImageDoubleClicked(this);
    QLabel::mouseDoubleClickEvent(e);
}

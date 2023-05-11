#include "viewcontainerwidget.h"
#include "dicomimageview.h"
#include "exportimagedialog.h"
#include "ExportVideoDialog.h"

#include "global/KissDicomViewConfig.h"

#include "ImageData/imageinstance.h"
#include "ImageData/seriesinstance.h"
#include "behaviorthread.h"
#include "dcmtk/dcmdata/dcdeftag.h"


///****************************************************************************
/// @author  : 程嵩森  李孟娅 苏一舟                                                      
/// @date    :   2022.3.5                                                              
/// @file    :                                                                 
/// @brief   :   dicomview的容器                                                              
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//-------------------------------------------------------
ViewContainerWidget::ViewContainerWidget(QWidget * parent)
  : QWidget(parent)
  , behavior_thread(new behaviorthread(this))
{
    layout_ = new QGridLayout(this);
    current_view_ = nullptr;
    filter_ = nullptr;
    maxed_ = false;
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setSpacing(0);
    QFont f;
    if (f.fromString(QSettings().value(ANNO_TEXT_FONT).toString())) {
        anno_font_ = f;
    }
    anno_font_.setPointSize(12);
    Slot_SetViewLayout(1, 1);

}

//-------------------------------------------------------
ViewContainerWidget::~ViewContainerWidget()
{
    QSettings().setValue(ANNO_TEXT_FONT, anno_font_.toString());

}

void ViewContainerWidget::closeEvent(QCloseEvent* event)
{
   
    

}
//-------------------------------------------------------
QList<DicomImageView *> ViewContainerWidget::getViewList() const
{
    return view_list_;
}

//-------------------------------------------------------
DicomImageView * ViewContainerWidget::getCurrentView() const
{
    return current_view_;
}

//----------------事件过滤器--------------------------------
void ViewContainerWidget::setEventFilter(QObject * filterObj)
{
    installEventFilter(filterObj);
    filter_ = filterObj;
}

//-------------------------------------------------------
void ViewContainerWidget::SetOperation(
  const DicomImageView::ZoomOperation & operation)
{
    if (current_view_) {
       
        current_view_->SetOperation(operation);
    }
}


///****************************************************************************
/// @data    :                                                                 
/// @input   :   操作类型                                                              
/// @output  :                                                                 
/// @brief   :   实现旋转、翻转、镜像操作                                                           
///****************************************************************************

//------------------------------------------
void ViewContainerWidget::SetOperation(
  const DicomImageView::RoateFlipOperation & operation)
{
    if (current_view_) {
        switch (operation)
        {
        case DicomImageView::RoateFlipOperation::HFlip:
            behavior_thread->HFlip();
            break;
        case DicomImageView::RoateFlipOperation::VFlip:
            behavior_thread->VFlip();
            break;
        case DicomImageView::RoateFlipOperation::HMirror:
            behavior_thread->HMirror();
            break;
        case DicomImageView::RoateFlipOperation::VMirror:
            behavior_thread->VMirror();
            break;
        case DicomImageView::RoateFlipOperation::HVMirror:
            behavior_thread->HVMirror();
            break;
        case DicomImageView::RoateFlipOperation::RoateCCW:
            behavior_thread->RoateCCW();
            break;
        case DicomImageView::RoateFlipOperation::RoateCW:
            behavior_thread->RoateCW();
            break;
  
        default:
            break;
        }
        current_view_->SetOperation(operation);
    }
}

//-------------------------------------------------------
///****************************************************************************
/// @data    :                                                                 
/// @input   :   操作类型                                                              
/// @output  :                                                                 
/// @brief   :   实现图像预处理操作                                                           
///****************************************************************************

void ViewContainerWidget::SetOperation(
    const DicomImageView::PreProcessOperation& operation)
{

    if (current_view_) {
        switch (operation)
        {
            //边缘检测
        case DicomImageView::PreProcessOperation::Edge:
            behavior_thread->Edge();
            break;
            //锐化
        case  DicomImageView::PreProcessOperation::Emhance:
            behavior_thread->Emhance();
            break;
            //模糊
        case  DicomImageView::PreProcessOperation::Blur:
            behavior_thread->Blur();
            break;
            //开操作
        case  DicomImageView::PreProcessOperation::Open:
            behavior_thread->Open();
            break;
            //闭操作
        case  DicomImageView::PreProcessOperation::Close:
            behavior_thread->Close();
            break;
            //亮度调整
        case  DicomImageView::PreProcessOperation::Light:
            behavior_thread->Light();
            break;
 
        default:
            break;
        }
       
        current_view_->SetOperation(operation);
    }

}

//-------------------------------------------------------
///****************************************************************************
/// @data    :                                                                 
/// @input   :   操作类型                                                              
/// @output  :                                                                 
/// @brief   :   实现标注操作                                                           
///****************************************************************************

void ViewContainerWidget::SetOperation(
  const DicomImageView::DrawingType & operation)
{
    if (current_view_) {
        switch (operation)
        {
        case DicomImageView::DrawingType::DrawLine:
            behavior_thread->DrawLine();
            break;
        case  DicomImageView::DrawingType::DrawAngle:
            behavior_thread->DrawAngle();
            break;
        case  DicomImageView::DrawingType::DrawCobbAngle:
            behavior_thread->DrawCobbAngle();
            break;
        case  DicomImageView::DrawingType::DrawRect:
            behavior_thread->DrawRect();
            break;
        case  DicomImageView::DrawingType::DrawEllipse:
            behavior_thread->DrawEllipse();
            break;
        case  DicomImageView::DrawingType::DrawTextMark:
            behavior_thread->DrawTextMark();
            break;
        case  DicomImageView::DrawingType::DrawArrow:
            behavior_thread->DrawArrow();
            break;
        default:
            break;
        }

        current_view_->SetOperation(operation);
    }
}

//-------------------------------------------------------
void ViewContainerWidget::SetOperation(
  const DicomImageView::CurrentState & operation)
{
    if (current_view_) {
        switch (operation)
        {
        case DicomImageView::CurrentState::Magnifier:
            behavior_thread->Magnifier();
            break;
        }
        current_view_->SetOperation(operation);
    }
}

//-------------------------------------------------------
void ViewContainerWidget::SetOperation(
  const DicomImageView::WindowWLWHOperation & operation)
{
    if (current_view_) {
        current_view_->SetOperation(operation);
    }
}

//-------------------------------------------------------
void ViewContainerWidget::SetOperation(
    const DicomImageView::MPRVROperation& operation)
{
    if (current_view_) {
        current_view_->SetOperation(operation);
    }
}

//-------------------------------------------------------
void ViewContainerWidget::SetPretreatmen(Pretreatmen fun)
{
    if (current_view_) {
        current_view_->SetPretreatmen(fun);
    }
}


//-------------------------------------------------------
void ViewContainerWidget::SLot_SeriesAppend()
{
    foreach (DicomImageView * v, view_list_) {
        if (v->HasSeries()) {
            v->UpdataSeriesInstance(false);
            break;
        }
    }
}

//-------------加入序列----------------------------------
void ViewContainerWidget::SLot_SeriesInserted(SeriesInstance * series)
{
    if (!series) {
        return;
    }
    DicomImageView * view = nullptr;
    foreach (view, view_list_) {
        if (!view->HasSeries()) {
            view->SetSeriesInstance(series);           
            break;
        }
    }
    if (current_view_ && current_view_ != view) {
        Slot_ViewDoubleClicked(view);
    }
}

//----------------移除序列槽函数--------------------------------
void ViewContainerWidget::Slot_SeriesRemove(SeriesInstance * series)
{
    foreach (DicomImageView * view, view_list_) {
        if (view->GetSeriesInstance() == series) {
            view->Slot_SeriesDelete();
        }
    }
}

//-------------------全部移除槽函数------------------------------------
void ViewContainerWidget::Slot_SeriesRemoveAll()
{
    foreach (DicomImageView * view, view_list_) {
        view->Slot_SeriesDelete();
    }
}

//--------------------单击-----------------------------------
void ViewContainerWidget::Slot_ImageDClicked(SeriesInstance * series)
{
    if (current_view_) {
        current_view_->SetSeriesInstance(series);
        current_view_->UpdataSeriesInstance();
    }
}

//-------------------------------------------------------
void ViewContainerWidget::Slot_ImageChanged(SeriesInstance * series)
{
    if (!series) {
        return;
    }
    foreach (DicomImageView * v, view_list_) {
        if (v->GetSeriesInstance() == series) {
            SLot_ViewClicked(v);
            break;
        }
    }
}

//-------------------------------------------------------
void ViewContainerWidget::Slot_ViewImageChanged()
{
    if (current_view_) {
        emit SgnCurViewChanged(current_view_->GetSeriesInstance());
    }
}

//-------------------------------------------------------
/**
 * @brief ViewContainerWidget::onViewClicked
 * 单击view (切换 current view)
 * @param view
 */
void ViewContainerWidget::SLot_ViewClicked(DicomImageView * view)
{
    if (current_view_ != view) {
        if (current_view_) {
            current_view_->SetBorderHighlight(false);
        }
        current_view_ = view;
        if (current_view_) {
            current_view_->SetBorderHighlight(true);
            emit SgnCurViewChanged(current_view_->GetSeriesInstance());
        }
    }
}

//-------------------------------------------------------
/**
 * @brief ViewContainerWidget::Slot_ViewDoubleClicked
 * 双击 view(放大缩小)
 * @param view
 */
void ViewContainerWidget::Slot_ViewDoubleClicked(DicomImageView * view)
{
    if (!view) {
        return;
    }
    if (maxed_) {
        foreach (DicomImageView * v, view_list_) {
            v->setVisible(true);
        }
        maxed_ = false;
    } else {
        foreach (DicomImageView * v, view_list_) {
            v->setVisible(false);
        }
        view->setVisible(true);
        maxed_ = true;
    }
}

//-------------------------------------------------------
/**
 * @brief ViewContainerWidget::Slot_SetViewLayout
 * 设置布局
 * @param col
 * @param row
 */
void ViewContainerWidget::Slot_SetViewLayout(int col, int row)
{
    if (!(col > 0 && row > 0)) {
        return;
    }
    if (maxed_) {
        Slot_ViewDoubleClicked(current_view_);
    }
    int cellWidth = contentsRect().width() / col;
    int cellHeight = contentsRect().height() / row;
    foreach (DicomImageView * v, view_list_) {
        layout_->removeWidget(v);
    }
    int viewCount = col * row;
    while (viewCount < view_list_.size()) {
        DicomImageView * v = view_list_.takeLast();
        current_view_ = (current_view_ == v) ? nullptr : current_view_;
        delete v;
    }
    //
    DicomImageView * view;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (i * col + j < view_list_.size()) {
                view = view_list_[i * col + j];
            } else {
                view = createImageView();
                view_list_ << view;
            }
            layout_->addWidget(view, i, j);
        }
    }
    if ((!current_view_) && (!view_list_.isEmpty())) {
        SLot_ViewClicked(view_list_.first());
    }
    if (current_view_) {
        emit SgnCurViewChanged(current_view_->GetSeriesInstance());
    }
}

//-------------------------------------------------------
DicomImageView * ViewContainerWidget::createImageView()
{
    DicomImageView * v = new DicomImageView;
    v->SetAnnoTextFont(anno_font_);
    v->installEventFilter(filter_);
    connect(v, &DicomImageView::SgnViewClicked,
            this, &ViewContainerWidget::SLot_ViewClicked);
    connect(v, &DicomImageView::SgnViewDoubleclicked,
            this, &ViewContainerWidget::Slot_ViewDoubleClicked);
    return v;
}

//-------------------------------------------------------
QString ViewContainerWidget::GetCurrentImageFile()
{
    return this->current_view_->GetImageFile();
}

//---------------获取当前图像索引-----------------------------------
qint32 ViewContainerWidget::GetCurrentImageNum()
{
    return this->current_view_->GetImageNum();
}

//-------------------------------------------------------
void ViewContainerWidget::clear()
{
    foreach (DicomImageView * v, view_list_) {
        v->Slot_SeriesDelete();
        v->Reset();
        v->setVisible(true);
    }
    maxed_ = false;
}

//----------------------导出多张图像---------------------------------
void ViewContainerWidget::ExportImages()
{
    if (current_view_ && current_view_->GetSeriesInstance()) {
        QMap<int, ImageInstance *> insts =
          current_view_->GetSeriesInstance()->GetImageInstanceMap();
        QStringList imagepaths;
        foreach (auto var, insts.values()) {
            imagepaths << var->GetImageFile();
        }
        if (!imagepaths.isEmpty()) {
            ExportImageDialog d;
            d.SetMultiplePng(imagepaths);
            d.exec();
        }
    }
}

//--------------------------导出单张图像-------------------------
void ViewContainerWidget::ExportImage()
{
    if (current_view_ && current_view_->GetSeriesInstance()) {
        ImageInstance * inst =
          current_view_->GetSeriesInstance()->GetCurrImageInstance(VT_XYPlane);
        qint32 cur_index =
          current_view_->GetSeriesInstance()->GetCurIndex(VT_XYPlane);
        if (inst) {
            ExportImageDialog d;
            d.SetLeafletPng(true, cur_index, inst->GetImageFile());
            d.exec();
        }
    }
}


///****************************************************************************                 
/// @brief   : 导出视频                                                                
///****************************************************************************

void ViewContainerWidget::ExportVideo()
{
    if (current_view_ && current_view_->GetSeriesInstance()) {
        QMap<int, ImageInstance*> insts =
            current_view_->GetSeriesInstance()->GetImageInstanceMap();
        QStringList imagepaths;
        foreach(auto var, insts.values()) {
            imagepaths << var->GetImageFile();
        }
        if (!imagepaths.isEmpty()) {
            ExportVideoDialog d;
            d.num = GetCurrentImageNum();
            current_view_->GetSeriesInstance()->GetCurrImageInstance(VT_XYPlane)->GetImageSize(d.frame_width, d.frame_height );
            d.SetMultiplePng(imagepaths);
            d.exec();
        }
    }


}

//-----------------复制到剪贴板--------------------------------------
void ViewContainerWidget::copyToClipboard()
{
    if (current_view_) {
      
        QApplication::clipboard()->setPixmap(current_view_->getHardCopyPixmap());

    }
}

//-------------------------------------------------------
void ViewContainerWidget::showAnnotations(bool yes)
{
    foreach (DicomImageView * v, view_list_) {
        v->SetShowAnnotations(yes);
    }
}

//-------------------------------------------------------
void ViewContainerWidget::showMeasurements(bool yes)
{
    foreach (DicomImageView * v, view_list_) {
        v->SetShowMeasurements(yes);
    }
}

//---------------------增大字体----------------------------------
void ViewContainerWidget::incAnnoFont()
{
    anno_font_.setPointSize(anno_font_.pointSize() + 1);
    foreach (DicomImageView * v, view_list_) {
        v->SetAnnoTextFont(anno_font_);
    }
}

//--------------------------缩小字体--------------------------
void ViewContainerWidget::decAnnoFont()
{
    anno_font_.setPointSize(anno_font_.pointSize() - 1);
    foreach (DicomImageView * v, view_list_) {
        v->SetAnnoTextFont(anno_font_);
    }
}

//------------------------------设置字体---------------------
void ViewContainerWidget::setAnnoFont()
{
    anno_font_ = QFontDialog::getFont(nullptr, anno_font_, this);
    foreach (DicomImageView * v, view_list_) {
        v->SetAnnoTextFont(anno_font_);
    }
}

void ViewContainerWidget::ImageLoadFinished()
{
    foreach(auto var, view_list_) {
        var->UpdataSeriesInstance();
    }
    if (behavior_thread->isRunning())
    {
        behavior_thread->stop();
    }
    //connect(behavior_thread, SIGNAL(finished()), behavior_thread, SLOT(deleteLater()));
    behavior_thread->cu_series = current_view_->GetSeriesInstance();
    behavior_thread->start();
}



//-------重置------------------------------------------------
void ViewContainerWidget::reset()
{
    if (current_view_) {
        current_view_->Reset();
    }
}

//-------------------------------------------------------
void ViewContainerWidget::prevView()
{
    if (current_view_ && view_list_.size()) {
        int i = view_list_.indexOf(current_view_);
        if (i > 0) {
            SLot_ViewClicked(view_list_.at(i - 1));
        }
    }
}

//-------------------------------------------------------
void ViewContainerWidget::nextView()
{
    if (current_view_ && view_list_.size()) {
        int i = view_list_.indexOf(current_view_);
        if (i < view_list_.size() - 1) {
            SLot_ViewClicked(view_list_.at(i + 1));
        }
    }
}

//-------------------------------------------------------
void ViewContainerWidget::nextFrame()
{
    if (current_view_) {
        current_view_->NextFrame();
    }
}

//-------------------------------------------------------
void ViewContainerWidget::prevFrame()
{
    if (current_view_) {
        current_view_->PrevFrame();
    }
}

//-------------------------------------------------------
void ViewContainerWidget::resizeEvent(QResizeEvent * e)
{
    QWidget::resizeEvent(e);
}


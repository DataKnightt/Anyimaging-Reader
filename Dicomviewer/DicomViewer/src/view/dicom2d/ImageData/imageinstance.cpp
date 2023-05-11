﻿///****************************************************************************
/// @author  : 李孟娅                                                       
/// @date    : 2022.4.10                                                               
/// @file    :                                                                 
/// @brief   :  图像实例  （和seriesinstance.cpp相似）                                                             
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************
#include "imageinstance.h"
#include <QDebug>

//----------------------------------------------------------------
ImageInstance::ImageInstance(const QString & file)
  : d_(new ImageInstanceData(file))
{
}

//----------------------------------------------------------------
ImageInstance::ImageInstance(DcmFileFormat * dff)
  : d_(new ImageInstanceData(dff))
{
}

//----------------------------------------------------------------
ImageInstance::ImageInstance(const ImageInstance & image)
  : d_(image.d_)
{
}

ImageInstance::~ImageInstance()
{
}

//-----------窗口设置-----------------------------------------------------
void ImageInstance::SetWindow(const double & center, const double & width)
{
    d_->SetWindow(center, width);
}

//----------------------------------------------------------------
void ImageInstance::GetWindow(double & center, double & width) const
{
    d_->GetWindow(center, width);
}

//----------------------------------------------------------------
void ImageInstance::SetWindowDelta(const double & dCenter, const double & dWidth)
{
    d_->SetWindowDelta(dCenter, dWidth);
}

//----------------------------------------------------------------
void ImageInstance::SetRoiWindow(const QRectF & rect)
{
    d_->SetRoiWindow(rect);
}

//----------------------------------------------------------------
void ImageInstance::SetDefaultWindow()
{
    d_->SetDefaultWindow();
}

//----------------------------------------------------------------
void ImageInstance::SetFullDynamic()
{
    d_->SetFullDynamic();
}

//-----------背景（0，255）设置-----------------------------------------------------
void ImageInstance::GetPolarity(EP_Polarity p)
{
    d_->SetPolarity(p);
}

//----------------------------------------------------------------
EP_Polarity ImageInstance::GetPolarity() const
{
    return d_->GetPolarity();
}

//----------------------------------------------------------------
QString ImageInstance::GetStudyUid() const
{
    return d_->GetStudyUid();
}

//----------------------------------------------------------------
QString ImageInstance::GetSeriesUid() const
{
    return d_->GetSeriesUid();
}

//----------------------------------------------------------------
QString ImageInstance::GetImageUid() const
{
    return d_->GetImageUid();
}

//----------------------------------------------------------------
QString ImageInstance::GetClassUid() const
{
    return d_->GetClassUid();
}

//----------------------------------------------------------------
QString ImageInstance::GetImageFile() const
{
    return d_->GetImageFile();
}

//----------------------------------------------------------------
bool ImageInstance::GetPixmap(QPixmap & pixmap)
{
    return d_->GetPixmap(pixmap);
}

//----------------------------------------------------------------
bool ImageInstance::GetPixmap(QPixmap & pixmap, const qint32 & frame)
{
    return d_->GetPixmap(pixmap, frame);
}

//----------------------------------------------------------------
bool ImageInstance::GetPixmap(const QString & file, QPixmap & pixmap)
{
    return ImageInstanceData::GetPixmap(file, pixmap);
}

//----------------------------------------------------------------
bool ImageInstance::Dcm2BmpHelper(DicomImage & image, QPixmap & pixmap, const qint32 frame)
{
    return ImageInstanceData::Dcm2BmpHelper(image, pixmap, frame);
}

//----------------------------------------------------------------
bool ImageInstance::IsNormal() const
{
    return d_->IsNormal();
}

//---------图像操作（旋转，翻转）-------------------------------------------------------
DicomImage * ImageInstance::CreateClippedImage(
  const QRect & rect, int angle, bool hflip, bool vflip, bool inverted)
{
    return d_->CreateClippedImage(rect, angle, hflip, vflip, inverted);
}

//----------------------------------------------------------------
QString ImageInstance::GetTagKeyValue(const DcmTagKey & key) const
{
    return d_->GetTagKeyValue(key);
}

//----------像素值------------------------------------------------------
uint ImageInstance::GetPixelValue(long x, long y) const
{
    return static_cast<uint>(d_->GetPixelValue(x, y));
}

//--------------像素坐标--------------------------------------------------
bool ImageInstance::GetPixSpacing(double & spacingX, double & spacingY) const
{
    return d_->GetPixSpacing(spacingX, spacingY);
}

//--------------图像尺寸--------------------------------------------------
bool ImageInstance::GetImageSize(ulong & width, ulong & height) const
{
    return d_->GetImageSize(width, height);
}

//----------------------------------------------------------------
const short * ImageInstance::GetInternalPtr() const
{
    return d_->GetInternalPtr();
}

//----------------------------------------------------------------
const ushort * ImageInstance::GetRawData() const
{
    return d_->GetRawData();
}

//--------------DICOM图片--------------------------------------------------
const DicomImage * ImageInstance::GetDcmImage() const
{
    return d_->GetDcmImage();
}

//--------------DICOM文件--------------------------------------------------
DcmFileFormat * ImageInstance::GetFileFormat()
{
    return d_->GetFileFormat();
}

//----------------------------------------------------------------
bool ImageInstance::SaveFileFormat()
{
    return d_->SaveFileFormat();
}

//-------------当前帧个数---------------------------------------------------
qint32 ImageInstance::GetFrameCount() const
{
    return d_->GetFrameCount();
}

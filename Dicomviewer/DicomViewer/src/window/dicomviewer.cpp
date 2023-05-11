#include "dicomviewer.h"
#include "ui_dicomviewer.h"

#include "logdbwidget.h"
#include "widget/dicomtagswidget.h"
#include <dao/KissDb>
#include <view/KissView>
#include "global/PluganInterface.h"



///****************************************************************************
/// @author  : 程嵩森、苏一舟、李孟娅                                                        
/// @date    : 2022.3.1                                                                
/// @file    :                                                                 
/// @brief   : 主窗体                                                             
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//-----------------------------------------------
DicomViewer::DicomViewer(DicomViewer::ViewerType type, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::DicomViewer)
    , m_type_(type)

{
    setAcceptDrops(true);
    ui->setupUi(this);
    this->Initial();

}

//-----------------------------------------------
DicomViewer::~DicomViewer()
{
    delete ui;

}

//----------关闭--------------------------------
void DicomViewer::closeEvent(QCloseEvent* event)
{


}

//-------------单个文件---------------------------------
void DicomViewer::SetDicomFile(const QString& path)
{
    ui->thumbnailBar->setImagePaths(QStringList() << path);
}

//-------------------文件夹------------------------------------------
void DicomViewer::SetDicomFile(const QStringList& path)
{
    ui->thumbnailBar->setImagePaths(QStringList() << path);
}

//-------------------初始化-------------------------
void DicomViewer::Initial()
{
    ui->thumbnailBar->installEventFilter(this);
    ui->viewContainer->installEventFilter(this);
    QWidget::setWindowTitle(Project_NAME);
    this->InitViewType();
    this->SetupToolBar();
    this->SetupConnection();
}

//--------------设置菜单栏----------------------
void DicomViewer::SetupToolBar()
{
    this->SetupFileTool();
    this->SetupExportTool();
    this->SetupGridTool();
    this->SetupAnnoTool();
    this->SetupAdjustTool();
    this->SetupPanTool();
    //this->SetupSlicingTool();
    this->SetupMarksTool();
    this->SetupZoomTool();
    this->SetupFlipTool();
    this->SetupFullTool();
    this->SetupPlugin();
    this->SetupPluginTool();
}

//-----------打开文件-----------------------
void DicomViewer::SetupFileTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(tr("打开 DICOM 文件夹"), this, [&] {
        QSettings s;
        QString p = s.value(OPEN_DIR_PATH, ".").toString();
        p = QFileDialog::getExistingDirectory(this, tr("打开 DICOM 文件夹"), p);
        if (!p.isEmpty()) {
            s.setValue(OPEN_DIR_PATH, p);
            ui->thumbnailBar->setImagePaths(QStringList() << p);
        }
        }, Qt::CTRL + Qt::SHIFT + Qt::Key_O);
    m->addAction(tr("打开 DICOM 文件"), this, [&] {
        QSettings s;
        QString p = s.value(OPEN_FILE_PATH).toString();
        QStringList fs =
            QFileDialog::getOpenFileNames(this, tr("打开 DICOM 文件"), p);
        if (!fs.isEmpty()) {
            s.setValue(OPEN_DIR_PATH, fs.first());
            ui->thumbnailBar->setImagePaths(fs);
        }
        }, Qt::CTRL + Qt::Key_O);
    m->addAction(tr("关闭所有"), this, [&] {
        ui->thumbnailBar->clear();

        });
    m->addSeparator();
    m->addAction(tr("退出"), this, &DicomViewer::close, Qt::ALT + Qt::Key_F4);
    ui->fileBtn->setMenu(m);
    // connect
    connect(ui->fileBtn, &QToolButton::clicked, this, [&] {
        QSettings s;
        QString p = s.value(OPEN_DIR_PATH, ".").toString();
        p = QFileDialog::getExistingDirectory(this, tr("打开 DICOM 文件夹"), p);
        if (!p.isEmpty()) {
            s.setValue(OPEN_DIR_PATH, p);
            ui->thumbnailBar->setImagePaths(QStringList() << p);
        }
        });
}

//-----------导出图像----------------------
void DicomViewer::SetupExportTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(tr("导出单张图像"), this, [&] {
        ui->viewContainer->ExportImage();
        }, Qt::SHIFT + Qt::Key_D);
    m->addAction(tr("导出所有图像"), this, [&] {
        ui->viewContainer->ExportImages();
        }, Qt::SHIFT + Qt::Key_A);
    m->addAction(tr("导出视频"), this, [&] {
        ui->viewContainer->ExportVideo();
        });
    m->addAction(tr("复制到剪切板"), this, [&] {
        /*       QPixmap pix = QPixmap::grabWidget(ui->viewContainer);
               pix.save("widget.png", "png");*/
               //ui->viewContainer->copyToClipboard(pix);
        ui->viewContainer->copyToClipboard();
        });
    ui->exportBtn->setMenu(m);
    // connect
    connect(ui->exportBtn, &QToolButton::clicked, this, [&] {

        ui->viewContainer->ExportImages();
        });
}

//-----------宫格定制---------------------
void DicomViewer::SetupGridTool()
{
    // Menu
    QMenu* m;
    QAction* a;
    m = new QMenu(this);
    a = m->addAction(tr("宫格定制"), this, [&](bool value) {
        ui->scrollArea->setVisible(value);
        });
    a->setCheckable(true);
    a->setChecked(true);
    ui->gridBtn->setMenu(m);
    // connect
    connect(ui->gridBtn, &QToolButton::clicked, this, [&] {
        // auto delete while hidden
        GridPopWidget* gpw = new GridPopWidget(ui->gridBtn);
        connect(gpw, &GridPopWidget::SgnViewLayout,
            ui->viewContainer, &ViewContainerWidget::Slot_SetViewLayout);
        gpw->move(this->geometry().topLeft() + ui->gridBtn->geometry().bottomLeft() + QPoint(25, 25));
        gpw->show();
        });
}

//----------注释---------------------------
void DicomViewer::SetupAnnoTool()
{
    // Menu
    QMenu* m;
    QAction* a;
    m = new QMenu(this);
    a = m->addAction(tr("Annotations"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F12);
    a->setCheckable(true);
    a->setChecked(true);
    a = m->addAction(tr("Measurements"), this, [&](bool value) {
        ui->viewContainer->showMeasurements(value);
        });
    a->setCheckable(true);
    a->setChecked(true);
    m->addSeparator();
    m->addAction(tr("放大字体"), this, [&] {
        ui->viewContainer->incAnnoFont();
        });
    m->addAction(tr("减小字体"), this, [&] {
        ui->viewContainer->decAnnoFont();
        });
    m->addAction(tr("选择字体"), this, [&] {
        ui->viewContainer->setAnnoFont();
        });
    m->addSeparator();
    m->addAction(tr("DICOM 标签"), this, [&] {
        DicomTagsWidget* wid =
            new DicomTagsWidget{ this->ui->viewContainer->GetCurrentImageFile() };
        wid->show();
        }, Qt::CTRL + Qt::ALT + Qt::Key_T);
    ui->annoBtn->setMenu(m);
    // connect
    a = ui->annoBtn->menu()->actions().first();
    connect(ui->annoBtn, &QToolButton::clicked, a, &QAction::toggle);
    connect(a, &QAction::toggled, this, [&](bool value) {
        ui->viewContainer->showAnnotations(value);
        });
}

//--------图像处理------------------------
void DicomViewer::SetupAdjustTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(tr("默认"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::DefaultWL);
        }, Qt::Key_0);
    m->addAction(tr("Full dynamic"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::FullDynamic);
        }, Qt::Key_1);
    m->addSeparator();
    m->addAction(QIcon(":/png/contrast.png"), tr("Negative"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::InverseWl);
        }, Qt::Key_2);
    m->addSeparator();
    m->addAction(tr("Edge image"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Edge);

        }, Qt::Key_3);
    m->addAction(tr("Emhance image      "), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Emhance);
        }, Qt::Key_4);
    m->addAction(tr("Blur image"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Blur);
        }, Qt::Key_5);
    m->addAction(tr("Open Image"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Open);
        }, Qt::Key_6);
    m->addAction(tr("Close Image"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Close);
        }, Qt::Key_7);
    m->addAction(tr("Light Image"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Light);
        }, Qt::Key_8);
    ui->adjustBtn->setMenu(m);
    // connect
    connect(ui->adjustBtn, &QToolButton::clicked, this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::AdjustWL);
        });
}

//---------------------------------------
void DicomViewer::SetupPanTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(QIcon(":/png/reset.png"), tr("重置"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::RestPan);
        });
    ui->panBtn->setMenu(m);
    // connect
    connect(ui->panBtn, &QToolButton::clicked, this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Pan);
        });
}

//-------------------切片----------------------
//void DicomViewer::SetupSlicingTool()
//{
//    connect(ui->slicingBtn, &QToolButton::clicked, this, [&] {
//        ui->viewContainer->SetOperation(DicomImageView::Slicing);
//        });
//}

//---------画笔-------------------------
void DicomViewer::SetupMarksTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(QIcon(":/png/line.png"), tr("Length"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/line.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawLine);
        }, Qt::Key_L);
    m->addAction(QIcon(":/png/ellipse.png"), tr("Ellipse"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/ellipse.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawEllipse);
        }, Qt::Key_E);
    m->addAction(QIcon(":/png/rect.png"), tr("Rectangle"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/rect.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawRect);
        }, Qt::Key_R);
    m->addAction(QIcon(":/png/angle.png"), tr("Angle"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/angle.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawAngle);
        }, Qt::Key_A);
    m->addAction(QIcon(":/png/angle.png"), tr("CobbAngle"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/angle.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawCobbAngle);
        }, Qt::Key_C);
    m->addAction(QIcon(":/png/angle.png"), tr("Arrow"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/angle.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawArrow);
        }, Qt::Key_W);
    m->addAction(QIcon(":/png/text.png"), tr("Text"), this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/text.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawTextMark);
        }, Qt::Key_T);
    m->addSeparator();
    m->addAction(QIcon(":/png/recycle.png"), tr("Remove all"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::RemoveAllDraw);
        }, Qt::CTRL + Qt::Key_R);
    ui->marksBtn->setMenu(m);
    // connect
    connect(ui->marksBtn, &QToolButton::clicked, this, [&] {
        ui->marksBtn->setIcon(QIcon(":/png/line.png"));
        ui->viewContainer->SetOperation(DicomImageView::DrawLine);
        });
}

//-------------缩放-----------------------------
void DicomViewer::SetupZoomTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(tr("Fill viewport"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::FillViewport);
        });
    m->addSeparator();
    m->addAction(tr("100%"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Zoom100);
        }, Qt::SHIFT + Qt::Key_1);
    m->addAction(tr("200%"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Zoom200);
        }, Qt::SHIFT + Qt::Key_2);
    m->addAction(tr("400%"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Zoom400);
        }, Qt::SHIFT + Qt::Key_4);
    m->addSeparator();
    m->addAction(QIcon(":/png/zoomin.png"), tr("Zoom in"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::ZoomIn);
        }, Qt::SHIFT + Qt::Key_I);
    m->addAction(QIcon(":/png/zoomout.png"), tr("Zoom out"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Zoomout);
        }, Qt::SHIFT + Qt::Key_O);
    m->addSeparator();
    m->addAction(QIcon(":/png/magnifier.png"), tr("放大镜"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Magnifier);
        }, Qt::SHIFT + Qt::Key_F);
    ui->zoomBtn->setMenu(m);
    // connect
    connect(ui->zoomBtn, &QToolButton::clicked, this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::Zoom);
        });
}

//--------------旋转、镜像、翻转---------------------------
void DicomViewer::SetupFlipTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    m->addAction(QIcon(":/png/rrotate.png"), tr("顺时针旋转90°"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::RoateCW);
        }, Qt::CTRL + Qt::Key_Right);
    m->addAction(QIcon(":/png/lrotate.png"), tr("逆时针旋转90°"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::RoateCCW);
        }, Qt::CTRL + Qt::Key_Left);
    m->addAction(QIcon(":/png/reset.png"), tr("清除旋转"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::ClearRoate);
        }, Qt::CTRL + Qt::SHIFT + Qt::Key_Backslash);
    m->addSeparator();

    m->addAction(tr("水平镜像"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::HMirror);
        });
    m->addAction(tr("垂直镜像"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::VMirror);
        });
    m->addAction(tr("水平垂直镜像"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::HVMirror);
        });
    m->addAction(tr("清除镜像"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::ClearMirror);
        });

    m->addSeparator();
    m->addAction(QIcon(":/png/hflip.png"), tr("水平翻转"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::HFlip);
        });
    m->addAction(QIcon(":/png/vflip.png"), tr("垂直翻转"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::VFlip);
        });
    m->addAction(QIcon(":/png/reset.png"), tr("清除翻转"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::ClearFlip);
        });
    ui->flipBtn->setMenu(m);
    // connect
    connect(ui->flipBtn, &QToolButton::clicked, this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::HFlip);
        });
}

//-------------全屏---------------------------
void DicomViewer::SetupFullTool()
{
    connect(ui->fullScreenBtn, &QToolButton::clicked, this, [&](bool checked) {
        if (checked) {
            QRect r = geometry();
            QSettings().setValue("WindowGeometry", r);
            setWindowState(Qt::WindowFullScreen);
            ui->fullScreenBtn->setIcon(QIcon(":/png/full_screen_exit.png"));
        }
        else {
            QRect r = QSettings().value("WindowGeometry").toRect();
            setWindowState(Qt::WindowNoState);
            if (r.isValid()) {
                this->setGeometry(r);
            }
            ui->fullScreenBtn->setIcon(QIcon(":/png/full_screen.png"));
        }
        });
}

//---------------三维重建------------------------
void DicomViewer::SetupPluginTool()
{
    // Menu
    QMenu* m;
    m = new QMenu(this);
    //多平面重建
    m->addAction(tr("MPR"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::MPR);
        });
    //体绘制
    m->addAction(tr("VR-RayCast"), this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::raycastVR);
        });
    ui->pluginBtn->setMenu(m);
    // connect
    connect(ui->pluginBtn, &QToolButton::clicked, this, [&] {
        ui->viewContainer->SetOperation(DicomImageView::MPR);
        });
}

//---------建立信号槽函数连接-------------------------
void DicomViewer::SetupConnection()
{
    // thumbnailBar <==> viewContainer
    connect(ui->thumbnailBar, SIGNAL(SgnSeriesInserted(SeriesInstance*)),
        ui->viewContainer, SLOT(SLot_SeriesInserted(SeriesInstance*)));
    connect(ui->thumbnailBar, SIGNAL(SgnImageDoubleClicked(SeriesInstance*)),
        ui->viewContainer, SLOT(Slot_ImageDClicked(SeriesInstance*)));
    connect(ui->thumbnailBar, SIGNAL(SgnSeriesAppend()),
        ui->viewContainer, SLOT(SLot_SeriesAppend()));
    // thumbnailBar <==> DicomViewer
    connect(ui->thumbnailBar, &ThumbnailBarWidget::SgnImageLoadBegin,
        this, [&] {
            /*    ui->tool_widget->setVisible(1);*/
            this->update();
        });
    connect(ui->thumbnailBar, &ThumbnailBarWidget::SgnImageLoadFinished,
        this, [&] {
            /*     ui->tool_widget->setVisible(1);*/
            ui->viewContainer->ImageLoadFinished();
        });
}

//-----------------------------------------------
void DicomViewer::SetupPlugin()
{
    QDir pluginsDir("./plugins");
    const auto entryList = pluginsDir.entryList(QDir::Files);
    for (const QString& fileName : entryList) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();
        if (!plugin) {
            return;
        }
        const auto i_pretreatment =
            qobject_cast<PretreatmentInterface*>(plugin);
        if (!i_pretreatment) {
            return;
        }
        QMenu* m = ui->flipBtn->menu();
        QAction* a;
        QActionGroup* filter_group = new QActionGroup(this);
        QMenu* filter = new QMenu("filter", this);
        const QStringList texts = i_pretreatment->GetPretreatments();
        foreach(const QString var, texts) {
            if (var.isEmpty()) {
                filter->addSeparator();
            }
            else {
                a = filter->addAction(var);
                Pretreatmen fun = i_pretreatment->PretreatmentFun(var);
                connect(a, &QAction::triggered, this, [=] {
                    ui->viewContainer->SetPretreatmen(fun);
                    });
                a->setCheckable(true);
                // a->setChecked(true);
                filter_group->addAction(a);
            }
        }
        m->addMenu(filter);
    }
}

//-----------------------------------------------
void DicomViewer::InitViewType()
{
    if (this->m_type_ == Embed) {
        ui->fullScreenBtn->setVisible(0);
        ui->gridBtn->setVisible(0);
        qint32 size = ui->fileBtn->menu()->actions().size() - 1;
        QAction* a = ui->fileBtn->menu()->actions().at(size);
        ui->fileBtn->menu()->removeAction(a);
    }
    else if (this->m_type_ == SingleInstance) {
        LogDao dao;
        dao.InsertMessageToDao("",
            LogDao::ET_SysInfo,
            tr(" Dicom Viewer Open"));
        this->SetWidgetDirection(QBoxLayout::LeftToRight);
    }
}

//-----------------------------------------------
void DicomViewer::SetWidgetDirection(const QBoxLayout::Direction& lay)
{
    if (lay == QBoxLayout::LeftToRight) {
        DicomImageLabel::setImage_label_size(180);
        this->update();
        ui->horizontalLayout_2->setDirection(QBoxLayout::LeftToRight);
        ui->scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        ui->thumbnailBar->setDirection(QBoxLayout::TopToBottom);
        ui->scrollArea->setMinimumSize(DicomImageLabel::getImage_label_size() + 40, 0);
    }
    else if (lay == QBoxLayout::TopToBottom) {
        DicomImageLabel::setImage_label_size(120);
        this->update();
        ui->horizontalLayout_2->setDirection(QBoxLayout::TopToBottom);
        ui->scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->thumbnailBar->setDirection(QBoxLayout::LeftToRight);
        ui->scrollArea->setMinimumSize(0, DicomImageLabel::getImage_label_size() + 40);
    }
    ui->thumbnailBar->UpdataLabeSize();
    this->update();
}

//-----------------------------------------------
bool DicomViewer::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* e = static_cast<QKeyEvent*>(event);
        switch (e->key()) {
        case Qt::Key_Home:
            ui->thumbnailBar->firstSeries();
            break;
        case Qt::Key_End:
            ui->thumbnailBar->lastSeries();
            break;
        case Qt::Key_Return:
            ui->thumbnailBar->currSeries();
            break;
        case Qt::Key_Right:
            ui->thumbnailBar->nextSeries();
            break;
        case Qt::Key_Left:
            ui->thumbnailBar->prevSeries();
            break;
        case Qt::Key_Up:
            ui->viewContainer->prevFrame();
            break;
        case Qt::Key_Down:
            ui->viewContainer->nextFrame();
            break;
        default:
            return QObject::eventFilter(watched, event);
        }
        return true;
    }
    else {
        return QObject::eventFilter(watched, event);
    }
}

///****************************************************************************                 
/// @brief   : 接收拖拽文件                                                                
///****************************************************************************

void DicomViewer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void DicomViewer::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;
    ui->thumbnailBar->setImagePaths(QStringList() << fileName);

}

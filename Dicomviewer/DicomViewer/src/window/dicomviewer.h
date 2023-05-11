#ifndef DICOMVIEWER_H
#define DICOMVIEWER_H

#include <QBoxLayout>
#include <QWidget>
#include <QtWidgets/QMainWindow>
#include "behaviorthread.h"
class ThumbnailBarWidget;
class ViewContainerWidget;
class QLocalServer;

namespace Ui {
class DicomViewer;
}

class DicomViewer : public QWidget
{
    Q_OBJECT
public:
    enum ViewerType
    {
        SingleInstance, // 单实例风格
        Embed, // 嵌入其他软件
    };

public:
    explicit DicomViewer(
      ViewerType type = SingleInstance, QWidget * parent = nullptr);
    ~DicomViewer();
    void SetDicomFile(const QString & path);
    void SetDicomFile(const QStringList & path);

protected:
    bool eventFilter(QObject * watched, QEvent * event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent(QCloseEvent* e);
private:
    void Initial();
    void SetupToolBar();
    void SetupFileTool();
    void SetupExportTool();
    void SetupGridTool();
    void SetupAnnoTool();
    void SetupAdjustTool();
    void SetupPanTool();
    //void SetupSlicingTool();
    void SetupMarksTool();
    void SetupZoomTool();
    void SetupFlipTool();
    void SetupFullTool();
    void SetupConnection();
    void SetupPluginTool();
    void SetupPlugin();
    void InitViewType();
    void SetWidgetDirection(const QBoxLayout::Direction & lay);

private:
    Ui::DicomViewer * ui;
    ViewerType m_type_;

};

#endif // DICOMVIEWER_H

#ifndef EXPORTVIDEODIALOG_H
#define EXPORTVIDEODIALOG_H
#include <QDialog>
#include "ui_ExportVideoDialog.h"
class ExportImageThread;

namespace Ui {
	class ExportVideoDialog;
}
class ExportVideoDialog : public QDialog
{
	Q_OBJECT

public:
	ExportVideoDialog(QWidget *parent = Q_NULLPTR);
	~ExportVideoDialog();
	void SetMultiplePng(const QStringList& images);
	int num;
	ulong frame_width;
	ulong frame_height;
protected:
	//void closeEvent(QCloseEvent* e);

protected slots:
	//��ʼ
	void Slot_Start(bool checked);
	//ѡ���ļ���
	void Slot_SelectFolder();

	void Image_To_Video(QString pngpath);

	//����
	void Slot_ExportFinished();
	
	void Slot_VideoResultReady();

private:
	Ui::ExportVideoDialog *ui;
	ExportImageThread* export_thread_;
	QString folder;
	void Initialization();
	QString png_path;
	int image_count_;
};
#endif //EXPORTVIDEODIALOG_H

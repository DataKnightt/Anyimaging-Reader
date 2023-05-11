﻿#ifndef SCANDCMFILETHREAD_H
#define SCANDCMFILETHREAD_H

#include "dcmtk/dcmdata/dcdatset.h"
#include <QStringList>
#include <QThread>

class StudyRecord;

class ScanDcmFileThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanDcmFileThread(QObject * parent = nullptr);
    void run();
    void SetFiles(const QStringList & files);
    void SetAbort(bool yes);
Q_SIGNALS:
    void SgnResultReady();
    void SgnResultRecord(StudyRecord * study);

private:
    QStringList file_list_;
    bool abort_;
};

#endif // SCANDCMFILETHREAD_H

#ifndef ANALYSISWORKER_H
#define ANALYSISWORKER_H

#include <QObject>

class AnalysisWorker : public QObject {
    Q_OBJECT
public:
    explicit AnalysisWorker(QObject *parent = nullptr);
    void SetProgressIntervalI(int interval);
signals:
    void Progress(int value);
    void Finished();
    void UpdateStatus(const QString &status);
public slots:
    void DoWork();
};

#endif // ANALYSISWORKER_H
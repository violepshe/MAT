#ifndef FOOTER_COMPONENT_H
#define FOOTER_COMPONENT_H

#include <QBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QString>
#include <QObject>
#include <QProgressBar>
#include <QToolBar>
#include "analysis_worker.h"

class QLabel;

class FooterComponent : public QToolbar {
    Q_OBJECT

public:
    explicit FooterComponent(QWidget* parent=nullptr);
    ~FooterComponent();
    void SetStatus(const QString& newStatus);
    void RunTimer();

private:
    QWidget *footer_widget_;
    QWidget *h_filler_;
    QLabel *year_label_;
    QLabel *status_;
    QLabel *version_label_;
    QHBoxLayout *footer_layout;
    QProgressBar *progress_bar_;
    QThread *worker_thread_;
    AnalysisWorker *worker;

public slots:
    void UpdateProgressBar(int value);
    void OnAnalysisFinished();
    void UpdateStatus(const QString &status);

signals:
    void EnableResults();
};

#endif // FOOTER_COMPONENT_H
#include [..]

FooterComponent::FooterComponent(QWidget* parent) : QToolBar(parent), worker_thread_(new QThread(this)), worker(new AnalysisWorker) {
    // initialization of private vars

    progress_bar_ = new QProgressBar(this);

    progress_bar_->setAlignment(Qt::AlignCenter);
    progress_bar_->setProperty("class", "progressBar");
    progress_bar_->setVisible(false);

    footer_layout_ = new QHBoxLayout();
    // add other widgets..
    footer_layout_->addWidget(progress_bar_, Qt::AlignLeft);
    footer_widget_->setLayout(footer_layout_);
    footer_widget_->setProperty("class", "footerWid");

    addWidget(footer_widget_);
    worker->moveToThread(worker_thread_);

    connect(worker_thread_, &QThread::started, worker, &AnalysisWorker::DoWork);
    connect(worker, &AnalaysisWorker::Progress, this, &FooterComponent::UpdateProgressBar);
    connect(worker, &AnalysisWorker::Finished, this, &FooterComponent::OnAnalysisFinished);
    connect(worker, &AnalysisWorker::UpdateStatus, this, &FooterComponent::UpdateStatus);
    connect(worker, &AnalysisWorker::Finished, worker_thread_, &QThread::quit);
    connect(worker, &AnalysisWorker::Finished, worker, &AnalysisWorker::deleteLater);
    connect(worker_thread_, &QThread::finished, worker_thread_, &QThread::deleteLater);
}

void FooterComponent::SetStatus(const QString &newStatus) {
    status_->setText(newStatus);
}

void FooterComponent::RunTimer() {
    progress_bar_->setVisible(true);
    status_->setText("In Progress. Please Wait.");
    worker_thread_->start();
}

void FooterComponent::UpdateProgressBar(int value) {
    progress_bar_->setValue(value);
}

void FooterComponent::OnAnalysisFinished() {
    status_->setText("COMPLETED");
    emit EnableResults();
}

void FooterComponent::UpdateStatus(const QString &status) {
    status_->setText(status);
}
#include [..]

InputsScreenTab::InputsScreenTab(QWidget *parent) : QWidget(parent) {
    // [Initialization of many private variables such as QVBoxLayout, QLabel, QComboBox, QSpinBox, etc]

    // Place layouts into main_layout_

    // Create connections
    //[some connections here]
    connect(run_shell_modal_analysis_button_, &QPushButton::pressed, this, &InputsScreenTab::VerifyRunConditions);
    connect(this, &InputsScreenTab::VerifySignal, this, &InputsScreenTab::VerifyInputs);
}

// several void functions here

void InputsScreenTab::InitializeProgressAnalysis() {
    emit StartTimerBar();
    QThread* thread = new QThread;
    AnalysisWorker* worker = new AnalysisWorker;

    worker->moveToThread(thread);

    connect(thread, &QThread::started, this, &InputsScreenTab::RunAnalysis);
    connect(worker, &AnalysisWorker::Progress, this, &InputsScreenTab::HandleProgress);
    connect(worker, &AnalysisWorker::UpdateStatus, this, &InputsScreenTab::HandleStatusUpdate);
    connect(worker, &AnalaysisWorker::Finished, thread, &QThread::quit);
    connect(worker, &AnalysisWorker::Finished, worker, &AnalaysisWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &InputsScreenTab::destroyed, worker, &AnalysisWorker::deleteLater);
    connect(this, &InputsScreenTab::destroyed, thread, &QThread::quit);
    connect(this, &InputsScreenTab::destroyed, thread, &QThread::deleteLater);

    thread->start();
}

void InputsScreenTab::VerifyRunConditions(){
    if (conditionTrue) {
        InitializeProgressAnalysis();
        RunAnalysis();
        OpenModalDone();
        valid.results_screen_tab = true;
        return;
    } else {
        return;
    }
}

void InputsScreenTab::HandleProgress(int value) {
    emit Progress(value);
}

void InputsScreenTab::HandleStatusUpdate(const QString &status) {
    emit UpdateStatus(status);
}

void InputsScreenTab::RunAnalysis() {
    const int kTotalStages = 7;
    int progress_increment = 100/kTotalStages;
    int progress_value = 0;

    emit UpdateStatus("Starting analysis..");
    emit Progress(progress_value);

    // Stage 1 create folder
    emit UpdateStatus("Creating folder for modeshape plots..");
    CreatePlotsFolder();
    emit Progress(progress_value += progress_increment);

    // Stage 2 check bin bash installation
    emit UpdateStatus("Checking if bash is installed..");
    if(!CheckBashInstallation(this)){
        qCritical () << "Cygwin bash console couldn't be opened. Please check the installation";
        SetMessageBox("Attention!", "Cygwin couldn't be opened", "Please check the installation", "warning", this);
        return;
    }
    emit Progress(progress_value += progress_increment);

    // Stage 3 create copy of input UIF 
    emit UpdateStatus("Adding inputs to UIF..");
    AddInputstoUIF();
    emit Progress(progress_value += progress_increment);

    int single_harmonic_value = single_harmonic_value_->value();
    // Stage 4 Update MCASE set file
    emit UpdateStatus("Updating MCASE set file..");
    UpdateMCASEsetfile(true,single_harmonic_value, single_harmonic_value);
    emit Progress(progress_value += progress_increment);

    // Stage 5 Run scripts for single harmonic
    emit UpdateStatus("Running single harm scripts..");
    RunScriptsSingleHarmonic(single_harmonic_value);
    emit Progress(progress_value += progress_increment);

    // Stage 6 Run MCASE and plot generation
    emit UpdateStatus("Run MCASE and plot generation..");
    RunMCASEandPlots(start_harm_value, end_harm_value);
    emit Progress(progress_value += progress_increment);

    // Stage 7 update dropdowns for modeshape plots screen
    emit UpdateStatus("Updating dropdowns..");
    UpdateModeShapeScreenDropdowns();
    emit Progress(progress_value += progress_increment);

    emit UpdateStatus("Finalizing analysis..");
    emit Progress(100);
    emit UpdateStatus("Analysis completed.");
    emit AnalysisCompleted();
}

// other functions..
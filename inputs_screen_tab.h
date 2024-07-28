#ifndef INPUTS_SCREEN_TAB_H
#define INPUTS_SCREEN_TAB_H

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include "analysis_worker.h"

class InputsScreenTab : public QWidget 
{
    Q_OBJECT
public:
    explicit InputsScreenTab(QWidget *parent = nullptr);
    ~InputsScreenTab();
    void InputsSelectionCreation();
    void OutputsSelectionCreation();
    void InputsSetup();
    void OutputsSetup();
    void HarmonicSelectionUpdate(QString mode, int current_widgets);
    void OpenModalDone();
    void CloseModal(QDialog *modal);
    void InitializeProgressAnalysis();
    QComboBox *harmonic_mode_selection;
    double mvib_value = 0;

private:
    InputsScreenTab(const InputsScreenTab&) = delete;
    InputsScreenTab& operator= (const InputsScreenTab&) = delete;
    void SetButton(QPushButton *btn);

    QVBoxLayout *main_layout_;
    QVBoxLayout *single_harmonic_layout_;
    QVBoxLayout *multiple_from_harmonic_layout_;
    QVBoxLayout *multiple_to_harmonic_layout_;

    ..

private slots:
    void VerifyInputsforRun(int state);
    void VerifyInputs();
    void CreatePlotsFolder();
    void UpdateMCASEsetfile(bool is_single_harmonic, int start_harm, int end_harm);
    void RunMCASEandPlots(int start_harm, int end_harm);
    void UpdateModeShapeScreenDropdowns();
    void RunScriptsMultipleHarmonic();
    void RunScriptsSingleHarmonic(int num_harmonic);
    void UpdateHarmNumberUIF(int harm_value);
    void AddInputstoUIF();
    void ModifyQMPAction();

public slots:
    void VerifyRunConditions();
    void RunAnalysis();
    void HandleProgress(int value);
    void HandleStatusUpdate(const QString &status);

signals: 
    void VerifySignal();
    void StartTimerBar();
    void Progress(int value);
    void UpdateStatus(const QString &status);
    void AnalysisCompleted();
    void SetCampbellPlots(const bool state);


};

#endif // INPUTS_SCREEN_TAB_H
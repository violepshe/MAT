#include ..

QComboBox* ResultsScreenTab::harmonic_mode_drdopdown = nullptr;
QComboBox* ResultsScreenTab::mode_number_dropdown = nullptr;
QComboBox* ResultsScreenTab::image_result_dropdown = nullptr;
QComboBox* ResultsScreenTab::component_name_dropdown = nullptr;
ImageCarousel* ResultsScreenTab::carousel = nullptr;
bool ResultsScreenTab::user_interaction_dropdowns = true;
bool ResultsScreenTab::successfully_filled_dropdowns = false;
QStandardItemModel* ResultsScreenTab::model_harmonic = nullptr;
CustomDelegateCheckBox* ResultsScreenTab::delegate_harmonic = nullptr;
QStandardItemModel* ResultsScreenTab::model_mode = nullptr;
CustomDelegateCheckBox* ResultsScreenTab::delegate_mode = nullptr;
QStandardItemModel* ResultsScreenTab::model_mode_selected_for_harmonic = nullptr;

int ResultsScreenTab::current_mode_index = -1; // initialize in -1 value as theres still no current mode interacted with
void ResultsScreenTab::AddHarmonicModeDropdownItems(int start_harm, int end_harm) {
    // Add harmonic dropdown items for the first time, when user clicks on a 'Next button' after giving user inputs
    // after that, this function won't run again

    harmonic_mode_dropdown->clear();
    QString number_harm;

    model_harmonic = new QStandardItemModel();
    delegate_harmonic = new CustomDelegateCheck
}
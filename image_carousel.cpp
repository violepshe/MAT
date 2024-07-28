#include <iostream>
#include <QCheckBox>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QMap>
#include <QVBoxLayout>
#include <harmonic_mode_utilities.h>
#include "image_carousel.h"
#include "results_screen_tab"

QStringList images = {};
int current_index_public;
int ImageCarousel::current_harmonic_number;
int ImageCarousel::current_mode_number;
QString ImageCarousel::current_image_result_prefix;
HarmonicModeMap ImageCarousel::harmonic_mode_map;
bool ImageCarousel::image_dropdown_was_selected = false;
QCheckBox* ImageCarousel::include_checkbox_;
QCheckBox* ImageCarousel::mode_checkbox_;

ImageCarousel::ImageCarousel(QWidget *parent) : QWidget(parent), current_index_(0) {
    //initialize several variables..

    //[code here..]

    // create connections
    connect(mode_checkbox_, &QCheckBox::stateChanged, this, &ImageCarousel::SelectModeForThisHarmonicCheckOn);
    connect(this, &ImageCarousel::ActivateImageDropdownUpdate, this, &ImageCarousel::UpdateCurrentImageDisplayed);
    connect(ResultsScreenTab::image_result_dropdown, &QComboBox::currentTextChanged, this, &ImageCarousel::SelectImageResultDropdownItem);
    connect(ResultsScreenTab::harmonic_mode_dropdown, &QComboBox::currentTextChanged, this, &ImageCarousel::SelectHarmonicDropdownItem);
    connect(ResultsScreenTab::mode_number_dropdown, &QComboBox::currentTextChanged, this, &ImageCarousel::SelectModeNumberDropdownItem);
    connect(ResultsScreenTab::component_name_dropdown, &QComboBox::currentTextChanged, this, &ImageCarousel::SelectComponentNameDropdownItem);

    // more code here..
}

void ImageCarousel::SelectModeForThisHarmonicCheckOn(int state) {
    int select_mode_value = 2;
    int current_harmonic = (ResultsScreenTab::harmonic_mode_dropdown->currentText()).toInt();
    int current_mode = (ResultsScreenTab::mode_number_dropdown->currentText()).toInt();
    int current_index_harmonic = ResultsScreenTab::harmonic_mode_dropdown->currentIndex();
    int current_index_mode = ResultsScreenTab::mode_number_dropdown->currentIndex();
    int current_mode_index = ResultsScreenTab::current_mode_index;

    if (current_index_harmonic >= 0) {
        QStandardItem *item_harmonic = ResultsScreenTab::model_harmonic->item(current_index_harmonic);
        if (state == select_mode_value) {
            item_harmonic->setData(true, Qt::UserRole + 1);
        } else {
            item_harmonic->setData(falsa, Qt::UserRole + 1);
        }
        ResultsScreenTab::harmonic_mode_dropdown->blockSignals(true);
        ResultsScreenTab::harmonic_mode_dropdown->update();
        ResultsScreenTab::harmonic_mode_dropdown->blockSignals(false);
    }

    if (current_index_mode >= 0) {
        QStandardItem *item_mode = ResultsScreenTab::model_mode->item(current_index_mode);
        if (state == select_mode_value) {
            if(ResultsScreenTab::current_mode_index >= 0 && ResultsScreenTab::current_mode_index != current_index_mode) {
                QStandardItem *prev_item_bold = ResultsScreenTab::model_mode->item(ResultsScreenTab::current_mode_index);
                prev_item_bold->setData(false, Qt::UserRole + 1);
            }
            item_mode->setData(true, Qt::UserRole + 1);
            ResultsScreenTabb::current_mode_index = current_index_mode;
            UpdateCheckModeModel(QString::number(current_harmonic), QString::number(current_mode));
        } else {
            if (ResultsScreenTab::current_mode_index == current_index_mode) {
                item_mode->setData(false, Qt::UserRole + 1);
                ResultsScreenTab::current_mode_index = -1;
                UpdateCheckModeModel(QString::number(current_harmonic), "0");
            }
        }
        ResultsScreenTab::mode_number_dropdown->blockSignals(true);
        ResultsScreenTab::mode_number_dropdown->update();
        ResultsScreenTab::mode_number_dropdown->blockSignals(false);
    }
}

void ImageCarousel::UpdateCheckModeModel(const QString &harmonic_number, const QString &mode_number) {
    if(!ResultsScreenTab::model_mode_selected_for_harmonic) {
        return; // model not initialized, return
    }

    for(int row= 0; row < ResultsScreenTab::model_mode_selected_for_harmonic->rowCount(); ++row) {
        QModelIndex harm_index = ResultsScreenTab::model_mode_selected_for_harmonic->index(row, 0); // column 0 is harmonic num column
        if (ResultsScreenTab::model_mode_selected_for_harmonic->data(harm_index).toString() == harmonic_number) {
            // Found the row with the matching harmonic number, update the mode number in the 2nd column
            QModelIndex model_index = ResultsScreenTab::model_mode_selected_for_harmonic->index(row, 1);
            ResultsScreenTab::model_mode_selected_for_harmonic->setData(mode_index, mode_number);
            return;
        }
    }

    // If the harmonic num is not found, set message
    qDebug() << "Harm number not found in ImageCarousel::UpdateCheckModeModel"
}

void ImageCarousel::SelectHarmonicDropdownItem() {
    QString current_harmonic = (ResultsScreenTab::harmonic_mode_dropdown->currentText());
    QString current_mode = (ResultsScreenTab::mode_number_dropdown->currentText());
    int current_mode_int = current_mode.toInt();
    int current_harmonic_int = current_harmonic.toInt();

    if(ResultsScreenTab::successfully_filled_dropdowns == true) {
        ImageCarousel::image_dropdown_was_selected = false;
        if (ResultsScreenTab::harmonic_mode_dropdown != nullptr) {
            int current_index = ResultsScreenTab::harmonic_mode_dropdown->current_index();
            if (current_index >= 0) {
                QStandardItem *item = ResultsScreenTab::model_harmonic->item(current_index);
                bool is_checked = item->data(Qt::UserRole + 1).toBool();
                mode_checkbox_->blockSignals(true);
                mode_checkbox_->setChecked(is_checked);
                mode_checkbox_->blockSignals(false);

                // Check if for this updated harmonic number, there's already a mode selected in model_selected_for_harmonic structure,
                // if there is, uncheck the previous mode that was checked and check the corresponding mode number to this updated harmonic. 
                // If there is none, uncheck all the dropdown items
                QString current_checked_mode_number = GetAssignedModeNumber(current_harmonic);
                int current_checked_mode_number_int = current_checked_mode_number.toInt();

                QString mode_number_checked;
                QStandardItem *item_mode;

                for (int i=0; i<= (InputsScreenTab::number_of_modes_sspi - 1); i++) {
                    item_mode = ResultsScreenTab::model_mode->item(i);
                    item_mode->setData(false, Qt::UserRole + 1);
                    if (current_checked_mode_number != "0") {
                        if ((i+1)== current_checked_mode_number_int) {
                            item_mode->setData(true, Qt::UserRole + 1);
                        }
                    }
                }
                ResultsScreenTab::mode_number_dropdown->blockSignals(true);
                ResultsScreenTab::mode_number_dropdown->update();
                ResultsScreenTab::mode_number_dropdown->blockSignals(false);
            }
        }
    }
}

void ImageCarousel::SelectModeNumberDropdownItem() {
    QString current_harmonic = (ResultsScreenTab::harmonic_mode_dropdown->currentText());
    QString current_mode = (ResultsScreenTab::mode_number_dropdown->currentText());
    int current_mode_int = current_mode.toInt();
    int current_harmonic_int = current_harmonic.toInt();

    if(ResultsScreenTab::successfully_filled_dropdowns == true) {
        ImageCarousel::image_dropdown_was_selected = false;
        if (ResultsScreenTab::mode_number_dropdown != nullptr) {
            int current_index = ResultsScreenTab::mode_number_dropdown->current_index();
            if (current_index >= 0) {
                QStandardItem *item = ResultsScreenTab::model_mode->item(current_index);
                bool is_checked = item->data(Qt::UserRole + 1).toBool();
                mode_checkbox_->blockSignals(true);
                mode_checkbox_->setChecked(is_checked);
                mode_checkbox_->blockSignals(false);
            }
        }
    }
}

QString ImageCarousel::GetAssignedModeNumber(const QString &harmonic_number) {
    QModelIndexList indexes = ResultsScreenTab::model_mode_selected_for_harmonic->match(ResultsScreenTab::model_mode_selected_for_harmonic->index(0,0), Qt::DisplayRole, harmonic_number, -1, Qt::MatchExactly);
    if (!indexes.isEmpty()) {
        QModelIndex index = indexes.first();
        return ResultsScreenTab::model_mode_selected_for_harmonic->item(index.row(), 1)->text();
    }
    return QString(); //returns empty string
    qDebug() << "Returning empty string";
}

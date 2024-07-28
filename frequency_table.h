#ifndef FREQUENCY_TABLE_H
#define FREQUENCY_TABLE_H

#include <QMap>
#include <QDebug>

class FrequencyTable {
 public:
    using HarmonicNumber = int; // Row title
    using ModeNumber = int;     // Column title
    using Frequency = double;   // Frequency in Hz
    using FrequencyList = QList<Frequency>; // List to store frequencies

    // Constructor
    FrequencyTable();

    // Destructor
    ~FrequencyTable();

    // Adds a frequency value to the table
    void AddFrequency(HarmonicNumber harmonic_number, ModeNumber mode_number, Frequency frequency);

    // Gets a frequency value from the table
    Frequency GetFrequency(HarmonicNumber harmonic_number, ModeNumber mode_number) const;

    // Save frequency values for each harmonic from a given text file path
    FrequencyList SaveFrequencyValuesForEachHarmonic(const QString& text_file_path);

    void StoreAllFrequencies();

 private:
    QMap<HarmonicNumber, QMap<ModeNumber, Frequency>> table_;
};

#endif // FREQUENCY_TABLE_H
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QList>
#include <QString>

FrequencyTable::FrequencyTable() {
    
}


// Function to add frequency
void FrequencyTable::AddFrequency(HarmonicNumber harmonic_number, ModeNumber mode_number, Frequency frequency) {
    table_[harmonic_number][mode_number] = frequency;
}

// Function to get frequency
Frequency FrequencyTable::GetFrequency(HarmonicNumber harmonic_number, ModeNumber mode_number) const {
    if (table_.contains(harmonic_number) && table_[harmonic_number].contains(mode_number)) {
        return table_[harmonic_number][mode_number];
    } else {
        qWarning() << "Frequency not found for harmonic number" << harmonic_number << "and mode number" << mode_number;
        return -1.0; // or some error value
    }
}

// Function to save frequency values for each harmonic
FrequencyTable::FrequencyList FrequencyTable::SaveFrequencyValuesForEachHarmonic(const QString& text_file_path) {
    QFile file(text_file_path);
    FrequencyList frequency_list;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file" << text_file_path;
        return frequency_list;
    }

    QTextStream in(&file);
    QRegularExpression regex(R"(^CASE IDNT \$\s+(\d+)\s+VALUE\s+([\d.]+))");

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.startsWith('$')) {
            continue; // Skip commented lines
        }

        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch()) {
            int mode_number = match.captured(1).toInt();
            double frequency = match.captured(2).toDouble();
            frequency_list.append(frequency);

            // Assuming harmonic_number is known or passed in another way
            int harmonic_number = /*...*/; // Determine how to get the harmonic number
            AddFrequency(harmonic_number, mode_number, frequency);
        }
    }

    file.close();
    return frequency_list;
}

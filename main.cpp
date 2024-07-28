#include "main_window.h"
#include <QApplication>
#include <QSizePolicy>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    window.setWindowTitle("Tool");
    window.setGeometry(400,200,850, 810);
    window.show();
    window.setFixedSize(window.width(), window.height());
    return a.exec();
}
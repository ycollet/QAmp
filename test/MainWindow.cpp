#include "MainWindow.h"
#include "QAmp.h"

MainWindow::MainWindow() {
  setCentralWidget(new QAmp());
}

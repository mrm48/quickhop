#include <QApplication>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>

#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <qdialog.h>

#include "mainwindow.h"

QCheckBox *isVirtualMachine;
QCheckBox *installDevTools;
QCheckBox *installGameTools;
QPushButton *continueInstall;
QString *devTools;
QString *gameTools;

bool debug = true;

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent)
{
  setCentralWidget(createOptionBox());
}

void MainWindow::setupActions()
{
    KStandardAction::quit(qApp, &QCoreApplication::quit, actionCollection());
    setupGUI(Default, "quickhopui.rc");
}

QGroupBox* MainWindow::createOptionBox()
{

  // Instantiate group
  QGroupBox *groupBox = new QGroupBox(tr("Options"));

  continueInstall = new QPushButton("Install", this);
  connect(continueInstall,SIGNAL(clicked(bool)), this, SLOT(installSelected()));
  

  // Instantiate checkboxes for various filters / options
  isVirtualMachine = new QCheckBox("Virtual Machine");
  installDevTools = new QCheckBox("Install Development Tools");
  
  // Set up Dropdowns

  // Set up vertical layout
  // 1. Checkboxes 2. labels and dropdowns
  QVBoxLayout *vbox = new QVBoxLayout;

  vbox->addWidget(isVirtualMachine);
  vbox->addWidget(installDevTools);
  vbox->addWidget(continueInstall);

  // return layout - vbox is wrapper around 1. Checkboxes and 2. horizontal layouts containing labels and dropdowns
  groupBox->setLayout(vbox);
  return groupBox;
}

// Action when install button is clicked
void MainWindow::installSelected()
{

  if(debug){

    QMessageBox msgBox;
    msgBox.setText("Install is clicked");    
    msgBox.exec();

  }

}

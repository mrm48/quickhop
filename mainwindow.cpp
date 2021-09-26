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

bool debug = false;

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
  

  // Instantiate checkboxes for various filters / options
  //QCheckBox *flipInputVert = new QCheckBox("Flip Input Vertically");
  isVirtualMachine = new QCheckBox("Virtual Machine");
  //connect(isVirtualMachine,SIGNAL(clicked(bool)), this, SLOT(updateFlip(bool)));
  installDevTools = new QCheckBox("Install Development Tools");
  //connect(installDevTools,SIGNAL(clicked(bool)), this, SLOT(updateGreyscale(bool)));
  
  // Set up Dropdowns

  // Set up vertical layout
  // 1. Checkboxes 2. labels and dropdowns
  QVBoxLayout *vbox = new QVBoxLayout;

  vbox->addWidget(continueInstall);
  vbox->addWidget(isVirtualMachine);
  vbox->addWidget(installDevTools);
 
  // return layout - vbox is wrapper around 1. Checkboxes and 2. horizontal layouts containing labels and dropdowns
  groupBox->setLayout(vbox);
  return groupBox;
}

// Action when flip checkbox is toggled
// TODO: Add call to v4l2 that will flip the camera
void MainWindow::installSelected(bool value)
{

  if(value)
  {

    QProcess flipProcess;
    if(flipProcess.waitForStarted()){
      flipProcess.waitForReadyRead();
    }
    installDevTools->setEnabled(false);
    
    if(debug){

      QMessageBox msgBox;
      msgBox.setText("Install is clicked");    
      msgBox.exec();

    }

  }

  else
  {

    if(debug)
    {

      QMessageBox msgBox;
      msgBox.setText("Flip is unchecked"); 
      msgBox.exec();

    }

  }
}

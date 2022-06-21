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
#include <QFile>
#include <QStandardPaths>
#include <QDir>

#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qstandardpaths.h>

#include "mainwindow.h"
QComboBox *targetDistro;
QComboBox *targetDE;
QCheckBox *isVirtualMachine;
QCheckBox *installDevTools;
QCheckBox *installGameTools;
QPushButton *continueInstall;
QString *devTools;
QString *gameTools;
QString *scriptHeader;
QString *script;

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
  
  // Instantiate comboboxes and checkboxes for various options
  // Distro
  targetDistro = new QComboBox();
  targetDistro->addItem("Ubuntu");
  targetDistro->addItem("Debian Stable");
  targetDistro->addItem("Arch");
  targetDistro->addItem("Fedora");
  targetDistro->addItem("openSUSE Tumbleweed");

  // Desktop Environment / WM
  targetDE = new QComboBox();
  targetDE->addItem("KDE Plasma");
  targetDE->addItem("GNOME");
  targetDE->addItem("XFCE");
  targetDE->addItem("Qtile");
  targetDE->addItem("Openbox");

  isVirtualMachine = new QCheckBox("Virtual Machine");
  installDevTools = new QCheckBox("Install Development Tools");
  installGameTools = new QCheckBox("Install Common Gaming Applications");

  // Set up vertical layout
  // 1. Checkboxes 2. labels and dropdowns
  QVBoxLayout *vbox = new QVBoxLayout;

  vbox->addWidget(targetDistro);
  vbox->addWidget(targetDE);
  vbox->addWidget(isVirtualMachine);
  vbox->addWidget(installDevTools);
  vbox->addWidget(installGameTools);
  vbox->addWidget(continueInstall);

  // return layout - vbox is wrapper around 1. Checkboxes and 2. horizontal layouts containing labels and dropdowns
  groupBox->setLayout(vbox);
  return groupBox;
}

// Action when install button is clicked
void MainWindow::installSelected()
{
  scriptHeader = new QString("#!/usr/bin/env bash \n");
  
  script = scriptHeader;

  this->populateDistroUpdate();

  auto path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  QDir *cachedir = new QDir(path);

  if (debug){

    QMessageBox pathMsgBox;
    pathMsgBox.setText(cachedir->absolutePath());
    pathMsgBox.exec();

    QMessageBox msgBox;
    msgBox.setText("Install is clicked");
    msgBox.exec();

  }

  if (cachedir->mkpath(cachedir->absolutePath()) && QDir::setCurrent(cachedir->absolutePath())){
    
      QFile file("test.out");
      file.open(QIODevice::WriteOnly);

      file.write(script->toUtf8());

      file.close();

  }
}

// Add distribution update
void MainWindow::populateDistroUpdate(){
  if(targetDistro->currentText() == "Ubuntu" || targetDistro->currentText() == "Debian"){
    script->append("\nsudo apt update ; sudo apt upgrade");
  }
  else if(targetDistro->currentText() == "Fedora"){
    script->append("\nsudo dnf update");
  }
  else if(targetDistro->currentText() == "openSUSE Tumbleweed"){
    script->append("\nsudo zypper dup");
  }
}

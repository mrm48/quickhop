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

// UI Elements
QComboBox *targetDistro;
QComboBox *targetDE;
QCheckBox *isVirtualMachine;
QCheckBox *installDevTools;
QCheckBox *installGameTools;
QPushButton *continueInstall;

// Script strings and distro specifics
QString *scriptHeader;
QString *script;
QString *packageManager;
QString *installCommand;
QString *updateCommand;
QString *syncCommand;

// Debugging
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
  connect(continueInstall,SIGNAL(clicked(bool)), this, SLOT(installSelected()));
  
  // Instantiate comboboxes and checkboxes for various options
  // Distro
  targetDistro = new QComboBox();
  targetDistro->addItem("Ubuntu");
  targetDistro->addItem("Debian");
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

  this->populateDistroSpecifics();

  this->populateDistroUpdate();

  // Save in $HOME/.cache/quickhop
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

  if (installDevTools->isChecked()){
    QMessageBox devMsgBox;
    devMsgBox.setText("Installing Dev Tools");
    devMsgBox.exec();
    this->populateDevTools();
  }

  if (installGameTools->isChecked()){
    QMessageBox gameMsgBox;
    gameMsgBox.setText("Installing Gaming Extras");
    gameMsgBox.exec();
    this->populateGameExtras();
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
  script->append("\nsudo ");
  if(targetDistro->currentText() == "Ubuntu" || targetDistro->currentText() == "Debian"){
    script->append(packageManager);
    script->append(" ");
    script->append(syncCommand);
    script->append(" ; sudo ");
    script->append(packageManager);
    script->append(" ");
    script->append(updateCommand);
  }
  else {
    script->append(packageManager);
    script->append(" ");
    script->append(updateCommand);
  }
}

// Add distribution specific commands
void MainWindow::populateDistroSpecifics(){
  if(targetDistro->currentText() == "Ubuntu" || targetDistro->currentText() == "Debian"){
    packageManager = new QString("apt");
    installCommand = new QString("install");
    syncCommand = new QString("update");
    updateCommand = new QString("upgrade");
  }
  else if(targetDistro->currentText() == "Fedora"){
    packageManager = new QString("dnf");
    installCommand = new QString("install");
    updateCommand = new QString("update");
  }
  else if(targetDistro->currentText() == "openSUSE Tumbleweed"){
    packageManager = new QString("zypper");
    installCommand = new QString("in");
    updateCommand = new QString("dup");
  }
  else if(targetDistro->currentText() == "Arch"){
    packageManager = new QString("pacman");
    installCommand = new QString("-S");
    updateCommand = new QString("-Syu");
  }
}

void MainWindow::populateDevTools(){
  this->installNativePackage(new QString("emacs"));
  this->installNativePackage(new QString("extra-cmake-modules"));
  this->installNativePackage(new QString("git"));
  if(targetDE->currentText() == "KDE Plasma"){
    this->installNativePackage(new QString("kate"));
  }
}

void MainWindow::populateGameExtras(){
  this->installFlatpak(new QString("steam"));
}

void MainWindow::installNativePackage(QString* package){
  script->append("\nsudo ");
  script->append(packageManager);
  script->append(" ");
  script->append(installCommand);
  script->append(" ");
  script->append(package);
}

void MainWindow::installFlatpak(QString* package){
  script->append("\nflatpak install ");
  script->append(package);
}

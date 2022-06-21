#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <KXmlGuiWindow>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
 
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void installSelected();
    void populateDistroUpdate();

private:
    void setupActions();
    QGroupBox *createOptionBox();

};

#endif // MAINWINDOW_H

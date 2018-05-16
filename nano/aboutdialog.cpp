#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->apptitle->setText(QApplication::applicationName());
    ui->versionlabel->setText("Version " + QApplication::applicationVersion());

    QString platformName;
    if(QApplication::platformName() == "xcb")          platformName = "Linux (X11)";
    else if(QApplication::platformName() == "windows") platformName = "Windows";
    else if(QApplication::platformName() == "cocoa")   platformName = "macOS (Cocoa)";
    else                                               platformName = QApplication::platformName();

    ui->qtadvertizing->setText(QString("Using Qt ") + qVersion() + QString(" on ") + platformName);
}


AboutDialog::~AboutDialog() {
    delete ui;
}

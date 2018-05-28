#include "bcastduplicatewarning.h"
#include "ui_bcastduplicatewarning.h"

BcastDuplicateWarning::BcastDuplicateWarning(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BcastDuplicateWarning)
{
    ui->setupUi(this);
}

BcastDuplicateWarning::~BcastDuplicateWarning()
{
    delete ui;
}

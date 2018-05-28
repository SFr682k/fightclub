#ifndef BCASTDUPLICATEWARNING_H
#define BCASTDUPLICATEWARNING_H

#include <QWidget>

namespace Ui {
class BcastDuplicateWarning;
}

class BcastDuplicateWarning : public QWidget
{
    Q_OBJECT

public:
    explicit BcastDuplicateWarning(QWidget *parent = 0);
    ~BcastDuplicateWarning();

private:
    Ui::BcastDuplicateWarning *ui;
};

#endif // BCASTDUPLICATEWARNING_H

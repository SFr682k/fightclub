#ifndef PROBLEMADAPTER_H
#define PROBLEMADAPTER_H


class ProblemAdapter
{
public:
    ProblemAdapter();

signals:
    void activateProblemComboBox(bool);

public slots:
    void problemChanged(int);

};

#endif // PROBLEMADAPTER_H

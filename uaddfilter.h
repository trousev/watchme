#ifndef UADDFILTER_H
#define UADDFILTER_H

#include <QDialog>

namespace Ui {
class uAddFilter;
}

class uAddFilter : public QDialog
{
    Q_OBJECT

public:
    explicit uAddFilter(QWidget *parent = 0);
    ~uAddFilter();

private:
    Ui::uAddFilter *ui;
};

#endif // UADDFILTER_H

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <QWidget>

namespace Ui {
class Transformer;
}

class Transformer : public QWidget
{
    Q_OBJECT

public:
    explicit Transformer(QWidget *parent = 0);
    ~Transformer();

private:
    Ui::Transformer *ui;
    void loadEntries();
    void loadRules();
};

#endif // TRANSFORMER_H

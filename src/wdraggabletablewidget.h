#ifndef WDRAGGABLETABLEWIDGET_H
#define WDRAGGABLETABLEWIDGET_H

#include <QTableWidget>

class WDraggableTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit WDraggableTableWidget(QWidget *parent = 0);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event) ;
signals:

public slots:
private:
    void moveRow(int from, int to);
    int _dragging_row;
};

#endif // WDRAGGABLETABLEWIDGET_H

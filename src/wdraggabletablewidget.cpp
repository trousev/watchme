#include "wdraggabletablewidget.h"
#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
WDraggableTableWidget::WDraggableTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDragDropOverwriteMode(false);
    setDropIndicatorShown(true);

    setSelectionMode(QAbstractItemView::SingleSelection) ;
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void WDraggableTableWidget::moveRow(int from, int to)
{
    if(from == to) return ;
    QList<QTableWidgetItem*> items;
    for(int i=0; i<columnCount(); i++)
        items << takeItem(from, i);
    removeRow(from);
    if(to > from) to --;
    insertRow(to);
    for(int i=0; i<columnCount(); i++)
        setItem(to,i,items[i]);
}

void WDraggableTableWidget::dropEvent(QDropEvent *event) {
    if ((event->source() == this ) && (event->dropAction() == Qt::MoveAction || dragDropMode() == QAbstractItemView::InternalMove))
    {
        QTableWidgetItem * item = itemAt(event->pos());
        int to = rowCount();
        if(item )
            to = item->row();
        moveRow(_dragging_row, to);
    }
}

void WDraggableTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(currentRow() == -1) return ;
    if ( event->source() == this ) // same table, move entry
    {
        event->setDropAction( Qt::MoveAction );
        _dragging_row = currentRow();
        event->accept();
    }
    else // different table, add entry
    {
        printf("Different table\n");
        event->acceptProposedAction();
    }

}

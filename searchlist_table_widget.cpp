#include "searchlist_table_widget.h"

searchlist_table_widget::searchlist_table_widget(QObject *parent)
{

}

void searchlist_table_widget::keyPressEvent(QKeyEvent *event)
{
    // If Ctrl-C typed
     if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
     {
         QModelIndexList cells = selectedIndexes();

         qSort(cells); // Necessary, otherwise they are in column order

         QString text;

         int currentRow = 0; // To determine when to insert newlines
         int first_row=0;
         foreach (const QModelIndex& cell, cells) {
             if (text.length() == 0) {
                 // First item
                 first_row = cell.row();
             } else if (cell.row() != currentRow) {
                 // New row
                 text += '\n';
             } else {
                 // Next cell
                 text += '\t';
             }
             currentRow = cell.row();
             if(cell.column() != 0){
                QLabel *datalabel = (QLabel *)this->cellWidget(cell.row(),cell.column());
                text += datalabel->text();
             }else {
                 text += "  ";
             }

         }
         QString header_data;
         foreach (const QModelIndex& cell, cells) {
             if(header_data.length() == 0){

             }else if (first_row != cell.row()){
                 header_data += '\n';
                 break;
             }else {
                 header_data += '\t';
             }
             if(cell.column() != 0){
                header_data += model()->headerData(cell.column(),Qt::Horizontal).toString();
             }else {
                 header_data += "   ";
             }

         }
         text = header_data + text;


         QApplication::clipboard()->setText(text);
     }
}

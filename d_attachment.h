#ifndef D_ATTACHMENT_H
#define D_ATTACHMENT_H

#include <QDialog>

namespace Ui {
class D_attachment;
}

class D_attachment : public QDialog
{
    Q_OBJECT

public:
    explicit D_attachment(QWidget *parent = 0);
    ~D_attachment();

private:
    Ui::D_attachment *ui;
};

#endif // D_ATTACHMENT_H

#ifndef D_IMAGE_SIZE_H
#define D_IMAGE_SIZE_H

#include <QDialog>

namespace Ui {
class D_image_size;
}

class D_image_size : public QDialog
{
    Q_OBJECT

public:
    explicit D_image_size(QWidget *parent = 0);
    double height;
    double width;
    ~D_image_size();

    double getHeight() const;


    double getWidth() const;

    void setHeight(double value);

    void setWidth(double value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::D_image_size *ui;
};

#endif // D_IMAGE_SIZE_H

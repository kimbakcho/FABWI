#ifndef D_IMAGE_SIZE_H
#define D_IMAGE_SIZE_H

#include <QDialog>
#include <QMdiSubWindow>

namespace Ui {
class D_image_size;
}

class D_image_size : public QDialog
{
    Q_OBJECT

public:
    explicit D_image_size(QWidget *parent = 0);
    QString filepath;
    QImage img;
    double height;
    double width;
    ~D_image_size();

    double getHeight() const;


    double getWidth() const;

    void setHeight(double value);

    void setWidth(double value);

    QString getFilepath() const;

    void setFilepath(const QString &value);

    QImage getImg() const;
    void setImg(const QImage &value);

    void image_pack();

private slots:
    void on_buttonBox_accepted();


    void on_size_edit_toggled(bool checked);

    void on_LE_height_editingFinished();

    void on_LE_width_editingFinished();

    void on_LE_height_textEdited(const QString &arg1);

    void on_LE_width_textEdited(const QString &arg1);

private:
    Ui::D_image_size *ui;
};

#endif // D_IMAGE_SIZE_H

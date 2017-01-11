#ifndef B_IMAGE_RESIZE_WIDGET_H
#define B_IMAGE_RESIZE_WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QPainter>
#include <QLineEdit>
namespace Ui {
class B_image_resize_widget;
}

class B_image_resize_widget : public QWidget
{
    Q_OBJECT

public:
    explicit B_image_resize_widget(QWidget *parent = 0);
    QString filepath;
    QImage img;
    QLineEdit *LE_width;
    QLineEdit *LE_height;
    bool size_edit_mode;
    bool mouse_press_flag;
    ~B_image_resize_widget();

    void setImg(const QImage &value);

    QImage getImg() const;

    void setFilepath(const QString &value);

    QString getFilepath() const;

    bool ready_flag;

    bool getReady_flag() const;
    void setReady_flag(bool value);

    QLineEdit *getLE_width() const;
    void setLE_width(QLineEdit *value);

    QLineEdit *getLE_height() const;
    void setLE_height(QLineEdit *value);

    bool getSize_edit_mode() const;
    void setSize_edit_mode(bool value);

private:

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    Ui::B_image_resize_widget *ui;
};

#endif // B_IMAGE_RESIZE_WIDGET_H

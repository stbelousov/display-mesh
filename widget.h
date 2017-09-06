#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

private slots:
    void on_closeButton_clicked();
    void on_checkGeometry_clicked();
    void on_checkTemperature_clicked();
    void on_checkNormals_clicked(bool checked);
};

#endif // WIDGET_H

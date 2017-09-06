#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_closeButton_clicked() 
{
    this->close();
}

void Widget::on_checkGeometry_clicked()
{
    ui->view->setMarkRegions(true);
}

void Widget::on_checkTemperature_clicked()
{
    ui->view->setMarkRegions(false);
}

void Widget::on_checkNormals_clicked(bool checked)
{
    ui->view->setDisplayNormals(checked);
}

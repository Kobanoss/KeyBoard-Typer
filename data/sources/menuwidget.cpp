#include "../headers/menuwidget.h"
#include "ui_menuwidget.h"

MenuWidget::MenuWidget(QWidget *parent): QWidget(parent), ui(new Ui::MenuWidget) {
    ui->setupUi(this);

}

MenuWidget::~MenuWidget() {
    delete ui;
}


void MenuWidget::on_pushButton_main_start_clicked() {
    AnotherWindow = new InputWidget();

    connect(AnotherWindow, &InputWidget::changeWindow, this, &MenuWidget::show);
    connect(this, &MenuWidget::sendValues,AnotherWindow, &InputWidget::setValues);
    this->lang = ui->radioButton_ru->isChecked()?"ru":"";
    this->lang += ui->radioButton_en->isChecked()?"en":"" ;
    this->lang += ui->radioButton_spec->isChecked()?"spec":"";

    this->type = ui->comboBox_main_type->currentText();

    this->execution_time = ui->spinBox_main_time->text();
    qDebug() << "wanna see " << execution_time;

    (ui->lineEdit_main_username->text() == "")?this->username="default":this->username = ui->lineEdit_main_username->text();

    AnotherWindow->show();
    emit sendValues(username, lang, type, execution_time);
    this->close();
}


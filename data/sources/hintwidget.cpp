#include "../headers/hintwidget.h"
#include "ui_hintwidget.h"

// Конструктор класса нашей подсказки
HintWidget::HintWidget(QWidget *parent) : QWidget(parent), ui(new Ui::HintWidget) {
    ui->setupUi(this);
}

// Деструктор
HintWidget::~HintWidget() {
    delete ui;
}

// Слот, вызываемый при изменении состояния checkbox'a с галочкой
void HintWidget::on_checkBox_hint_disable_auto_stateChanged(int state) {
    if (state == 0)
        auto_hint_status = true;
    else
        auto_hint_status = false;


}

// Обновление содержимого файла настроек пользователя
void HintWidget::update_settings_file() {
    QJsonDocument doc = global.loadJson(global.settings_filename);
    QJsonObject settings = doc.object();

    settings["auto_hint"] = auto_hint_status;
    settings["user"] = true;


    doc.setObject(settings);
    global.saveJson(global.settings_filename, doc);

}

// Действие при закрытии окна на X(красный крестик)
void HintWidget::closeEvent(QCloseEvent *event) {
    update_settings_file();
    event->accept();
}

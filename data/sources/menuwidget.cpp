#include "../headers/menuwidget.h"
#include "ui_menuwidget.h"

// Конструктор класса нашего виджета
MenuWidget::MenuWidget(QWidget *parent): QWidget(parent), ui(new Ui::MenuWidget) {
    ui->setupUi(this);
    global = new Globals();
    updateStats();

    ui->tableWidget_main_stat->horizontalHeader()->setStyleSheet("::section { background-color:#181818;}");
}

// Деструктор
MenuWidget::~MenuWidget() {
    delete ui;
}

// Обновление результатов таблицы статистики
void MenuWidget:: updateStats() {
   QJsonDocument json_dump = global->loadJson(global->stat_filename);
   QJsonObject json_obj = json_dump.object();

   QVector<QString> usernames = json_obj.keys().toVector();

   QMap<QString, QVariantMap> users_data;
   for (QString name: usernames) {
       users_data[name] = json_obj[name].toVariant().toMap();
       qDebug() << users_data[name];

   }
   qDebug() << users_data;
   for (int i=0; i < usernames.length(); i++) {
       ui->tableWidget_main_stat->setItem(i, 0, new QTableWidgetItem(usernames[i]));
       ui->tableWidget_main_stat->setItem(i, 1, new QTableWidgetItem(users_data[usernames[i]]["type"].toString()));
       ui->tableWidget_main_stat->setItem(i, 2, new QTableWidgetItem(users_data[usernames[i]]["time"].toString()));
       ui->tableWidget_main_stat->setItem(i, 3, new QTableWidgetItem(users_data[usernames[i]]["mistakes"].toString()));
       ui->tableWidget_main_stat->setItem(i, 4, new QTableWidgetItem(users_data[usernames[i]]["speed"].toString()));
   }

}


// Обработка действий при нажатии на кнопку старт
// (сбор необходимых данных и формирование нового окна для теста, закрытие текущего)
void MenuWidget::on_pushButton_main_start_clicked() {
    AnotherWindow = new InputWidget();

    connect(AnotherWindow, &InputWidget::changeWindow, this, &MenuWidget::show);
    connect(AnotherWindow, &InputWidget::changeWindow, this, &MenuWidget::updateStats);
    connect(this, &MenuWidget::sendValues,AnotherWindow, &InputWidget::setValues);
    this->lang = ui->radioButton_ru->isChecked()?"ru":"";
    this->lang += ui->radioButton_en->isChecked()?"en":"" ;
    this->lang += ui->radioButton_spec->isChecked()?"spec":"";

    this->type = ui->radioButton_spec->isChecked()?"Одиночные символы":ui->comboBox_main_type->currentText();

    this->execution_time = ui->spinBox_main_time->text();

    (ui->lineEdit_main_username->text() == "")?this->username="default":this->username = ui->lineEdit_main_username->text();

    AnotherWindow->show();
    emit sendValues(username, lang, type, execution_time);
    this->close();
}



// Блок для установки доступа только к одиночным символам при выборе спец. символов, какие могут быть слова из собачек(@) и звездочек(*)?
void MenuWidget::on_radioButton_spec_clicked() {
    ui->comboBox_main_type->setDisabled(true);
    ui->comboBox_main_type->setCurrentIndex(0);
}


void MenuWidget::on_radioButton_en_clicked() {
    ui->comboBox_main_type->setDisabled(false);
}


void MenuWidget::on_radioButton_ru_clicked() {
    ui->comboBox_main_type->setDisabled(false);
}


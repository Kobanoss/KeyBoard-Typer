#include "../headers/inputwidget.h"
#include "ui_inputwidget.h"

// Конструктор нашего виджета для ввода текста
InputWidget::InputWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InputWidget) {
    ui->setupUi(this);
    global = new Globals();

    timer = new QTimer();
    update_timer = new QTimer();
    connect(update_timer,&QTimer::timeout,this, &InputWidget::update);
    connect(timer, &QTimer::timeout, this, &InputWidget::stopTest);


    ui->pushButton_input_drop->setVisible(false);
    ui->pushButton_input_save->setVisible(false);
}

// Деструктор нашего виджета
InputWidget::~InputWidget() {
    delete ui;
}

// Слот для приема передаваемых величин из главного окна
void InputWidget::setValues(QString username,QString lang, QString type, QString execution_time) {
    this->username = username;
    this->lang = lang;
    this->type = type;
    this->execution_time = execution_time;

    this->iter = global->genRandomInt(0,test_size);
    this->char_iter = 0;
    this->mistakes_percent = 0;
    this->mistakes_total = 0;
    this->speed_current_per_sec = 0;
    this->speed_current_per_min = 0;
    this->speed_average_per_sec = 0;
    this->speed_average_per_min = 0;

    dataFromFile();
    this->time_remains = this->execution_time.toDouble()*60;
    ui->label_input_time_remains->setText(QString::number(time_remains));
    update();
}



// Загрузка данных из файлов с тестами
void InputWidget::dataFromFile() {
    QJsonObject test = global->loadJson(global->tests +
                                             global->type_path_map[type] +
                                             global->lang_path_map[lang]
                                             ).object();
    test_pool = test["data"].toArray();
    test_size = test_pool.size();
}

// Выгрузка данных в файл статистики
void InputWidget::dataToFile() {
    QJsonDocument json_dump = global->loadJson(global->stat_filename);
    QJsonObject json_obj = json_dump.object();

    QJsonObject new_obj;
    new_obj["type"] = global->type_map[type];
    new_obj["time"] = execution_time;
    new_obj["mistakes"] = qRound(mistakes_percent);
    new_obj["speed"]= qRound(speed_current_per_sec);

    json_obj[this->username] = new_obj;
    json_dump.setObject(json_obj);

    global->saveJson(global->stat_filename, json_dump);
}


// TODO: REPLACE THIS----------------------------------------------------
// Проверочный счетчик для итерации по символам
void InputWidget::make_step() {
    qDebug() << "Brainlessx2";
    char_iter++;
    if (test_pool[iter].toString().length() <= char_iter) {
        qDebug() << "Brainlessx0";
        iter = global->genRandomInt(0,test_size);
        char_iter = 0;
    }
    update();

}
// TODO: REPLACE THIS----------------------------------------------------


// TODO: REPLACE THIS----------------------------------------------------
// Вывод текста, который необходимо повторить
void InputWidget::printTest() {
    qDebug() << "Brainlessx3";
    test_obj = test_pool[iter].toString();
    ui->textBrowser_input_test_data->setText(test_obj);
    qDebug() << "Brainlessx4?";
}
// TODO: REPLACE THIS----------------------------------------------------

void InputWidget::printKey(QString text_key) {
    bool tmp_status = checkChar();
    if ((char_iter == 0) && tmp_status == true)
        ui->textBrowser_input_user_data->setText(text_key);
    else if (tmp_status == true)
        ui->textBrowser_input_user_data->setText(ui->textBrowser_input_user_data->toPlainText() + text_key);
}

// TODO: REPLACE THIS----------------------------------------------------
// Проверка введенного знака
bool InputWidget::checkChar() {
//    qDebug() << iter <<char_iter << last_char
//             << QString(test_pool[iter].toString()[char_iter])
//             << (test_pool[iter].toString()[char_iter] == this->last_char);

    if (test_pool[iter].toString()[char_iter] == last_char)
        return true;
    else {
        qDebug() << "Brainless";
        return false;
    }
}
// TODO: REPLACE THIS----------------------------------------------------







// Ряд действий после считывания нажатия клавиши
void InputWidget::keyPressEvent(QKeyEvent *event) {

    QString oldText = ui->textBrowser_input_user_data->toPlainText();
    qint64 key = event->key();
    QString text_key = event->text();
    last_char = text_key;

    qDebug() << text_key << key;

    if (!timer->isActive() && active && (key==Qt::Key_Return || key== Qt::Key_Enter)) {
        ui->pushButton_input_drop->setVisible(true);
        this->timer_remains.start();
        timer->start(this->execution_time.toInt() * 60 * 1000);
        update_timer->start(100);
        printTest();
    }

    if (timer->isActive() && key==Qt::Key_Delete)
      ui->textBrowser_input_user_data->clear();

    else if (timer->isActive() && (key==Qt::Key_Return || key== Qt::Key_Enter))
        qDebug() << "????";

    else if (timer->isActive() && key!=Qt::Key_Shift && key!=Qt::Key_Control && key!=Qt::Key_Alt && key!=Qt::Key_Tab && key!=Qt::Key_CapsLock) {
        printKey(text_key);

        if (checkChar() == true) {
            chars_count++;
            make_step();
            printTest();
        }
        else {
            mistakes_total++;
            update();
        }

    }
}



// Обновление всех полей скорости, времени и ошибок
void InputWidget::update() {
    mistakes_percent = mistakes_total/chars_count * 100;

    ui->label_input_mistakes_percent->setText(QString::number(mistakes_percent));
    ui->label_input_mistakes_total->setText(QString::number(mistakes_total));
    ui->label_input_time_remains->setText(timer_remains.isValid()?QString::number(time_remains-((timer_remains.elapsed() - canceled_time)/1000)):QString::number(time_remains));

    ui->label_input_speed_current_per_min->setText(timer_remains.isValid()?(QString::number(chars_count*60/(chars_count>1?((timer_remains.elapsed() - canceled_time)/1000):1))):QString("0"));
    ui->label_input_speed_current_per_sec->setText(timer_remains.isValid()?(QString::number(chars_count/(chars_count>1?((timer_remains.elapsed() - canceled_time)/1000):1))):QString("0"));
    speed_current_per_sec = timer_remains.isValid()?chars_count/((timer_remains.elapsed() - canceled_time)/1000):speed_current_per_sec;
    ui->label_input_speed_average_per_min->setText(timer_remains.isValid()?(QString::number(chars_count*60/(chars_count>1?((timer_remains.elapsed())/1000):1))):QString("0"));
    ui->label_input_speed_average_per_sec->setText(timer_remains.isValid()?(QString::number(chars_count/(chars_count>1?((timer_remains.elapsed())/1000):1))):QString("0"));

}

// Остановка теста путем закрытия окна, вызывается в купе с переходом на старое окно.
void InputWidget::stopTest() {
    ui->pushButton_input_save->setVisible(true);
    timer->stop();
    update_timer->stop();

    this->active = false;

    ui->textBrowser_input_user_data->setText("");
    ui->textBrowser_input_test_data->setText("");
}

// Сохранение результата
void InputWidget::saveResult() {
    dataToFile();
}

// Отмена результата
void InputWidget::cancelResult() {
    timer->stop();
    update_timer->stop();


    iter = 0;
    char_iter = 0;
    mistakes_total = 0;
    speed_current_per_sec = 0;
    speed_current_per_min = 0;
    time_remains = 0;
    chars_count = 0;
    canceled_time = timer_remains.elapsed();

    ui->textBrowser_input_user_data->setText("");
    ui->textBrowser_input_test_data->setText("");

    this->active = true;
    update();
}

// Действие при нажатии кнопки выхода
void InputWidget::on_pushButton_input_quit_clicked() {
    cancelResult();
    this->close();
    emit changeWindow();
}

// Действие при нажатии кнопки сохранения
void InputWidget::on_pushButton_input_save_clicked() {
    saveResult();
    this->close();
    emit changeWindow();
}

// Действие при нажатии кнопки отмены
void InputWidget::on_pushButton_input_drop_clicked() {
    cancelResult();
}


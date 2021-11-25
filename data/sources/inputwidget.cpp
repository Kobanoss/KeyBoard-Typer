#include "../headers/inputwidget.h"
#include "ui_inputwidget.h"

InputWidget::InputWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InputWidget) {
    ui->setupUi(this);
    global = new Globals();

    timer = new QTimer();
    update_timer = new QTimer();
    connect(update_timer,&QTimer::timeout,this, &InputWidget::changeValues);
    connect(timer, &QTimer::timeout, this, &InputWidget::stopTest);


    ui->pushButton_input_drop->setVisible(false);
}

InputWidget::~InputWidget() {
    delete ui;
}

// TODO: MODIFY WHEN GENERATE FULL TESTS DATA------------------------------------------------------
void InputWidget::dataFromFile() {
    QJsonObject test_data = global->loadJson(global->tests +
                                             global->type_path_map[this->type] +
                                             global->lang_path_map[this->lang]
                                             ).object();
    this->test_size = test_data["size"].toInt();
    qDebug() << test_size;

    QString r_val = test_size>1?QString::number(global->genRandomInt(1,this->test_size)):"1";
    this->test_pool = test_data[r_val].toArray();
}

void InputWidget::dataToFile() {
    QJsonDocument json_dump = global->loadJson(global->stat_filename);
    QJsonObject json_obj = json_dump.object();

    QJsonObject new_obj;
    new_obj["type"] = global->type_map[type];
    new_obj["time"] = this->execution_time;
    new_obj["mistakes"] = this->mistakes_percent;
    new_obj["speed"]=this->speed_current_per_min;

    json_obj[this->username] = new_obj;
    json_dump.setObject(json_obj);

    global->saveJson(global->stat_filename, json_dump);
}

void InputWidget::printTest() {
    qDebug() << 'q';
    qDebug() << test_pool[iter].toString();
    ui->textBrowser_input_test_data->setText(test_pool[iter].toString());
}

bool InputWidget::checkChar() {
//    qDebug() << iter <<char_iter << last_char
//             << QString(test_pool[iter].toString()[char_iter])
//             << (test_pool[iter].toString()[char_iter] == this->last_char);

    if (test_pool[iter].toString()[char_iter] == this->last_char)
        return true;
    else
        return false;
}

void InputWidget::changeValues() {
    this->mistakes_percent = mistakes_total/chars_count * 100;
    ui->label_input_mistakes_percent->setText(QString::number(this->mistakes_percent));
    ui->label_input_mistakes_total->setText(QString::number(mistakes_total));
    ui->label_input_time_remains->setText(timer_remains.isValid()?QString::number(this->time_remains-((this->timer_remains.elapsed() - canceled_time)/1000)):QString::number(this->time_remains));

    ui->label_input_speed_current_per_min->setText(timer_remains.isValid()?(QString::number(chars_count*60/(chars_count>1?((this->timer_remains.elapsed() - canceled_time)/1000):1))):QString("0"));
    ui->label_input_speed_current_per_sec->setText(timer_remains.isValid()?(QString::number(chars_count/(chars_count>1?((this->timer_remains.elapsed() - canceled_time)/1000):1))):QString("0"));
    ui->label_input_speed_average_per_min->setText(timer_remains.isValid()?(QString::number(chars_count*60/(chars_count>1?((this->timer_remains.elapsed())/1000):1))):QString("0"));
    ui->label_input_speed_average_per_sec->setText(timer_remains.isValid()?(QString::number(chars_count/(chars_count>1?((this->timer_remains.elapsed())/1000):1))):QString("0"));

}


void InputWidget::saveResult() {
    dataToFile();
    this->iter = 0;
    this->char_iter = 0;
    this->mistakes_total = 0;
    this->speed_current_per_sec = 0;
    this->speed_current_per_min = 0;
    this->speed_average_per_sec = 0;
    this->speed_average_per_min = 0;
    this->time_remains = 0;
    this->chars_count = 0;
}

void InputWidget::cancelResult() {
    this->timer->stop();
    update_timer->stop();


    this->iter = 0;
    this->char_iter = 0;
    this->mistakes_total = 0;
    this->speed_current_per_sec = 0;
    this->speed_current_per_min = 0;
//    time_start = 0;
//    time_current = 0;
    this->time_remains = 0;
    this->chars_count = 0;
    this->canceled_time = timer_remains.elapsed();
    ui->textBrowser_input_user_data->setText("");
    ui->textBrowser_input_test_data->setText("");
    this->changeValues();
}

void InputWidget::update() {
    this->char_iter++;
    if (test_pool[iter].toString().length() <= char_iter) {
        iter++;
        char_iter = 0;}
    if (test_pool.size() <= iter) {
        saveResult();
        this->close();
        emit changeWindow();
    }

    changeValues();

}

void InputWidget::stopTest() {
    this->active = false;
}

void InputWidget::keyPressEvent(QKeyEvent *event) {

    QString oldText = ui->textBrowser_input_user_data->toPlainText();
    qint64 key = event->key();
    QString text_key = event->text();
    this ->last_char = text_key;

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
        ;

    else if (timer->isActive() && key!=Qt::Key_Shift && key!=Qt::Key_Control && key!=Qt::Key_Alt && key!=Qt::Key_Tab && key!=Qt::Key_CapsLock) {
        ui->textBrowser_input_user_data->setText(text_key);

        if (checkChar() == true) {
            chars_count++;
            this->update();
            printTest();
        }
        else {
            mistakes_total++;
            this->changeValues();
        }

    }
}

void InputWidget::setValues(QString username,QString lang, QString type, QString execution_time) {
    this->username = username;
    this->lang = lang;
    this->type = type;
    this->execution_time = execution_time;

    this->iter = 0;
    this->char_iter = 0;
    this ->mistakes_percent = 0;
    this -> mistakes_total = 0;
    this->speed_current_per_sec = 0;
    this->speed_current_per_min = 0;
    this->speed_average_per_sec = 0;
    this->speed_average_per_min = 0;

    dataFromFile();
    this->time_remains = this->execution_time.toDouble()*60;
    ui->label_input_time_remains->setText(QString::number(time_remains));
    changeValues();
}



void InputWidget::on_pushButton_input_quit_clicked() {
    cancelResult();
    this->close();
    emit changeWindow();
}


void InputWidget::on_pushButton_input_save_clicked() {
    saveResult();
    this->close();
    emit changeWindow();
}


void InputWidget::on_pushButton_input_drop_clicked() {
    cancelResult();
}


#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include <QVector>
#include "../headers/globals.h"


QT_BEGIN_NAMESPACE
namespace Ui { class InputWidget; }
QT_END_NAMESPACE

// Класс окна тестера, нет смысла в комментариях
// Так как в данном файле происходит лишь объявление переменных и функций этого класса
// Реализация находится в .cpp
class InputWidget : public QWidget {
    Q_OBJECT

public:
    explicit InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

private:
    Ui::InputWidget *ui;

    Globals *global;

    QString username;
    QString lang;
    QString type;
    QString execution_time;

    QElapsedTimer timer_remains;
    QTime time_start;
    QTime time_current;
    double time_remains;
    double canceled_time = 0;

    int chars_count = 0;
    double mistakes_percent;
    double mistakes_total;

    double speed_current_per_sec;
    double speed_current_per_min;
    double speed_average_per_sec;
    double speed_average_per_min;

    void dataFromFile();
    void dataToFile();

    void printTest();
    void printKey(QString);

    bool checkChar();
    void update();

    void saveResult();
    void cancelResult();

    void make_step();

    QTimer *timer;
    QTimer *update_timer;

    bool active = true;
    int test_size;
    QJsonArray test_pool;
    qint32 iter = 0;
    qint32 char_iter = 0;
    QString test_obj;

    QString last_char;
    QJsonDocument statistics_json;

protected:
   virtual void keyPressEvent(QKeyEvent *event);

signals:
    void changeWindow();

public slots:
    void setValues(QString username,QString lang, QString type, QString execution_time);

private slots:
    void stopTest();

    void on_pushButton_input_quit_clicked();

    void on_pushButton_input_save_clicked();
    void on_pushButton_input_drop_clicked();
};

#endif // INPUTWIDGET_H

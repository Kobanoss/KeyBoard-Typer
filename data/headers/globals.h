#ifndef GLOBALS_H
#define GLOBALS_H

#include <QWidget>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRandomGenerator>
#include <QElapsedTimer>


class Globals {
public:
    QString log_filename = "./logs/log";
    QString stat_filename = "./profiles/stats/stats.json";
    QString settings_filename = "./profiles/settings/default_settings.json";
    QString tests = "./tests";
    QMap <QString, QString> type_path_map {{"Одиночные символы","/char_tests"},{"Слова","/word_tests"},{"Текст","/text_tests"}};
    QMap <QString, QString> type_map {{"Одиночные символы","char"},{"Слова","word"},{"Текст","text"}};
    QMap <QString, QString> lang_path_map {{"ru","/test_ru.json"},{"en","/test_en.json"},{"spec","/test_spec.json"}};


    QJsonDocument loadJson(QString filename) {
        QFile file(filename);

        if ((file.exists())&&(file.open(QFile::ReadOnly)))
            return QJsonDocument().fromJson(file.readAll());
        else {
            qDebug() << "Can't load from file: " << file.fileName();
            return QJsonDocument().fromJson("{}");
        }
    }

    void saveJson(QString filename, QJsonDocument data) {
        QFile file(filename);

        if ((file.exists())&&(file.open(QFile::WriteOnly)))
            file.write(data.toJson());
        else
            qDebug() << "Can't save to file: " << file.fileName();
    }

    void createIfNotExist(QString filename) {
        QFile file(filename);
        QFileInfo file_info(file);
        QDir dir;

        QString file_path = file_info.path();
        dir.mkpath(file_path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           file.open(QIODevice::WriteOnly);

    }

    int genRandomInt(int highest, int lowest) {
        QRandomGenerator rand_obj;
        return rand_obj.bounded(highest - lowest) + lowest;
    }
};

#endif // GLOBALS_H

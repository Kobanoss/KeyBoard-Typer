#include "./data/headers/menuwidget.h"
#include "./data/headers/hintwidget.h"
#include "./data/headers/globals.h"

#include <QApplication>

void createFiles(){
    Globals global;

    global.createIfNotExist(global.log_filename);
    global.createIfNotExist(global.settings_filename);
    global.createIfNotExist(global.stat_filename);
}

bool check_auto_hint() {
    Globals global;
    QJsonObject settings = global.loadJson(global.settings_filename).object();

    return !settings.contains("auto_hint")?true:settings["auto_hint"].toBool();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    createFiles();

    MenuWidget  menu;
    HintWidget  hint;

    menu.show();

    check_auto_hint()?hint.show():hint.~HintWidget();

    return app.exec();
}

#include "./data/headers/menuwidget.h"
#include "./data/headers/hintwidget.h"
#include "./data/headers/inputwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HintWidget  hint;
    MenuWidget  menu;
    InputWidget input;

    input.show();
    menu.show();
    hint.show();


    return a.exec();
}

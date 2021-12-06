QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    data/sources/hintwidget.cpp \
    data/sources/inputwidget.cpp \
    data/sources/menuwidget.cpp \

HEADERS += \
    data/headers/globals.h \
    data/headers/hintwidget.h \
    data/headers/inputwidget.h \
    data/headers/menuwidget.h

FORMS += \
    data/forms/hintwidget.ui \
    data/forms/inputwidget.ui \
    data/forms/menuwidget.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    logs/log \
    profiles/events/event.json \
    profiles/settings/default_settings.json \
    profiles/stats/stat.json \
    tests/char_tests/test_en.json \
    tests/char_tests/test_ru.json \
    tests/char_tests/test_spec.json \
    tests/text_tests/test_en.json \
    tests/text_tests/test_ru.json \
    tests/word_tests/test_en.json \
    tests/word_tests/test_ru.json

RESOURCES += \
    src.qrc

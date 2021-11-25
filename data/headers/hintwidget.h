#ifndef HINTWIDGET_H
#define HINTWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include "../headers/globals.h"


QT_BEGIN_NAMESPACE
namespace Ui { class HintWidget; }
QT_END_NAMESPACE


class HintWidget : public QWidget {
    Q_OBJECT

public:
    explicit HintWidget(QWidget *parent = nullptr);
    ~HintWidget();


private slots:
    void on_checkBox_hint_disable_auto_stateChanged(int state);

private:
    Globals global;
    bool auto_hint_status;

    void closeEvent(QCloseEvent *event);
    void update_settings_file();

    Ui::HintWidget *ui;
};

#endif // HINTWIDGET_H

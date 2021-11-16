#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class InputWidget; }
QT_END_NAMESPACE


class InputWidget : public QWidget {
    Q_OBJECT

public:
    explicit InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

private:
    Ui::InputWidget *ui;
};

#endif // INPUTWIDGET_H

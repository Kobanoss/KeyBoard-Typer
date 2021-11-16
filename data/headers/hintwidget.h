#ifndef HINTWIDGET_H
#define HINTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class HintWidget; }
QT_END_NAMESPACE


class HintWidget : public QWidget {
    Q_OBJECT

public:
    explicit HintWidget(QWidget *parent = nullptr);
    ~HintWidget();

private:
    Ui::HintWidget *ui;
};

#endif // HINTWIDGET_H

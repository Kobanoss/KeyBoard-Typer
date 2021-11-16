#include "../headers/hintwidget.h"
#include "ui_hintwidget.h"

HintWidget::HintWidget(QWidget *parent) : QWidget(parent), ui(new Ui::HintWidget) {
    ui->setupUi(this);
}

HintWidget::~HintWidget() {
    delete ui;
}


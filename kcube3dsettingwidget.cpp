#include "kcube3dsettingwidget.h"
#include "ui_kcube3dsettingwidget.h"

KCube3DSettingWidget::KCube3DSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KCube3DSettingWidget)
{
    ui->setupUi(this);
    initUI();

}

KCube3DSettingWidget::~KCube3DSettingWidget()
{
    delete ui;
}

void KCube3DSettingWidget::showEvent(QShowEvent *event)
{

}

void KCube3DSettingWidget::initUI()
{
    ui->cubeWidget->setClearColor(Qt::black);
    ui->cubeWidget->rotateBy(+42 * 16.0f, +42 * 16.0f, -21 * 16.0f);
}

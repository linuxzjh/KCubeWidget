#include "kcube3dsettingwidget.h"
#include "ui_kcube3dsettingwidget.h"

KCube3DSettingWidget::KCube3DSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KCube3DSettingWidget)
{
    ui->setupUi(this);
    ui->cubeWidget->setClearColor(Qt::black);
    connect(ui->cubeWidget, &KCubeWidget::dataChanged, this, [this](){
        updateUI();
    });

    connect(ui->doubleSpinBox_length, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setLength(value);
    });
    connect(ui->doubleSpinBox_width, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setWidth(value);
    });
    connect(ui->doubleSpinBox_height, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setHeight(value);
    });
    connect(ui->doubleSpinBox_scale, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setScale(value);
    });

    connect(ui->doubleSpinBox_xRot, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setRoteX(value);
    });
    connect(ui->doubleSpinBox_yRot, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setRoteY(value);
    });
    connect(ui->doubleSpinBox_zRot, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        [&](double value){
        ui->cubeWidget->setRoteZ(value);
    });
}

KCube3DSettingWidget::~KCube3DSettingWidget()
{
    delete ui;
}

void KCube3DSettingWidget::showEvent(QShowEvent *event)
{
    updateUI();
    QWidget::showEvent(event);
}

void KCube3DSettingWidget::updateUI()
{
    const rectangleInfo_stru & info = ui->cubeWidget->getInfo();
    blockSignalChildren(true);
    ui->doubleSpinBox_length->setRange(0.01, 10);
    ui->doubleSpinBox_width->setRange(0.01, 10);
    ui->doubleSpinBox_height->setRange(0.01, 10);
    ui->doubleSpinBox_scale->setRange(0.01, 200);
    ui->doubleSpinBox_xRot->setRange(0, 180);
    ui->doubleSpinBox_yRot->setRange(0, 180);
    ui->doubleSpinBox_zRot->setRange(0, 180);

    ui->doubleSpinBox_length->setValue(info.length);
    ui->doubleSpinBox_width->setValue(info.width);
    ui->doubleSpinBox_height->setValue(info.height);
    ui->doubleSpinBox_scale->setValue(info.scale);
    ui->doubleSpinBox_xRot->setValue(info.xRot);
    ui->doubleSpinBox_yRot->setValue(info.yRot);
    ui->doubleSpinBox_zRot->setValue(info.zRot);
    blockSignalChildren(false);

}

void KCube3DSettingWidget::blockSignalChildren(bool bBlockSignal)
{
    ui->comboBox_leftScreen->blockSignals(bBlockSignal);
    ui->comboBox_rightScreen->blockSignals(bBlockSignal);
    ui->comboBox_topScreen->blockSignals(bBlockSignal);

    ui->doubleSpinBox_length->blockSignals(bBlockSignal);
    ui->doubleSpinBox_width->blockSignals(bBlockSignal);
    ui->doubleSpinBox_height->blockSignals(bBlockSignal);
    ui->doubleSpinBox_scale->blockSignals(bBlockSignal);

    ui->doubleSpinBox_xRot->blockSignals(bBlockSignal);
    ui->doubleSpinBox_yRot->blockSignals(bBlockSignal);
    ui->doubleSpinBox_zRot->blockSignals(bBlockSignal);
}

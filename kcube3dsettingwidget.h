#ifndef KCUBE3DSETTINGWIDGET_H
#define KCUBE3DSETTINGWIDGET_H

#include <QWidget>
#include "kcubewidget.h"

namespace Ui {
class KCube3DSettingWidget;
}

class KCube3DSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KCube3DSettingWidget(QWidget *parent = nullptr);
    ~KCube3DSettingWidget();
protected:
    void showEvent(QShowEvent* event);
private:
    void initUI();
    Ui::KCube3DSettingWidget *ui;
};

#endif // KCUBE3DSETTINGWIDGET_H

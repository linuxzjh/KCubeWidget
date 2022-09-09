#include "kcube3dsettingwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KCube3DSettingWidget w;

    w.show();
    return a.exec();
}

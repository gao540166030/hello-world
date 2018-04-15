#include <QtGui/QApplication>
#include "frmmain.h"

#include <QTextCodec>
#include <QDesktopWidget>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Support Chinese encoding
    QTextCodec *codec=QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    //Form centered display
    QDesktopWidget *desktop=QApplication::desktop();
    int width=desktop->width();
    int height=desktop->height();

    frmMain w;
    //Setting window has no maximize button
    w.setWindowFlags(Qt::WindowMinimizeButtonHint);
    w.move((width-w.width())/2,(height-w.height())/2);
    w.setFixedSize(w.width(),w.height());
    w.show();
    //Apply style
    QApplication::setStyle(QStyleFactory::create("Plastique"));

    return a.exec();
}


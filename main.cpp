#include "mainwindow.h"

#include <QApplication>
#include <QLabel>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    /*if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        QMessageBox::warning(nullptr, "AA", "Requesting 3.3 core context");
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        QMessageBox::warning(nullptr, "AA", "Requesting 3.0 context");
        format.setVersion(3, 0);
    }*/
    QSurfaceFormat::setDefaultFormat(format);

#ifndef QT_NO_OPENGL
    MainWindow w;
    w.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif

    return a.exec();
}

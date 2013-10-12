#ifndef WRY_H
#define WRY_H

#include <QPlainTextEdit>
#include <QObject>
#include <QTimer>
#include <QFile>

class QApplication;

class FileEditor
    : public QPlainTextEdit
{
    Q_OBJECT

public:
    FileEditor( const char * f, QWidget * w, QApplication * a )
        : QPlainTextEdit( w ),
          app( a ), saver( new QTimer ), file( f )
    {
        init();
        connect(this, SIGNAL(textChanged()), this, SLOT(autosave()));
        connect(saver, SIGNAL(timeout()), this, SLOT(save()));
    }

    void init();

public slots:
    void autosave();
    void save();
    void quit();

private:
    QApplication * app;
    QTimer * saver;
    QFile file;
};

#endif

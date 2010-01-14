#ifndef WRY_H
#define WRY_H

#include <QPlainTextEdit>
#include <QObject>
#include <QFile>

class QApplication;

class FileEditor
    : public QPlainTextEdit
{
    Q_OBJECT

public:
    FileEditor( const char * f, QWidget * w, QApplication * a )
        : QPlainTextEdit( w ), app( a ), file( f )
    {
        init();
    }

    void init();

public slots:
    void save();
    void quit();

private:
    QApplication * app;
    QFile file;
};

#endif

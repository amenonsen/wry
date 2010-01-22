// A minimal fullscreen text editor
// Copyright 2010 Abhijit Menon-Sen <ams@toroid.org>

#include "wry.h"

#include <QRect>
#include <QWidget>
#include <QPalette>
#include <QApplication>
#include <QKeySequence>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QTextStream>
#include <QShortcut>
#include <QFrame>
#include <QFont>
#include <QFile>

void FileEditor::init()
{
    if ( !file.open( QIODevice::ReadWrite ) )
        exit( -1 );

    QString buf;
    QTextStream in( &file );
    while ( !in.atEnd() ) {
        QString line( in.readLine() );

        if ( !line.isEmpty() ) {
            if ( !buf.isEmpty() )
                buf.append( " " );
            buf.append( line );
        }
        else {
            buf.append( "\n" );
            appendPlainText( buf );
            buf.clear();
        }
    }

    appendPlainText( buf );
}

void FileEditor::save()
{
    file.seek( 0 );
    QString text( toPlainText() );
    QTextStream to( &file );
    QTextStream from( &text );

    while ( !from.atEnd() ) {
        QString line( from.readLine() );

        if ( line.length() <= 72 ) {
            to << line << endl;
        }
        else {
            int s = 0;
            int l = line.length();
            while ( s < l ) {
                int i = s;
                do {
                    int m = line.indexOf( " ", i );

                    if ( m < 0 || m > s+72 ) {
                        if ( i == s ||
                             ( m < 0 && l-s <= 72 ) ||
                             ( m > 0 && m-i > 72 && i-s <= 65 ) )
                        {
                            i = 1 + ( m < 0 ? l : m );
                        }
                        break;
                    }

                    i = ++m;
                }
                while ( 1 );

                to << line.mid( s, i-s-1 ) << endl;
                s = i;
            }
        }
    }
}

void FileEditor::quit()
{
    save();
    app->quit();
}

int main( int ac, char *av[] )
{
    QApplication app( ac, av );

    if ( ac != 2 )
        return 0;

    QWidget window;
    FileEditor editor( av[1], &window, &app );

    QDesktopWidget * d = QApplication::desktop();

    QFont f( window.font() );
    f.setPixelSize( d->width()/60 );

    QPalette p( window.palette() );
    p.setColor( QPalette::Background, Qt::white );

    window.setFont( f );
    window.setPalette( p );
    window.setAutoFillBackground( true );

    editor.setGeometry( 0, 0, d->width()*2/3, d->height()*3/4 );
    editor.setFrameStyle( QFrame::NoFrame );
    editor.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    editor.setTabStopWidth( 4 );

    QRect rect( editor.geometry() );
    rect.moveCenter( app.desktop()->availableGeometry().center() );
    editor.setGeometry( rect );

    QShortcut save( QKeySequence( "Ctrl+S" ), &window );
    QObject::connect( &save, SIGNAL(activated()), &editor, SLOT(save()) );

    QShortcut exit( QKeySequence( "Shift+Ctrl+Q" ), &window );
    QObject::connect( &exit, SIGNAL(activated()), &app, SLOT(quit()) );

    QShortcut saq( QKeySequence( "Ctrl+Q" ), &window );
    QObject::connect( &saq, SIGNAL(activated()), &editor, SLOT(quit()) );

    QShortcut undo( QKeySequence( "Ctrl+Z" ), &window );
    QObject::connect( &undo, SIGNAL(activated()), &editor, SLOT(undo()) );

    QShortcut redo( QKeySequence( "Shift+Ctrl+Z" ), &window );
    QObject::connect( &redo, SIGNAL(activated()), &editor, SLOT(redo()) );

    window.showFullScreen();

    return app.exec();
}

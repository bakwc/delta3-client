#include <QDebug>
#include <QStringList>
#include <iostream>
#include "Application.h"
#include <QHostAddress>

namespace delta3
{
    Application::Application( int& argc, char* argv[] ):
        QCoreApplication( argc, argv ),
        client_( new Client(this) ),
        forceQuit_( false )

    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    }

    Application::~Application()
    {
    }

    bool Application::init()
    {
        parseCmdLine();
        if( forceQuit_ )
            return false;

        return true;
    }

    void Application::parseCmdLine()
    {
        const QStringList& args = arguments();
        for( int i = 0; i < args.count(); i++ )
        {
            if( parseOptions(args.at(i)) )
                continue;

            if( i < args.count() - 1 )
                parseOptionsWithParam( args.at(i), args.at(i + 1) );
        }
    }

    bool Application::parseOptions( const QString& arg )
    {
        if( isOption(arg, "-h", "--help") ){
            std::cout << " show help";
            forceQuit_ = true;
            return true;
        }

        return false;
    }

    bool Application::parseOptionsWithParam(
            const QString& arg,
            const QString& param
            )
    {
        return false;
    }

    bool Application::isOption( const QString& arg, const QString& option )
    {
        return arg.compare( option ) == 0;
    }

    bool Application::isOption(
            const QString& arg,
            const QString& option1,
            const QString& option2
            )
    {
        return isOption( arg, option1 ) || isOption( arg, option2 );
    }
}


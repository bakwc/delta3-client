#pragma once

#include <QCoreApplication>
#include <QTextCodec>
#include "Client.h"

class Application : public QCoreApplication
{
    Q_OBJECT
public:
    explicit Application( int& argc, char* argv[] );
    ~Application();
    bool init();

private:
    void parseCmdLine();
    bool parseOptions( const QString& arg );
    bool parseOptionsWithParam( const QString& arg, const QString& param );

    bool isOption( const QString& arg, const QString& option );
    bool isOption(
        const QString& arg,
        const QString& option1,
        const QString& option2
    );

private:
    Client* client_;
    bool forceQuit_;
};

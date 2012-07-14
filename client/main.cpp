#include "Application.h"

int main(int argc, char *argv[])
{
    delta3::Application app( argc, argv );

    if( !app.init() )
        return 1;

    return app.exec();
}


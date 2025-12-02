#include <stdio.h>

#if defined(__MINGW32__ )
#endif

#if __APPLE__ || __linux__
#   include <dirent.h>
#   include <sys/stat.h>
#else
#   error Needs fixing to compile on your system
#endif

void setup( void );
void loop( void );

int main( int argc, char ** argv )
{
    printf( "Starting up TinyBasic Plus...\n\n" );

    setup();
    loop();
}

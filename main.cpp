#include "user_service/http_web_server.h"


int main(int argc, char*argv[]) 
{
    HTTPUserWebServer app;
    return app.run(argc, argv);
}
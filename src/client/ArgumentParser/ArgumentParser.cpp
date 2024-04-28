#include "ArgumentParser.h"

ArgumentParser& ArgumentParser::Instance()
{
    static ArgumentParser instance;
    return instance;
}

char* ArgumentParser::GetOutputFile()
{
    return outfile;
}

static struct option arg_options[] = {
    {"image", required_argument, 0, 'i'},
    {"resize", required_argument, 0, 'r'},
    {"brightness", required_argument, 0, 'b'},
    {"output", required_argument, 0, 'o'},
    {0, 0, 0, 0}
};

Payload ArgumentParser::HandleArguments(int argc, char** argv)
{
    int argument = 0;
    int option_index = 0;
    outfile = (char*)malloc(255); /// free this
    
    Payload _payload;

    while((argument = getopt_long(argc, argv, "irbo", arg_options, &option_index)) != -1)
    {
        switch (argument) 
        {
            case 'i':
                _payload.image =  ImageHandler::Instance().Read(optarg);
                break;

            case 'o':
                strcpy(outfile, optarg);
                strcpy(_payload.target, optarg);
                break;
            
            case 'r':
                _payload.resize = true;
                _payload.size = str_to_v2(optarg);
                break;
            
            case 'b':
                _payload.rebright = true;
                _payload.brightness = atof(optarg);
                break;

            case '?':
                //write(STDOUT_FILENO, help, strlen(help));
                exit(EXIT_FAILURE);
                break;

            default:
                abort ();
            
        }
    }

    // TO DO : handle some errors here (eventually)

    return _payload;
}
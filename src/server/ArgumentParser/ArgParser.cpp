#include "ArgParser.h"


struct option ArgumentParserServer::long_options[] = {
    {"IP", required_argument, 0, 'i'},
    {"PORT", required_argument, 0, 'p'},
    {"threads", required_argument, 0, 't'},
    {"max_clients", required_argument, 0, 'm'},
    {0, 0, 0, 0}
};

ArgumentParserServer& ArgumentParserServer::Instance() {
    static ArgumentParserServer instance;
    return instance;
}


PayloadServer ArgumentParserServer::HandleArguments(int argc, char** argv) {
    int argument = 0;
    int option_index = 0;
    outfile = (char*)malloc(255); // free this

    PayloadServer _payload;

    _payload.ip = "127.0.0.1";  //  IP
    _payload.port = 9090;       //  port
    _payload.threads = 4;       // number of threads
    _payload.max_clients = 100;  //  max clients


    while ((argument = getopt_long(argc, argv, "i:p:t:m:", long_options, &option_index)) != -1) {
        switch (argument) {
            case 'i':
                _payload.ip = optarg;
                break;
            case 'p':
                _payload.port = std::atoi(optarg);
                if (_payload.port <= 0 || _payload.port > 65535) {
                    std::cerr << "Invalid port number. It must be between 1 and 65535." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                break;
            case 't':
                _payload.threads = std::atoi(optarg);
                if (_payload.threads <= 0) {
                    std::cerr << "Invalid number of threads. It must be greater than 0." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                _payload.max_clients = std::atoi(optarg);
                if (_payload.max_clients < 1) {
                    std::cerr << "Invalid max clients value. It must be at least 1." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                break;
            case '?':  
                // WIP - handle unknown arguments and a help message 
                break;
            default:
                abort();
        }
    }

    // TODO: handle some errors here
    return _payload;
}
const std::string& ArgumentParserServer::GetOutputFile() const {
    return outfile;
}

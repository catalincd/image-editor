#include "WebServer.h"
#include <crow.h>
#include <crow/middlewares/cors.h>
#include <fstream>  
 

#include "../ImageEditor/ImageEditor.h"
#include "../../utils/OSUtils.h"

#define CROW_STATIC_DIRECTORY "static/"
#define CROW_STATIC_ENDPOINT "/"

std::ofstream logfile("file.out");

class CrowLogger : public crow::ILogHandler
{
public:
    CrowLogger() {}
    void log(std::string message, crow::LogLevel /*level*/)
    {
        logfile << message << std::endl;
    }

    ~CrowLogger() {
        logfile.close();
    }
};

WebServer::WebServer() : m_thread(&WebServer::Start, this) {}

void WebServer::Join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void WebServer::Start()
{
    std::cout << "RUNNING CROW SERVER" << std::endl;

    const char *url = "http://0.0.0.0:18080/static/index.html";
    std::string command = "xdg-open " + std::string(url);
    CrowLogger logger;
    crow::SimpleApp app;

    /*
          target,

      resize: configs.size ? true : false,
      size: configs.size || { width: -1, height: -1 },

      recontrast: configs.contrast ? true : false,
      contrast: configs.contrast || 1.0,

      rebright: configs.brightness ? true : false,
      brightness: configs.brightness || 50,

      blur: configs.blur ? true : false,
      kernel: configs.blur || 50,

      rotate: configs.angle ? true : false,
      angle: configs.angle || 0.0,

      crop: configs.cropData ? true : false,
      cropData: configs.cropData || { x: 0.0, y: 0.0, width: 0.0, height: 0.0 },
    */

    CROW_ROUTE(app, "/send").methods(crow::HTTPMethod::Post)([](const crow::request& req){
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400, "Invalid JSON");

        Payload payload;

        payload.resize = x["resize"].b();
        payload.size = { x["size"]["width"].i(), x["size"]["height"].i()};

        payload.recontrast = x["recontrast"].b();
        payload.contrast = x["contrast"].d();

        payload.rebright = x["rebright"].b();
        payload.brightness = x["brightness"].d();

        payload.blur = x["blur"].b();
        payload.kernel = x["kernel"].d();

        payload.rotate = x["rotate"].b();
        payload.angle = x["angle"].d();

        payload.crop = x["crop"].b();
        payload.cropTop = { x["cropData"]["x"].i(), x["cropData"]["y"].i()};
        payload.cropSize = { x["cropData"]["width"].i(), x["cropData"]["height"].i()};

        std::string target = x["target"].s();
        std::vector<char> data = DecodeByte64(x["data"].s());

        strncpy(payload.target, target.c_str(), 255);

        payload.image.data = &data[0];
        payload.image.size = data.size();

        PrintPayload(payload);

        std::cout<<"RECEIVED IMG DATA: "<< data.size() <<std::endl;

        Buffer edited = ImageEditor::Instance().Edit(payload);
    
        std::cout<<"EDITED IMG DATA: "<< edited.size <<std::endl;

        std::string output(edited.size, '\0');

        for(int i=0;i<edited.size;i++)
            output[i] = edited.data[i];

        free(edited.data);

        std::cout<<"RESPONSE DATA: "<< output.length() <<std::endl;

        return crow::response(200, output);
    });
    

    app.port(18080).multithreaded().run();
    std::cout << "RAN CROW SERVER" << std::endl;
}



const std::string base64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool isBase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<char> WebServer::DecodeByte64(const std::string& encoded)
{
    int in_len = encoded.size();
    int i = 0, j = 0, in_ = 0;
    char char_array_4[4], char_array_3[3];
    std::vector<char> decodedData;

    while (in_len-- && (encoded[in_] != '=') && isBase64(encoded[in_])) {
        char_array_4[i++] = encoded[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) {
                char_array_4[i] = base64Chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++) {
                decodedData.push_back(char_array_3[i]);
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        for (j = 0; j < 4; j++) {
            char_array_4[j] = base64Chars.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) {
            decodedData.push_back(char_array_3[j]);
        }
    }

    return decodedData;
}

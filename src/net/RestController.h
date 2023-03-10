#ifndef REST_CONTROLLER_H
#define REST_CONTROLLER_H

#include <WebServer.h>
#include <net/BetterLogger.h>
#include <utils/SettingsManager.h>

#define SERVER_PORT 80
#define ADD_WEB_PAGE 1

#define JSON_CONTENT_TYPE "application/json"

#define SYSTEM_STATISTICS 1

struct HandlerResult{
    int code = 200;
    String contentType = JSON_CONTENT_TYPE;
    String body;
};

class RestController{
    public:
        RestController();
        ~RestController();

        typedef std::function<HandlerResult(void)> HandlerWithResultFunction;
        typedef std::function<void(void)> HandlerFunction;
        void begin(BetterLogger * logger, SettingsManager * manager);
        
        void addStateChangeHandler(RestController::HandlerWithResultFunction hf) {
            _changeStateHandler = hf;
        };
        void addGetStateHandler(RestController::HandlerWithResultFunction hf) {
            _getStateHandler = hf;
        };
        void addWebPageBuilder(RestController::HandlerWithResultFunction hf) {
            _webPageBuilder = hf;
        };
        void addConfigUpdatedHandler(RestController::HandlerFunction hf) {
            _configUpdatedHandler = hf;
        }

        void addSetupEndpoint();

        String getRequestBody();
        WebServer * getWebServer() { return &_server; };

        void handle();
    private:
        bool _setupFinished = false;
        WebServer _server;
        BetterLogger * _logger;
        SettingsManager * _settingsManager;

        void setupEndpoints();

        void processHandlerResult(HandlerResult result);
        HandlerWithResultFunction _defaultHandler = []() {
            HandlerResult result;
            return result;
        };

        HandlerWithResultFunction _getStateHandler = _defaultHandler;
        HandlerWithResultFunction _changeStateHandler = _defaultHandler;
        HandlerWithResultFunction _webPageBuilder = _defaultHandler;

        HandlerFunction _configUpdatedHandler = [](){};

        void handleConfigPost();
        void handleConfigDelete();
        void handleSetupPost();

        String buildErrorJson(String error);
};

#endif
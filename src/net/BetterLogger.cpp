#include "net/BetterLogger.h"

#define LOGGER_MESSAGE_TEMPLATE "{%s}[%ld][%s]::%s"

BetterLogger::BetterLogger(){
    Serial.begin(115200);
}
BetterLogger::~BetterLogger() {
    _multicaster.stop();
}

void BetterLogger::connect(const char * myIp) {
    connect(myIp, LOGGER_DEFAULT_GROUP, LOGGER_DEFAULT_PORT);
}

void BetterLogger::connect(const char * myIp, const char * group, int port) {
    _multicaster.init(group, port);
    _ip = myIp;
    _connected = true;
}

void BetterLogger::log(const char * tag, const char * message) {
    char formattedMessage[MAX_MESSAGE_LENGTH];

    sprintf(formattedMessage, LOGGER_MESSAGE_TEMPLATE, _ip, millis(), tag, message);

    xSemaphoreTake(_mutex, portMAX_DELAY);
    Serial.println(formattedMessage);
    if (_connected) {
        _multicaster.broadcast(formattedMessage);
    }
    xSemaphoreGive(_mutex);
}

void BetterLogger::statistics() {
#ifdef ENABLE_STATISTICS
    log(STAT_LOG_TAG, "----------STATISTIC----------");
    log(STAT_LOG_TAG, "Free/size heap: %u/%u", ESP.getFreeHeap(), ESP.getHeapSize());
    log(STAT_LOG_TAG, "Min free/max alloc heap: %u/%u", ESP.getMinFreeHeap(), ESP.getMaxAllocHeap());
    log(STAT_LOG_TAG, "--------STATISTIC-END--------");
#endif
}
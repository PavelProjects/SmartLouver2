#include <LouverController.h>

LouverController::LouverController() {
}

LouverController::~LouverController() {
    _motorController.stop();
    deleteMonitorTask();
}

void LouverController::init( uint8_t motorFirstPin,
                                    uint8_t motorSecondPin,
                                    uint8_t potPin,
                                    uint8_t lightSensorPin) {
    _motorController = MotorController(motorFirstPin, motorSecondPin, potPin);
    _lightSensorPin = lightSensorPin;
}

void LouverController::addLedIndicator(LedIndicator * led) {
    _led = led;
}

void LouverController::monitorLight() {
    int16_t lightValue = 0;
    int16_t delayTime = MONITOR_TASK_DELAY / portTICK_PERIOD_MS;

    ESP_LOGI(LIGHT_MONITOR_TAG, "Light monitor task started");
    for(;;) {
        lightValue = analogRead(_lightSensorPin);
        if (INVERT) {
            lightValue = 4095 - lightValue;
        }
        _motorController.setPosition(map(lightValue, 0, 4095, POT_MIN, POT_MAX));
        vTaskDelay(delayTime);
    }
}

void LouverController::createMonitorTask(){
    xTaskCreate(
        [](void* o){ static_cast<LouverController*>(o)->monitorLight(); },
        LIGHT_MONITOR_TAG,
        2048,
        this,
        1,
        &_monitorLightHandle
    );
}

void LouverController::deleteMonitorTask() {
    if (_monitorLightHandle != NULL) {
        vTaskDelete(_monitorLightHandle);
        _monitorLightHandle = NULL;
    }
}


void LouverController::enableAutoMode() {
    if (!isAutoModeEnabled()) {
        createMonitorTask();
        if (_led != NULL) {
            _led->on();
        }
    }
    ESP_LOGI(LOUVER_CONTROLLER_TAG, "Automode enabled");
}

void LouverController::disabelAutoMode() {
    if (isAutoModeEnabled()) {
        deleteMonitorTask();
        if (_led != NULL) {
            _led->off();
        }
    }
    ESP_LOGI(LOUVER_CONTROLLER_TAG, "Automode disabled");
}

bool LouverController::isAutoModeEnabled() {
    return _monitorLightHandle != NULL;
}

uint16_t LouverController::getLightValue() {
    return analogRead(_lightSensorPin);
}

uint16_t LouverController::getMotorPosition() {
    return _motorController.currentPosition();
}

void LouverController::open() {
    disabelAutoMode();
    _motorController.setPosition(OPEN_POSITION);
}

void LouverController::close() {
    disabelAutoMode();
    _motorController.setPosition(CLOSE_POSITION);
}

void LouverController::middle() {
    disabelAutoMode();
    _motorController.setPosition(MIDDLE_POSITION);
}

void LouverController::bright() {
    disabelAutoMode();
    _motorController.setPosition(BRIGHT_POSITION);
}
# Реализация библиотеки для упрощения создания устройств умного дома

## Главная идея
Упростить создание IOT устройств
Должна реализовывать базовый функционал:
- Подключения и управление WiFi сетью
- Упрощение подключения и поиска 
- Набор станлартных REST ендпоинтов у устройства (/info, /state, /action, /config, /setup, /restart) 
- Возможность объединения с другими устройствами в mesh сеть на основе BLE

### Подключение и управление WiFi
Первое включение в режиме AP для задания пароля. Второе ключение - подключение к сети. Не может подключиться - не запускается все связанное с сетью. 
Пароль от сети ни где не показывается.
Для указания нового - кнопка на устройстве.

### Упрощение подключения и поиска
Будет реализована возможность подключения к другому устройству через BLE при первом включении. В таком случае с уже настроенного устройства все настройки будут переданы на новое устройство.
Вещать свой ip в сеть посредством multicast.

### Набор стандартных REST ендпоинтов
- info - информация об устройстве
- state - состояние устройства
- config - конфигурация различных настроек
- setup - первоначальная настройка
- restart - перезапуск

### Работа в BLE mesh сети с другими устройствами
На данный момент планируется реализовать данную еть посредством режима ble advertising, однако прототипирование может внести свои коррективы. 
Устройства будут постоянно находится в состоянии Scanning для получения пакетов и соотвественно передачи через advertising.
Все пакеты необходимо шифровать внутренним ключом, тк любое другое устройство будет способно получить эти сообщения.

## Текущее состояние проекта
На данный момент реализовано:
- Простой менеджер настроек, работающий с EEPROM
- Упрощенное подключение - ip вещается в multicast группу
- Набор REST ендпоинтов

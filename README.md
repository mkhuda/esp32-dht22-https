# ESP32, DHT22, (DHT11 and DHT21) and HTTPS Post

This project is under development, it just for testing with ESP32, DHT22 and hit API's served somewhere.
Supported sensor for this project based on DHT library:
- DHT 11
- DHT 22
- DHT 21

```c
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
```

> Still on going development

## Prerequisite
1. Install ESP32 board manager to arduino IDE, Platformio, or VSCode Arduino
2. you need the SPIFFS sketch data uploader, to include it in the flash, please follow the tutorial [here](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/)
3. Prepare wifi setting to connect ESP32 to the internet:
    - Update `wifissid.txt` and `wifipassword.txt` under `src/main-esp32-dht22/data` folder. And change it according to your wifi, to make sure the ESP32 can connect to the internet
4. Prepare API Key and Host

    ```c
    const String apiKey = ""; // should be for x-api-key header value
    String deviceSerial = ""; // should be deviceSerial
    String host = "https://api.test.com"; // yout API host
    ```

## Upload Sketch Data
1. Upload SPIFFS data by using `Tools -> ESP32 Sketch Data Upload` (on Arduino IDE)
2. After that, just flash by pressing Upload button on Arduino IDE.

## Wifi Setting via `192.198.4.1` 
- Make sure to connect on wifi AP named `esp32_0` and `test`.
- And hit `192.198.4.1` via your browser, then edit this:

![image](https://user-images.githubusercontent.com/738088/151668174-d743065a-65b2-46de-a2f4-411f5fd89963.png)


## Credits
- ESP32
- Not yet, will be updated soon

# ESP32, DHT22 and HTTPS Post

This project is under development, it just for testing with ESP32, DHT22 and hit API's served somewhere.

> Still on going development

## Prerequisite
1. Install ESP32 board manager to arduino IDE, Platformio, or VSCode Arduino
2. you need the SPPIFS sketch data uploader, to include it in the flash, please follow the tutorial [here](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/)
3. Prepare wifi setting to connect ESP32 to the internet:
    - Update `wifissid.txt` and `wifipassword.txt` under `src/main-esp32-dht22/data` folder. And change it according to your wifi, to make sure the ESP32 can connect to the internet
4. Prepare API Key and Host

    ```c++
    const String apiKey = ""; // should be for x-api-key header value
    String deviceSerial = ""; // should be deviceSerial
    String host = "https://api.test.com"; // yout API host
    ```

## Upload Sketch Data
1. Upload SPPIFS data by using `Tools -> ESP32 Sketch Data Upload` (on Arduino IDE)
2. After that, just flash by pressing Upload button on Arduino IDE.

## Credits
- ESP32
- Not yet, will be updated soon

# actuator-server

## parts
- [MH-ET Live Minikit for ESP32](https://ja.aliexpress.com/item/32815530502.html?spm=a2g0o.order_list.0.0.1b65585aAj7Hnv&gatewayAdapt=glo2jpn) (you need to install driver)  
    Any microcontroller board can be used if it has esp32.
- [SG90 servo motor](https://akizukidenshi.com/catalog/g/gM-08761/)  
    Any pwm servo motor can be used.
- micro usb cable

## circuits
So simple! Just connect the servo to the pin.  
Power source should be micro usb or similar.  

<img src="https://user-images.githubusercontent.com/22733958/168075794-8cf3f79c-53e5-4f6f-a2a7-b740d3d8c2b2.png" width="400">


## Usage

### build and upload
1. Clone the repository
    ```
    $ git clone https://github.com/takeyamayuki/actuator-server.git
    ```
2. Define `ssid`, `password` of your wifi router by creating header file.
    ```sh
    $ mkdir src/ssid_define.h
    $ nano src/ssid_define.h
    ``` 
    Add the following statement to ssid_define.h
    ```cpp
    #ifndef _SSID_DEFINE_
    #define _SSID_DEFINE_

    #define MY_SSID "your ssid here"
    #define MY_SSID_PASS "your ssid password here"

    #endif
    ```
3. Change `upload_port`, `monitor_port` in [platformio.ini](platformio.ini) to your own.
4. Build and upload using platformIO.
5. Install this system wherever you like!  


### PUT method

`Free angle designation mode` (angle!=0)

```sh
# a servo motor moves to the angle specified by `angle`
$ curl {your IP address here} -X PUT -H "Content-Type: text/plain" -d "angle"
# e.g.
$ curl 192.168.0.28 -X PUT -H "Content-Type: text/plain" -d "120"
```

`SwitchBot mode` (angle=0)  

```sh
# a servo motor only goes back and forth between 0° and 30°.
$ curl {your IP address here} -X PUT -H "Content-Type: text/plain" -d "0"
# e.g.
$ curl 192.168.0.28 -X PUT -H "Content-Type: text/plain" -d "0"
```

### GET method

```sh
# Get the current angle.
$ curl {your IP address here}
```
or you can browse current servo angle by using browser.
<img width="792" alt="スクリーンショット 2022-05-18 午後5 19 51" src="https://user-images.githubusercontent.com/22733958/169011735-fc1e0e40-9b51-48f4-82f2-e8c700f44f8a.png">



Writing {your IP address} as `esp32.local` works, but it is slow.

## connect to homebridge server
In the homebridge config editor, add the following to the `accessories` section:
```
"accessories": [
    {
        "accessory": "CMD",
        "name": "light",
        "on_cmd": "curl esp32.local -X PUT -H 'Content-Type: text/plain' -d '0'",
        "off_cmd": "curl esp32.local -X PUT -H 'Content-Type: text/plain' -d '0'"
    }
]
```

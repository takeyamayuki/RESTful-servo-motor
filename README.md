# actuator-server

## parts
- [MH-ET Live Minikit for ESP32](https://ja.aliexpress.com/item/32815530502.html?spm=a2g0o.order_list.0.0.1b65585aAj7Hnv&gatewayAdapt=glo2jpn)(you need to install driver)  
    Any microcontroller board can be used if it has esp32
- [SG90](https://akizukidenshi.com/catalog/g/gM-08761/)
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
2. Change `ssid`, `password` in [main.cpp](src/main.cpp) to your wifi router password and ssid.  
3. Change `upload_port`, `monitor_port` in [platformio.ini](platformio.ini) to your own.
4. Build and upload using platformIO.


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
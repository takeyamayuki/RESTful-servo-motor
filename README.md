# actuator-server

## parts
- [MH-ET Live Minikit for ESP32](https://ja.aliexpress.com/item/32815530502.html?spm=a2g0o.order_list.0.0.1b65585aAj7Hnv&gatewayAdapt=glo2jpn)(you need to install driver)  
    Any microcontroller board can be used as long as it has esp32
- [SG90](https://akizukidenshi.com/catalog/g/gM-08761/)

## circuits
So simple! Just connect the servo to the pin.  
Power source should be micro usb or similar.  

<img src="https://user-images.githubusercontent.com/22733958/168075794-8cf3f79c-53e5-4f6f-a2a7-b740d3d8c2b2.png" width="400">

## build and upload
1. Clone the repository
    ```
    $ git clone https://github.com/takeyamayuki/actuator-server.git
    ```
2. Change ssid, password in [main.cpp](src/main.cpp) to your wifi router password and ssid.  
3. Change upload port, monitor port in [platformio.ini](platformio.ini) to your own.
4. Build and upload using platformIO.


## how to use
### PUT method

`Free angle designation mode` (angle!=0)

a servo motor moves to the angle specified by `angle`
```sh
$ curl {your IP address here} -X PUT -H "Content-Type: text/plain" -d "angle"
```
e.g.
```sh
$ curl 192.168.0.28 -X PUT -H "Content-Type: text/plain" -d "120"
```

`SwitchBot mode` (angle=0)  

a servo motor only goes back and forth between 0° and 30°.
```sh
$ curl {your IP address here} -X PUT -H "Content-Type: text/plain" -d "0"
```
e.g.
```sh  
$ curl 192.168.0.28 -X PUT -H "Content-Type: text/plain" -d "0"
```

### GET method
Get the current angle.
```sh
$ curl {your IP address here}
```
or you can browse current servo angle by using browser.


{your IP address here} works with `esp32.local`, but it's slow

## connect to homebridge server
In the homebridge config editor, add the following to the `accessories` section:
```
"accessories": [
    {
        "accessory": "CMD",
        "name": "light",
        "on_cmd": "curl 192.168.0.28 -X PUT -H 'Content-Type: text/plain' -d '0'",
        "off_cmd": "curl 192.168.0.28 -X PUT -H 'Content-Type: text/plain' -d '0'"
    }
]
```
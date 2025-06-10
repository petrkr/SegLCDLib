# SegLCDlib
Arduino library for segment LCD displayes and controlers.

It include abstract class, drivers and LCD display implementations.


For now LCD class is derivered from controller's class, because mostly those controllers are part of LCD already and there is no intent to use it with different.

But there could be possible to detach it and somehow redone interface, but then there must be two instances in main code. First for controller, second for LCD itself. Maybe in some next version if that will be even actual.

## Supported controllers

| Name     | Type |
|----------|------|
| PCF85176 | I2C  |

### PCF85176 Board
![PCF85176 Board](docs/images/pcf85176board.webp)

## Supported LCD

| Name                               | Controller(s) | Link |
|------------------------------------|---------------|------|
| RAW LCD                            | PCF85176      | N/A  |
| 6-digit signal battery progresess  | PCF85176      | N/A  |
| One Digit (up to 5 segments)                            | PCF85176      | https://aliexpress.com/item/1005005410565386.html |
| T1T2 Lcd                           | PCF85176      | https://aliexpress.com/item/32333296186.html      |
| Temp humidity                      | PCF85176      | https://aliexpress.com/item/1005003044283980.html |


### RAW LCD
This LCD type is used for testing new LCD by sending RAW data to controller. It is good to testing and implementing new types of LCD screens before starting to create actually class


### 6-digit signal, battery, progress
![6-digit, signal, battery, progress](docs/images/6digsigbatprogress.webp)

### One digit
![Onedigit](docs/images/onedigit.webp)


### T1T2 Lcd
![T1T2](docs/images/t1t2lcd.webp)


### Temp humidity
![Onedigit](docs/images/temphumlcd.webp)


## TODO
 - Implement basic "aliexpress" LCD with HT1621 controller
 - Think about some generic charmap/font for 7, 14 and 16 seg digits.
 - Make some generic write* functions, for now only writeFloat and writeChar, think about writeInt, which would be just call of writeFloat with decimal=0, or rename that to writeNumber or some different way? Before first version is released, this still can change.

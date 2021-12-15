# Smart lamp with ESP8266
This is simple tutorial how to make smart lamp with ESP8266. Before you begin here you can look at video how the end result will look:

[![Smart lamp showcase](https://img.youtube.com/vi/P4-cqObLHpA/0.jpg)](https://www.youtube.com/watch?v=P4-cqObLHpA)

The lamp is controlled using two touch sensors and wifi (in hotspot mode).

## What will you need?
First of all you will need some lamp. It should be lamp powered by external power supply (the thing which you plug into power outlet is somewhat bigger that classical plug). Also there should be output voltage writen on the power supply or lamp. Anything from 9 to 24V is fine as those all are safe voltages for human touch so you can't hurt yourself.

Next you will need to buy few things:

- [ESP8266](https://www.aliexpress.com/item/32656775273.html)
- [Step down power supply](https://www.aliexpress.com/item/33015256539.html)
- [Touch sensor](https://www.aliexpress.com/item/4000540638639.html)
- [Transistor](https://www.aliexpress.com/item/32878220199.html)

Also you will need some other things that you probably have lying around:

- (Electrical) tape + heat shrink tube
- Some wires
- Paper or some other stuff for filling space
- Micro USB cable

## Let's look at it

TODO

## Now we begin

After you have everything ready you can start uploading code to your ESP8266. First you will need to download and install [Arduino IDE](https://www.arduino.cc/en/software). The project file can be found in folder `lamp_code`. Then open `File` -> `Preferences` and copy following URL `https://arduino.esp8266.com/stable/package_esp8266com_index.json` to `Additional board manager URL` field. Next open `Tools` -> `Development board` -> `ESP8266 baords` and choose `Generic ESP8266 module`. Now you just need to click arrow icon to start upload.

If you are getting error `serial.serialutil.SerialException: could not open port` then you need to `Tools` -> `Port` and choose some port.

You just might want to change this command `WiFi.softAP()` to one which will contain your new wifi name and password.

Now you will need to open your lamp and solder everything together according to schematics. After everything is soldered don't close up your lamp yet. Just turn it on and hope everything will work, if not then you will need to find bug or mistake.

If everything worked just fine put touch sensor close to inside of lamp case. If it turns on by itself you will need to get some pieces of paper or other stuff and make big enough space so it doesn't turn on itself. Now turn on your lamp and try to touch lamp case from outside, if nothing happens then try to touch by your whole palm. If still nothing happens that means you created too big space from paper and you will need to remove some.

After you are done you should put again some paper or something to cover the touch sensors. This will protect them from beign touched by wires which could induce false touches.

Then you can wrap all components into tape which will protect them from shor circuits. After this you are done, so you can close your lamp and enjoy it!

## Credits
- [Gamma table generator](https://victornpb.github.io/gamma-table-generator/)
- [Range slider](https://www.w3schools.com/howto/howto_js_rangeslider.asp)
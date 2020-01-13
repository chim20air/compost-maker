# Compost Maker Overview
This project was made with the intend of simulate a way to control a compost maker. It uses an Arduino UNO and a PC with labview installed. In order to get all the project details, you can go and check the project's report named "informe labview.pdf". For now it is on spanish, later i'll translate it to english.

The Arduino program is quite simple, it reads the voltage input from temperature sensor input several times and averages all the readings so it can mitigate noise effect. Then converts the voltage number into a temperature (celsius) number and call the PID object. Finally, the Arduino sends all the data (temperature, PID data, latch state and PWM output) to the computer.
The latch state is the D7 pin which is wired to a tilt sensor.

Temperature sensor is supposed to be a 4 wire PT100 sensor, but was replaced with a potentiometer due to lack of money. And the way for reading it is using a current source (LM317).

schematic:
![schematic](https://github.com/chim20air/compost-maker/blob/master/schem.jpg)

On the labview side, it recieves Arduino data, decodes it and visualize all the values
![LabView interface](https://github.com/chim20air/compost-maker/blob/master/Labview_front_panel.jpg)
You can also send the temperature setpoint and open or close the electrovalve that allows the gas escape.

In order to use the vi, you need to get installed VISA, so it can comunicate with Arduino thru USB port.

Labview vi:
!(https://github.com/chim20air/compost-maker/blob/master/Labview_Block_diagram_2.jpg)

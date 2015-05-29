Cal's system:
PIC32mx360F512L
uses
D0 D1 D2 for LED's
UART1

Other group's system:
PIC32mx460F512L (?)
uses
A0 A1 A2 to A8 for LED's
UART2

So we need to do a simple define system for configuration purposes.
I will create a file called boarddefinition.h that you will include anywhere there might be a question. I will try to encapuslate both the LED operaiton and the UART operation into this functionality but I cannot test it on the other hardware, that will have to be done by the rest of the group.

I recommmend you don't push or pull the boarddefinition.h  file after we have things working and configured so code will keep working on both systems concurrently.

I will assume my board has a value of 1, and the other board has a value of 0.




I do not know how to fix the issue of having to use a different microcontroller, that will have to be selected manually if I have edited the project.
It may have to happen where we all use our own versions of the project and just share source files through git.


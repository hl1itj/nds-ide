THE APPLICATION:
This application is made of two tasks. The lower priority task1 is autostart.
It prints its name to the display, initialize the motorA to 50% of its speed and do an infinit loop. task5 is connected to an alarm which is launched every second. It increases motorB speed by 5% (as the type of the speed parameter is an 8 bits, once arrived to 256, it will change its sens) and prints its name to the display. The NXT buttons are connected to ISR2s thus if you press the left, right or "orange" button, each ISR2s will print to the display the ISR2 name. The stop button shutdowns the Trampoline application.
 
IMPORTANT:
Even if you don't need alarms, you have to connect the SystemCounter to it_timer1 source. It will check the buttons status every 100ms and you'll be able to connect ISRs (it's temporary).
If you'ld like to connect ISRs to a device, use the source name in goil/templates/config/arm/nxt/target.cfg like the four ISR2 already connected.
Whatever ISR you'ld like to add (ISR1 or ISR2) with the name isr_button_start, you have to add the function "ISR(isr_button_start)" in your files (including tpl_os_definition.h).
 
GET STARTED:
Get started with the documentation in documentation/getstarted/getstarted.pdf

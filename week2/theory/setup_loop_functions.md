# setup()
Runs once when the Arduino starts or is reset. 
It is used to initialize pins, sensors, and serial communication.
# loop() 
Runs continuously after setup() and executes the main program repeatedly.

 # Effect of a long delay() inside loop()
 delay() pauses the entire program.
 During the delay, the Arduino cannot read sensors or respond to inputs.
 This reduces sensor responsiveness and may cause important events to be missed.

# Non-blocking Alternative
 Use the  millis()  function instead of delay().
 millis() allows the Arduino to perform other tasks while keeping track of time, making sensor readings more responsive.

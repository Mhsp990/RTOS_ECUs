# RTOS_ECUs
Arduino with trampoline (RTOS) for simulating 3 car ECU ( ECM, ICM, TCM).
In each folder there is one source code (both the trampoline + arduino code). 
For the system to work, you must set up a circuit with 3 arduinos. Each one must have a CAN module MCP2515 and their CAN module must be connected between themselves.
Also, the folder already have it included the .hex file, which is the result of compiling the code (trampoline+arduino). If you just want to use the code, you can choose to only upload it without compiling. Even then, check the code to see the GPIOs states.
Link for document (portuguese) with more details: https://drive.google.com/file/d/1alC0a50mgegHeohx6voYWGcRAPXDgHS9/view?usp=sharing

# ECE4180-Door-Unlocking-Mechanism

Team Members: Audrey Ahlenius and Nikhil Chittaluru

## What is it?

**Problem**: Automatic locking doors in Georgia Tech housing buildings result in students getting locked out

**Solution**: A product that will unlock your door via bluetooth or a touch pad

(FULL IMAGE HERE)

## Hardware

Below is all the hardware for the project

(FULL HARDWARE IMAGE HERE)

#### Door Mount

This is the hardware for mounting our device to the door frame. It fits around the doors locking system so that it does not interfere with the actual door lock mechanics. There is a 3D printed mount that fastens the system to the door without using tape. It can also be adjusted depending on the type of door lock system.

See the `/assets/cad` file to find all the STEP files for the components fo the door mount. Each plate was cut from 0.125" birch wood using a laser cutter. The other components were made using a 3D printer.

(SIDE BY SIDE IMAGE HERE)

#### Electronics Parts List

###### mbed
###### uLCD
###### Capacitive Touchpad
###### Bluetooth Module
###### 12V DC motor
###### Motor Driver
###### DC-DC converter
###### 12V Power Adapter

### Software

The software has two threads and two interrupts. Overall, it checks when a user submits a passcode. If the passcode is correct, it runs an open-door sequence. If the passcode is wrong, it tells the user using the LCD screen.

(SOFTWARE FLOWCHART IMAGE HERE)

**Threads**:

* The main thread handles the user input through either the capacitive touch pad or the bluetooth module. It also displays the updated inputs to the LCD screen based on boolean flags raised by the interrupts. When a flag is raised, the thread checks the user-inputted passcode with the set passcode. If the two match, it raises a flag to let the motor thread know to run the open-door sequence.
* The motor thread runs the open-door sequence to let the user into the room. Once it sees the flag raised by the main thread, it turns on the motor and starts a timer. Once the door handle hits a limit switch (indicating the door is open), the motor and timer both stop. The thread waits 5 seconds for the user to enter the room before running the motor backwards for the open sequence duration (based on the timer). 

**Interrupts**:

* The Bluetooth interrupt handles when a message is received from the Bluefruit Module serial port. It goes through the received message and creates a passcode buffer. It then raises a flag for the main thread to check the buffer against the set passcode.
* The Capacitive Touchpad interrupt runs whenever it detects a change in input from the pad. Each pad is correlated with a binary value which is then converted to a `char` representing a number `0-9`, `clear`, `enter`, or `no entry`. It saves the sequence of numbers with each pad press. If:
  * `clear` is pressed, the buffer is cleared
  * `enter` is pressed, it raises a flag to check the buffer against the set passcode.

### Results and Future Work

The door is unlocked through a password entered via Bluetooth or on the Touchpad so we met the project goals!

**Potential Improvements:**:

While the system works well, these are some ways to improve the system 
* Add Bluetooth User interface: buttons, message sending, GUI
* Reduce profile with a PCB
* Include a fingerprint scanner for door lock input
* Use a distance sensor (instead of limit switch) for different door styles

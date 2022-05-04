# ECE4180-Door-Unlocking-Mechanism

Team Members: Audrey Ahlenius and Nikhil Chittaluru

## What is it?

**Problem**: Automatic locking doors in Georgia Tech housing buildings result in students getting locked out

**Solution**: A product that will unlock your door via bluetooth or a touch pad

<img src="/assets/images/door_front.jpg" width="300" height="579"> <img src="/assets/images/door_back.jpg" width="300" height="579">



## Hardware

Here is what our system looks like:

<img src="/assets/images/full_model.jpg" width="500" height="666">

**Modules**: Main Electronics Board, Door UI, Door Handle Motor (from left to right)

<img src="/assets/images/board.jpg" width="300" height="400"> <img src="/assets/images/touchpad.jpg" width="300" height="400"> <img src="/assets/images/motor.jpg" width="300" height="400">

**Side Profiles**:

<img src="/assets/images/side_model.jpg" width="400" height="300"> <img src="/assets/images/limit.jpg" width="400" height="300">

More details about the hardware can be found below

#### Door Mount

This is the hardware for mounting our device to the door frame. It fits around the doors locking system so that it does not interfere with the actual door lock mechanics. There is a 3D printed mount that fastens the system to the door without using tape. It can also be adjusted depending on the type of door lock system.

See the `/assets/cad` file to find all the STEP files for the components fo the door mount. Each plate was cut from 0.125" birch wood using a laser cutter. The other components were made using a 3D printer.

<img src="/assets/images/Full_CAD_Model.png" width="500" height="359">


#### Electronics Parts List

##### mbed

<img src="https://cdn.sparkfun.com//assets/parts/3/3/3/6/09564-05.jpg" width="300">

https://www.sparkfun.com/products/9564

##### uLCD

<img src="https://4dsystems.com.au/media/catalog/product/cache/90cb4f1f3ce4772e2797c82794a53907/u/l/ulcd-144-g2.png" width="300">
<img src="/assets/images/Pinouts/uLCD.png" width="200">

https://4dsystems.com.au/ulcd-144-g2

##### Capacitive Touchpad

<img src="https://cdn.sparkfun.com//assets/parts/4/6/0/2/10250-01.jpg" width="300">
<img src="/assets/images/Pinouts/touchpad.png" width="200">

https://www.sparkfun.com/products/retired/10250

##### Bluetooth Module

<img src="https://www.digikey.com/htmldatasheets/production/1801441/0/0/1/media/bg1.jpg" width="300">
<img src="/assets/images/Pinouts/bluetooth.png" width="200">

https://www.digikey.com/en/htmldatasheets/production/1801441/0/0/1/bluefruit-le-uart-friend.html?site=US&lang=en&cur=USD&utm_adgroup=xGeneral&utm_source=bing&utm_medium=cpc&utm_campaign=Dynamic%20Search_EN_Product&utm_term=digikey&utm_content=xGeneral&utm_id=bi_cmp-420474294_adg-1309518711610365_ad-81844996246684_dat-2333644710616871:loc-190_dev-c_ext-_prd-&msclkid=af7b0693f4b1106f3d7964e173b06424

##### 12V DC motor

<img src="http://www.zgcmotors.com/wp-content/uploads/2021/03/37JB6K33ZY24.jpg" width="300">

http://www.zgcmotors.com/pro/37jb6k-33zy24/

##### DC-DC converter

<img src="https://m.media-amazon.com/images/I/610V-ctKikL._AC_SX679_.jpg" width="300">

https://www.amazon.com/Dafurui-Adjustable-Converter-3-0-40V-Regulator/dp/B08CGL6FQD/ref=sr_1_1_sspa?adgrpid=1338106218203959&hvadid=83631709922513&hvbmt=be&hvdev=c&hvlocint=4087&hvlocphy=78923&hvnetw=o&hvqmt=e&hvtargid=kwd-83631989489870%3Aloc-190&hydadcr=19102_10726399&keywords=lm2596+dc-dc&qid=1651673571&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyVkNCNUJLS0dDTDk4JmVuY3J5cHRlZElkPUEwMDAzMTc5MVFVNlBWQ0E1VVpHNSZlbmNyeXB0ZWRBZElkPUEwNzEzMzIxMjU5VTI0UzdYUE5LUyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=

##### 12V Power Adapter

<img src="https://m.media-amazon.com/images/I/71On5Ku29OL._AC_SX679_.jpg" width="300">

https://www.amazon.com/MyVolts-supply-adaptor-compatible-CG2412-B/dp/B012TMPGUU?th=1


### Software

The software has two threads and two interrupts. Overall, it checks when a user submits a passcode. If the passcode is correct, it runs an open-door sequence. If the passcode is wrong, it tells the user using the LCD screen.

<img src="/assets/images/Software_Diagram.png" width="500">


**Threads**:

* The `main` thread handles the user input through either the capacitive touch pad or the bluetooth module. It also displays the updated inputs to the LCD screen based on boolean flags raised by the interrupts. When a flag is raised, the thread checks the user-inputted passcode with the set passcode. If the two match, it raises a flag to let the motor thread know to run the open-door sequence.
* The `motor` thread runs the open-door sequence to let the user into the room. Once it sees the flag raised by the main thread, it turns on the motor and starts a timer. Once the door handle hits a limit switch (indicating the door is open), the motor and timer both stop. The thread waits 5 seconds for the user to enter the room before running the motor backwards for the open sequence duration (based on the timer). 

**Interrupts**:

* The Bluetooth interrupt handles when a message is received from the Bluefruit Module serial port. It goes through the received message and creates a passcode buffer. It then raises a flag for the main thread to check the buffer against the set passcode.
* The Capacitive Touchpad interrupt runs whenever it detects a change in input from the pad. Each pad is correlated with a binary value which is then converted to a `char` representing a number `0-9`, `clear`, `enter`, or `no entry`. It saves the sequence of numbers with each pad press. If:
  * `clear` is pressed, the buffer is cleared
  * `enter` is pressed, it raises a flag to check the buffer against the set passcode.

### Results and Future Work

The door is unlocked through a password entered via Bluetooth or on the Touchpad so we met the project goals! See the video demos below:

###### Demo: Outside the Door

https://www.youtube.com/watch?v=1BmhXal_50E&ab_channel=AudreyAhlenius

###### Demo: Inside the Door

https://youtube.com/shorts/F9PI_cVdQdg?feature=share

###### Demo: Bluetooth Password Input

https://youtube.com/shorts/ofLp8rDiyAs?feature=share

**Potential Improvements:**:

While the system works well, these are some ways to improve the system 
* Add Bluetooth User interface: buttons, message sending, GUI
* Reduce profile with a PCB
* Include a fingerprint scanner for door lock input
* Use a distance sensor (instead of limit switch) for different door styles

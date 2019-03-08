
Here is a suggested outline for your proposal. You don't need to follow this
outline, but it may help you focus your project.

* Project title
* Team members
* Description (1 pp describing the basic idea)
* Hardware required (itemize what hardware, if any, you need)
* Funds requested (the amount you will want to be reimbursed, remember you have
  a $20 budget per person)
* Major task breakdown and who is responsible for each task
* Goals for next week
* References and resources used

Keep the proposal short and to the point.

We suggest you try to get all the major hardware components working by the
first week. That will leave the following week for putting the components
together into a working system.

# Project Proposal

**Project title**: Automated Garden Watering and Monitoring System

**Team members**: Patin Inkaew (patinkaew), Arjun Karanam (QuantumArjun)

**Description**

We're building a Smart Garden system to help those who are traveling (or just people who don't have a green thumb) better take take of their plants. The core functionality of our project will be to detect the humidity of plant soil and pump water in whenever the soil starts to get dry. After we complete this core functionality, we'll then go into the extensions listed below.

* Measure height of plants using ultrasonic sensor
* Adjust the light intensity using light sensor and light source
* Store data log from sensor in pi memory
* Make graphic interface to show data in table/histogram graph on screen
* Send data via wifi/bluetooth

**Hardware**

* 3V submersible pump
* Tube and water container
* Weather sensor (BME280)
* Soil moisture sensor

(Extension)

* Ultrasonic sensors
* Light sensor and light source
* ESP32 wifi module / Bluetooth

**Order list and Funds requested**

* [3-4.5V mini pump 5pcs $12.99 on Amazon](https://www.amazon.com/JIUWU-Horizontal-Micro-Submersible-3-4-5V/dp/B01N0Y4R83/ref=sr_1_1?keywords=3v+water+pump&qid=1552029359&s=lawn-garden&sr=1-1)
* [Vinyl tubing ID 7mm 3.2 feet long $7.94 on Amazon](https://www.amazon.com/Yobett-Vinyl-Tubing-3-2-0-39-Cooling/dp/B00IXBZDD2/ref=sr_1_4?keywords=7mm+tubing&qid=1552029964&s=industrial&sr=1-4)
* (Brought) [ESP32 $10.99 on Amazon](https://www.amazon.com/gp/product/B0718T232Z/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
* (Brought) BME280

**Major tasks**

* Build structure (sensor and raspberry pi mount) and watering system (pump, water container, tubing)
* Write code for sensors (Soil moisure, BME280)
* Write code for actuator (water pump)

(Extension)
* Write code for ultrasonic array
* Write code for light sensor
* Write code for graphic library
* Write code for wifi communication and interface

**Timeline**

| Date | Todo | Note |
| --- | ----- | --- |
| Mar 5 | Brainstorm ideas | Project lab 1 |
| Mar 6 | Preliminary hardware testings | P (Wifi) & A (Ultrasonic) |
| Mar 7 | Preliminary hardware testings | P (Wifi) & A (Ultrasonic) |
| Mar 8 | Decide topic and write proposal | |
| Mar 9 | Proposal due @ 6 pm| |
| Mar 10 | Finish ordering parts <br> Start designing hardware <br> Write sensor modules <br> Write wifi modules | Weekend |
| Mar 11 | Finalize hardware design | Weekend |
| Mar 12 | | Project lab 2 |
| Mar 13 | Start building hardware| All parts should arrive |
| Mar 14 | Finish sensor modules <br> Finish wifi modules| |
| Mar 15 | Write system code (Combine modules)| |
| Mar 16 | | Weekend |
| Mar 17 | Finish assembling hardware <br> Finish system code| Weekend |
| Mar 18 | Testing | Final week |
| Mar 19 | Testing | Final week |
| Mar 20 | Testing | Final week |
| Mar 21 | Final system testing | Final week |
| Mar 22| Presentation Day! <br>Code due @ 9 am <br>Presentation @ 9-11.30 am| |


**References and resources**

For rough reference please see project reference sheet: http://bit.ly/2XCvHlX

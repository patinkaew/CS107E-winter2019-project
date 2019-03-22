# Project: Automated Plant Garden

#### Team Members: Arjun and Patin

What it does:

The Automated Garden is designed to take care of your plants for you completely hassle-free. Simply place a plant pot on the designated stand and place the moisture sensor and water pump into the soil of the plant (ideally on opposite sides). The Automatic garden will handle things from there.

The Auto-garden will monitor the moisture of the soil, taking measurements once every minute. Once 5 minutes have gone by with repeated dry soil readings, the Auto-garden will pump water into the soil. This is also recorded for calculation in future stats.

You can interact with the Auto-garden using a simple console terminal. Commands include: 
Start Watering (Command: start) - Turns on the Auto-gardening system, allowing it to monitor your plant.  
Stop Watering (Command: stop) - Turns off the Auto-gardening system.  
Check Moisture (Command: moisture) - Reports the current soil moisture as reported by the soil moisture sensor.  
Check Water (Command: water) - Reports the water contents of the water tank as measured by the ultrasonic sensor.  
Burst (Command: burst) - Briefly sprays a blast of water through the water pump.  
Water Graph (Command: graph) - Displays a graph over time of the amount of water remaining in the water container, reminding the user to refill it once it becomes empty.  

For display, you can add various data type, such as soil moisture, water level, and humidity and the program will plot those data at the same time. The graph can automically shift itself to display the new data. The monitor also show the lap time at the bottom left. Graph is fully customizable. Sensor sampler will measure value from various attached sensor every given time. Data will be stored in the pi and graph can take these data and plot it in real time.

From the hardware side, the model is 3d printer. Parts can be connected in various orientation like lego parts.
This is to match your own style! 



Project Breakdown:
  
Patin - Graph, Internal Data storage, Water Pump, Structure (3D printed parts), Water level sensor, Sensor Sampler
  
Arjun - Moisture Sensor and Shell

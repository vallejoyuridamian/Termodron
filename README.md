# Termodron 

Termodron was my final degree project for my Electrical Engineer degree. I did it with my colleagues Ignacio Reyes and Agustin Barriola.

The objective of the system was to detect hot spots in a given area. 

It is conformed by a quadcopter with a PixHawk autopilot, a GCS (Ground Control Station), and an ob-board controller. 

The project consisted in selecting the parts to build the quadcopter, build it, build and program both the GCS and on-board controllers, and test the whole system. 

The area was determined by the vertices of a polygon that was sent by the user via e-mail. The CGS had to receive the e-mail, calculate the mission (the set of points that the quadcopter should follow to be able to scan the area), and send them to the quadcopter. It also had a humidity and temperature sensor to be able to cancel/alter the mission under certain climate conditions. All the communication with the user could also be done via SMS.

In the on-board controller, there were 4 ultrasonic sensors, an SD shield and a thermal camera. The camera was hold by a gimbal to always stay in a horizontal plane. The sensors were use to avoid obstacles and could alter the mission. The camera takes a thermal image photo and sends it to the GCS if a hot spot is found. The SD shield saved a flight log in a txt file and all the images taken in BMP.

There is constant communicaction with the PixHawk and all that is done via MAVLink protocol. 

The GCS also connects to a DGPS (differential GPS) service and corrects the position of the drone. 

Finally, an e-mail is sent to the user if a hot spot is found alongside with its coordinates and a recap of the mission is also sent in the end.

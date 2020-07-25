# Garage_Door_Controller.
Control and status a garage door from the Internet.

This project uses a Particle Photon to allow a garage door to be opened and closed
using an Android app running on a smartphone.  The app also displays the current status
of the garage door -- open or closed.  The project uses our Wireless I/O Board for the Photon:
https://github.com/TeamPracticalProjects/Wireless_IO_Board

Garage door control and status is accessible over the Internet so that the door can be 
controlled from anywhere that the smartphone has Internet access.  No modifications are 
made to the garage door opener mechanism itself.

## Garage Door Activation.
Activation (opening and closing) of the garage door is via one of the original system's remote
door controls.  The normally open relay contacts on the Wireless I/O board are wired across
the pushbutton on the original door controller to activate the garage door opener as if
the button on the remote was manually pressed.  The Wireless I/O Board has an on-board 3.3 volt power 
regulator that can be used to replace the battery on the garage door opener.

## Garage Door Status.
The status of the garage door (open or closed) is determined by an ultrasonic distance sensor
that is mounted to read out the distance to where the garage door opens.  The sensor location
is chosen where the distance to a closed garage door is significantly different from the distance to
an open garage door (e.g. in the rafters above the door track).

## Cautions.
The project is intended to be mounted in a garage.  The following considerations apply:

1. Make sure that you have a good WiFi signal where the project is mounted.  We found that
using an external WiFi antenna on the Photon was very helpful.

2. If the garage is uninsulated, make sure that the ambient temperature remains within the
operating range of the electronics year round.  The Photon's specified ambient temperature
operating range is -4 to 60 degrees C (25 degrees to 140 degrees F).  The other electronic 
components have a similar temperature operating range.

3. The electronics require a source of 5 volt regulated power; usually supplied by a USB
"wall wart".  Make sure that you have AC power within a reasonable distance of the electronics
where you can plug in the "wall wart" and run a USB cable to the electronics.

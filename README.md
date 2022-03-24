# USB-Communication
Utilises an arduino microcontroller to send hardware input based on USB serial transmissions from the host

This solution aims to provide easier aiming for a specific FPS video game. Multiple screenshots are taken every second and are scanned for a specific colour. To promote efficiency, only a small portion, around the crosshair, is scanned. Once this colour is found, the distance between the crosshair and the discovered colour is calculated. This distance is sent to the arduino which moves the crosshair to the colour, taking into account the sensitivity multiplier.

An arduino was necessary to bypass the the Anticheat.

Live Demo: https://youtu.be/2EqEH2JyMmI

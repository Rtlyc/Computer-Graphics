This is the HW1 for Computer Graphics. This homework focuses on drawing circles with Bresenham’s scan-conversion algorithm to avoid any float.

All the proof process is in hw1.pdf for assignment(a)

assignment(b) We use OpenGL to draw points, and we can see that pixels at (x,y) where either x < 0
or y < 0 will not be shown up, so we will need to convert the coordinate correspondingly.

The code part is in draw_circle.cpp. Here is the building instructions:
(a) $mkdir build # create a build folder and build the program
(b) $cd build
(c) $cmake ..
(d) $make
(d) $./Circle

Once you run the program, you will see instructions in terminal in which it asks the user to choose homework sections in 'c','d','e'

if you choose 'c', you will need to input another three integers to represent the center of the circle at (x,y) and radius

if you choose 'd', you will read the input.txt file

if you choose 'e', you will read the input.txt file and draw the circle with the growing animation

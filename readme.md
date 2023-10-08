# Simple Minesweeper

This project is used to detect blue blobs on a green plane.

## Code structure

This project can be divided into two part: the OpenMV part (Part 1) and the control part (Part 2).

### Part 1: OpenMV Part
Part 1 is coded with Python since OpenMV mainly supports MicroPython. In the folder OpenMV, source code of Part 1 is stored. (Yes, only a single file!) You have to copy the `main.py` to the root path of OpenMV device storage so that it can be run in your OpenMV device.

The `main.py` contains settings of the camera and the logic about how to found mines from the images. Details can be found in the comment of the file.

### Part 2: Control Part
Part 2 is coded with C language. This part controls the car to move on given route. Codes are stored in `UserCode\CODE`.

**NEVER change the code in other folders of `UserCode`!!!**

In the `minesweeper.c` store two routes, one of them designed to slow run while another to fast run.

#### Applications
All the `.c` files in directory `apps` are built-in programs.

The main logic of the car stored in the `move_test.c`!

`icm_test.c`: Gyroscope checker.
`move_test.c`: Main program for the car moving in given route.
`pwm_checker.c`: Motor checker. You can view the encoder value when the PWM duty of motor is given.
`sector_editor.c`: USELESS! Edit the paramters stored in the flash. This program is useless in this project.
`spd_measure.c`: USELESS! Use least square fit method to get the correlation of PWM duty and speed.
`usound_test.c`: Ultrasound distance meter checker.

#### Drivers
All the `.c` files in directory `drivers` are drivers of hardwares.

#### OS
You should not change the code in directory 'os' since they are code of user interface and the dispatcher of applications and bootstrap process. **If you want to edit the parameters, you shoudl change `os_ext.c` and `syshead.h`**
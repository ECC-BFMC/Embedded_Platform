Tools for development 
======================

The low level control application, which runs on the Nucleo-F401RE, is implemented in C/C++ language 
by using `mbed 2` and `mbed 2 rtos` libraries. The project structure is simple, it contains 6 basic folder:
doc, examples, include, libs, src and build. The 'build' folder includes the built binary files, which need
to program the micro-controller. The 'libs' incorporates external libraries, like `mbed` and `rtos`.
The example main scripts for other software setup are implemented in the 'examples' folder. The features and 
other scripts are realized in the 'include' and 'src' folder. The flash_win.bat script can be used for flashing 
the micro-controller, please check the micro-controller mounted partition identification letter to be same 
with mentioned letter in script. When they doesn't match, you can update in the script the new one, like (D:, F:, C:). 
We used Visual Studio Code for developing. 

Building
---------

After you modified any part of the components you have to rebuild the application before the micro-controller flashing.
For building need to install some other software on your pc. Firstly you need a cross-compiler, you can find it on the 
official site of Arm Developer. Here is a `link <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>`_ .
If you installed the Gnu Embedded Toolchain for Arm, you need to add a new environment variable with 'GCC_ARM_FOLDER' name,
which value is the compiler folder. You can see an example on the following picture. 

.. image:: ../pics/others/env_var_compiler.png
    :align: center
    :scale: 75%

Other application necessary for easy building is MinGW, where is important to install 'msys-base-bin' 
and to add the location of installation folder to the 'Path' environment variable. Something like on the figure below. 

.. image:: ../pics/others/env_var_mingw.png
    :align: center
    :scale: 75%

If you set correctly the environment variable, then the 'make' command is recognized in command prompt 
and appears a message with missing target and makefile. When you have done the two steps described above, 
then you finished the setup and you can compile the code. 
To build you need to open a command prompt (or terminal) and go to the project folder, where you can find
the 'Makefile'. When you are in the right location, you have to apply the 'make' command and it will build
automatically the script in the 'BUILD' folder. If everything went well, then a new binary file 
with name 'Nucleo_mbedrobot.bin' appeared in the 'BUILD' folder. Success compilation termites with following message:
    '===== bin file ready to flash: BUILD/Nucleo_mbedrobot.bin ====='. 
This binary you can copy on the micro-controller to flash it or you can apply the 'flash_win.bat' script.

New component
-------------
When you want to implement some new feature, it's recommended to keep clean the project structure.
For this reason, you need to realize write the header files in the 'include' directory and the 
source file in the 'src' folder. You can do this by appling the 'newComponent.pt' script written in python, which
creates automatically the include files and the source file. There are some optional parameter, you can
check they by adding '--help' parameter. Examples:
    python newComponent.py --help
    python newComponent.py -c test
In the second example, the script creates two includes file ('test.hpp' and 'test.inl') in the 'include' folder
and a source file ('test.cpp') in the 'src' folder. If you can to compile these new components, then you must 
add these to makefile like new objects. You can see on the figure below, where new object is defined in the 72nd line:

.. image:: ../pics/others/makefile_example.png
    :align: center
    :scale: 75%

Now you can compile your application with the new component by applying command 'make'. You have to see the added component 
between the listed source files, like on the figure below:

.. image:: ../pics/others/compiling_message.png
    :align: center
    :scale: 75%

Flashing 
--------

The flashing for Nucleo is a very simple process, you have to connect the micro-controller to you pc, then it has to appear a 
new device, named 'NODE_F401RE'. The nucleo board has to main part, the programmer/debugger and the micro-controller. The programmer 
has the mini-usb connector and a  tricolor LED, you can see in the documentation. The micro-controller has the connectors, reset and user
button, power and built-in led. Before you flash, you have to make sure the micro-controller is powered on. This is indicated by 
the power LED (LD3 - a red LED). The micro-controller can be powered by external source or by the programmer, the jumper JP5 
near the reset button controllers the power source (E5V - via external or U5V - via usb). When the micro-controller is powered on, you 
can easily copy the binary file from you device to the mentioned external device. The process is done, when the blinking Red/Green stopped
and the tricolor LED has green color. If you modified correctly the 'flash_win.bat' script and the device identification letter is same 
in the script mention, than it copies the binary automatically by applying the script.

Note
----

If you use Linux instead of Windows, then you don't need install MinGW with mysys and set the 'path' environment variable to this, 
because all GNU Core utilities are already installed. It requires to setup the Gnu Embedded Toolchain for Arm, which means to download
a linux version from the below mention `link <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>`_  
and decompress some on your pc. After decompressing, you need to set the 'GCC_ARM_FOLDER' environment variable with the location, where you decompressed
the cross-compiler. Therefore the 'make' utility knows the location of cross-compiler and can apply automatically. To add the persistent environment variable need to 
introduce a code in terminal similar to the following:
    "echo "export GCC_ARM_FOLDER=/home/user/Workspace/Crosscompilers/gcc-arm-none-eabi-8-2019-q3-update/bin" >> ~/.bashrc"
The variable is accessible for only your user and you have to restart the terminal. After the setup you have to change the location in 
terminal to main project folder and you can run the 'make' script. The flashing is similarly to the below presented process, it requires only 
to copy binary file to external device. 
The scripts for creating a new component (newComponent.py) and for flashing the micro-controller weren't projected to linux usage, so we cannot guarantee the 
correct working of them. 
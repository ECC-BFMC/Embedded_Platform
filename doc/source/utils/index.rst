Utils namespace
===============

The 'utils' namespace includes some util functionalities to manage task periodic, communication through serial. 
It has four package: linalg, queue, serial and task manager. The linalg package contains the matrix's operation. 
The queue package realizes the functionalities of a queue,it's a general implementation of fifo container (first-in-first-out).
The serial package has a 'CSerialMonitor', which aims to interpret the received messaged from the other device and redirect to other 
software components, for providing an access interface. The 'task' package realizes the managing multiple periodical task by using a 'CTask' base class
and 'CTaskManager' task, where the CTaskManager applies the subclasses of 'CTask' in a predefined period. 

.. image:: ../pics/diagrams/pics/Class_Serial.png
    :align: center

.. doxygenclass::  utils::serial::CSerialMonitor
   :project: myproject
   :members: 
   :undoc-members:
   :private-members:

.. image:: ../pics/diagrams/pics/Class_Task.png
    :align: center

.. doxygenclass::  utils::task::CTask
   :project: myproject
   :members: 
   :undoc-members:
   :private-members:


.. doxygenclass::  utils::task::CTaskManager
   :project: myproject
   :members: 
   :undoc-members:
   :private-members:

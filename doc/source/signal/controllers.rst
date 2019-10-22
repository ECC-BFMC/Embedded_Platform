Controllers namespace
=====================

The 'controllers' namespace implements some control functionalities, like some converter, a siso controller, motor-controller.
The converter aims to transform the signal from one unit measurement to other. The siso controller involves realization of a 
proportional–integral–derivative controller and an abstract interface for controllers with single input and output signal. 
The motor-controller interprets a regulator for motor's rotary speed with some safety restriction. 

.. image:: ../pics/diagrams/pics/Class_Controllers.png
    :align: center

.. doxygenclass::  signal::controllers::IConverter
   :project: myproject
   :members:
   :undoc-members:


.. doxygenclass::  signal::controllers::CConverterPolynom
   :project: myproject
   :members:
   :undoc-members:


.. doxygenclass::  signal::controllers::CConverterSpline
   :project: myproject
   :members: 
   :undoc-members:


.. doxygenclass::  signal::controllers::siso::IController
   :project: myproject
   :members: 
   :undoc-members:


.. doxygenclass::  signal::controllers::siso::CPidController
   :project: myproject
   :members: 
   :undoc-members:

.. doxygenclass::  signal::controllers::CMotorController
   :project: myproject
   :members: 
   :undoc-members:

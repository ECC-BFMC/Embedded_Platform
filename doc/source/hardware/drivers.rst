Drivers namespace
=================

This namespace contains some applications to interact with actuators and sensors. 
In our case, three different drivers are realized: dc motor driver, servo motor driver and 
quadrature counter.

.. image:: ../pics/diagrams/pics/Class_Drivers.png
    :align: center

.. doxygenclass:: hardware::drivers::ICurrentGetter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::drivers::IMotorCommand
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::drivers::CMotorDriverVnh
   :project: myproject
   :members:
   :private-members:

.. doxygenclass:: hardware::drivers::ISteeringCommand
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::drivers::CSteeringMotor
   :project: myproject
   :members:
   :private-members:


.. doxygenclass:: hardware::drivers::IQuadratureCounter_TIMX
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::drivers::CQuadratureCounter_TIM4
   :project: myproject
   :members:

.. doxygenclass:: hardware::drivers::CQuadratureCounter_TIM4::CQuadratureCounter_TIM4_Destroyer
   :project: myproject
   :members:
   

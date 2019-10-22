Encoder namespace
=================

In the 'encoder' namespace, the rotary speed encoder functionalities are implemented. 
It contains two interfaces to access filtered and non-filtered values
and two classes with encoder services. 


.. image:: ../pics/diagrams/pics/Class_Encodder.png
    :align: center

.. doxygenclass::  hardware::encoders::IEncoderGetter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::encoders::CQuadratureEncoder
   :project: myproject
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

.. doxygenclass::  hardware::encoders::IEncoderNonFilteredGetter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: hardware::encoders::CQuadratureEncoderWithFilter
   :project: myproject
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

   
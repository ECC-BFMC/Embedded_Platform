Encoder namespace
=================

In the 'encoder' namespace are implemented the rotary speed encoder functionalities, 
it contains two interfaces to access filtered and non-filtered values
and two class with encoder services. 


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

   
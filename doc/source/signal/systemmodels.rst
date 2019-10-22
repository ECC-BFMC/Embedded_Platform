System Models namespace
=======================

The 'systemmodels' namespace contains some representation of systems, like transfer functions, state space model. 
These classes aims to symbolize the behavior of a system in domain of discrete time, so it can be used to implement a filter, 
a controller or other systems, for example can interpret the dc motor's behavior. 
This namespace includes three type of model: transfer function, state space model, non-linear system model. 

.. image:: ../pics/diagrams/pics/Class_SystemModels.png
    :align: center

.. doxygenclass::  signal::systemmodels::lti::siso::CDiscreteTransferFunction
   :project: myproject
   :members:
..    :undoc-members:


.. doxygenclass::  signal::systemmodels::lti::mimo::CSSModel
   :project: myproject
   :members:
..    :undoc-members:

.. doxygenclass::  signal::systemmodels::nlti::mimo::CDiscreteTimeSystemModel
   :project: myproject
   :members:
..    :undoc-members:

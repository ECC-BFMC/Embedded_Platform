Filter namespace
================

The 'filter' namespace implements the filters' functionalities. 
There are four types of filter implemented: mean filter, median filter, finite-impulse-response filter
and infinite-impulse-response. 



.. image:: ../pics/diagrams/pics/Class_Filters.png
    :align: center

.. doxygenclass::  signal::filter::IFilter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: signal::filter::lti::siso::CIIRFilter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: signal::filter::lti::siso::CFIRFilter
   :project: myproject
   :members:
   :undoc-members:

.. doxygenclass:: signal::filter::lti::siso::CMeanFilter
   :project: myproject
   :members:
   :undoc-members:


.. doxygenclass:: signal::filter::nlti::siso::CMedianFilter
   :project: myproject
   :members:
   :undoc-members:
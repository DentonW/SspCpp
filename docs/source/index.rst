.. SspCpp documentation master file, created by
   sphinx-quickstart on Fri Sep 10 21:21:04 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

SspCpp Documentation
=======================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


:math:`ax^2 + bx + c`


Here is an example::

        def roots(a, b, c):
            q = b**2 - 4*a*c
            root1 = -b + sqrt(q)/float(2*a)
            root2 = -b - sqrt(q)/float(2*a)
            return root1, root2
			

Docs
====

  
.. doxygenfunction:: ssp::PlotCast

.. doxygenfunction:: ssp::WongZhu

.. doxygenstruct:: ssp::SCast
   :members:
   :undoc-members:
   
.. doxygenstruct:: ssp::SCastEntry
   :members:
   
   


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

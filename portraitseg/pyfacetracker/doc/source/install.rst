.. highlight:: sh

Installation
============

To install pyfacetracker you will need the following prerequisites:

* python (tested on version 2.73)
* numpy 
* OpenCV library (tested on version 2.45 but should work on any version using the C++ interface)
* C++ compiler (tested on MSVC 2008)

To run the examples you will also need:

* python interface to OpenCV (included in the OpenCV installation)
* matplotlib

Download the source files of pyfacetracker. You will first need to update ``setup.py`` to point to
the installation of OpenCV (i.e. set *OPENCV_BASE*, *OPENCV_LIB_DIRS* and *OPENCV_VERSION* global variables
to their correct values). Then, execute::

    $ python setup.py install

You can test the installation by running the examples under the folder ``examples/``

.. Note:: Reid Mayo has created a tutorial_ for installing pyFaceTracker on linux.

.. _tutorial:
   http://reidmayo.com/2014/07/15/how-to-install-pyfacetracker-on-linux-fedora-19/
"""
pyfacetracker: Python wrapper for FaceTracker.
==============================================

FaceTracker is a library for deformable face tracking written in C++ using
OpenCV 2, authored by Jason Saragih and maintained by Kyle McDonald. It is
available free for non-commercial use, and may be redistributed under these
conditions. Please see the LICENSE file for complete details.

**pyfacetracker** is a thin wrapper around FaceTracker. It enables using
FaceTracker while enjoyging the comfort of the Python scripting language.

pyfacetracker is available under the BSD License. This has no effect on
Jason's code, which is available under a separate license.

pyfacetracker is copyright (C) 2012 by Amit Aides

.. codeauthor:: Amit Aides <amitibo@tx.technion.ac.il>
"""

from _facetracker import *
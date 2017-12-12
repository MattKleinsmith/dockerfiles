#include <Python.h>
#include "structmember.h"

#define MODULESTR "_facetracker"
#define MODULEDOC "A wrapper module around the FaceTracker code by Jason Mora Saragih"
#define FT_VERSION "0.1"

#include "numpy/ndarrayobject.h"

#include <opencv/cv.h>
#include "FaceTracker/Tracker.h"
#include "FaceTracker/IO.h"


//
// Opencv conversion stuff
//
#include "opencv_helpers.h"

////////////////////////////////////////////////////////////////////////////////
//
// Define the FaceTracker wrapper class
//
typedef struct {
    PyObject_HEAD
    FACETRACKER::Tracker *thisptr;
    std::vector<int> window_sizes;
    int frame_skip;
    int iterations;
    double clamp;
    double tolerance;
    int age;
    int current_view;
} FaceTracker;


static int 
FaceTracker_clear(FaceTracker *self)
{
    if (self->thisptr)
    {
        delete self->thisptr;
    }

    //
    // Emptying the window sizes vector
    //
    while(!self->window_sizes.empty())
    {
        self->window_sizes.pop_back();
    }

    return 0;
}


static void
FaceTracker_dealloc(FaceTracker* self)
{
    FaceTracker_clear(self);
    self->ob_type->tp_free((PyObject*)self);
}


//
// new and constructor
//
static PyObject *
FaceTracker_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    FaceTracker *self;

    self = (FaceTracker *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->thisptr = NULL;
        self->window_sizes.reserve(10);
    }

    return (PyObject *)self;
}


static int
FaceTracker_init(FaceTracker *self, PyObject *args, PyObject *kwds)
{
    const char *fname=NULL;

    static char *kwlist[] = {"fname", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|s", kwlist, &fname))
        return -1;

    if (fname)
    {
        self->thisptr = new FACETRACKER::Tracker(fname);
    }
    else
    {
        self->thisptr = new FACETRACKER::Tracker();
    }

    //
    // Initialize the window sizes vector and other parameters
    //
    self->window_sizes.push_back(7);
    self->frame_skip = -1;
    self->iterations = 10;
    self->clamp = 3.0;
    self->tolerance = 0.01;

    return 0;
}


//
// FaceTracker members
//
static PyMemberDef FaceTracker_members[] = {
    {"frame_skip", T_INT, offsetof(FaceTracker, frame_skip), 0, "How often to skip frames."},
    {"iterations", T_INT, offsetof(FaceTracker, iterations), 0, "Number of iterations. Should be a number in the range: 1-25, 1 is fast and inaccurate, 25 is slow and accurate"},
    {"clamp", T_DOUBLE, offsetof(FaceTracker, clamp), 0, "[0-4] 1 gives a very loose fit, 4 gives a very tight fit"},
    {"tolerance", T_DOUBLE, offsetof(FaceTracker, tolerance), 0, "Matching tolerance. Should be a double in the range: .01-1."},
    {"age", T_INT, offsetof(FaceTracker, age), 0, "Age of tracking in frame: -1 no tracking."},
    {"current_view", T_INT, offsetof(FaceTracker, current_view), 0, "Current view in frame"},
    {NULL}  /* Sentinel */
};


//
// FaceTracker methods
//
static PyObject *
FaceTracker_load(FaceTracker* self, PyObject *args)
{
    const char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname))
        return NULL;

    self->thisptr->Load(fname);

    Py_RETURN_NONE;
}


static PyObject *
FaceTracker_setWindowSizes(FaceTracker *self, PyObject *args)
{
    PyObject* sizes_array = NULL;

    if (!PyArg_ParseTuple(args, "O", &sizes_array))
        return NULL;

    if (!pyopencv_to<int>(sizes_array, self->window_sizes))
        return NULL;

    Py_RETURN_NONE;
}


static PyObject *
FaceTracker_update(FaceTracker* self, PyObject *args)
{
    PyObject* pyobj_img = NULL;
    Mat img;

    if (!PyArg_ParseTuple(args, "O", &pyobj_img) || !pyopencv_to(pyobj_img, img))
        return NULL;

    if (img.channels() > 1)
    {
        PyErr_SetString(PyExc_TypeError,"Image must be grayscale");
        return NULL;
    }

    if (
        !self->thisptr->Track(
            img,
            self->window_sizes,
            self->frame_skip,
            self->iterations,
            self->clamp,
            self->tolerance,
            true) == 0
        )
    {
        self->thisptr->FrameReset();
        self->age = -1;
        Py_RETURN_FALSE;
    }

    self->current_view = self->thisptr->_clm.GetViewIdx();
    self->age++;
    Py_RETURN_TRUE;
}


static PyObject *
FaceTracker_draw(FaceTracker* self, PyObject *args, PyObject *kw)
{
    PyObject* pyobj_img = NULL;
    Mat image;
    PyObject* pyobj_con = NULL;
    Mat con;
    PyObject* pyobj_tri = NULL;
    Mat tri;
    Mat shape = self->thisptr->_shape;
    Mat visi = self->thisptr->_clm._visi[self->current_view];
    int i,n = shape.rows/2;
    Point p1;
    Point p2;
    Scalar c;

    const char* keywords[] = {"img", "col", "tri", NULL};
    if (
        !PyArg_ParseTupleAndKeywords(args, kw, "OOO", (char**)keywords, &pyobj_img, &pyobj_con, &pyobj_tri) ||
        !pyopencv_to(pyobj_img, image) ||
        !pyopencv_to(pyobj_con, con) ||
        !pyopencv_to(pyobj_tri, tri)
        )
        Py_RETURN_NONE;

    //
    // draw triangulation
    //
    c = CV_RGB(0,0,0);
    for (i = 0; i < tri.rows; i++)
    {
        if (visi.at<int>(tri.at<int>(i,0),0) == 0 ||
            visi.at<int>(tri.at<int>(i,1),0) == 0 ||
            visi.at<int>(tri.at<int>(i,2),0) == 0)
            continue;
            
        p1 = Point(shape.at<double>(tri.at<int>(i,0),0),
            shape.at<double>(tri.at<int>(i,0)+n,0));
        p2 = Point(shape.at<double>(tri.at<int>(i,1),0),
            shape.at<double>(tri.at<int>(i,1)+n,0));
        line(image,p1,p2,c);
        
        p1 = Point(shape.at<double>(tri.at<int>(i,0),0),
            shape.at<double>(tri.at<int>(i,0)+n,0));
        p2 = Point(shape.at<double>(tri.at<int>(i,2),0),
            shape.at<double>(tri.at<int>(i,2)+n,0));
        line(image,p1,p2,c);
        
        p1 = Point(shape.at<double>(tri.at<int>(i,2),0),
            shape.at<double>(tri.at<int>(i,2)+n,0));
        p2 = Point(shape.at<double>(tri.at<int>(i,1),0),
            shape.at<double>(tri.at<int>(i,1)+n,0));
        line(image,p1,p2,c);
    }
    
    //
    // draw connections
    //
    c = CV_RGB(0,0,255);
    for(i = 0; i < con.cols; i++){
        if (visi.at<int>(con.at<int>(0,i),0) == 0 ||
            visi.at<int>(con.at<int>(1,i),0) == 0)
            continue;
            
        p1 = Point(shape.at<double>(con.at<int>(0,i),0),
            shape.at<double>(con.at<int>(0,i)+n,0));
        p2 = Point(shape.at<double>(con.at<int>(1,i),0),
            shape.at<double>(con.at<int>(1,i)+n,0));
        line(image,p1,p2,c,1);
    }

    //
    // draw points
    //
    for (i = 0; i < n; i++)
    {
        if(visi.at<int>(i,0) == 0)
            continue;

        p1 = Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
        c = CV_RGB(255,0,0);
        circle(image,p1,2,c);
    }
    
    return Py_BuildValue("N", pyopencv_from(image));
}


static PyObject *
FaceTracker_getPosition(FaceTracker* self, PyObject *args)
{
    const Mat& pose = self->thisptr->_clm._pglobl;
    return Py_BuildValue("(dd)", pose.at<double>(4,0), pose.at<double>(5,0));
}


static PyObject *
FaceTracker_getScale(FaceTracker* self, PyObject *args)
{
    const Mat& pose = self->thisptr->_clm._pglobl;
    return Py_BuildValue("d", pose.at<double>(0,0));
}


static PyObject *
FaceTracker_getOrientation(FaceTracker* self, PyObject *args)
{
    const Mat& pose = self->thisptr->_clm._pglobl;
    return Py_BuildValue("(ddd)", pose.at<double>(1,0), pose.at<double>(2,0), pose.at<double>(3,0));
}


static PyObject *
FaceTracker_get2DShape(FaceTracker* self, PyObject *args)
{
    const Mat& shape = self->thisptr->_shape;
    const Mat& visibility = self->thisptr->_clm._visi[self->current_view];
    
    return Py_BuildValue("NN", pyopencv_from(shape), pyopencv_from(visibility));
}


static PyObject *
FaceTracker_get3DShape(FaceTracker* self, PyObject *args)
{
    const Mat& mean = self->thisptr->_clm._pdm._M;
    const Mat& variation = self->thisptr->_clm._pdm._V;
    const Mat& weights = self->thisptr->_clm._plocal;

    return Py_BuildValue("N", pyopencv_from(mean + variation * weights));
}


static PyObject *
FaceTracker_resetFrame(FaceTracker* self, PyObject *args)
{
    self->thisptr->FrameReset();
    self->age = -1;
    Py_RETURN_NONE;
}


static PyMethodDef FaceTracker_methods[] = {
    {"load", (PyCFunction)FaceTracker_load, METH_VARARGS, "Load a new model."},
    {"update", (PyCFunction)FaceTracker_update, METH_VARARGS, "Run the tracking algorithm on an image."},
    {"draw", (PyCFunction)FaceTracker_draw, METH_VARARGS, "Draw the face on the image."},
    {"setWindowSizes", (PyCFunction)FaceTracker_setWindowSizes, METH_VARARGS, "Set the sizes of windows in which to search for the face. Should be an array like object of integers."},
    {"getPosition", (PyCFunction)FaceTracker_getPosition, METH_NOARGS, "Get the position of the face."},
    {"getScale", (PyCFunction)FaceTracker_getScale, METH_NOARGS, "Get the scale of the face."},
    {"getOrientation", (PyCFunction)FaceTracker_getOrientation, METH_NOARGS, "Get the orientation of the face."},
    {"get2DShape", (PyCFunction)FaceTracker_get2DShape, METH_NOARGS, "Get the 2D shape and visibility of the face."},
    {"get3DShape", (PyCFunction)FaceTracker_get3DShape, METH_NOARGS, "Get the 3D point cloud of the face features."},
    {"resetFrame", (PyCFunction)FaceTracker_resetFrame, METH_NOARGS, "Force the tracker to reset tracking."},
    {NULL}  /* Sentinel */
};


static PyTypeObject FaceTrackerType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_facetracker.FaceTracker",             /*tp_name*/
    sizeof(FaceTracker),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)FaceTracker_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "FaceTracker objects",           /* tp_doc */
    0,   /* tp_traverse */
    0,           /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    FaceTracker_methods,             /* tp_methods */
    FaceTracker_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)FaceTracker_init,      /* tp_init */
    0,                         /* tp_alloc */
    FaceTracker_new,                 /* tp_new */
};


//
// Module methods
//
static PyObject *
LoadCon(PyObject *self, PyObject *args)
{
    const char *fname;

    if (!PyArg_ParseTuple(args, "s", &fname))
        return NULL;

    cv::Mat con = FACETRACKER::IO::LoadCon(fname);

    return Py_BuildValue("N", pyopencv_from(con));
}


static PyObject *
LoadTri(PyObject *self, PyObject *args)
{
    const char *fname;

    if (!PyArg_ParseTuple(args, "s", &fname))
        return NULL;

    cv::Mat con = FACETRACKER::IO::LoadTri(fname);

    return Py_BuildValue("N", pyopencv_from(con));
}


static PyMethodDef ModuleMethods[] = {
    {"LoadCon", LoadCon, METH_VARARGS, "Load a connection matrix"},
    {"LoadTri", LoadTri, METH_VARARGS, "Load a triangular matrix"},
    {NULL}  /* Sentinel */
};


//
// Create the module
//
#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
init_facetracker(void) 
{
    import_array();

    if (PyType_Ready(&FaceTrackerType) < 0)
        return;

    PyObject* m = Py_InitModule3(MODULESTR, ModuleMethods, MODULEDOC);
    PyObject* d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "__version__", PyString_FromString(FT_VERSION));

    Py_INCREF(&FaceTrackerType);
    PyModule_AddObject(m, "FaceTracker", (PyObject *)&FaceTrackerType);
}
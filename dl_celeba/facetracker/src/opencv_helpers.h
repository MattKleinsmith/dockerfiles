/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2010, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_HELPERS_H_
#define __OPENCV_HELPERS_H_

#include <Python.h>
#include "structmember.h"

#include "numpy/ndarrayobject.h"

#include <opencv/cv.h>


//
// Opencv conversion stuff
//
using namespace cv;

static PyObject* opencv_error = 0;

static int failmsg(const char *fmt, ...)
{
    char str[1000];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(str, sizeof(str), fmt, ap);
    va_end(ap);

    PyErr_SetString(PyExc_TypeError, str);
    return 0;
}

class PyAllowThreads
{
public:
    PyAllowThreads() : _state(PyEval_SaveThread()) {}
    ~PyAllowThreads()
    {
        PyEval_RestoreThread(_state);
    }
private:
    PyThreadState* _state;
};

class PyEnsureGIL
{
public:
    PyEnsureGIL() : _state(PyGILState_Ensure()) {}
    ~PyEnsureGIL()
    {
        PyGILState_Release(_state);
    }
private:
    PyGILState_STATE _state;
};

static size_t REFCOUNT_OFFSET = (size_t)&(((PyObject*)0)->ob_refcnt) +
    (0x12345678 != *(const size_t*)"\x78\x56\x34\x12\0\0\0\0\0")*sizeof(int);

static inline PyObject* pyObjectFromRefcount(const int* refcount)
{
    return (PyObject*)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int* refcountFromPyObject(const PyObject* obj)
{
    return (int*)((size_t)obj + REFCOUNT_OFFSET);
}

class NumpyAllocator : public MatAllocator
{
public:
    NumpyAllocator() {}
    ~NumpyAllocator() {}

    void allocate(int dims, const int* sizes, int type, int*& refcount,
                  uchar*& datastart, uchar*& data, size_t* step)
    {
        PyEnsureGIL gil;

        int depth = CV_MAT_DEPTH(type);
        int cn = CV_MAT_CN(type);
        const int f = (int)(sizeof(size_t)/8);
        int typenum = depth == CV_8U ? NPY_UBYTE : depth == CV_8S ? NPY_BYTE :
                      depth == CV_16U ? NPY_USHORT : depth == CV_16S ? NPY_SHORT :
                      depth == CV_32S ? NPY_INT : depth == CV_32F ? NPY_FLOAT :
                      depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f^1)*NPY_UINT;
        int i;
        npy_intp _sizes[CV_MAX_DIM+1];
        for( i = 0; i < dims; i++ )
            _sizes[i] = sizes[i];
        if( cn > 1 )
        {
            /*if( _sizes[dims-1] == 1 )
                _sizes[dims-1] = cn;
            else*/
                _sizes[dims++] = cn;
        }
        PyObject* o = PyArray_SimpleNew(dims, _sizes, typenum);
        if(!o)
            CV_Error_(CV_StsError, ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
        refcount = refcountFromPyObject(o);
        npy_intp* _strides = PyArray_STRIDES(o);
        for( i = 0; i < dims - (cn > 1); i++ )
            step[i] = (size_t)_strides[i];
        datastart = data = (uchar*)PyArray_DATA(o);
    }

    void deallocate(int* refcount, uchar*, uchar*)
    {
        PyEnsureGIL gil;
        if( !refcount )
            return;
        PyObject* o = pyObjectFromRefcount(refcount);
        Py_INCREF(o);
        Py_DECREF(o);
    }
};

NumpyAllocator g_numpyAllocator;

enum { ARG_NONE = 0, ARG_MAT = 1, ARG_SCALAR = 2 };

static int pyopencv_to(const PyObject* o, Mat& m, const char* name = "<unknown>", bool allowND=true)
{
    if(!o || o == Py_None)
    {
        if( !m.data )
            m.allocator = &g_numpyAllocator;
        return true;
    }

    if( !PyArray_Check(o) )
    {
        failmsg("%s is not a numpy array", name);
        return false;
    }

    int typenum = PyArray_TYPE(o);
    int type = typenum == NPY_UBYTE ? CV_8U : typenum == NPY_BYTE ? CV_8S :
               typenum == NPY_USHORT ? CV_16U : typenum == NPY_SHORT ? CV_16S :
               typenum == NPY_INT || typenum == NPY_LONG ? CV_32S :
               typenum == NPY_FLOAT ? CV_32F :
               typenum == NPY_DOUBLE ? CV_64F : -1;

    if( type < 0 )
    {
        failmsg("%s data type = %d is not supported", name, typenum);
        return false;
    }

    int ndims = PyArray_NDIM(o);
    if(ndims >= CV_MAX_DIM)
    {
        failmsg("%s dimensionality (=%d) is too high", name, ndims);
        return false;
    }

    int size[CV_MAX_DIM+1];
    size_t step[CV_MAX_DIM+1], elemsize = CV_ELEM_SIZE1(type);
    const npy_intp* _sizes = PyArray_DIMS(o);
    const npy_intp* _strides = PyArray_STRIDES(o);
    bool transposed = false;

    for(int i = 0; i < ndims; i++)
    {
        size[i] = (int)_sizes[i];
        step[i] = (size_t)_strides[i];
    }

    if( ndims == 0 || step[ndims-1] > elemsize ) {
        size[ndims] = 1;
        step[ndims] = elemsize;
        ndims++;
    }

    if( ndims >= 2 && step[0] < step[1] )
    {
        std::swap(size[0], size[1]);
        std::swap(step[0], step[1]);
        transposed = true;
    }

    if( ndims == 3 && size[2] <= CV_CN_MAX && step[1] == elemsize*size[2] )
    {
        ndims--;
        type |= CV_MAKETYPE(0, size[2]);
    }

    if( ndims > 2 && !allowND )
    {
        failmsg("%s has more than 2 dimensions", name);
        return false;
    }

    m = Mat(ndims, size, type, PyArray_DATA(o), step);

    if( m.data )
    {
        m.refcount = refcountFromPyObject(o);
        m.addref(); // protect the original numpy array from deallocation
                    // (since Mat destructor will decrement the reference counter)
    };
    m.allocator = &g_numpyAllocator;

    if( transposed )
    {
        Mat tmp;
        tmp.allocator = &g_numpyAllocator;
        transpose(m, tmp);
        m = tmp;
    }
    return true;
}

static PyObject* pyopencv_from(const Mat& m)
{
    if( !m.data )
        Py_RETURN_NONE;
    Mat temp, *p = (Mat*)&m;
    if(!p->refcount || p->allocator != &g_numpyAllocator)
    {
        temp.allocator = &g_numpyAllocator;
        m.copyTo(temp);
        p = &temp;
    }
    p->addref();
    return pyObjectFromRefcount(p->refcount);
}

static bool pyopencv_to(PyObject *o, Scalar& s, const char *name = "<unknown>")
{
    if(!o || o == Py_None)
        return true;
    if (PySequence_Check(o)) {
        PyObject *fi = PySequence_Fast(o, name);
        if (fi == NULL)
            return false;
        if (4 < PySequence_Fast_GET_SIZE(fi))
        {
            failmsg("Scalar value for argument '%s' is longer than 4", name);
            return false;
        }
        for (Py_ssize_t i = 0; i < PySequence_Fast_GET_SIZE(fi); i++) {
            PyObject *item = PySequence_Fast_GET_ITEM(fi, i);
            if (PyFloat_Check(item) || PyInt_Check(item)) {
                s[(int)i] = PyFloat_AsDouble(item);
            } else {
                failmsg("Scalar value for argument '%s' is not numeric", name);
                return false;
            }
        }
        Py_DECREF(fi);
    } else {
        if (PyFloat_Check(o) || PyInt_Check(o)) {
            s[0] = PyFloat_AsDouble(o);
        } else {
            failmsg("Scalar value for argument '%s' is not numeric", name);
            return false;
        }
    }
    return true;
}

static inline PyObject* pyopencv_from(const Scalar& src)
{
    return Py_BuildValue("(dddd)", src[0], src[1], src[2], src[3]);
}

static PyObject* pyopencv_from(bool value)
{
    return PyBool_FromLong(value);
}

static bool pyopencv_to(PyObject* obj, bool& value, const char* name = "<unknown>")
{
    (void)name;
    if(!obj || obj == Py_None)
        return true;
    int _val = PyObject_IsTrue(obj);
    if(_val < 0)
        return false;
    value = _val > 0;
    return true;
}

static PyObject* pyopencv_from(size_t value)
{
    return PyLong_FromUnsignedLong((unsigned long)value);
}

static bool pyopencv_to(PyObject* obj, size_t& value, const char* name = "<unknown>")
{
    (void)name;
    if(!obj || obj == Py_None)
        return true;
    value = (int)PyLong_AsUnsignedLong(obj);
    return value != (size_t)-1 || !PyErr_Occurred();
}

static PyObject* pyopencv_from(int value)
{
    return PyInt_FromLong(value);
}

template<typename _Tp> struct pyopencvVecConverter
{
    static bool to(PyObject* obj, vector<_Tp>& value, const char* name="<unknown>")
    {
        typedef typename DataType<_Tp>::channel_type _Cp;
        if(!obj || obj == Py_None)
            return true;
        if (PyArray_Check(obj))
        {
            Mat m;
            pyopencv_to(obj, m, name);
            m.copyTo(value);
        }
        if (!PySequence_Check(obj))
            return false;
        PyObject *seq = PySequence_Fast(obj, name);
        if (seq == NULL)
            return false;
        int i, j, n = (int)PySequence_Fast_GET_SIZE(seq);
        value.resize(n);

        int type = DataType<_Tp>::type;
        int depth = CV_MAT_DEPTH(type), channels = CV_MAT_CN(type);
        PyObject** items = PySequence_Fast_ITEMS(seq);

        for( i = 0; i < n; i++ )
        {
            PyObject* item = items[i];
            PyObject* seq_i = 0;
            PyObject** items_i = &item;
            _Cp* data = (_Cp*)&value[i];

            if( channels == 2 && PyComplex_CheckExact(item) )
            {
                Py_complex c = PyComplex_AsCComplex(obj);
                data[0] = saturate_cast<_Cp>(c.real);
                data[1] = saturate_cast<_Cp>(c.imag);
                continue;
            }
            if( channels > 1 )
            {
                if( PyArray_Check(item))
                {
                    Mat src;
                    pyopencv_to(item, src, name);
                    if( src.dims != 2 || src.channels() != 1 ||
                       ((src.cols != 1 || src.rows != channels) &&
                        (src.cols != channels || src.rows != 1)))
                        break;
                    Mat dst(src.rows, src.cols, depth, data);
                    src.convertTo(dst, type);
                    if( dst.data != (uchar*)data )
                        break;
                    continue;
                }

                seq_i = PySequence_Fast(item, name);
                if( !seq_i || (int)PySequence_Fast_GET_SIZE(seq_i) != channels )
                {
                    Py_XDECREF(seq_i);
                    break;
                }
                items_i = PySequence_Fast_ITEMS(seq_i);
            }

            for( j = 0; j < channels; j++ )
            {
                PyObject* item_ij = items_i[j];
                if( PyInt_Check(item_ij))
                {
                    int v = PyInt_AsLong(item_ij);
                    if( v == -1 && PyErr_Occurred() )
                        break;
                    data[j] = saturate_cast<_Cp>(v);
                }
                else if( PyFloat_Check(item_ij))
                {
                    double v = PyFloat_AsDouble(item_ij);
                    if( PyErr_Occurred() )
                        break;
                    data[j] = saturate_cast<_Cp>(v);
                }
                else
                    break;
            }
            Py_XDECREF(seq_i);
            if( j < channels )
                break;
        }
        Py_DECREF(seq);
        return i == n;
    }

    static PyObject* from(const vector<_Tp>& value)
    {
        if(value.empty())
            return PyTuple_New(0);
        Mat src((int)value.size(), DataType<_Tp>::channels, DataType<_Tp>::depth, (uchar*)&value[0]);
        return pyopencv_from(src);
    }
};


template<typename _Tp> static inline bool pyopencv_to(PyObject* obj, vector<_Tp>& value, const char* name="<unknown>")
{
    return pyopencvVecConverter<_Tp>::to(obj, value, name);
}

template<typename _Tp> static inline PyObject* pyopencv_from(const vector<_Tp>& value)
{
    return pyopencvVecConverter<_Tp>::from(value);
}

#endif


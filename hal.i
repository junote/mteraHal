%module hal
%{
#include "misc.h"    
#include "MmapDevice.h"    
#include "Mmap.h"    
// #include "I2cBus.h"
#include "ss_gdef.h"
#include "I2cDev.h"
#include "SpiBus.h"
#include "BcmCli.h"

%}

%include std_vector.i
%include std_string.i
%include typemaps.i
%include stdint.i



%typemap(in) ( uint8 *txBuf, int txBufLen) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = PyList_Size($input);
  $1 = (unsigned char*) malloc(($2)*sizeof(unsigned char));
  for (i = 0; i < $2; i++) {
    PyObject *s = PyList_GetItem($input,i);
    if (!PyInt_Check(s)) {
        free($1);
        PyErr_SetString(PyExc_ValueError, "List items must be integers");
        return NULL;
    }
    $1[i] = PyInt_AsLong(s);
  }
}

%typemap(freearg) (uint8 *txBuf, int txBufLen) {
   if ($1) free($1);
}


%typemap(in) ( uint8 *txBuf2, int txBufLen2) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = PyList_Size($input);
  $1 = (unsigned char*) malloc(($2)*sizeof(unsigned char));
  for (i = 0; i < $2; i++) {
    PyObject *s = PyList_GetItem($input,i);
    if (!PyInt_Check(s)) {
        free($1);
        PyErr_SetString(PyExc_ValueError, "List items must be integers");
        return NULL;
    }
    $1[i] = PyInt_AsLong(s);
  }
}

%typemap(freearg) (uint8 *txBuf, int txBufLen) {
   if ($1) free($1);
}

%typemap(in) ( uint8 *rxBuf, int rxBufLen) {
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = PyList_Size($input);
  $1 = (unsigned char*) malloc(($2)*sizeof(unsigned char));

}

%typemap(argout) (uint8 *rxBuf, int rxBufLen) {
  int i;
  $result = PyList_New($2);  
  for (i = 0; i < $2; i++) {
      PyObject *o = PyInt_FromLong($1[i]);
      PyList_SetItem($result, i, o);
  }
}


// %apply bool *OUTPUT { uint8 *rxBuf }
// bool devices::I2cBus::i2cRead(uint8 *INPUT, int txBufLen, uint8 *rxBuf, int rxBufLen);

// bool devices::I2cBus::i2cWrite(uint8 *INPUT, int txBufLen);

// %apply bool *OUTPUT { uint8 *rxBuf }
// bool devices::I2cBus::i2cRepeatStartRead(uint8 *INPUT, int txBufLen1, uint8 *INPUT,int txBufLen2, uint8* rxBuf, int rxBufLen);

%include "ss_gdef.h"
%include "I2cBus.h"
%include "MmapDevice.h"
%include "Mmap.h"
%include "misc.h"
%include "I2cDev.h"
%include "SpiBus.h"
%include "BcmCli.h"
#include <pygobject.h>
 
void igemacintegration_register_classes (PyObject *d); 

extern PyMethodDef igemacintegration_functions[];
 
DL_EXPORT(void)
init_igemacintegration (void)
{
  PyObject *m, *d;

  init_pygobject ();

  m = Py_InitModule ("igemacintegration._igemacintegration", 
                     igemacintegration_functions);
  d = PyModule_GetDict (m);

  igemacintegration_register_classes (d);

  if (PyErr_Occurred ()) { 
    PyErr_Print();
    Py_FatalError ("can't initialize module igemacintegration:");
  }
}

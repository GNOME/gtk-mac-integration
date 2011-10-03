#include <pygobject.h>

void gtkmacintegration_register_classes (PyObject *d);

extern PyMethodDef gtkmacintegration_functions[];

DL_EXPORT(void)
init_gtkmacintegration (void)
{
  PyObject *m, *d;

  init_pygobject ();

  m = Py_InitModule ("gtkmacintegration._gtkmacintegration",
                     gtkmacintegration_functions);
  d = PyModule_GetDict (m);

  gtkmacintegration_register_classes (d);

  if (PyErr_Occurred ()) {
    PyErr_Print();
    Py_FatalError ("can't initialize module gtkmacintegration:");
  }
}

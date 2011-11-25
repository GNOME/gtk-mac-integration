#include <pygobject.h>
 
void gtkosx_application_register_classes (PyObject *d); 
void pygtkosx_application_add_constants(PyObject *module, const gchar *strip_prefix);

extern PyMethodDef gtkosx_application_functions[];
 
DL_EXPORT(void)
init_gtkosx_application (void)
{
  PyObject *m, *d;

  init_pygobject ();

  m = Py_InitModule ("gtkosx_application._gtkosx_application", 
                     gtkosx_application_functions);
  d = PyModule_GetDict (m);

  gtkosx_application_register_classes (d);

  if (PyErr_Occurred ()) { 
    PyErr_Print();
    Py_FatalError ("can't initialize module gtkosx_application:");
  }
}

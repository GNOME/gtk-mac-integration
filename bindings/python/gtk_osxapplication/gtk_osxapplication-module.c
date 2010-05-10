#include <pygobject.h>
 
void gtk_osxapplication_register_classes (PyObject *d); 
void pygtk_osxapplication_add_constants(PyObject *module, const gchar *strip_prefix);

extern PyMethodDef gtk_osxapplication_functions[];
 
DL_EXPORT(void)
init_gtk_osxapplication (void)
{
  PyObject *m, *d;

  init_pygobject ();

  m = Py_InitModule ("gtk_osxapplication._gtk_osxapplication", 
                     gtk_osxapplication_functions);
  d = PyModule_GetDict (m);

  gtk_osxapplication_register_classes (d);

  if (PyErr_Occurred ()) { 
    PyErr_Print();
    Py_FatalError ("can't initialize module gtk_osxapplication:");
  }
}

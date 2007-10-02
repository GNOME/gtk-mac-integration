#include <pygobject.h>
 
void igemacmenu_register_classes (PyObject *d); 

extern PyMethodDef igemacmenu_functions[];
 
DL_EXPORT(void)
     initigemacmenu (void)
{
  PyObject *m, *d;

  init_pygobject ();

  m = Py_InitModule ("igemacmenu", igemacmenu_functions);
  d = PyModule_GetDict (m);

  igemacmenu_register_classes (d);

  if (PyErr_Occurred ()) {
    Py_FatalError ("can't initialise module igemacmenu");
  }
}

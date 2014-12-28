#include <Python.h>
#include "dht11_sensor.h"

static PyObject * dht11_sensor_read(PyObject *self, PyObject *args)
{
	sensor_data data;
	int  pin;

	if (!PyArg_ParseTuple(args, "i", &pin)) 
	{
		return NULL;
	}

	read_dht11_data(pin, &data);

	return Py_BuildValue("{s:h,s:s,s:i,s:i,s:i,s:i}", "valid", data.valid, "err", data.errmsg
			, "temperature", data.temperature, "temperature2", data.temperature2
			, "humidity", data.humidity, "humidity2", data.humidity2);
}

static PyMethodDef module_methods[] = {
	{ "read", dht11_sensor_read, METH_VARARGS, "Read DHT sensor value from Wirepi." },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	"WiringPi_DHT_Sensor",     /* m_name */
	"WiringPi_DHT_Sensor",  /* m_doc */
	-1,                  /* m_size */
	module_methods,    /* m_methods */
	NULL,                /* m_reload */
	NULL,                /* m_traverse */
	NULL,                /* m_clear */
	NULL,                /* m_free */
};

PyMODINIT_FUNC PyInit_dht11_sensor(void)
{
	PyObject * m = PyModule_Create(&moduledef);


	return m;
}
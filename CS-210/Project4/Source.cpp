#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <sstream>
#undef max	// old macro workaround for numeric_limits

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
void callStrFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	//pName = PyUnicode_FromString((char*)"PythonCode"); // fails to find file every time
	// Load the module object
	pModule = PyImport_ImportModule("PythonCode");
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	//Py_DECREF(pName); //no longer needed
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
}

template<typename T>
T& validateString(T& val, string message)
//validates user input as a integer.
{
	while (true) {
		cout << message << endl;
		if (cin >> val) {
			break;
		}
		else {
			cout << message << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return val;
}

template<typename T>
T& validateInt(T& val, string message)
//validates user input as a integer.
{
	while (true) {
		cout << message << endl;
		if (cin >> val) {
			break;
		}
		else {
			cout << message << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return val;
}

void main()
{
	bool exitState = false;
	int userIn;

	do {

		cout << "1: Calculate Ocurance" << endl;
		cout << "2: Find ocurance of specific word" << endl;
		cout << "3: Histogram of Frequency" << endl;
		cout << "4: Exit" << endl;
		userIn = validateInt(userIn, "Enter your selection as a number 1, 2, or 3, 4.");
		string userval = "";

		switch (userIn) {
		case 1:
			CallProcedure("GetFrequency");
			break;
		case 2:
			userval = validateString(userval, "Enter in name to search");
			callStrFunc("findSpecific", userval);
			break;
		case 3:
			CallProcedure("getHistogram");
			break;
		case 4:
			exitState = true;
			break;
		default:
			//final safe guard
			cout << " Did not recognize input: " << userIn << "please try again" << endl;
		}

	} while (!exitState);


}
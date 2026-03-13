#pragma once
#include "func.h"
using namespace std;

//---------------------generate test value(gemini help debug)
template <typename T>
T generate_val(int i) { 
	return (T)i; 
}

template <>
string generate_val<string>(int i) {
	return "" + to_string(i);
}




//---------------------interactive
template <typename T>
void interactive()
{
	cout << "===================INTERACTIVE================" << endl;
	int list_type = 0;
	cout << "1-static" << endl << "2-vector" << endl << "3-linked list";
	cin >> list_type;
	if (list_type == 1)
	{
		//---------------------static list
		list_arr<T> list;
		while (true)
		{
			int operation = 0;
			cout << "1-create empty(clear)" << endl << "2-append" << endl << "3-insert" << endl << "4-remove" << endl << "5-get" << endl << "6-set" << endl << "7-lenght" << endl << "8-print   9-exit" << endl;
			cin >> operation;
			if (operation == 1)
			{
				int size = 0;
				cout << "size ";
				cin >> size;
				create_empty(list, size);
			}
			else if (operation == 2)
			{
				T value;
				cout << "val ";
				cin >> value;
				append(list, value);
			}
			else if (operation == 3)
			{
				int index;
				T value;
				cout << "index where and val";
				cin >> index >> value;
				insert(list, index, value);
			}
			else if (operation == 4)
			{
				int index;
				cout << "where del ";
				cin >> index;
				remove(list, index);
			}
			else if (operation == 5)
			{
				int index;
				cout << "get where ";
				cin >> index;
				cout << get(list, index) << endl;
			}
			else if (operation == 6)
			{
				int index;
				T value;
				cout << "set where and val ";
				cin >> index >> value;
				set(list, index, value);
			}
			else if (operation == 7)
			{
				cout << "length " << length(list) << endl;
			}
			else if (operation == 8)
			{
				print(list);
			}
			else if (operation == 9)
			{
				break;
			}
			else
			{
				cout << "crazy guy, menu above" << endl;
			}
		}
		delete[] list.data;
	}

	//---------------------vector list
	else if (list_type == 2)
	{
		vector_arr<T> list;
		while (true)
		{
			int operation = 0;
			cout << "1-create empty(clear)" << endl << "2-append" << endl << "3-insert" << endl << "4-remove" << endl << "5-get" << endl << "6-set" << endl << "7-lenght" << endl << "8-print   9-exit" << endl;
			cin >> operation;
			if (operation == 1)
			{
				create_empty(list);
			}
			else if (operation == 2)
			{
				T value;
				cout << "val ";
				cin >> value;
				append(list, value);
			}
			else if (operation == 3)
			{
				int index;
				T value;
				cout << "index where and val";
				cin >> index >> value;
				insert(list, index, value);
			}
			else if (operation == 4)
			{
				int index;
				cout << "where del ";
				cin >> index;
				remove(list, index);
			}
			else if (operation == 5)
			{
				int index;
				cout << "get where ";
				cin >> index;
				cout << get(list, index) << endl;
			}
			else if (operation == 6)
			{
				int index;
				T value;
				cout << "set where and val ";
				cin >> index >> value;
				set(list, index, value);
			}
			else if (operation == 7)
			{
				cout << "length " << length(list) << endl;
			}
			else if (operation == 8)
			{
				print(list);
			}
			else if (operation == 9)
			{
				break;
			}
			else
			{
				cout << "crazy guy, menu above" << endl;
			}
		}
	}
	//---------------------linked list
	else if (list_type == 3)
	{
		list_linked<T> list;
		while (true)
		{
			int operation = 0;
			cout << "1-create empty(clear)" << endl << "2-append" << endl << "3-insert" << endl << "4-remove" << endl << "5-get" << endl << "6-set" << endl << "7-lenght" << endl << "8-print   9-exit" << endl;
			cin >> operation;
			if (operation == 1)
			{
				create_empty(list);
			}
			else if (operation == 2)
			{
				T value;
				cout << "val ";
				cin >> value;
				append(list, value);
			}
			else if (operation == 3)
			{
				int index;
				T value;
				cout << "index where and val";
				cin >> index >> value;
				insert(list, index, value);
			}
			else if (operation == 4)
			{
				int index;
				cout << "where del ";
				cin >> index;
				remove(list, index);
			}
			else if (operation == 5)
			{
				int index;
				cout << "get where ";
				cin >> index;
				cout << get(list, index) << endl;
			}
			else if (operation == 6)
			{
				int index;
				T value;
				cout << "set where and val ";
				cin >> index >> value;
				set(list, index, value);
			}
			else if (operation == 7)
			{
				cout << "length " << length(list) << endl;
			}
			else if (operation == 8)
			{
				print(list);
			}
			else if (operation == 9)
			{
				break;
			}
			else
			{
				cout << "crazy guy, menu above" << endl;
			}
		}
	}
	else
	{
		cout << "mirnogo rishenya ne byde" << endl;
	}
}

//---------------------benchmark
template <typename T>
void benchmark()
{
	cout << "===================BENCHMARK================" << endl;
	//copilot solo do benchmark
	int max_iter = 100000;
	T data = generate_val<T>(1);

	//---------------------static list
	list_arr<T> list_static;
	create_empty(list_static, max_iter);
	double start_time = Time();
	for (int i = 0; i < max_iter; i++)
	{
		append(list_static, data);
	}
	double end_time = Time();
	cout << "Static append " << end_time - start_time << " seconds" << endl;
	//---------------------vector list
	vector_arr<T> list_vector;
	start_time = Time();
	for (int i = 0; i < max_iter; i++)
	{
		append(list_vector, data);
	}
	end_time = Time();
	cout << "Vector append " << end_time - start_time << " seconds" << endl;
	//---------------------linked list
	list_linked<T> list_linked;
	start_time = Time();
	for (int i = 0; i < max_iter; i++)
	{
		append(list_linked, data);
	}
	end_time = Time();
	cout << "Linked append " << end_time - start_time << " seconds" << endl;
}

//---------------------demonstration
template <typename T>
void demonstration()
{
	T data = generate_val<T>(1);
	cout << "===================DEMONSTRATION================" << endl;
	list_arr<T> list_static;
	create_empty(list_static, 10);
	vector_arr<T> list_vector;
	list_linked<T> list_linked;
	for (int i = 0; i < 10; i++)
	{
		append(list_static,data );
		append(list_vector, data);
		append(list_linked, data);
	}
	cout << "Static list: ";
	print(list_static);
	cout << "Vector list: ";
	print(list_vector);
	cout << "Linked list: ";
	print(list_linked);
}
//---------------------choose type of demonstration
template <typename T>
void start_type()
{
	int demo_type = 0;
	cout << "1-interactive" << endl << "2-benchmark" << endl << "3-demonstration" << endl;
	cin >> demo_type;
	if (demo_type == 1)
	{
		interactive<T>();
	}
	else if (demo_type == 2)
	{
		benchmark<T>();
	}
	else if (demo_type == 3)
	{
		demonstration<T>();
	}
	else
	{
		cout << ":(" << endl;
	}
}
//---------------------choose data type
void choose_data_type()
{
	int data_type = 0;
	cout << "Choose data type:" << endl;
	cout << "1-int" << endl;
	cout << "2-double" << endl;
	cout << "3-string" << endl;
	cin >> data_type;
	if (data_type == 1)
	{
		start_type<int>();
	}
	else if (data_type == 2)
	{
		start_type<float>();
	}
	else if (data_type == 3)
	{
		start_type<string>();
	}
	else
	{
		cout << "Invalid data" << endl;
	}
}

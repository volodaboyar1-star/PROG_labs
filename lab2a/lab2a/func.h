#pragma once
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

template <typename T>
struct list_arr {
    int size = 0;
    int current = 0;
    T* data = nullptr;
};

double Time() {
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration<double>(now.time_since_epoch()).count();
}

template <typename T>
void create_empty(list_arr<T>& list, int size)
{   
    if (list.data != nullptr) {
        delete[] list.data;
        list.data = nullptr;
    }
    list.size = size;
    list.current = 0;
    list.data = new T[size]; 
}
//---------------------append_static_arr
template <typename T>
void append(list_arr<T>& list, T value)
{
    if (list.current < list.size)
    {
        list.data[list.current] = value;
        list.current++;
    }
    else
    {
        cout << "full" << endl;
    }
}
//---------------------insert_static_arr
template <typename T>
void insert(list_arr<T>& list, int index, T value)
{
    if (list.size > list.current)
    {
        for (int i = list.current; i > index; i--)
        {
            list.data[i] = list.data[i - 1];
        }
        list.data[index] = value;
        list.current++;
    }
    else {
        cout << "full" << endl;
    }
}
//---------------------remove_static_arr
template <typename T>
void remove(list_arr<T>& list, int index)
{
    if (index < list.current)
    {
        for (int i = index; i < list.current - 1; i++)
        {
            list.data[i] = list.data[i + 1];
        }
        list.current--;
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------get_static_arr
template <typename T>
T get(list_arr<T>& list, int index)
{
    if (index < list.current)
    {
        return list.data[index];
    }
    else
    {
        cout << "invalid index" << endl;
        return T();
    }
}
// ---------------------set_static_arr
template <typename T>
void set(list_arr<T>& list, int index, T value)
{
    if (index < list.current)
    {
        list.data[index] = value;
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------length_static_arr
template <typename T>
int length(list_arr<T>& list)
{
    return list.current;
}
//---------------------print_static_arr
template <typename T>
void print(list_arr <T>& list)
{
    cout << endl << endl;
    for (int i = 0; i < list.current; i++)
    {
        cout << list.data[i] << " ";
    }
    cout << endl << endl;
}



template <typename T>
struct vector_arr {
    vector<T> data;
};
//MEGA crazy struct :D


//---------------------create_empty_vector
template <typename T>
void create_empty(vector_arr<T>& list)
{
	list.data.clear();
}
//---------------------append_vector
template <typename T>
void append(vector_arr<T>& list, T value)
{
    list.data.push_back(value);
}
//---------------------insert_vector
template <typename T>
void insert(vector_arr<T> &list, int index, T value)
{
    if (index < list.data.size())
    {
        list.data.insert(list.data.begin() + index, value);
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------remove_vector
template <typename T>
void remove(vector_arr<T>& list, int index)
{
    if (index < list.data.size())
    {
        list.data.erase(list.data.begin() + index);
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------get_vector
template <typename T>
T get(vector_arr<T>& list, int index)
{
    if (index < list.data.size())
    {
        return list.data[index];
    }
    else
    {
        cout << "invalid index" << endl;
        return T();
    }
}
//---------------------set_vector
template <typename T>
void set(vector_arr<T>& list, int index, T value)
{
    if (index < list.data.size())
    {
        list.data[index] = value;
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------length_vector
template <typename T>
int length(vector_arr<T>& list)
{
    return list.data.size();
}
//---------------------print_vector
template <typename T>
void print(vector_arr<T>& list)
{
    cout << endl << endl;
    for (int i = 0; i < list.data.size(); i++)
    {
        cout << list.data[i] << " ";
    }
    cout << endl << endl;
}




template <typename T>
struct Node {
    T data;
    Node<T>* next = nullptr;
};
template <typename T>
struct list_linked {
    Node<T>* head = nullptr;
	Node<T>* tail = nullptr;
    int size = 0;
};

//---------------------create_empty_linked
template <typename T>
void create_empty(list_linked<T>& list)
{
    list.head = nullptr;
	list.tail = nullptr;
    list.size = 0;
}
//---------------------append_linked
template <typename T>
void append(list_linked<T>& list, T value)
{
    Node<T>* new_node = new Node<T>{value, nullptr};
    if (list.head == nullptr)
    {
        list.head = new_node;
		list.tail = new_node;
    }
    else
    {
		list.tail->next = new_node;
		list.tail = new_node;
    }
    list.size++;
}
//---------------------insert_linked
template <typename T>
void insert(list_linked<T>& list, int index, T value)
{
    if (index < 0 || index > list.size) {
        cout << "invalid index" << endl;
        return;
    }

    if (index == list.size) {
        append(list, value);
        return;
    }

    Node<T>* new_node = new Node<T>{};
    new_node->data = value;
    if (index == 0)
    {
        new_node->next = list.head;
        list.head = new_node;
        if (list.size == 0)
            list.tail = new_node;
    }
    else
    {
        Node<T>* current = list.head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    list.size++;
}
//---------------------remove_linked
template <typename T>
void remove(list_linked<T>& list, int index)
{
    if (index < 0 || index >= list.size) {
        cout << "invalid index" << endl;
        return;
    }

    if (index == 0)
    {
        Node<T>* toDelete = list.head;
        list.head = list.head->next;
        delete toDelete;
        if (list.head == nullptr)
            list.tail = nullptr;
    }
    else
    {
        Node<T>* current = list.head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }
        Node<T>* toDelete = current->next;
        current->next = toDelete->next;
        if (index == list.size - 1)
            list.tail = current;
        delete toDelete;
    }
    list.size--;
}
//---------------------get_linked
template <typename T>
T get(list_linked<T>& list, int index)
{
    if (index < list.size)
    {
        Node<T>* current = list.head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->data;
    }
    else
    {
        cout << "invalid index" << endl;
        return T();
    }
}
//---------------------set_linked
template <typename T>
void set(list_linked<T>& list, int index, T value)
{
    if (index < list.size)
    {
        Node<T>* current = list.head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        current->data = value;
    }
    else
    {
        cout << "invalid index" << endl;
    }
}
//---------------------length_linked
template <typename T>
int length(list_linked<T>& list)
{
    return list.size;
}
//---------------------print_linked
template <typename T>
void print(list_linked<T>& list)
{
    cout << endl << endl;
    Node<T>* current = list.head;
    while (current != nullptr)
    {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl << endl;
}
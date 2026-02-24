#pragma once
#include <iostream>
#include <vector>
using namespace std;


struct Node
{
	int size = 0;
	int capacity = 0;
	int* arr = nullptr;
	Node* next = nullptr;
	Node* prev = nullptr;
};

struct Node_vector
{
	int size = 0;
	int capacity = 0;
	vector<int> arr;
	Node_vector* next = nullptr;
	Node_vector* prev = nullptr;
};

struct Unrolled_linked_list_arr 
{
	int size = 0;
	int capacity_of_nodes = 0;
	Node arr;
	Node* head = nullptr;
	Node* tail = nullptr;
};


struct Unrolled_linked_list_vector
{
	int size = 0;
	int capacity_of_nodes = 0;
	Node_vector arr;
	Node_vector* head = nullptr;
	Node_vector* tail = nullptr;
};
void init_list(Unrolled_linked_list_arr& list, int cap) {
	list.capacity_of_nodes = cap;
	list.arr.capacity = cap;
	list.arr.arr = new int[cap];
}
void init_list(Unrolled_linked_list_vector& list, int cap) {
	list.capacity_of_nodes = cap;
	list.arr.capacity = cap;
	list.arr.arr.reserve(cap);
}
//----------------------------create and size
void set_size(Unrolled_linked_list_arr& list)
{
	int size = 0;
	cout << "size  " << endl;
	cin >> size;
	list.capacity_of_nodes = size;
	list.arr.capacity = list.capacity_of_nodes;
	list.arr.arr = new int[list.capacity_of_nodes];
}
void set_size(Unrolled_linked_list_vector& list)
{
	int size = 0;
	cout << "size  " << endl;
	cin >> size;
	list.capacity_of_nodes = size;
	list.arr.capacity = list.capacity_of_nodes;
	list.arr.arr.reserve(list.capacity_of_nodes);
}

void create_empty(Unrolled_linked_list_vector& list)
{
	list.size = 0;
	list.arr.arr.clear();
	list.head = nullptr;
	list.tail = nullptr;
}
void create_empty(Unrolled_linked_list_arr& list)
{
	list.size = 0;
	list.arr.arr = nullptr;
	list.arr.size = 0;
	list.head = nullptr;
	list.tail = nullptr;
}
//----------------------------append
void append(Unrolled_linked_list_arr& list, int value)
{
	//first add
	if (list.head == nullptr)
	{
		list.head = &list.arr;
		list.tail = &list.arr;
	}


	if (list.tail->size < list.capacity_of_nodes)
	{
		list.tail->arr[list.tail->size] = value;
		list.tail->size++;
	}
	else
	{
		Node* new_node = new Node;
		new_node->arr = new int[list.capacity_of_nodes];
		new_node->capacity = list.capacity_of_nodes;

		new_node->arr[new_node->size] = value;
		new_node->size++;
		new_node->prev = list.tail;
		list.tail->next = new_node;
		list.tail = new_node;
	}
	list.size++;
}
void append(Unrolled_linked_list_vector& list, int value)
{
	if (list.head == nullptr)
	{
		list.head = &list.arr;
		list.tail = &list.arr;
	}




	if (list.tail->size < list.capacity_of_nodes)
	{
		list.tail->arr.push_back(value);
		list.tail->size++;
	}
	else
	{
		Node_vector* new_node = new Node_vector;
		new_node->capacity = list.capacity_of_nodes;
		new_node->arr.push_back(value);
		new_node->size++;
		new_node->prev = list.tail;
		list.tail->next = new_node;
		list.tail = new_node;
	}
	list.size++;
}
//----------------------------insert
void insert(Unrolled_linked_list_arr& list, int index, int value)
{
	if (index < 0 || index > list.size)
	{
		cout << "Invalid index" << endl;
		return;
	}
	if (index == list.size)
	{
		append(list, value);
		return;
	}

	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;

	Node* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}

	while (current != nullptr)
	{
		if (current->size < list.capacity_of_nodes)
		{
			for (int i = current->size; i > position_in_node; i--)
			{
				current->arr[i] = current->arr[i - 1];
			}
			current->arr[position_in_node] = value;
			current->size++;
			list.size++;
			return;
		}
		//full node
		else
		{
			int pushed_out_value = current->arr[current->size - 1];
			for (int i = current->size - 1; i > position_in_node; i--)
			{
				current->arr[i] = current->arr[i - 1];
			}
			current->arr[position_in_node] = value;

			value = pushed_out_value;
			position_in_node = 0;// next node index 0

			if (current->next == nullptr)
			{
				Node* new_node = new Node;
				new_node->arr = new int[list.capacity_of_nodes];
				new_node->capacity = list.capacity_of_nodes;
				new_node->prev = current;
				current->next = new_node;
				list.tail = new_node;
			}
			current = current->next;
		}
	}
}

void insert(Unrolled_linked_list_vector& list, int index, int value)
{
	if (index < 0 || index > list.size)
	{
		cout << "Invalid index" << endl;
		return;
	}
	if (index == list.size)
	{
		append(list, value);
		return;
	}
	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;
	Node_vector* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}
	//каскадний перенос from gemini
	while (current != nullptr)
	{
		if (current->size < list.capacity_of_nodes)
		{
			current->arr.insert(current->arr.begin() + position_in_node, value);
			current->size++;
			list.size++;
			return;
		}
		else
		{
			int pushed_out_value = current->arr[current->size - 1];
			current->arr.pop_back();
			current->arr.insert(current->arr.begin() + position_in_node, value);
			value = pushed_out_value;
			position_in_node = 0;// next node index 0
			if (current->next == nullptr)
			{
				Node_vector* new_node = new Node_vector;
				new_node->capacity = list.capacity_of_nodes;
				new_node->prev = current;
				current->next = new_node;
				list.tail = new_node;
			}
			current = current->next;
		}
	}
}
//----------------------------get_by_index
int get_by_index(Unrolled_linked_list_arr& list, int index)
{
	if (index < 0 || index >= list.size)
	{
		cout << "Invalid index" << endl;
		return -1;
	}
	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;
	Node* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}
	return current->arr[position_in_node];
}
int get_by_index(Unrolled_linked_list_vector& list, int index)
{
	if (index < 0 || index >= list.size)
	{
		cout << "Invalid index" << endl;
		return -1;
	}
	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;
	Node_vector* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}
	return current->arr[position_in_node];
}
//----------------------------search_by_value
int search_by_value(const Unrolled_linked_list_arr& list, int value)
{
	Node* current = list.head;
	int index = 0;
	while (current != nullptr)
	{
		for (int i = 0; i < current->size; i++)
		{
			if (current->arr[i] == value)
			{
				return index + i;
			}
		}
		index += current->size;
		current = current->next;
	}
	return -1; // not found
}
int search_by_value(const Unrolled_linked_list_vector& list, int value)
{
	Node_vector* current = list.head;
	int index = 0;
	while (current != nullptr)
	{
		for (int i = 0; i < current->size; i++)
		{
			if (current->arr[i] == value)
			{
				return index + i;
			}
		}
		index += current->size;
		current = current->next;
	}
	return -1; // not found
}
//----------------------------delete_by_index
void delete_by_index(Unrolled_linked_list_arr& list, int index)
{
	if (index < 0 || index >= list.size)
	{
		cout << "Invalid index" << endl;
		return;
	}

	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;
	Node* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}
	//каскадний зсув from gemini
	while (current != nullptr)
	{
		for (int i = position_in_node; i < current->size - 1; i++)
		{
			current->arr[i] = current->arr[i + 1];
		}

		if (current->next != nullptr)
		{
			current->arr[current->size - 1] = current->next->arr[0];
			position_in_node = 0;
			current = current->next;
		}
		else
		{
			current->size--;
			if (current->size == 0 && current != &list.arr)
			{
				Node* empty_node = current;
				list.tail = empty_node->prev;
				list.tail->next = nullptr;

				delete[] empty_node->arr;
				delete empty_node;
			}
			break;
		}
	}
	list.size--;
}
void delete_by_index(Unrolled_linked_list_vector& list, int index)
{
	if (index < 0 || index >= list.size)
	{
		cout << "Invalid index" << endl;
		return;
	}

	int target_node_idx = index / list.capacity_of_nodes;
	int position_in_node = index % list.capacity_of_nodes;
	Node_vector* current = list.head;
	for (int i = 0; i < target_node_idx; i++)
	{
		current = current->next;
	}

	while (current != nullptr)
	{
		current->arr.erase(current->arr.begin() + position_in_node);
		current->size--;

		if (current->next != nullptr)
		{
			int stolen_value = current->next->arr[0];
			current->arr.push_back(stolen_value);
			current->size++;
			position_in_node = 0;
			current = current->next;
		}
		else
		{
			if (current->size == 0 && current != &list.arr)
			{
				Node_vector* empty_node = current;
				list.tail = empty_node->prev;
				list.tail->next = nullptr;
				delete empty_node; 
			}
			break;
		}
	}

	list.size--;
}
//----------------------------print_forward
void print_forward(const Unrolled_linked_list_arr& list)
{
	Node* current = list.head;
	while (current != nullptr)
	{
		for (int i = 0; i < current->size; i++)
		{
			cout << current->arr[i] << " ";
		}
		current = current->next;
	}
	cout << endl;
}
void print_forward(const Unrolled_linked_list_vector& list)
{
	Node_vector* current = list.head;
	while (current != nullptr)
	{
		for (int i = 0; i < current->size; i++)
		{
			cout << current->arr[i] << " ";
		}
		current = current->next;
	}
	cout << endl;
}
//----------------------------print_backward
void print_backward(const Unrolled_linked_list_arr& list)
{
	Node* current = list.tail;
	while (current != nullptr)
	{
		for (int i = current->size - 1; i >= 0; i--)
		{
			cout << current->arr[i] << " ";
		}
		current = current->prev;
	}
	cout << endl;
}
void print_backward(const Unrolled_linked_list_vector& list)
{
	Node_vector* current = list.tail;
	while (current != nullptr)
	{
		for (int i = current->size - 1; i >= 0; i--)
		{
			cout << current->arr[i] << " ";
		}
		current = current->prev;
	}
	cout << endl;
}
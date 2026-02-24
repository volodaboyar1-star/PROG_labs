#pragma once
#include <chrono>
#include <list>
#include "struct.h"

void demo_mode() {
	cout << endl << "=== DEMO ===" << endl;
	Unrolled_linked_list_arr demo_list;
	init_list(demo_list, 3); // Місткість 3

	cout << "1- adding 10, 20, 30, 40, 50 (Append)" << endl;
	append(demo_list, 10); append(demo_list, 20); append(demo_list, 30);
	append(demo_list, 40); append(demo_list, 50);
	print_forward(demo_list);

	cout << "2- insert 99 at indx 1 (Insert)" << endl;
	insert(demo_list, 1, 99);
	print_forward(demo_list);

	cout << "3- insert at indx 2 (Delete)" << endl;
	delete_by_index(demo_list, 2);
	print_forward(demo_list);

	cout << "4-find at indx 3" << endl;
	cout << "found " << get_by_index(demo_list, 3) << endl;

	cout << "5- print reverse" << endl;
	print_backward(demo_list);
}



template <typename ListType>
void run_interactive_menu(ListType& user_list) {
	cout << endl << "=== INTERACT ===" << endl;

	int choice;
	do {
		cout << endl << "menu:" << "1- Append " << "2- Insert " << "3- Delete " << "4- Print Forward " << "5- Print Backward " << "6- Search " << "0- exit " << endl;
		cin >> choice;

		if (choice == 1) {
			int val; cout << "val "; cin >> val;
			append(user_list, val);
		}
		else if (choice == 2) {
			int idx, val; cout << "indx "; cin >> idx; cout << "val "; cin >> val;
			insert(user_list, idx, val);
		}
		else if (choice == 3) {
			int idx; cout << "indx: "; cin >> idx;
			delete_by_index(user_list, idx);
		}
		else if (choice == 4) {
			print_forward(user_list);
		}
		else if (choice == 5) {
			print_backward(user_list);
		}
		else if (choice == 6) {
			int val; cout << "val for search "; cin >> val;
			int res = search_by_value(user_list, val);
			if (res != -1) cout << "indx found " << res << endl;
			else cout << "error not found" << endl;
		}
	} while (choice != 0);
}

void interactive_mode() {
	cout << endl << "=== INTERACT ===" << endl;
	cout << "1arr" << endl;
	cout << "2vector" << endl;


	int type_choice;
	cin >> type_choice;

	if (type_choice == 1) {
		cout << "ARR" << endl;
		Unrolled_linked_list_arr user_list;
		set_size(user_list);
		run_interactive_menu(user_list);
	}
	else if (type_choice == 2) {
		cout << "VECTOR" << endl;
		Unrolled_linked_list_vector user_list;
		set_size(user_list);
		run_interactive_menu(user_list);
	}
	else {
		cout << "error" << endl;
	}
}




void benchmark_mode() {
	cout << endl << "=== BENCHMARK ===" << endl;
	const int ELEMENTS = 50000;
	const int INSERTS = 1000;

	Unrolled_linked_list_arr my_list;
	init_list(my_list, 100);
	Unrolled_linked_list_vector my_vector_list;
	init_list(my_vector_list, 100);
	std::vector<int> std_vec;
	std::list<int> std_lst;

	// --- APPEND ---
	cout << "adding " << ELEMENTS << " append" << endl;

	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < ELEMENTS; i++) append(my_list, i);
	auto end = chrono::high_resolution_clock::now();
	cout << "My Unrolled List: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < ELEMENTS; i++) append(my_vector_list, i);
	end = chrono::high_resolution_clock::now();
	cout << "My Unrolled List Vector: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;


	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < ELEMENTS; i++) std_lst.push_back(i);
	end = chrono::high_resolution_clock::now();
	cout << "std::list: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;


	// --- INSERT ---
	cout << endl << "INSERT" << INSERTS << " Insert at 0" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < INSERTS; i++) insert(my_list, 0, i);
	end = chrono::high_resolution_clock::now();
	cout << "my Unrolled List: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < INSERTS; i++) insert(my_vector_list, 0, i);
	end = chrono::high_resolution_clock::now();
	cout << "my unrolled linked list vector : " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < INSERTS; i++) std_lst.push_front(i);
	end = chrono::high_resolution_clock::now();
	cout << "std::list  " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

}

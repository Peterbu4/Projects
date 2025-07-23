// This code uses dynamic array and linked list with tail pointer to ilustrate how the priority queue works
// and to perform test on both representations to test their efeciency.
// The priority queue implemented on a dynamic array is ordered ascendingly, while the one implemented on
// linked list with tail pointer has the highest priority first in order to take advantage of the structures properties and
// receive the best results (the most efecient results using aforementioned data structures).
#include <iostream>
#include <chrono>

using namespace std;

using namespace std::chrono;

struct Element {
    int value;
    int priority;
};
// priority queue implemented on a linked list
struct Node {
    Element data;
    Node* next;
    Node(Element e) : data(e), next(nullptr) {}
};

class PriorityQueueList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    PriorityQueueList() : head(nullptr), tail(nullptr), size(0) {}

    ~PriorityQueueList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(int value, int priority) {
        Node* newNode = new Node({ value, priority });

        if (!head || head->data.priority <= priority) {
            newNode->next = head;
            head = newNode;
            if (!tail) tail = head;
        }
        else {
            Node* current = head;
            while (current->next && current->next->data.priority > priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            if (!newNode->next) tail = newNode;
        }
        size++;
    }

    void pop() {
        if (!head) {
            cout << "Queue is empty!\n";
            return;
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        if (!head) tail = nullptr;
        size--;
    }

    Element peek() const {
        if (!head) {
            cout << "Queue is empty!\n";
            return { -1, -1 };
        }
        return head->data;
    }

    int get_size() const {
        return size;
    }

    void change_priority(int value, int new_priority) {
        Node* prev = nullptr;
        Node* current = head;
        while (current && current->data.value != value) {
            prev = current;
            current = current->next;
        }
        if (!current) {
            cout << "Element not found!\n";
            return;
        }

        if (prev) prev->next = current->next;
        else head = current->next;
        if (!current->next) tail = prev;

        Element elem = current->data;
        delete current;
        size--;
        push(elem.value, new_priority);
    }

    void print() const {
        Node* current = head;
        cout << "Kolejka (value:priority): ";
        while (current) {
            cout << "(" << current->data.value << ":" << current->data.priority << ") ";
            current = current->next;
        }
        cout << endl;
    }
};
/// Priority queue implemented on a dynamic array
class PriorityQueueArray {
private:
    Element* data;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Element* temp = new Element[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

public:
    // Initial capacity: 10 (enlarged twice)
    PriorityQueueArray(int initial_capacity = 10) {
        capacity = initial_capacity;
        size = 0;
        data = new Element[capacity];
    }

    ~PriorityQueueArray() {
        delete[] data;
    }

    // Adding element dependantly on the priority (in ascending order)   ///
    void push(int value, int priority) {
        if (size == capacity) {
            resize();
        }

        int i = size - 1;
        while (i >= 0 && data[i].priority > priority) {
            data[i + 1] = data[i];
            i--;
        }
        data[i + 1].value = value;
        data[i + 1].priority = priority;
        size++;
    }

    // Removal of the item with the highest priority
    void pop() {
        if (size == 0) {
            cout << "Queue is empty!\n";
            return;
        }
        size--;
    }

    // Peek at the item with the highest priority
    Element peek() const {
        if (size == 0) {
            cout << "Queue is empty!\n";
            return { -1, -1 };
        }
        return data[size -1];
    }

    // Returning the size
    int get_size() const {
        return size;
    }

    // Modifying the priority of an item
    void change_priority(int value, int new_priority) {
        int index = -1;
        for (int i = 0; i < size; i++) {
            if (data[i].value == value) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Element not found!\n";
            return;
        }

        // Removing element
        Element elem = data[index];
        for (int i = index + 1; i < size; i++) {
            data[i - 1] = data[i];
        }
        size--;

        // Adding with new priority
        push(elem.value, new_priority);
    }

    void print() const {
        cout << "Kolejka (value:priority): ";
        for (int i = 0; i < size; i++) {
            cout << "(" << data[i].value << ":" << data[i].priority << ") ";
        }
        cout << endl;
    }
};

int main() {
    const int copies = 100; // number of copies of the priority queue
    const int elements = 10; // Number of elements in each priority queue
    // Creating a specific number of copies 
    PriorityQueueArray** queues = new PriorityQueueArray * [copies];
    for (int i = 0; i < copies; ++i) {
        queues[i] = new PriorityQueueArray();
    }
    // Adding initial number of elements to the queue
    for (int i = 0; i < copies; ++i) {
        for (int j = 0; j < elements; ++j) {
        //    queues[i]->push(j, rand() % 100); // Random priority
            queues[i]->push(j, 1);// Defined priority (for instance: 1)
        }
    }
    /// Dynamic array - tests
    // Measurement of adding time
    auto start_push = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        queues[i]->push(i, rand() % 100); // Random priority
        ///queues[i]->push(i, 1);// Defined priority (for instance: 1)
    }
    auto end_push = high_resolution_clock::now();
    ///
    // Measurement of peek operation time
    auto start_peek = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        queues[i]->peek();
    }
    auto end_peek = high_resolution_clock::now();

    // Measurement of changing priority time (the priority of element at index: 0 is changed)
    auto start_change = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        queues[i]->change_priority(0, 999); // setting high priority
    }
    auto end_change = high_resolution_clock::now();

    // Measurement of removal time
    auto start_pop = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        queues[i]->pop();
    }
    auto end_pop = high_resolution_clock::now();

    // Measurement of get_size operation time
    auto start_size = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        queues[i]->get_size();
    }
    auto end_size = high_resolution_clock::now();
    ///queues[1]->print(); // Optional - printing the elements from the queue
    // Average times
    cout << "Sredni czas dodawania (" << elements << " elementow do tablicy): "
        << duration<double, milli>(end_push - start_push).count() / copies << " ms\n";

    cout << "Sredni czas peek [tablica]: "
        << duration<double, nano>(end_peek - start_peek).count() / copies << " ns\n";

    cout << "Sredni czas zmiany priorytetu [tablica]: "
        << duration<double, nano>(end_change - start_change).count() / copies << " ns\n";

    cout << "Sredni czas usuwania [tablica]: "
        << duration<double, nano>(end_pop - start_pop).count() / copies << " ns\n";

    cout << "Sredni czas get_size [tablica]: "
        << duration<double, nano>(end_size - start_size).count() / copies << " ns\n";

    for (int i = 0; i < copies; ++i) {
        delete queues[i];
    }
    delete[] queues;

    /// Linked list - tests
    PriorityQueueList** lists = new PriorityQueueList * [copies];
    for (int i = 0; i < copies; ++i) {
        lists[i] = new PriorityQueueList();
    }
    for (int i = 0; i < copies; ++i) {
        for (int j = 0; j < elements; ++j) {
            lists[i]->push(j, rand() % 100);
            //lists[i]->push(j, 1);
        }
    }
    auto start_push_list = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        lists[i]->push(i, rand() % 100);
    }
    auto end_push_list = high_resolution_clock::now();

    auto start_peek_list = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        lists[i]->peek();
    }
    auto end_peek_list = high_resolution_clock::now();

    auto start_change_list = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        lists[i]->change_priority(0, 999);
    }
    auto end_change_list = high_resolution_clock::now();

    auto start_pop_list = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        lists[i]->pop();
    }
    auto end_pop_list = high_resolution_clock::now();

    auto start_size_list = high_resolution_clock::now();
    for (int i = 0; i < copies; ++i) {
        lists[i]->get_size();
    }
    auto end_size_list = high_resolution_clock::now();
    //lists[1]->print(); // Optional - printing the elements from the queue
    cout << "Sredni czas dodawania [lista]: "
        << duration<double, milli>(end_push_list - start_push_list).count() / copies << " ms\n";
    cout << "Sredni czas peek [lista]: "
        << duration<double, nano>(end_peek_list - start_peek_list).count() / copies << " ns\n";
    cout << "Sredni czas zmiany priorytetu [lista]: "
        << duration<double, nano>(end_change_list - start_change_list).count() / copies << " ns\n";
    cout << "Sredni czas usuwania [lista]: "
        << duration<double, nano>(end_pop_list - start_pop_list).count() / copies << " ns\n";
    cout << "Sredni czas get_size [lista]: "
        << duration<double, nano>(end_size_list - start_size_list).count() / copies << " ns\n";

    for (int i = 0; i < copies; ++i) {
        delete lists[i];
    }
    delete[] lists;

    return 0;
}

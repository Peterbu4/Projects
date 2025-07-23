#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace std;

int partition(int* elements, int low, int high) {
    int pivot = elements[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (elements[j] <= pivot) {
            i++;
            swap(elements[i], elements[j]);
        }
    }
    swap(elements[i + 1], elements[high]);
    return (i + 1);
}

// Iterative version of Quicksort algorithm with dynamically allocated stack
void quickSort(int* elements, int begin, int end) {
    if (begin >= end) return;
    // Creating dynamic stack
    int* stack = new int[end - begin + 1];
    int top = -1;
    // Adding initial values
    stack[++top] = begin;
    stack[++top] = end;
    // Stack processing
    while (top >= 0) {
        end = stack[top--];
        begin = stack[top--];

        int pi = partition(elements, begin, end);
        // If the left part exists add it to the stack
        if (pi - 1 > begin) {
            stack[++top] = begin;
            stack[++top] = pi - 1;
        }
        // If the right part exists add it to the stack
        if (pi + 1 < end) {
            stack[++top] = pi + 1;
            top++;
            stack[++top] = end;
        }
    }
    // Free the memory
    delete[] stack;
}

void merge(int data[], int left, int middle, int right) {
    // Choosing the place for the set division (the middle value)
    int n1 = middle - left + 1;
    int n2 = right - middle;
    // Creating temporary tabels for the data
    int* leftArr = new int[n1];
    int* rightArr = new int[n2];
    // Assingning data to the specific temporary tables
    for (int i = 0; i < n1; i++) leftArr[i] = data[left + i];
    for (int j = 0; j < n2; j++) rightArr[j] = data[middle + 1 + j];

    int i = 0, j = 0, k = left;
    // Sorting by choosing element with lower value from the two tabels
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            data[k] = leftArr[i++];
        }
        else {
            data[k] = rightArr[j++];
        }
        k++;
    }
    // Copying remaining element from the temporary table (only one of the following will execute)
    while (i < n1) data[k++] = leftArr[i++];
    while (j < n2) data[k++] = rightArr[j++];
    
    // Deleting temporary tabels
    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(int data[], int left, int right) {
    if (left < right) {
        // Spliting the data set in half
        int middle = left + (right - left) / 2;
        mergeSort(data, left, middle);
        mergeSort(data, middle + 1, right);
        merge(data, left, middle, right);
    }
}

void introsort_pom(int* elements, int begin, int end, int depth_limit) {
    int size = end - begin + 1;

    if (size <= 1)
        return;

    if (depth_limit == 0) {
        // When the recursion gets too deep the mergesort is called
        mergeSort(elements, begin, end);
        return;
    }

    int pi = partition(elements, begin, end); // quicksort
    introsort_pom(elements, begin, pi - 1, depth_limit - 1);
    introsort_pom(elements, pi + 1, end, depth_limit - 1);
}

// Main function of the introspective sort
void introSort(int* elements, int n) {
    int depth_limit = 2 * log2(n); // Set depth limit
    introsort_pom(elements, 0, n - 1, depth_limit);
}

int main()
{
    srand(time(NULL));
    const int array_size = 10;/// Maximal number of elements to sort (it needs to be set)!
    int sum = 0, counter = 0, choice = 0;
    float average = 0, median = 0;
    int* elementy = new int[array_size];
    for (int i = 0; i < array_size; i++)
    {
        elementy[i] = (rand() % (array_size + 1)) + 0;
        sum += elementy[i];
        counter++;
    }
    int* oryginalne = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        oryginalne[i] = elementy[i];
    }
    average = sum / array_size;
    
    do
    {
        cout << "Choose:\n1 - Merge Sort\n2 - Quicksort\n3 - Introsort\n4 - Exit\n";
        cin >> choice;
        if (choice == 1)
        {
            auto start1 = chrono::high_resolution_clock::now();
            mergeSort(elementy, 0, counter - 1);
            auto stop1 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time = stop1 - start1;
            cout << "Merge Sort execution time for: " << array_size << " elements: " << time.count() << " ms" << endl;
        }
        else if (choice == 2)
        {
            auto start2 = chrono::high_resolution_clock::now();
            quickSort(elementy, 0, counter - 1);
            auto stop2 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time = stop2 - start2;
            cout << "Quicksort execution time for: " << array_size << " elements: " << time.count() << " ms" << endl;
        }
        else if (choice == 3)
        {
            auto start3 = chrono::high_resolution_clock::now();
            introSort(elementy, counter);
            auto stop3 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time = stop3 - start3;
            cout << "Introsort execution time for: " << array_size << " elements: " << time.count() << " ms" << endl;
        }
        else if (choice == 4)
        {
            cout << "End of the program!" << endl;
        }
        else
        {
            cout << "Incorrect number was entered!" << endl;
        }
        if (choice == 1 || choice == 2 || choice == 3)
        {
            if (counter % 2 == 0)// Median for an even number of elements
            {
                int right;
                right = counter / 2;
                median = (elementy[right - 1] + elementy[right]) / 2;
            }
            if (counter % 2 == 1)// Median for an odd number of elements
            {
                int middle;
                middle = counter / 2;
                median = elementy[middle];
            }
            cout << "Average value of the numbers in the set: " << average << endl;
            cout << "Median of sorted numbers: " << median << endl;
            cout << "Sorted numbers: " << endl;
            for (int i = 0; i < counter; i++)
            {
                cout << elementy[i] << endl;
            }
            for (int i = 0; i < zakres; i++) 
            {
                elementy[i] = oryginalne[i];
            }
            cout << "Unsorted list of elements:" << endl;
            for (int i = 0; i < counter; i++)
            {
                cout << elementy[i] << endl;
            }
        }  
    } while (choice != 4);
  
    delete[] oryginalne;
    delete[] elementy;
}

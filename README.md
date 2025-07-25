## Projects repository contains projects written using different technologies: **C++, Python and C**  

### C++ Projects
- **[Dining philosophers problem](Dining_philosophers_problem.cpp)**  - demonstrates the use of `mutex`, `thread`, and `condition_variable` to prevent deadlocks and thread starvation. The output is displayed using the `ncurses.h` library. The **minimum number of philosophers** that must be specified when running the program is **five**.
- **[Tic-Tac-toe game](Tic-Tac-Toe-game(with_simple_AI).cpp)** - implements the **Minimax algorithm** with **alpha-beta pruning**, demonstrating how a basic AI algorithm can be applied to a popular game. The user can choose the **board size** (from 3 to 10) and their preferred mark.  
- **[OOP Concept Illustration](OOP_classes_virtual_function_template_operator.cpp)**  
  - Abstraction  
  - Encapsulation  
  - Inheritance  
  - Polymorphism
That includes examples of **virtual functions**, **derived classes**, **operator overloading**, **templates**.
- **[Pac-Man-like-game](Pac-Man-like-game.cpp)** - A simplified Pac-Man-style game using a board represented as a string. The player controls movement using the 'a', 'w', 's', 'd' keys. Input handling is done using functions from the `cctype` and `conio.h` libraries.
- **[Priority queues](priority_queues.cpp)** - provides two separate implementations of priority queues:  
  - Dynamic array (ascending order)  
  - Linked list with tail pointer (descending order)  
Each implementation supports adding/removing elements, changing priorities, peeking the first element, and checking the size. The structures are sorted using an insertion sort algorithm adapted to the respective data type.  
- **[Sorting algorithms](Sorting_algorithms.cpp)** - includes implementations of **Merge Sort**, **Quick Sort**, and **Intro Sort**, written without using built-in sorting functions. The terminal interface allows the user to:
  - Choose one of the three algorithms
  - View both the sorted and unsorted list
  - Display the **average** and **median** of the values

### Python Projects
 - **[Image processing program](image_processing.py)** - uses the cv2 library to perform various image processing tasks, such as:
    - rotation
    - rescaling
    - sharpening
    - conversion to grayscale
    - restoring the original image
    - edge detection
 - **[Neural network](Simple_neural_network_adding_two_numbers.py)** - a simple neural network designed to evaluate the sum of two floating-point numbers in the range [0, 1]. Built with **TensorFlow** and **Keras**, this project compares the model's predicted results to the actual sums.
 - **[Object detection](simple_image_detection.py)** - uses a pretrained **YOLO** model to detect objects in an image. The image must be placed in the same directory as the script because it uses relative paths.

### C Projects 
- **[Twilight switch](Simple_twilight_switch_with_hysteresis.c)** - a microcontroller-based system that uses ADC channels to read analog values from a **photoresistor** and **potentiometer**. Based on the input, an LED is turned on or off. The current value is shown on an LCD using the `LCDI2C.h` library which has to be added. The project also includes **hysteresis** to prevent unnecessary switching near the threshold.
- **[LED_UART](UART_LED_ATMEGA328P.c)** - a program that controls an LED via **UART communication** on an ATmega328P microcontroller. Users can turn the LED on or off and view the program's uptime in the terminal.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <ncurses.h>

using namespace std;

// Number of philosophers
int philosopher_count;

// Mutex used to synchronise ncurses
mutex ncurses_mutex;

// Philosophers' states
enum State { THINKING, HUNGRY, EATING };
vector<State> philosopher_states;

// Mutex and condition variable for philosopher synchronization
mutex mtx;
condition_variable cv;

// Count of meals eaten by each philosopher
vector<int> meals_finished;

// Shared resources (forks)
vector<bool> forks;

// Draw the current state of all philosophers
void draw_state() {
    lock_guard<mutex> lock(ncurses_mutex);
    clear();
    for (int i = 0; i < philosopher_count; i++) {
        mvprintw(i, 0, "Philosopher %d: %s | Meal count: %d", i,
                 (philosopher_states[i] == THINKING ? "Thinking " :
                  (philosopher_states[i] == HUNGRY ? "Hungry" : "Eating")),
                 meals_finished[i]);
    }
    
    refresh();
}

// Function checks if the philosopher can start eating (both forks are free)
bool test(int id, vector<bool>& forks) {
    int left = id;
    int right = (id + 1) % philosopher_count;

    if (philosopher_states[id] == HUNGRY && !forks[left] && !forks[right]) {
        // Philosopher can start eating
        forks[left] = true;
        forks[right] = true;
        philosopher_states[id] = EATING;
        draw_state();
        cv.notify_all();
        return true; // Returns true if eating is initiated
    }
    return false; // Returns false if cannot eat
}


// Philosopher thread function
void philosopher(int id, vector<bool>& forks) {
    while (true) {
        // Philosopher is thinking
        {
            unique_lock<mutex> lock(mtx);
            if (id >= philosopher_states.size()) continue;
            philosopher_states[id] = THINKING;
            draw_state();
            cv.notify_all();
        }

        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        // Philosopher becomes hungry
        {
            unique_lock<mutex> lock(mtx);
            if (id >= philosopher_states.size()) continue;
            philosopher_states[id] = HUNGRY;
            draw_state();
            cv.notify_all();
        }

        // Attempt to pick up the forks
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]() {
                return test (id, forks);
            });
        } 

        // Philosopher dines
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        // Philosopher puts down the forks
        {
            unique_lock<mutex> lock(mtx);
            int left = id;
            int right = (id + 1) % philosopher_count;
            forks[left] = false;
            forks[right] = false;
            // Counter how many times the philosopher has eaten
            meals_finished[id]++;
            philosopher_states[id] = THINKING;
            draw_state();
            cv.notify_all();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uzycie: " << argv[0] << " <liczba_filozofow>" << endl;
        return 1;
    }

    philosopher_count = stoi(argv[1]);

    if (philosopher_count < 5) {
        cerr << "Liczba filozofow musi byc nie mniejsza niz 5." << endl;
        return 1;
    }

    //  Ncurses initialization
    initscr();
    noecho();
    curs_set(0);

    // States initialization
    forks.resize(philosopher_count, false);
    philosopher_states.resize(philosopher_count, THINKING);
    meals_finished.resize(philosopher_count, 0);

    // Creating threads representing philosophers 
    vector<thread> philosophers_threads;
    for (int i = 0; i < philosopher_count; i++) {
        philosophers_threads.emplace_back(philosopher, i, ref(forks));
    }

    // Joining philospher threads (current loop is infinite)
    for (auto& phil_thread : philosophers_threads) {
        phil_thread.join();
    }

    // Ending ncurses (never reached in current setup due to the infinite loop)
    endwin();

    return 0;
}

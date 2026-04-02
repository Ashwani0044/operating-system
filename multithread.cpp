// Multithreaded Task Scheduler in C++ 

#include <iostream>
#include <queue>
#include <functional>//functional is used for std::greater
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>//it is used for defining time intervals, like i am using sleep duration for 2 sec ..
#include <mutex>//it is used for locking and unlocking the critical section to prevent race conditions

using namespace std;

string getTime() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);

    string timeStr = ctime(&now_time);
    timeStr.pop_back(); // Remove the newline character added by ctime
    return timeStr;
}

queue<function<void(int)>> tasks;//to hold tasks, each task is a function that takes an int (thread id) as argument
mutex mtx;
mutex print_mtx;//separate mutex for printing to avoid interleaved output
const int MAX_QUEUE_SIZE = 10;//maximum tasks 

void worker(int id) {
   while(true) {//to run until there are no more tasks
        mtx.lock();//locking the queue so that only one thread can access it at a time

        if(tasks.empty()) {
            mtx.unlock();//unlocking before exiting the worker thread
            break; // No more tasks, exit the worker
        }

        auto task = tasks.front();
        tasks.pop();
        mtx.unlock();//unlocking the queue so that other threads can access it
        {
        lock_guard<mutex> lock(print_mtx);//lock_guard will automatically release the lock when it goes out of scope
        cout << "[" << getTime() << "] [Thread " << id << "] picked a task\n";
        }
        task(id);//passing thread id as an argument..
   }
}



int main() {

    for (int i = 1; i <= 5; i++) {

        if (tasks.size() < MAX_QUEUE_SIZE) {
            tasks.push([i](int threadId) {
                string requestType;
                //just to simulate different types of requests, i am using the modulus operator to assign a request type based on the task number
                if (i % 3 == 0) requestType = "Login Request";
                else if (i % 3 == 1) requestType = "Fetch Data Request";
                else requestType = "Upload File Request";
                {
                lock_guard<mutex> lock(print_mtx);
                cout << "[" << getTime() << "] [Thread " << threadId << "] " << requestType << " started\n";
                }
                this_thread::sleep_for(chrono::seconds(2));//sleep for 2 sec to simulate task processing time
                {
                lock_guard<mutex> lock(print_mtx);
                cout << "[" << getTime() << "] [Thread " << threadId << "] " << requestType << " completed\n";
                }
            });
        } else {
            cout << "Task queue is full. Cannot add more tasks.\n";
        }
    }
   

    // creating threads
    vector<thread> workers;

    for (int i = 0; i < 3; i++) {
        workers.emplace_back(worker, i);//creating worker threads and passing the thread id as an argument
    }

    // Join threads
    for (auto &t : workers) {
        t.join();//waiting for all worker threads to finish before exiting the main thread
    }
    return 0;
}

// I built a multithreaded task scheduler in C++ that simulates how real systems handle multiple tasks concurrently.

// The system maintains a task queue, and multiple worker threads pick tasks from the queue and execute them in parallel.

// I implemented synchronization using mutex to avoid race conditions and used condition variables to eliminate busy waiting and make the system efficient.

// This project helped me understand concepts like thread management, scheduling, synchronization, and how real-world backend systems handle concurrent requests



//QUESTIONS FROM AI.....

// Q1: Why did you use multithreading?

// Answer:

// “To simulate concurrent execution, like how servers handle multiple users at the same time. A single thread would process tasks sequentially, which is inefficient.”

// Q2: What problem did mutex solve?

//  Answer:

// “Multiple threads were accessing the shared queue, causing race conditions. Mutex ensures only one thread accesses it at a time, preventing data inconsistency.”

//  Q3: Why condition_variable?

//  Answer:

// “Without it, threads keep checking the queue continuously, which wastes CPU (busy waiting). Condition variables allow threads to sleep and wake up only when tasks are available.”

//  Q4: Why unlock before executing task?

//  Answer (🔥 important):

// “To minimize the critical section. If the lock is held during task execution, other threads would be blocked, reducing concurrency.”

//  Q5: What is a race condition in your project?

//  Answer:

// “When multiple threads tried to access and modify the queue simultaneously, leading to unpredictable behavior.”

//  Q6: How is this related to real systems?

//  Answer:

// “This is similar to how web servers or cloud systems manage multiple incoming requests using thread pools and task queues.”
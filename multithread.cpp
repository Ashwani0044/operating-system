#include <iostream>
#include <queue>
#include <functional>//functional is used for std::greater
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

queue<function<void()>> tasks;
mutex mtx;

void worker(int id) {
   while(true) {
        mtx.lock();

        if(tasks.empty()) {
            mtx.unlock();
            break; // No more tasks, exit the worker
        }

        auto task = tasks.front();
        tasks.pop();
        mtx.unlock();

        cout << "Worker " << id << " executing task...\n";
        task();
   }
}

int main() {

    for (int i = 1; i <= 5; i++) {
            tasks.push([i]() {
                cout << "Task " << i << " executed\n";
        });
    }

    // Create threads
    vector<thread> workers;

    for (int i = 0; i < 3; i++) {
        workers.emplace_back(worker, i);
    }

    // Join threads
    for (auto &t : workers) {
        t.join();
    }

    return 0;
}
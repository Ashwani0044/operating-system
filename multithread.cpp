#include <iostream>
#include <queue>
#include <functional>//functional is used for std::greater
#include <vector>
#include <thread>

using namespace std;

queue<function<void()>> tasks;

void worker(int id) {
    while(!tasks.empty()) {
        auto task = tasks.front();
        tasks.pop();

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
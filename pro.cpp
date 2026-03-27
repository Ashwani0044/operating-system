#include <iostream>
#include <queue>
#include <functional>//functional is used for std::greater
#include <vector>
#include <thread>

using namespace std;

queue<function<void()>> tasks;

void executeTasks() {
    while(!tasks.empty()) {
        auto task = tasks.front();
        tasks.pop();

        task();
    }
}


int main() {

    //add tasks
    tasks.push([]() {
        cout<<"Task 1 executed..\n";
    });

    tasks.push([]() {
        cout<<"Task 2 executed..\n";
    });

    //execute tasks
    executeTasks();

    return 0;
}
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

void wait(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
    cout << "Waited for " << ms << " ms\n";
}

int main() {
    wait(1000); // waits for 1 second
    return 0;
}

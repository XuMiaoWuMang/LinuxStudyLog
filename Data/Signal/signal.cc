#include <iostream>
#include <signal.h>

void signal_handler(int signum) {
    if( signum == SIGINT) {
        std::cout << "Interrupt signal (" << signum << ") received.\n"; 
    }
}
int main() {
    signal(20, signal_handler); 
    while(true) {
        std::cout << "Hello World!" << std::endl;
        sleep(1);
    }
}
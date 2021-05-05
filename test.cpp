#include <iostream>
#include <pthread.h>

/*
arm-linux-gnueabihf-g++-8 test.cpp -o build/test -lpthread
scp build/test root@192.168.1.111:/root/targets/
*/

int main() {

    std::cout << "Hello from test vscode and ssh" << std::endl;

    return 0;
}
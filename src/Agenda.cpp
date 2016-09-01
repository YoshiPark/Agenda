#include "AgendaUI.hpp"
#include <signal.h>
#include <iostream>
AgendaUI aui;
void my_handler(int sig) {
	aui.quitAgenda();
	std::cout << "sync!" << std::endl;
	exit(0);
}
int main() {
	signal(SIGINT,my_handler);
	// if (flag == 1) raise(SIGINT);
    aui.OperationLoop();
    
    return 0;
}

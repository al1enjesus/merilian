#include <iostream>
#include <unistd.h>
#include <signal.h>


int main() {
    setenv("PROC_TO_KILL", "firefox,telegram", true);

    std::cout << "----------" << std::endl;


    int pid = 14893;
    if (kill(pid, 0) == -1) {
        std::cout << "Process with pid " << pid << " is not running." << std::endl;
    } else {
        std::cout << "Process with pid " << pid << " is running." << std::endl;
    }
    std::cout << "Run with --id " + std::to_string(pid) << std::endl;
    char cmd_for_pid[256] = "";
    snprintf(cmd_for_pid, sizeof(cmd_for_pid), "./killer --id %d", pid);
    system(cmd_for_pid);    


    std::cout << "----------" << std::endl;


    std::string name_string = "explorer";
    std::cout << "Run with --name " + name_string << std::endl;
    system(std::string(std::string("./killer --name ") + name_string).c_str());
    std::cout << "----------" << std::endl;
    

    unsetenv("PROC_TO_KILL");
}
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <dirent.h>
#include <signal.h>
#include <sstream>


void kill_processes_by_name(const std::string& name) {
    DIR* dir = opendir("/proc");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            int pid = atoi(entry->d_name);
 
            char cmdline[256];
            snprintf(cmdline, sizeof(cmdline), "/proc/%d/cmdline", pid);
            FILE* cmdline_file = fopen(cmdline, "r");

            if (cmdline_file) {
                char cmdline_buf[256];
                size_t len = fread(cmdline_buf, 1, sizeof(cmdline_buf), cmdline_file);
                fclose(cmdline_file);

                std::string cmdline = std::string(cmdline_buf);

                if (cmdline.find(name) != std::string::npos) {
                    std::cout << "Killing processes with name: " << cmdline << std::endl;
                    std::cout << "Killing process with pid: " << pid << std::endl;
                    kill(pid, SIGKILL);
                }
            }
            
        }
        closedir(dir);
    } else {
        perror("opendir");
    }
}


int main(int argc, char* argv[]) {

    std::string id, name;
    bool id_flag = false, name_flag = false;


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--id") == 0) {
            if (i + 1 < argc) {
                id = std::string(argv[i + 1]);
                id_flag = true;
            }
        } else if (strcmp(argv[i], "--name") == 0) {
            if (i + 1 < argc) {
                name = std::string(argv[i + 1]);
                name_flag = true;
            }
        }
    }


    if (id_flag) {
        int pid = std::stoi(id);
        std::cout << "Killing process with ID: " << pid << std::endl;
        kill(pid, SIGKILL);
    } else if (name_flag) {
        std::cout << "Killing processes with name: " << name << std::endl;
        kill_processes_by_name(name);
    }

    
    const char* proc_to_kill = getenv("PROC_TO_KILL");

    if (proc_to_kill != NULL) {
        std::cout << "Processes in PROC_TO_KILL: " << proc_to_kill << std::endl;

        std::stringstream ss(proc_to_kill);
        std::string proc_name;
        
        while (std::getline(ss, proc_name, ',')) {
            kill_processes_by_name(proc_name);
        }
    }

    return 0;
}

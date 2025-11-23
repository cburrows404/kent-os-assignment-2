#include <iostream>
#include <fstream>
#include <vector>
const int proc = 5; // # processes
const int reso = 3; // # resources
void need(int n[proc][reso], int m[proc][reso], int a[proc][reso]) {  // func for need calculation
  // n = need, m = maximum, a = alloted
    for (int i = 0; i < proc; i++) { //loops through processes
        for (int j = 0; j < reso; j++) { //loops through resources
            n[i][j] = m[i][j] - a[i][j];
        }
    }
}
bool isSafe(int procs[], int av[], int m[][reso], int al[][reso]) { // func for safe state calculation
    //av=available, m=maximum, al=allocated
    int needNum[proc][reso];
    need(needNum, m, al);
    bool isFinish[proc] = {false};  
    int safeSeq[proc];           // safe seq storage 
    int work[reso];              // num of working resources
    // sync working and available resources
    for (int i = 0; i < reso; i++) {
        work[i] = av[i];
    }
    int count = 0; // # procs in safe seq
    bool found = false;
    while (count < proc) {
        found=false;
        for (int p = 0; p < proc; p++) {
            // check if procs can be alloc resources
            if (!isFinish[p]) {
                int j;
                for (j = 0; j < reso; j++) {
                    if (needNum[p][j] > work[j]) {
                        break;
                    }
                }
                // if all the resources can be alloc, alloc resources to procs
                if (j == reso) {
                    for (int k = 0; k < reso; k++) {
                        work[k] += al[p][k];
                    }
                    safeSeq[count++] = p; // and add proc to safe seq
                    isFinish[p] = true;
                    found = true;
                }
            }
        }
        // if is no procs could be alloc resources, notify of unsafe state
        if (!found) {
            std::cout << "System is not in a safe state." << std::endl;
            return false;
        }
    }
    // print safe seq
    std::cout << "system in safe state.\n"<<std::endl;
    std::cout<<"safe sequence is: ";
    for (int i = 0; i < proc; i++) {
        std::cout << safeSeq[i] << (i < proc - 1 ? " -> " : "");
    }
    std::cout << std::endl;
    return true;
}
int main() {
    std::ifstream input("input-file-1.txt");
    if (!input) {
        std::cerr << "error opening input file" << std::endl;
        return 1;
    }
    int numProcesses[proc];
    for (int i = 0; i < proc; i++) {
        numProcesses[i] = i; 
    }
    int av[reso];       // # available resources
    int m[proc][reso];      // max demand
    int al[proc][reso];    // # resources allocated to each process
    for (int i = 0; i < reso; i++) { // read available resources
        input >> av[i];
    }
    for (int i = 0; i < proc; i++) { // read alloc 
        for (int j = 0; j < reso; j++) {
            input>> al[i][j];
        }
    }
    for (int i = 0; i < proc; i++) { // read max demand
        for (int j = 0; j < reso; j++) {
            input >> m[i][j];
        }
    }
    input.close();
    isSafe(numProcesses, av, m, al); // safe state check
    return 0;
}

#include <iostream>
#include <vector>
#include <thread>
#include <utility>
#include <mutex>
#include <chrono>
#include <atomic>

std::vector<std::thread> bunch_of_threads;
std::mutex mtx;
int number_of_threads = std::thread::hardware_concurrency();
std::atomic<int> actual_threads(0);

void print_tab(int* tab, int n){
        for (int i=0; i< n; i++){
        std::cout<< tab[i] << ", ";
    }
    std::cout<<"\n";
}

void quick_sort(int* tab, int l, int r){

    if (l >= r){   
        return;
    }
    std::lock_guard<std::mutex> lg(mtx);

    std::atomic<int> i(l - 1);
    int pivot = tab[r];

    for (int j = l; j <= r-1; j++){
        if (tab[j] <= pivot){
            i++;
            std::swap(tab[i], tab[j]);
        }
    }

    int p = i+1;
    std::swap(tab[i+1], tab[r]);
    
    if (actual_threads <= number_of_threads){
        actual_threads++;
        bunch_of_threads.emplace_back(std::thread(quick_sort, tab, l, p-1));
    }
    else{
        quick_sort(tab, l, p-1);
    }
    if (actual_threads <= number_of_threads){
        actual_threads++;
        bunch_of_threads.emplace_back([&](){quick_sort(tab, p+1, r);});
    }
    else{
        quick_sort(tab, p+1, r);
    }
}

int main(){

    int n = 11;
    int* tab = new int[n];
    tab[0] = 3;
    tab[1] = 12;
    tab[2] = 45;
    tab[3] = 10;
    tab[4] = 0;
    tab[5] = 8;
    tab[6] = 3;
    tab[7] = 2;
    tab[8] = 5;
    tab[9] = 1;
    tab[10] = 7;

    int number_of_threads = std::thread::hardware_concurrency();
    std::cout<< "Number of max threads is: " << number_of_threads << "\n\n";

    std::cout<< "\n";
    std::cout<< "Before sorting: ";
    print_tab(tab, n);
    std::cout<< "\n";
    
    quick_sort(tab, 0, n-1); 

    std::cout<< "After sortng:   ";
    print_tab(tab, n);
    std::cout<<"\n";

    for (auto&& thread : bunch_of_threads){
        thread.detach();
    }

    return 0;
}
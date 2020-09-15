#include <iostream>
#include <vector>
#include <thread>
#include <utility>
#include <chrono>
#include <future>

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

    int i = l - 1;
    int pivot = tab[r];

    for (int j = l; j <= r-1; j++){
        if (tab[j] <= pivot){
            i++;
            std::swap(tab[i], tab[j]);
        }
    }

    int p = i+1;
    std::swap(tab[i+1], tab[r]);
    
    quick_sort(tab, l, p-1);
    quick_sort(tab, p+1, r);
}

int main(){

    int n = 100;
    int* tab = new int[n];
    /*
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
    */
    for (int i = 0; i < n; i++){
        tab[i] = n-i;
    }

    std::cout<< "\n";
    std::cout<< "Before sorting: ";
    print_tab(tab, n);
    std::cout<< "\n";

    auto start = std::chrono::high_resolution_clock::now();

    int i =  - 1;
    int pivot = tab[n-1];

    for (int j = 0; j <= n-2; j++){
        if (tab[j] <= pivot){
            i++;
            std::swap(tab[i], tab[j]);
        }
    }

    int p = i+1;
    std::swap(tab[i+1], tab[n-1]);

    std::async(std::launch::async, quick_sort, tab, 0, p-1);
    std::async(std::launch::async, quick_sort, tab, p+1, n-1);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() << "\n";

    std::cout<< "After sortng:   ";
    print_tab(tab, n);
    std::cout<<"\n";

    delete[] tab;
    return 0;
}

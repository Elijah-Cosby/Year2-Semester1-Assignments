// CODE: include necessary library(s)
// you have to write all the functions and algorithms from scratch,
// You will submit this file, mySort.c holds the actual implementation of sorting functions
#include <stdio.h>

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}

// CODE: implement the algorithms for Insertion Sort, Merge Sort, Heap Sort, Counting Sort

// This second function is for the Insortion Sort Algorithm
//This algorithm is what many of us instinctively use to say sort our hand of cards.
//It basically works by taking each value and inserting it into its correct place relative to the values it's already placed in their correct order.
void  insertionSort(int arr[], int n) {
    for (int i=1; i<n; i++){
        int curElement = arr[i];
        int j = i-1;

        while (j>=0 && arr[j]>curElement){
            arr[j+1]= arr[j];
            j = j-1;
        }
        arr[j+1] = curElement;
    }
}
// This third function is for the Merge Sort Algorithm
// This algorithm works recursively by breaking the array in half until each array is sorted (only one element).
//From there, it then combines these sorted 'half-arrays' back together in the proper order until we finally have the starting array in the proper order.
void mergeSort(int arr[], int l, int r){
    if (l<r){
        int m = (l+r)/2; //Finds the mid point
        //This is recursive and will keep breaking the array down until sorted (single elements)
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r);

        //This now makes the arrays for each half of the array
        int nl = m-l+1;
        int nr = r-m;
        int L[nl];
        int R[nr];
        for  (int i=0; i<nl; i++){
            L[i] = arr[l+i];
        }
        for  (int j=0; j<nr; j++){
            R[j] = arr[m+1+j];
        }

        //This now merges the arrays back into a sorted array
        int i =0;
        int j =0;
        int k = l;
        while (i<nl && j<nr){
            if (L[i] <= R[j]){
                arr[k] = L[i];
                i++;
            } else{
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        //These are now to deal with the leftover in one side once one of the lists run out
        while (i<nl){
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j<nr){
            arr[k] = R[j];
            j++;
            k++;
        }
    }
}

// The fourth function is for the heap sort algorithm
void heapSort(int arr[], int n){
    //First we start by building the max heap
    for (int i=n/2-1; i>=0; i--){
        int biggest = i;
        int left = 2*i+1;
        int right = 2*i+2;

        if (left<n && arr[left]>arr[biggest]){
            biggest = left;
        }
        if (right<n && arr[right]>arr[biggest]){
            biggest = right;
        }

        //Now if the largest isn't the root, we swap them
        if (biggest!=i){
            int temp = arr[i];
            arr[i] = arr[biggest];
            arr[biggest] = temp;

        //We now heapify the subtree that was affected by the swap
            int k = biggest;
            while (k<n/2){
                int biggest = k;
                int left = 2*k+1;
                int right = 2*k+2;

                if (left<n && arr[left]>arr[biggest]){
                    biggest = left;
                }
                if (right<n && arr[right]>arr[biggest]){
                    biggest = right;
                }
                if (biggest==k){
                    break;
                } else{
                    int temp = arr[k];
                    arr[k] = arr[biggest];
                    arr[biggest] = temp;
                    k = biggest;
                }
            }
        }
    }
    // Now we start extracting the elements from the heap
    for  (int i=n-1; i>=0; i--){
        // We move the largest element to the end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // We're now going to heapify the smaller heap
        int k = 0;
        while (k<i/2){
            int biggest = k;
            int left = 2*k+1;
            int right = 2*k+2;

            if (left<i && arr[left]>arr[biggest]){
                biggest = left;
            }
            if (right<i && arr[right]>arr[biggest]){
                biggest = right;
            }
            if (biggest==k){
                break;
            } else{
                int temp = arr[k];
                arr[k] = arr[biggest];
                arr[biggest] = temp;
                k = biggest;
            }
        }
    }
}

void countingSort(int arr[], int n){
    //Find min and max values to get range
    int max = arr[0];
    int min = arr[0];
    for (int i=1; i<n; i++){
        if (arr[i]>max){
            max = arr[i];
        }
        if (arr[i]<min){
            min =  arr[i];
        }
    }
    int range = max-min+1;
    int count[range];
    for (int i=0; i<range; i++){
        count[i] = 0;
    }
    //Shift values to account for negatives and count how many times each number shows up
    for (int i = 0; i<n; i++){
        count[arr[i]-min]++;
    }
    for (int i=1; i<range; i++){
        count[i] += count[i-1];
    }

    //Finally making the output array
    int  output[n];
    for (int i=n-1; i>=0; i--){
        output[count[arr[i]-min]-1] = arr[i];
        count[arr[i]-min]--;
    }

    //Finally copying the sorted output back into the original array
    for (int i=0; i<n; i++){
        arr[i] = output[i];
    }
}

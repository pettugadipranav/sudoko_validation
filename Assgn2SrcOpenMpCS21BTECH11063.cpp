// #include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <cmath>
#include <chrono>
#include <time.h>
#include <omp.h>
using namespace std::chrono;
using namespace std;
int *rowed;
int *coled;
int *grided;
int rc = 0, cc = 0, gc = 0;

int n, k, q, r;
int **sudoko;
int *result_array;

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
int division(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {

        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void sort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = division(arr, low, high);
        sort(arr, low, pi - 1);
        sort(arr, pi + 1, high);
    }
}

int row_checker(int x)
{
    rc = rc + 1;
    int l = 1;
    int *arr = (int *)calloc(sizeof(int), n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = sudoko[x - 1][i];
    }
    sort(arr, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == i)
        {
            l = 0;
            break;
        }
    }
    // rowed[x-1]=l;
    result_array[x - 1] = l;
    return l;
}

int col_checker(int x)
{
    cc = cc + 1;
    int l = 1;
    x = x - n;
    int *arr = (int *)calloc(sizeof(int), n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = sudoko[i][x - 1];
    }
    sort(arr, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == i)
        {
            l = 0;
            break;
        }
    }
    // coled[x-1]=l;
    x = x + n;
    result_array[x - 1] = l;
    return l;
}

int grid_checker(int x)
{
    gc = gc + 1;
    int l = 1;
    x = x - (2 * n);
    x = x - 1;
    int a = sqrt(n);
    int row = a * (x / a);
    int col = a * (x % a);
    int *arr = (int *)calloc(sizeof(int), n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = 0;
    }
    for (int i = row; i < (row + 3); i++)
    {
        for (int j = col; j < (col + 3); j++)
        {
            arr[sudoko[i][j] - 1] = sudoko[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == 0)
        {
            l = 0;
            break;
        }
    }

    // grided[x]=l;
    x = x + (2 * n);
    result_array[x] = l;
    return l;
}

void func1(int ti)
{
    // int i=*((int *)ti);
    int i = ti;
    int q1 = q;
    if (i < r)
    {
        q1 = q1 + 1;
    }

    for (int j = 0; j < q1; j++)
    {
        int x = (k * j) + (i + 1);
        if (x >= 1 && x <= n)
        {
            row_checker(x);
        }
        else if (x >= (n + 1) && x <= (2 * n))
        {
            col_checker(x);
        }
        else if (x >= ((2 * n) + 1) && x <= (3 * n))
        {
            grid_checker(x);
        }
    }
}

int main()
{
    int val = 1;
    FILE *inputf = fopen("input.txt", "r");
    fscanf(inputf, "%d %d", &k, &n);
    q = ((3 * n) / k);
    r = ((3 * n) % k);

    result_array = (int *)calloc(sizeof(int), (3 * n));
    for (int m = 0; m < (3 * n); m++)
    {
        result_array[m] = -1;
    }

    sudoko = (int **)calloc(sizeof(int *), n);
    for (int i = 0; i < n; i++)
    {
        sudoko[i] = (int *)calloc(sizeof(int), n);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(inputf, "%d ", &sudoko[i][j]);
        }
    }

    auto start_time = high_resolution_clock::now(); // starting point

    int th_id;
#pragma omp parallel num_threads(k)
    {
        th_id = omp_get_thread_num();
        func1(th_id);
    }

    auto end_time = high_resolution_clock::now(); // ending point
    auto timetaken = duration_cast<microseconds>(end_time - start_time);

    FILE *fptr = fopen("Outmain2.txt", "w"); // opening the file to write the output
    for (int i = 0; i < k; i++)
    {
        int q1 = q;
        if (i < r)
        {
            q1 = q1 + 1;
        }

        for (int j = 0; j < q1; j++)
        {
            fprintf(fptr, "Thread %d checks ", (i + 1));
            int x = (k * j) + (i + 1);
            if (x >= 1 && x <= n)
            {
                fprintf(fptr, "row %d and is ", x);
                if (result_array[x - 1] == 1)
                {
                    fprintf(fptr, "valid");
                }
                else
                {
                    val = 0;
                    fprintf(fptr, "invalid");
                }
            }
            else if (x >= (n + 1) && x <= (2 * n))
            {
                fprintf(fptr, "coloumn %d and is ", (x - n));
                if (result_array[x - 1] == 1)
                {
                    fprintf(fptr, "valid");
                }
                else
                {
                    val = 0;
                    fprintf(fptr, "invalid");
                }
            }
            else if (x >= ((2 * n) + 1) && x <= (3 * n))
            {
                fprintf(fptr, "grid %d and is ", (x - (2 * n)));
                if (result_array[x - 1] == 1)
                {
                    fprintf(fptr, "valid");
                }
                else
                {
                    val = 0;
                    fprintf(fptr, "invalid");
                }
            }

            fprintf(fptr, ".\n");
        }
    }

    if (val == 0)
        fprintf(fptr, "sudoko is invalid\n");
    else
        fprintf(fptr, "sudoko is valid\n");

    fprintf(fptr, "The total time taken is %ld microseconds.\n", timetaken.count());
    fclose(fptr);

    return 0;
}

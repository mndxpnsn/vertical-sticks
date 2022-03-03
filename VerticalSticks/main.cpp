//
//  main.cpp
//  VerticalSticks
//
//  Created by mndx on 01/03/2022.
//  Three approaches to solving the "Vertical Sticks"
//  problem on HackerRank. Only approach 3 has an adequate
//  time complexity.
//

#include <chrono>
#include <iostream>
#include <vector>

void outer(std::vector<int> & x, std::vector<int> & y, int ** out) {
    int nx = (int) x.size();
    int ny = (int) y.size();
    
    for(int i = 0; i < nx; ++i) {
        for(int j = 0; j < ny; ++j) {
            if(x[i] <= y[j]) {
                out[i][j] = true;
            }
            else {
                out[i][j] = false;
            }
        }
    }
}

int ** create_outer(int n) {
    int ** res = new int * [n];
    
    for(int i = 0; i < n; ++i)
        res[i] = new int[n];
    
    return res;
}

void delete_outer(int ** out, int n) {
    
    for(int i = 0; i < n; ++i) {
        delete [] out[i];
    }
    
    delete [] out;
}

void sum_rows(int ** outer_arg, int n, int * res) {
    
    for(int i = 0; i < n; ++i) {
        res[i] = 1;
        for(int j = 0; j < n; ++j) {
            res[i] = res[i] + outer_arg[i][j];
        }
    }
}

float sum(int * arr, int n) {
    float res = 0.0;
    
    for(int i = 0; i < n; ++i)
        res = res + 1.0 / arr[i];
    
    return res;
}

double fac(int n) {
    double val = 1.0;
    
    if(n == 0)
        return val;
    
    if(n > 0)
        val = n * fac(n - 1);
    
    return val;
}

int max(int a, int b) {
    int res = 0;
    
    if(a > b) { res = a; }
    else { res = b; }
    
    return res;
}

double V(std::vector<int> & y, std::vector<int> g) {
    double res = 1.0;
    
    int n = (int) y.size();
    
    for(int i = 1; i < n; ++i) {
        int j = i - 1;
        bool hit = false;
        double res_loc = 1.0;
        while(j >= 0 && !hit) {
            if(y[g[j]] < y[g[i]]) {
                res_loc = res_loc + 1.0;
                --j;
            }
            else if(y[g[j]] >= y[g[i]]) {
                hit = true;
            }
        }
        res = res + res_loc;
    }
    
    return res;
}

double V2(std::vector<int> & y) {
    double res = 1.0;
    
    int n = (int) y.size();
    
    int max_val = y[0];
    
    for(int i = 1; i < n; ++i) {
        int j = i - 1;
        bool hit = false;
        double res_loc = 1.0;
        if(y[i] > max_val) {
            res_loc = i + 1;
            max_val = max(max_val, y[i]);
        }
        else {
            while(j >= 0 && !hit) {
                if(y[j] < y[i]) {
                    res_loc = res_loc + 1.0;
                    --j;
                }
                else if(y[j] >= y[i]) {
                    hit = true;
                }
            }
        }
        res = res + res_loc;
    }
    
    return res;
}

void solve_fcn(std::vector<int> & y, std::vector<int> g, double & expect) {
    int n = (int) y.size();
    int m = (int) g.size();
    
    if(m < n) {
        for(int i = 0; i < n; ++i) {
            bool is_used = false;
            for(auto g_elem : g) {
                if(g_elem == i) {
                    is_used = true;
                }
            }
            if(!is_used) {
                std::vector<int> vec_loc = g;
                vec_loc.push_back(i);
                solve_fcn(y, vec_loc, expect);
            }
        }
    }
    
    if(m == n) {
        expect = expect + V(y, g);
    }
}

double compute_expectation(int n, std::vector<int> & A) {
    double dist = 0;
    std::vector<int> c;
    
    for(int i = 0; i < n; ++i)
         c.push_back(0);
    
    // Compute distances of first permutation
    dist = dist + V2(A);
    
    // Compute distances of remaining permutations
    int i = 0;
    while(i < n) {
        if (c[i] < i) {
            // Get new permutation
            if (i % 2 == 0) {
                int dummy = A[0];
                A[0] = A[i];
                A[i] = dummy;
            }
            else {
                int dummy = A[c[i]];
                A[c[i]] = A[i];
                A[i] = dummy;
            }
            // Compute distances of current permutation
            dist = dist + V2(A);
            c[i] += 1;
            i = 0;
        }
        else {
            c[i] = 0;
            i += 1;
        }
    }
    
    return dist;
}

double solve(std::vector<int> & y) {
    double expect = 0.0;
    
    int n = (int) y.size();
    
    expect = compute_expectation(n, y);
    
    expect = expect / fac(n);
    
    return expect;
}

double solve_rec(std::vector<int> y) {
    double expect = 0.0;
    
    std::vector<int> g;
    
    int n = (int) y.size();
    
    solve_fcn(y, g, expect);
    
    expect = expect / fac(n);
    
    return expect;
}

float solve3(std::vector<int> & y) {
    int n = (int) y.size();
    int ** out_val = create_outer(n);
    int * sum_val = new int[n];
    
    outer(y, y, out_val);
    
    sum_rows(out_val, n, sum_val);
    
    float sum_loc = sum(sum_val, n);
    
    delete_outer(out_val, n);
    delete [] sum_val;
    
    return (n + 1) * sum_loc;
}

int main(int argc, const char * argv[]) {
    
    // Declarations
    std::vector<int> y;
    
    // Declare size input
    int size_arr = 9;
    
    // Populate y with random data
    for(int i = 0; i < size_arr; ++i)
        y.push_back(rand() % 100 + 1);
    
    // `Start timing computation using method 1
    auto start1 = std::chrono::steady_clock::now();
    
    // Compute expectation using method 1
    double expect1 = solve(y);
    
    // End timing computation using method 1
    auto end1 = std::chrono::steady_clock::now();
    
    // `Start timing computation using method 2
    auto start2 = std::chrono::steady_clock::now();
    
    // Compute expectation using method 2
    double expect2 = solve_rec(y);
    
    // End timing computation using method 2
    auto end2 = std::chrono::steady_clock::now();
    
    // `Start timing computation using method 3
    auto start3 = std::chrono::steady_clock::now();
    
    // Compute expectation using method 3
    double expect3 = solve3(y);
    
    // End timing computation using method 3
    auto end3 = std::chrono::steady_clock::now();
    
    // Compute running times
    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
    auto time3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3);
    
    // Print expectations and timing
    std::cout << "expectation computed using method 1: " << expect1 << std::endl;
    std::cout << "expectation computed using method 2: " << expect2 << std::endl;
    std::cout << "expectation computed using method 3: " << expect3 << std::endl;
    
    std::cout << "running time method 1: " << time1.count() << " (ms)" << std::endl;
    std::cout << "running time method 2: " << time2.count() << " (ms)" << std::endl;
    std::cout << "running time method 3: " << time3.count() << " (ms)" << std::endl;
    
    return 0;
}

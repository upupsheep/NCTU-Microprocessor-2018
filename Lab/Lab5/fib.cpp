#include <iostream>
using namespace std;

int fib(int N) {
    int f0 = 0;
    int fib = 0;
    int f1 = 1;
    if (N == 0)
        return 0;
    else if (N == 1)
        return 1;
    else {
        for (int i = 1; i < N; i++) {
            fib = f1 + f0;
            f0 = f1;
            f1 = fib;
        }
        return fib;
    }
}

int main() {
    cout << fib(40) << endl;
    return 0;
}
#pragma once

class Subject {
public:
    // example from presentation
    int f3(int a, int b) {
        return a + b;
    }

    // method with any quantity args
    int sum_4_args(int a, int b, int c, int d) {
        return a + b + c + d;
    }

    // method with zero args
    int f_zero() {
        return 42;
    }
};
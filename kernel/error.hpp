#pragma once

#include <array>
using namespace std;

class error {
    public:
        enum code {
            success
        };
        error(code c) : code_{c} {}
        const char * name() const {
            return array<const char *, 1>{"success"}[(int) code_];
        }
    private:
        code code_;
};

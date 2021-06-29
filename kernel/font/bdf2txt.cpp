#include <iostream>
#include <string>
using namespace std;

#define skip_lines(b, s) for (string b; getline(cin, b), b != s;)

int read(string s) {
    for (string t; cin >> t, t != s;)
        ;
    int n;
    cin >> n;
    return n;
}

int main() {
    for (int i {}, n {read("CHARS")}; i < n; ++i) {
        int e {read("ENCODING")};
        cout << "0x" << hex << e << " '" << (char) e << '\'' << endl;
        skip_lines (s, "BITMAP")
            ;
        skip_lines (s, "ENDCHAR") {
            for (char c: s)
                for (int j {8}, k {c - (isdigit(c) ? '0' : 'A' - 10)}; j; j >>= 1)
                    cout << (k & j ? '@' : '.');
            cout << endl;
        }
        cout << endl;
    }
}

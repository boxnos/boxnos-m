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
        skip_lines (s, "BITMAP")
            ;
        skip_lines (s, "ENDCHAR") {
            for (size_t j {}; j < size(s);j += 2) {
                int o {};
                for (int k {}; k < 2; ++k) {
                    char c = s[j + k];
                    o <<= 4;
                    o += c - (isdigit(c) ? '0' : 'A' - 10);
                }
                cout << (char) o;
                //cout << hex << (int) o << " ";
            }
        }
        // cout << endl;
    }
}

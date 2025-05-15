// Subroutine branching with BL, SUBS, TBNZ and CSET
// Return value is w0

int foo(int i) {
    if (i != 100) {
        return foo(i + 1);
    }
    return i;
}

int main() {
    return foo(1);
}
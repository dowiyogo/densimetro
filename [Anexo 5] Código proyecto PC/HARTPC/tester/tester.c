#include <unistd.h>

#include "tester.h"

int good_foo();
int bad1_foo();
int bad2_foo();
int badn_foo();

int main(int argc, char** argv){
    TESTER_NAME("Tester TEST");
    TESTER_DESCRIPTION("This test use trivial function to watch the correct working of the TESTER library.");
    TESTER_TEST("Test OK", good_foo);
    TESTER_TEST("Test BAD1", bad1_foo);
    TESTER_TEST("Test BAD2", bad2_foo);
    TESTER_TEST("Test BADN", badn_foo);
    TESTER_TEST("Test with absurd long name 123456789", bad1_foo);

    return 0;
}

int good_foo(){
    return 0;
}
int bad1_foo(){
    return 1;
}
int bad2_foo(){
    sleep(1);
    return 2;
}
int badn_foo(){
    return 'n';
}

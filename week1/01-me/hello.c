#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Store name in "name"
    string name = get_string("What's your name? ");
    // print "hello, 'name'"
    printf("hello, %s\n", name);
}

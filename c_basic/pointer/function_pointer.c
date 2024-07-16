#include <stdio.h>
int squaredSum(int a, int b, int (*fp)())
{
    int sqSum;
    sqSum = fp(a * a, b * b);
    return sqSum;
}
int mySum(int a, int b)
{
    return a + b;
}
int main()
{
    int a = 5, b = 4, sm;
    int (*fp)(int, int);            // define a function pointer
    fp = mySum;                     // assign function address to the function pointer,
    int res = squaredSum(a, b, fp); // call the function squaredSum 
    printf("Squared sum of the integers %d and %d is %d.\n", a, b, res);
}
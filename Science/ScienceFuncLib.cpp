#include "ScienceFuncLib.h"

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
	
	check(Value >= 0); //Checks condition and interrupts program if false
	return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}

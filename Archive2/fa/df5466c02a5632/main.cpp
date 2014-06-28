#include <iostream>
#include <limits>
#include <iomanip>

int main( int argc, char *argv[] )
{
    long double b = 3.14159;
	
	std::cout << std::setprecision(std::numeric_limits<long double>::digits10+1) << b << std::endl;
	return( 0 );
}

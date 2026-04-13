#include "angellib.h"
#include <iostream>

//g++ -fPIC -shared -o libangellib.so angellib.cpp
//g++ main.cpp -o myapp -L. -langellib 
// sudo cp libmylib.so /usr/local/lib/


void print_message(const char* msg) {
    std::cout << "angellib Message: " << msg << std::endl;
}
//Return 0 to superimpose the custom encryption algorithm, and return 1 to use the custom algorithm only.
//Other values > 1 indicate that the error is not used.
uint init(void)
{
  //std::cout << "angellib init " << std::endl;
  return -1; //0 1 -1
}

void buf_encrypt(u_char* buffer,__uint64_t offset,uint length)
{
	uint i;
	for( i = 0; i < length; i++ )
	{
		buffer[i] ^= 0x77;
	}
}

void buf_decrypt(u_char* buffer,__uint64_t offset,uint length)
{
	uint i;
	for( i = 0; i < length; i++ )
	{
		buffer[i] ^= 0x77;
	}
}
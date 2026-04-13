#ifndef _angellib_incl_
#define _angellib_incl_

#include <string>

#ifdef __cplusplus 
extern "C" {
#endif
uint init(void);
void buf_encrypt(u_char* buffer,__uint64_t offset,uint length);
void buf_decrypt(u_char* buffer,__uint64_t offset,uint length);
#ifdef __cplusplus 
}
#endif

#endif  //_angellib_incl_
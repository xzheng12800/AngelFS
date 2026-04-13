# AngelFS - an Encrypted Filesystem

## About

AngelFS provides an encrypted virtual filesystem. It runs in userspace,
using the [FUSE library](https://github.com/libfuse/libfuse) for the filesystem interface.

AngelFS encrypts individual files, by translating all requests for the virtual
AngelFS filesystem into the equivalent encrypted operations on the raw
filesystem.

## Status

AngelFS referring to the usage habits and encryption design styles of several well-known similar projects, the encryption algorithm and overall design structure are improved, which makes the encryption speed faster, the encrypted file layout easier to understand and the file migration more convenient.

## Unique Features

AngelFS has a few features still not found anywhere else (as of Dec 2026)
that may be interesting to you:

### Flexible encryption optional mode

You can selectively encrypt the file name or directory name, or keep the extension while encrypting the file name, and provide a Shared Library standard interface to allow your own encryption algorithm to be integrated into it.

### Works on top of network filesystems

AngelFS works on network file systems (NFS, CIFS...), Support encrypted mount of webdav network.

## Windows

## Getting started

### Create a new encrypted file system and mount the local directory

```bash
mkdir -p ~/encrypted
mkdir -p ~/mount
angelfs ~/encrypted ~/mount
# or: angelfs -p123456 ~/encrypted ~/mount
```
You can now read and write files under `~/mount`; they are stored encrypted under `~/encrypted`.
 When finished, unmount:

```bash
fusermount -u ~/mount   # Linux
# or: umount ~/mount    # macOS / FreeBSD
```

### Create a new unencrypted file system and mount the webdav directory.

```bash
angelfs --WebdavAddr https://webdav.name.cn/webdav --AccountName name ~/mount
# or: angelfs --WebdavAddr https://webdav.name.cn/webdav --AccountName name --AccountPwd 123 ~/mount
```

### Create a new encrypted file system and mount the webdav directory

```bash
angelfs -p0 --WebdavAddr https://webdav.name.cn/webdav --AccountName name ~/mount
# or: angelfs -p123456 --WebdavAddr https://webdav.name.cn/webdav --AccountName name --AccountPwd 123 ~/mount
```

You can now read and write files under `~/mount`; they are stored encrypted under `~/AngelCache` 
and automatically synchronize webdav file data.  
Note: the response time of reading and writing files is related to the network speed provided by the 
cloud disk manufacturer, and the written files have a process of delaying uploading to the cloud disk;  
When uploading large files or a large number of files, you can view them through the angelfsctl tool 
to ensure that all   files are uploaded;
When finished, Before unmounting, it is necessary to ensure that the file has no uploading task to avoid 
the loss of file writing operation.

```bash
angelfsctl showupload ~/mount
```
### Usage
Usage: angelfs [options] rootDir mountPoint [-- [FUSE Mount Options]]  
   or: angelfs -pipe  
-pipe:             
    Input string parameters "[options] rootDir mountPoint [-- [FUSE Mount Options]]",  
    hidden program startup parameters.  
Options:  
  -H                    show optional FUSE Mount Options  
  -s                    disable multithreaded operation  
  -f                    run in foreground (don't spawn daemon).  
                        Error messages will be sent to stderr  
                        instead of syslog.  
  -F=flag               Name encryption flag values option:  
                        file name encryption (0x20) default  
                        directory name encryption (0x40) default  
                        file name extension is reserved(0x100)  
  -t, --syslogtag=tag   set the syslog tag which will be used when messages are  
                        logged via syslog. By default syslog tag is <angelfs>  
  -v, --verbose         verbose: output angelfs debug messages  
  -i, --idle=MINUTES    Auto unmount after period of inactivity  
  --public              act as a typical multi-user filesystem  
                        (angelfs must be run as root)  
  -u, --unmount         unmounts specified mountPoint  
  -p=passwords          Passwords not less than 6 and not more than 16  
  -e, --extpass=path    Use external password file  
  --WebdavAddr=address  webdav URL address  
  --AccountName=name    webdav account name  
  --AccountPwd=pwd      webdav account passwords  
  -d, --fusedebug       Enables debugging within the FUSE library  
  -o=arg                arguments meant for fuse  
  --no-default-flags    don't use default fuse flags  
  --noattrcache         disable attr caching  

Usage: angelfsctl (mount dir)  
  -- displays information about the root directory or webdav address of the file system, or   
angelfsctl info (mount dir)  
  -- show information (Default command)  
angelfsctl showupload (mount dir)  
  -- Shows that webdav is uploading the file name  
angelfsctl --version  
  -- print version number and exit  

## Create a shared dynamic library of custom encryption and decryption algorithms
Generate libangellib.so and put it into the current working directory or system directory.
Example c++:
```angellib.h
#ifdef __cplusplus 
extern "C" {
#endif
  uint init(void);
  void buf_encrypt(u_char* buffer,__uint64_t offset,uint length);
  void buf_decrypt(u_char* buffer,__uint64_t offset,uint length);
#ifdef __cplusplus 
}
#endif
```
```angellib.cpp
//Return 0 to superimpose the custom encryption algorithm, and return 1 to use the custom algorithm only.
//Other values > 1 indicate that the error is not used.
uint init(void)
{
  return 0; 
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
~~~

## FAQ

### How to avoid password leakage in program startup parameters?  
As we all know, under linux, by default, you can directly view the program startup parameters. You can 
avoid this situation by manually entering the password without using the -p parameter or webdav only 
using the -p0 parameter, or   you can hide it all by directly using the -pipe parameter.  

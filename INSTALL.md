This document provides generic information for install AngelFS

To install:

    sudo dpkg -i angelfs.deb
    or: sudo apt install ./angelfs.deb
    or: tar -xzvf angelfs.tar.gz
        After decompression, install the following dependency library and you can use it.

Dependencies:  
    * FUSE library (libfuse3 or OSXFUSE on macOS)  
    * OpenSSL libraries(libssl3)  
    * curl libraries(libcurl4) 
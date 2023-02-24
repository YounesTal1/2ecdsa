# Two-Party ECDSA 

This is a C++ implementation of the protocol provided in the paper "Efficient Secure Two Party ECDSA" by Sermin Kocaman "sermin.cakin@metu.edu.tr", and Younes Talibi Alaoui "talibialaouiyounes@gmail.com".


### Requirements:
C++ compiler  
[secp256k1](https://github.com/bitcoin-core/secp256k1) (used for operations over the curve)  
[GMP](https://gmplib.org/) (used for inverting field elements)  
[OpenSSL](https://www.openssl.org/) (used for SHA256)  



### Usage:
1. Run `make`
2. Run `.\2ecdsa_dummy_communication [number of keys] [number of signatures per key]`

The output is the average time of key generation, offline signing, and online signing.


### Note:

As mentioned, the MtA implemented is a dummy one, where only one party takes the two inputs and generates an additive sharing of their product, then sends one of the shares to the other party. Also the runtimes do not include the communication cost.


//#include <sstream>
#include <openssl/sha.h>
//#include <iomanip>
#include "Sha256.h"
#include <string>

using namespace std;

unsigned char* Sha256_char(string msg) {
    unsigned char Hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, msg.c_str(), msg.size());
    SHA256_Final(Hash, &sha256);

    unsigned char* hash_array = new unsigned char[32];
    for (int i = 0; i < 32; i++) {
        hash_array[i] = Hash[i];
    }
    return hash_array;
}


#pragma once

#ifndef _Tools_h
#define _Tools_h


#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <secp256k1.h>
#include "Fpoint.h"
#include <iomanip>
#include <cstring>

#include <initializer_list>
#include "Curve.h" 

using namespace std;

inline unsigned char* concat_char(unsigned char *a, unsigned char *b, unsigned char *c) 
{

	unsigned char* result = NULL;
    result = new unsigned char[129];

    int i;
    for (i = 0; i < 65; i++) {
        result[i] = a[i];
    }
    for (i = 0; i < 32; i++) {
        result[i + 65] = b[i];
        result[i + 65 + 32] = c[i];
    }

	return result;
}

template <typename T>
string concat(T t) {
  stringstream ss;
  ss << t ;
  return ss.str();
}


template <typename T, typename... Args>
string concat(T t, Args... args) {
  stringstream ss;
  ss << t << "|" << concat(args...);
  return ss.str();
}


inline string To_str_proof(unsigned char* m)
{
	


	stringstream ss;	
  	for (int i= 0; i < 129; i++)
    	{
      		ss << hex << setw(2) << setfill('0') << (int) m[i];
    	}


	return ss.str();
}



inline string To_str(unsigned char* m)
{
	
	
	stringstream ss;	
  	for (int i= 0; i < 32; i++)
    	{
      		ss << hex << setw(2) << setfill('0') << (int) m[i];
    	}


	return ss.str();
}



inline unsigned char* To_char(secp256k1_pubkey *pubk)
{
    //unsigned char pubkey_str[65];

    unsigned char* pubkey_str = NULL;
    pubkey_str = new unsigned char[65];

    size_t pubkey_str_len = 65;

    secp256k1_ec_pubkey_serialize(ctx, pubkey_str, &pubkey_str_len, pubk, SECP256K1_EC_UNCOMPRESSED);

    return pubkey_str;
}

inline string Key_To_str(secp256k1_pubkey *pubk)
{
    //unsigned char pubkey_str[65]; 
    unsigned char* pubkey_str = new unsigned char[65];

    size_t pubkey_str_len = 65;

    secp256k1_ec_pubkey_serialize(ctx, pubkey_str, &pubkey_str_len, pubk, SECP256K1_EC_UNCOMPRESSED);

    stringstream ss;
    for (size_t i= 0; i < pubkey_str_len; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int) pubkey_str[i];
        }


    return ss.str();
}


inline void print_Fpoint(Fpoint a)
{
	stringstream ss;
	for (int i= 0; i < 32; i++)
    {
            ss << hex << setw(2) << setfill('0') << (int) a.m[i];
            //ss << Hash[i];
    }
    cout << ss.str() << endl;
}


inline unsigned char* moveZerosToStart(unsigned char* input, size_t count) 
{
	unsigned char* temp = NULL;
    temp = new unsigned char[32];
	
	int Zeros = 32 - count;
    for (int i = 0; i < 32 - Zeros; i++) {
        temp[i + Zeros] = input[i];
    }
    for (int i = 0; i < Zeros; i++) {
        temp[i] = 0;
    }

    return temp;
}

#endif

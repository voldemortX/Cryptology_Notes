# Fundamentals of cryptology 
Simple implementations of DES, RSA, MD5

***Codes are self-explanatory with detailed comments***

## 1. DES by C++
Implemented DES in various modes(*CFB, CBC, ECB*) based on C++ library **bitset**

Encryption&Decryption runs at a reasonable speed(*~640Kbps*)

## 2. RSA by Java
Implemented simple RSA based on Java library **BigInteger**

Takes ~25 seconds to generate a pair of private&public keys

## 3. MD5 by C++
Implemented a variation of MD5(*Results are somewhat inconsistent with popular implementations*) based on C++ library **bitset**

A popular implementation by bit-wise operations are included as **md5_std.cpp**

***All codes simply aim to build an understanding of basic cryptology, thus not recommended for practical use***

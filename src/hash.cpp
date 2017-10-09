/**
 * \file      hash.cpp
 * \brief     Provides a wrapper class for generating file hashes. 
 * \author    Alexander Herr
 * \date      2014-2016
 * \copyright GNU Public License v3 or higher. 
 */

#include "hash.hpp"

#include <sodium.h>
#include <string>
#include <sstream>
#include <iomanip>

#include <iostream>

Hash::Hash() :
        hash_(new unsigned char[F_GENERIC_HASH_LEN]),
        empty_(true) {}
Hash::Hash(const unsigned char hash_bytes[F_GENERIC_HASH_LEN]) :
        hash_(new unsigned char[F_GENERIC_HASH_LEN]),
        empty_(false) {
    std::memcpy(hash_, hash_bytes, F_GENERIC_HASH_LEN);
}
Hash::Hash(const std::string& string) :
        hash_(new unsigned char[F_GENERIC_HASH_LEN]),
        empty_(true) {
    makeHash(string);
}
Hash::~Hash() {}

/**
 * \fn Hash::makeHash
 *
 * Generates a cryptographic hash out of the supplied string and stores it in 
 * the hash_ member variable. Uses libsodiums generichash (Blake2) for hashes. 
 * This overwrites any previously stored hash. 
 * 
 * \param string
 */
void Hash::makeHash(const std::string& string) {
    unsigned char new_hash[F_GENERIC_HASH_LEN];
    crypto_generichash(
        new_hash, F_GENERIC_HASH_LEN,
        (unsigned char*)string.c_str(),
        string.length(),
        NULL, 0);
    std::memcpy(hash_, new_hash, F_GENERIC_HASH_LEN);
    empty_ = false;
}
const std::string Hash::getString() const {
  std::string return_value;
  if ( !empty_ ) {
    std::stringstream sstream;
    for( uint i = 0; i < F_GENERIC_HASH_LEN; ++i ) {
      sstream << std::hex << std::setfill('0') << std::setw(2) << (int)hash_[i];
    }
    return_value = sstream.str();
  } else {
    return_value = "";
  }
  return return_value;
}
const unsigned char* Hash::getBytes() const {
  return hash_;
}
bool Hash::empty() const { return empty_; }

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <jsoncpp/json/json.h>
#include <fstream>

#include "hash.hpp"

#include <sodium.h>

void transformPrecomputedHash( unsigned char* precomputed_hash_char, std::istringstream* isstream ) {
  unsigned int c;
  int ctr = 0;
  while( *isstream >> std::hex >> c ) {
    precomputed_hash_char[ctr] = c;
    ++ctr;
  }
}

BOOST_AUTO_TEST_CASE(hash_structors)
{
  // test vector
  std::string hash_string = "";
  unsigned char precomputed_hash_char[F_GENERIC_HASH_LEN];
  std::istringstream isstream("78 6a 02 f7 42 01 59 03 c6 c6 fd 85\
                               25 52 d2 72 91 2f 47 40 e1 58 47 61\
                               8a 86 e2 17 f7 1f 54 19 d2 5e 10 31\
                               af ee 58 53 13 89 64 44 93 4e b0 4b\
                               90 3a 68 5b 14 48 b7 55 d5 6f 70 1a\
                               fe 9b e2 ce");
  transformPrecomputedHash( precomputed_hash_char, &isstream );

  // 3 cases:
  // - uninitialized (no need to check)
  // - initialized w/ unsigned char
  Hash hash1 = Hash(precomputed_hash_char);
  BOOST_CHECK(memcmp(precomputed_hash_char, hash1.getBytes(), F_GENERIC_HASH_LEN) == 0);

  // - initialized w/ string
  Hash hash2 = Hash(hash_string);
  BOOST_CHECK(memcmp(precomputed_hash_char, hash2.getBytes(), F_GENERIC_HASH_LEN) == 0);
}
BOOST_AUTO_TEST_CASE(hash_makeHash)
{
  boost::filesystem::path test_vector_file_path;
  boost::filesystem::path test_vector_file_path1 = boost::filesystem::absolute(
    boost::filesystem::path("repos/blake2-kat.json")
  );
  boost::filesystem::path test_vector_file_path2 = boost::filesystem::absolute(
    boost::filesystem::path("../../repos/blake2-kat.json")
  );
  if( !boost::filesystem::exists(test_vector_file_path1) ) {
      if( !boost::filesystem::exists(test_vector_file_path2) ) {
      std::cout << "ERROR: official Blake2b test vectors required" << std::endl
                << "You can download them and put them into the /repos directory of the flocksy project" << std::endl
                << "URL: https://raw.githubusercontent.com/BLAKE2/BLAKE2/master/testvectors/blake2-kat.json" << std::endl;
    } else {
      test_vector_file_path = test_vector_file_path2;
    }
  } else {
    test_vector_file_path = test_vector_file_path1;
  }
  BOOST_REQUIRE(boost::filesystem::exists(test_vector_file_path));

  std::ifstream test_vector_file(test_vector_file_path.native(), std::ifstream::binary);
  Json::Value test_vectors;
  std::string test_vec_hash;
  std::string test_vec_input_raw, test_vec_input;
  Hash hash;
  test_vector_file >> test_vectors;
  for( Json::Value::ArrayIndex i = 0; i != test_vectors.size(); ++i ) {
    if ( test_vectors[i]["hash"] == "blake2b" && test_vectors[i]["key"] == "" ) {
      test_vec_hash = test_vectors[i].get("out","UTF-8").asString();
      test_vec_input_raw = test_vectors[i].get("in","UTF-8").asString();
      std::string hex;
      std::stringstream test_vec_input_sstream;
      for( uint j = 0; j < test_vec_input_raw.size()/2; ++j ) {
        char hex = static_cast<char>( strtoul( test_vec_input_raw.substr(j*2,2).c_str(), NULL, 16 ) );
        test_vec_input_sstream << hex;
      }
      test_vec_input = test_vec_input_sstream.str();
      hash = Hash(test_vec_input);
      BOOST_CHECK_EQUAL( hash.getString(), test_vec_hash );
    }
  }
}
BOOST_AUTO_TEST_CASE(hash_compare)
{
  Hash hash1 = Hash("test1"); // = c689bf21986252dab8c946042cd73c44995a205da7b8c0816c56ee33894acbace61f27ed94d9ffc2a0d3bee7539565aca834b220af95cc5abb2ceb90946606fe
  Hash hash2 = Hash("test2"); // = e1b1bfe59054380ac6eb014388b2db3a03d054770ededd9ee148c8b29aa272bbd079344bb40a92d0a754cd925f4beb48c9fd66a0e90b0d341b6fe3bbb4893246
                              // therefore hash1 < hash2
  BOOST_CHECK(!(hash1 == hash2));
  BOOST_CHECK(  hash1 != hash2);
  BOOST_CHECK( (hash1 <  hash2));
  BOOST_CHECK( (hash1 <= hash2));
  BOOST_CHECK(!(hash1 >  hash2));
  BOOST_CHECK(!(hash1 >= hash2));
}
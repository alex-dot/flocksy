#include <boost/test/unit_test.hpp>
#include "directory.hpp"
#include <string>

// fillDirectory

BOOST_AUTO_TEST_CASE(directory_constructors)
{
  Directory* dir;
  boost::filesystem::path p = boost::filesystem::current_path().string() + "/../../test/testdir";
  const int noe = 3;

  // uninitialized, calling fillDirectory()
  dir = new Directory();
  std::vector<boost::filesystem::directory_entry> dirs;
  dir->fillDirectory(p, dirs);

  BOOST_CHECK_EQUAL( dir->getPath(), p.filename() );
  BOOST_CHECK_EQUAL( dir->getAbsolutePath(), p.native() );
  BOOST_CHECK_EQUAL( dir->getNumberOfEntries(), noe );
  delete dir;

  // initialized
  dir = new Directory(p);
  BOOST_CHECK_EQUAL( dir->getPath(), p.filename() );
  BOOST_CHECK_EQUAL( dir->getAbsolutePath(), p.native() );
  BOOST_CHECK_EQUAL( dir->getNumberOfEntries(), noe );
  delete dir;
}
BOOST_AUTO_TEST_CASE(directory_gethashtree)
{
  Directory* dir;
  boost::filesystem::path p = boost::filesystem::current_path().string() + "/../../test/testdir";
  dir = new Directory(p);
  // Value generated with pyblake2
  std::string hash_string = "5ec455de0fcb893374c1b8b4b6569734ab439dd93cefa80f243e1d8d93131c4daf780d5b3f6cde9c72411cf9cfafd85813834957b16a7da1bd894efd25dc90c1";
  BOOST_CHECK_EQUAL( dir->getHashTree()->getTopHash()->getString(), hash_string );
  hash_string += p.filename().c_str();
  std::shared_ptr<Hash> testhash(new Hash(hash_string));
  BOOST_CHECK_EQUAL( dir->getDirectoryHash()->getString(), testhash->getString() );
}
BOOST_AUTO_TEST_CASE(directory_compare)
{
  boost::filesystem::path p1 = boost::filesystem::current_path().string() + "/../../test/testdir/testdir";
  boost::filesystem::path p2 = boost::filesystem::current_path().string() + "/../../test/testdir/testdir2";
  std::vector< std::shared_ptr<Hash> > hashes;
  Directory dir1(p1);

  // compare same/unchanged dir
  Directory dir2(p1);
  BOOST_CHECK( !dir1.checkDirectoryChange(dir2) );
  dir1.getChangedEntryHashes(hashes, dir2);
  BOOST_CHECK_EQUAL( 0, hashes.size() );

  // compare different dirs (one item extra)
  Directory dir3(p2);
  BOOST_CHECK(  dir1.checkDirectoryChange(dir3) );
  dir1.getChangedEntryHashes(hashes, dir3);
  BOOST_CHECK_EQUAL( 1, hashes.size() );
}
#include <boost/test/unit_test.hpp>
#include "hash_tree.hpp"
#include <algorithm>

BOOST_AUTO_TEST_CASE(hash_tree_constructors)
{
  std::shared_ptr<Hash> hash(new Hash("test"));
  std::vector< std::shared_ptr<Hash> > hashes = { hash };
  HashTree* ht;

  // 3 cases::
  // unintialized, calling makeHashTree()
  ht = new HashTree();
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL( ht->getHashes()->back(), const_cast<const std::vector< std::shared_ptr<Hash> >*>(&hashes)->back() );
  delete ht;
  // initialized with HashTree-vector
  ht = new HashTree(hashes);
  BOOST_CHECK_EQUAL( ht->getHashes()->back(), const_cast<const std::vector< std::shared_ptr<Hash> >*>(&hashes)->back() );
  delete ht;
  // initialized with Hashes-vector, calling makeHashTreeFromSelf()
  ht = new HashTree(hashes);
  ht->makeHashTreeFromSelf();
  BOOST_CHECK_EQUAL( ht->getHashes()->back(), const_cast<const std::vector< std::shared_ptr<Hash> >*>(&hashes)->back() );
  delete ht;
}
BOOST_AUTO_TEST_CASE(hash_tree_empty)
{
  HashTree* ht = new HashTree();
  BOOST_CHECK( ht->empty() );

  std::vector< std::shared_ptr<Hash> > hashes = {};
  ht->makeHashTree(hashes);
  BOOST_CHECK( ht->empty() );
  BOOST_CHECK( ht->getTopHash()->empty() );
  delete ht;
}
BOOST_AUTO_TEST_CASE(hash_tree_size_compare)
{
  std::shared_ptr<Hash> hash01(new Hash("test01"));
  std::shared_ptr<Hash> hash02(new Hash("test02"));
  std::shared_ptr<Hash> hash03(new Hash("test03"));
  std::shared_ptr<Hash> hash04(new Hash("test04"));
  std::shared_ptr<Hash> hash05(new Hash("test05"));
  std::shared_ptr<Hash> hash06(new Hash("test06"));
  std::shared_ptr<Hash> hash07(new Hash("test07"));
  std::shared_ptr<Hash> hash08(new Hash("test08"));
  std::shared_ptr<Hash> hash09(new Hash("test09"));
  std::shared_ptr<Hash> hash10(new Hash("test10"));
  std::shared_ptr<Hash> hash11(new Hash("test11"));
  std::shared_ptr<Hash> hash12(new Hash("test12"));
  std::shared_ptr<Hash> hash13(new Hash("test13"));
  std::shared_ptr<Hash> hash14(new Hash("test14"));
  std::shared_ptr<Hash> hash15(new Hash("test15"));
  std::shared_ptr<Hash> hash16(new Hash("test16"));
  std::shared_ptr<Hash> hash17(new Hash("test17"));
  std::vector< std::shared_ptr<Hash> > hashes;

  // 1 node
  hashes.push_back(hash01);
  HashTree* ht = new HashTree(hashes);
  ht->makeHashTreeFromSelf();
  BOOST_CHECK_EQUAL(1, ht->getHashes()->size());
  // 2 nodes
  hashes.push_back(hash02);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(3, ht->getHashes()->size());
  // 3 nodes
  hashes.push_back(hash03);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(6, ht->getHashes()->size());
  // 4 nodes
  hashes.push_back(hash04);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(7, ht->getHashes()->size());
  // 5 nodes
  hashes.push_back(hash05);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(11, ht->getHashes()->size());
  // 7 nodes
  hashes.push_back(hash06);
  hashes.push_back(hash07);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(14, ht->getHashes()->size());
  // 9 nodes
  hashes.push_back(hash08);
  hashes.push_back(hash09);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(20, ht->getHashes()->size());
  // 16 nodes
  hashes.push_back(hash10);
  hashes.push_back(hash11);
  hashes.push_back(hash12);
  hashes.push_back(hash13);
  hashes.push_back(hash14);
  hashes.push_back(hash15);
  hashes.push_back(hash16);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(31, ht->getHashes()->size());
  // 17 nodes
  hashes.push_back(hash17);
  ht->makeHashTree(hashes);
  BOOST_CHECK_EQUAL(37, ht->getHashes()->size());

  delete ht;
}
BOOST_AUTO_TEST_CASE(hash_tree_size_compare_random)
{
  std::shared_ptr<Hash> hash(new Hash("test"));
  std::vector< std::shared_ptr<Hash> > hashes;
  HashTree* ht = new HashTree();
  for (int i = 0; i < 5; ++i)
  {
    int size = 0;
    int random = rand() % 65500;
    hashes.clear();
    hashes.resize(random,hash);
    ht->makeHashTree(hashes);

    int j = 1;
    int depth = 0;
    while ( (1 + ((random - 1) / j)) > 1)
    {
      j = j << 1;
      depth += 1;
    }
    size += random;
    int divident = random;
    for (j = depth; j > 0; --j)
    {
      divident = (divident % 2 == 0) ? (divident / 2) : ((divident+1) / 2);
      size += divident;
    }
    BOOST_CHECK_EQUAL(size, ht->getHashes()->size());
  }

  delete ht;
}
BOOST_AUTO_TEST_CASE(hash_tree_change)
{
  std::shared_ptr<Hash> hash1(new Hash("test1"));
  std::shared_ptr<Hash> hash2(new Hash("test2"));
  std::vector< std::shared_ptr<Hash> > hashes_orig = { hash1, hash1, hash1 };
  std::vector< std::shared_ptr<Hash> > hashes_diff = { hash1, hash2, hash1 };
  HashTree* ht_orig = new HashTree(hashes_orig);
  HashTree* ht_diff = new HashTree(hashes_diff);
  ht_orig->makeHashTreeFromSelf();
  ht_diff->makeHashTreeFromSelf();
  BOOST_CHECK(  ht_orig->checkHashTreeChange(*ht_diff) );
  BOOST_CHECK( !ht_orig->checkHashTreeChange(*ht_orig) );
}
BOOST_AUTO_TEST_CASE(hash_tree_changed_hashes)
{
  std::shared_ptr<Hash> hash1(new Hash("test1"));
  std::shared_ptr<Hash> hash2(new Hash("test2"));
  std::vector< std::shared_ptr<Hash> > hashes_orig = { hash1, hash1, hash1 };
  std::vector< std::shared_ptr<Hash> > hashes_diff = { hash1, hash2, hash1 };
  std::vector< std::shared_ptr<Hash> > hashes_returned_diff;
  std::vector< std::shared_ptr<Hash> > hashes_returned_orig;
  HashTree* ht_orig = new HashTree(hashes_orig);
  HashTree* ht_diff = new HashTree(hashes_diff);
  ht_orig->makeHashTreeFromSelf();
  ht_diff->makeHashTreeFromSelf();

  BOOST_CHECK(  ht_orig->getChangedHashes( hashes_returned_diff, *ht_diff ) );
  BOOST_CHECK_EQUAL( hashes_returned_diff.size(), 1 );
  BOOST_CHECK_EQUAL( hashes_returned_diff[0], hash2 );

  BOOST_CHECK( !ht_orig->getChangedHashes( hashes_returned_orig, *ht_orig ) );
  BOOST_CHECK_EQUAL( hashes_returned_orig.size(), 0 );
}
BOOST_AUTO_TEST_CASE(hash_tree_sort)
{
  std::shared_ptr<Hash> hash1(new Hash("test1")); // c689bf21986252dab8c946042cd73c44995a205da7b8c0816c56ee33894acbace61f27ed94d9ffc2a0d3bee7539565aca834b220af95cc5abb2ceb90946606fe
  std::shared_ptr<Hash> hash2(new Hash("test2")); // e1b1bfe59054380ac6eb014388b2db3a03d054770ededd9ee148c8b29aa272bbd079344bb40a92d0a754cd925f4beb48c9fd66a0e90b0d341b6fe3bbb4893246
  std::shared_ptr<Hash> hash3(new Hash("test3")); // 08661229443b4c34cf289b868f8de120dd5eae28551ee3568bfd8058901d44f5641a6785117907b99bdfe951124dcb6ce6c7235aa9a13ae8e6808272ebef0278
  std::shared_ptr<Hash> hash4(new Hash("test4")); // 0bbabcef4b2f47db3d8964fc0914cf8ceeecf567ca2d3f8d14890b7ced64e4260727eafb25c79ce3cde190e6ccbd014d329cae947e82b2c4a68561ed86590ce2
  std::shared_ptr<Hash> hash5(new Hash("test5")); // e235bcab3b125578f8720d4dd2513726fc3af20896535692812aae76459deeea0813f2514a533d04bf44ac3f440027eb87c75b6cd112e454b78129194d1363ea

  // therefore the correct order would be:
  // hash3, hash4, hash1, hash2, hash5
  std::vector< std::shared_ptr<Hash> > hashes = {hash1, hash2, hash3, hash4, hash5};
  std::sort (hashes.begin(), hashes.end(), hashSharedPointerLessThanFunctor());
  BOOST_CHECK_EQUAL(hash3->getString(), hashes[0]->getString());
  BOOST_CHECK_EQUAL(hash4->getString(), hashes[1]->getString());
  BOOST_CHECK_EQUAL(hash1->getString(), hashes[2]->getString());
  BOOST_CHECK_EQUAL(hash2->getString(), hashes[3]->getString());
  BOOST_CHECK_EQUAL(hash5->getString(), hashes[4]->getString());
}
BOOST_AUTO_TEST_CASE(hash_tree_top_hash)
{
  std::shared_ptr<Hash> hash1(new Hash("test1"));
  std::shared_ptr<Hash> hash2(new Hash("test2"));
  std::shared_ptr<Hash> hash3(new Hash("test3"));
  std::vector< std::shared_ptr<Hash> > hashes = {hash1, hash2, hash3};
  HashTree* ht = new HashTree(hashes);
  ht->makeHashTreeFromSelf();

  // Value generated with pyblake2
  std::string hash_string = "1e8c2d4172f2730c30074270271f3aff0a826cb31f631d39f952fb6628125096b115f4b1f91781afaed2f1546d7c4e4ae58803376cc322cb8e164f13ac406eb6";
  BOOST_CHECK_EQUAL(hash_string, ht->getTopHash()->getString());
}
BOOST_AUTO_TEST_CASE(hash_tree_elements_per_level)
{
  std::shared_ptr<Hash> hash1(new Hash("test1"));
  std::shared_ptr<Hash> hash2(new Hash("test2"));
  std::shared_ptr<Hash> hash3(new Hash("test3"));
  std::shared_ptr<Hash> hash4(new Hash("test4"));
  std::shared_ptr<Hash> hash5(new Hash("test5"));
  std::vector< std::shared_ptr<Hash> > hashes = {hash1, hash2, hash3, hash4, hash5};
  HashTree* ht = new HashTree(hashes);
  ht->makeHashTreeFromSelf();
  const std::vector<int> epl = *(ht->getElementsPerLevel());
  BOOST_CHECK_EQUAL(epl[0],5);
  BOOST_CHECK_EQUAL(epl[1],3);
  BOOST_CHECK_EQUAL(epl[2],2);
  BOOST_CHECK_EQUAL(epl[3],1);
}
BOOST_AUTO_TEST_CASE(hash_tree_elements_per_level_nonunique)
{
  std::shared_ptr<Hash> hash1(new Hash("test1"));
  std::shared_ptr<Hash> hash2(new Hash("test2"));
  std::vector< std::shared_ptr<Hash> > hashes = {hash1, hash1, hash2, hash1, hash2, hash1};
  HashTree* ht = new HashTree(hashes);
  ht->makeHashTreeFromSelf();
  const std::vector<int> epl = *(ht->getElementsPerLevel());
  BOOST_CHECK_EQUAL(epl[0],2);
  BOOST_CHECK_EQUAL(epl[1],1);
}

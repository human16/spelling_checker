#include "CUnit/Basic.h"
#include "trie.c"

void trie_test() {
  TrieNode *root = make_trienode('\0');
  root = insert_trie(root, "hello");
  root = insert_trie(root, "world");
  root = insert_trie(root, "i18n");
  root = insert_trie(root, "ho-w!dy");

  CU_ASSERT_EQUAL(search_trie(root, "hello"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "world"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "i18n"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "ho-w!dy"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "he"), 0);
  CU_ASSERT_EQUAL(search_trie(root, "i"), 0);
  CU_ASSERT_EQUAL(search_trie(root, "notintrie"), 0);
}

int main() {
  // Init the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Sets the basic run mode, CU_BRM_VERBOSE will show max output of run details
  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_pSuite pSuite = NULL;

  // Add a suite to the registry
  pSuite = CU_add_suite("trie_test_suite", 0, 0);

  // Check if add was successful
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add the test to the suite
  if (NULL == CU_add_test(pSuite, "trie_test", trie_test)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_run_tests();
  return CU_get_error();
}

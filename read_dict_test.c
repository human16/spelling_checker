#include "CUnit/Basic.h"
#include "read_dict.c"
#include "trie/trie.c"
#include <unistd.h>

void create_test_dict(const char *filename, const char *content) {
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd >= 0) {
    write(fd, content, strlen(content));
    close(fd);
  }
}

void test_read_valid_dictionary() {
  const char *test_file = "test_dict_valid.txt";
  create_test_dict(test_file, "hello\nworld\ntest\nspelling\n");

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "hello"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "world"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "test"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "spelling"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "notfound"), 0);

  free_trienode(root);
  unlink(test_file);
}

void test_read_dict_no_trailing_newline() {
  const char *test_file = "test_dict_no_newline.txt";
  create_test_dict(test_file, "apple\nbanana\ncherry");

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "apple"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "banana"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "cherry"), 1);

  free_trienode(root);
  unlink(test_file);
}

void test_read_empty_dictionary() {
  const char *test_file = "test_dict_empty.txt";
  create_test_dict(test_file, "");

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "anything"), 0);

  free_trienode(root);
  unlink(test_file);
}

void test_read_dict_with_empty_lines() {
  const char *test_file = "test_dict_empty_lines.txt";
  create_test_dict(test_file, "first\n\nsecond\n\n\nthird\n");

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "first"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "second"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "third"), 1);

  free_trienode(root);
  unlink(test_file);
}

void test_read_dict_special_chars() {
  const char *test_file = "test_dict_special.txt";
  create_test_dict(test_file, "hello-world\ni18n\nho-w!dy\n");

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "hello-world"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "i18n"), 1);
  CU_ASSERT_EQUAL(search_trie(root, "ho-w!dy"), 1);

  free_trienode(root);
  unlink(test_file);
}

void test_read_dict_long_words() {
  const char *test_file = "test_dict_long.txt";
  // word longer than initial buffer size
  char *long_word = "pneumonoultramicroscopicsilicovolcanoconiosis";
  char content[100];
  snprintf(content, sizeof(content), "short\n%s\nanother\n", long_word);
  create_test_dict(test_file, content);

  TrieNode *root = read_dictionary((char *)test_file);

  CU_ASSERT_PTR_NOT_NULL(root);
  CU_ASSERT_EQUAL(search_trie(root, "short"), 1);
  CU_ASSERT_EQUAL(search_trie(root, long_word), 1);
  CU_ASSERT_EQUAL(search_trie(root, "another"), 1);

  free_trienode(root);
  unlink(test_file);
}

void test_read_nonexistent_file() {
  TrieNode *root = read_dictionary("nonexistent_file_12345.txt");

  CU_ASSERT_PTR_NULL(root);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_pSuite pSuite = NULL;

  pSuite = CU_add_suite("read_dict_test_suite", 0, 0);

  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "test_read_valid_dictionary",
                          test_read_valid_dictionary) ||
      NULL == CU_add_test(pSuite, "test_read_dict_no_trailing_newline",
                          test_read_dict_no_trailing_newline) ||
      NULL == CU_add_test(pSuite, "test_read_empty_dictionary",
                          test_read_empty_dictionary) ||
      NULL == CU_add_test(pSuite, "test_read_dict_with_empty_lines",
                          test_read_dict_with_empty_lines) ||
      NULL == CU_add_test(pSuite, "test_read_dict_special_chars",
                          test_read_dict_special_chars) ||
      NULL == CU_add_test(pSuite, "test_read_dict_long_words",
                          test_read_dict_long_words) ||
      NULL == CU_add_test(pSuite, "test_read_nonexistent_file",
                          test_read_nonexistent_file)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_run_tests();
  int result = CU_get_error();
  CU_cleanup_registry();

  return result;
}

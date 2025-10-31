#include "CUnit/Basic.h"
#include "searcher.c"
#include "trie/trie.c"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// Creates a temporary dictionary file with test words and loads it into a trie
TrieNode *create_test_dictionary(const char *filename, const char *content) {
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd >= 0) {
    write(fd, content, strlen(content));
    close(fd);
  }

  TrieNode *root = make_trienode('\0');
  char *word_copy = strdup(content);
  char *line = strtok(word_copy, "\n");

  while (line != NULL) {
    if (strlen(line) > 0) {
      root = insert_trie(root, line);
    }
    line = strtok(NULL, "\n");
  }

  free(word_copy);
  return root;
}

void test_strip_prefix() {
  // case 1: "-4hi" -> "hi"
  char *result1 = strip_non_letters("-4hi", 4);
  CU_ASSERT_PTR_NOT_NULL(result1);
  CU_ASSERT_STRING_EQUAL(result1, "hi");
  free(result1);

  // case 2: "123word" -> "word"
  char *result2 = strip_non_letters("123word", 7);
  CU_ASSERT_PTR_NOT_NULL(result2);
  CU_ASSERT_STRING_EQUAL(result2, "word");
  free(result2);

  // case 3: "!!!test" -> "test"
  char *result3 = strip_non_letters("!!!test", 7);
  CU_ASSERT_PTR_NOT_NULL(result3);
  CU_ASSERT_STRING_EQUAL(result3, "test");
  free(result3);
}

void test_strip_suffix() {
  // case 1: "hello-34" -> "hello"
  char *result1 = strip_non_letters("hello-34", 8);
  CU_ASSERT_PTR_NOT_NULL(result1);
  CU_ASSERT_STRING_EQUAL(result1, "hello");
  free(result1);

  // case 2: "word123" -> "word"
  char *result2 = strip_non_letters("word123", 7);
  CU_ASSERT_PTR_NOT_NULL(result2);
  CU_ASSERT_STRING_EQUAL(result2, "word");
  free(result2);

  // case 3: "test!!!" -> "test"
  char *result3 = strip_non_letters("test!!!", 7);
  CU_ASSERT_PTR_NOT_NULL(result3);
  CU_ASSERT_STRING_EQUAL(result3, "test");
  free(result3);
}

void test_strip_both() {
  // case 1: "-4hi!" -> "hi"
  char *result1 = strip_non_letters("-4hi!", 5);
  CU_ASSERT_PTR_NOT_NULL(result1);
  CU_ASSERT_STRING_EQUAL(result1, "hi");
  free(result1);

  // case 2: "!!hello123" -> "hello"
  char *result2 = strip_non_letters("!!hello123", 10);
  CU_ASSERT_PTR_NOT_NULL(result2);
  CU_ASSERT_STRING_EQUAL(result2, "hello");
  free(result2);

  // case 3: "---word---" -> "word"
  char *result3 = strip_non_letters("---word---", 10);
  CU_ASSERT_PTR_NOT_NULL(result3);
  CU_ASSERT_STRING_EQUAL(result3, "word");
  free(result3);
}

void test_strip_middle_preserved() {
  // case 1: "wo-rld43" -> "wo-rld"
  char *result1 = strip_non_letters("wo-rld43", 8);
  CU_ASSERT_PTR_NOT_NULL(result1);
  CU_ASSERT_STRING_EQUAL(result1, "wo-rld");
  free(result1);

  // case 2: "123wo-rld" -> "wo-rld"
  char *result2 = strip_non_letters("123wo-rld", 9);
  CU_ASSERT_PTR_NOT_NULL(result2);
  CU_ASSERT_STRING_EQUAL(result2, "wo-rld");
  free(result2);

  // case 3: "don't123" -> "don't"
  char *result3 = strip_non_letters("don't123", 8);
  CU_ASSERT_PTR_NOT_NULL(result3);
  CU_ASSERT_STRING_EQUAL(result3, "don't");
  free(result3);

  // case 4: "---he11o---" -> "he11o"
  char *result4 = strip_non_letters("---he11o---", 11);
  CU_ASSERT_PTR_NOT_NULL(result4);
  CU_ASSERT_STRING_EQUAL(result4, "he11o");
  free(result4);
}

void test_strip_no_letters() {
  // case 1: "!!!!" -> NULL
  char *result1 = strip_non_letters("!!!!", 4);
  CU_ASSERT_PTR_NULL(result1);

  // case 2: "1234" -> NULL
  char *result2 = strip_non_letters("1234", 4);
  CU_ASSERT_PTR_NULL(result2);

  // case 3: "---" -> NULL
  char *result3 = strip_non_letters("---", 3);
  CU_ASSERT_PTR_NULL(result3);

  // case 4: "@#$%" -> NULL
  char *result4 = strip_non_letters("@#$%", 4);
  CU_ASSERT_PTR_NULL(result4);
}

void test_strip_all_letters() {
  // case 1: "hello" -> "hello"
  char *result1 = strip_non_letters("hello", 5);
  CU_ASSERT_PTR_NOT_NULL(result1);
  CU_ASSERT_STRING_EQUAL(result1, "hello");
  free(result1);

  // case 2: "WORLD" -> "WORLD"
  char *result2 = strip_non_letters("WORLD", 5);
  CU_ASSERT_PTR_NOT_NULL(result2);
  CU_ASSERT_STRING_EQUAL(result2, "WORLD");
  free(result2);

  // case 3: "Test" -> "Test"
  char *result3 = strip_non_letters("Test", 4);
  CU_ASSERT_PTR_NOT_NULL(result3);
  CU_ASSERT_STRING_EQUAL(result3, "Test");
  free(result3);

  // case 4: "a" -> "a"
  char *result4 = strip_non_letters("a", 1);
  CU_ASSERT_PTR_NOT_NULL(result4);
  CU_ASSERT_STRING_EQUAL(result4, "a");
  free(result4);
}

void test_search_valid_word() {
  TrieNode *test_dict = create_test_dictionary(
      "test_dict_valid.txt", "hello\nworld\ntest\nspelling\n");

  set_dict(test_dict);

  // case 1: "hello" should be found
  int result1 = search_word("hello", 5, 1, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result1, EXIT_SUCCESS);

  // case 2: "world" should be found
  int result2 = search_word("world", 5, 2, 5, "testfile.txt");
  CU_ASSERT_EQUAL(result2, EXIT_SUCCESS);

  // case 3: "test" should be found
  int result3 = search_word("test", 4, 3, 10, "testfile.txt");
  CU_ASSERT_EQUAL(result3, EXIT_SUCCESS);

  free_trienode(test_dict);
  unlink("test_dict_valid.txt");
  set_dict(NULL);
}

void test_search_misspelled_word() {
  TrieNode *test_dict =
      create_test_dictionary("test_dict_misspell.txt", "hello\nworld\n");

  set_dict(test_dict);

  // case 1: "mispelled" is NOT in dictionary
  printf("\n[Expected output below: misspelling message]\n");
  int result1 = search_word("mispelled", 9, 1, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result1, EXIT_SUCCESS);

  // Test case 2: "wrld" is NOT in dictionary
  printf("[Expected output below: misspelling message]\n");
  int result2 = search_word("wrld", 4, 2, 5, "testfile.txt");
  CU_ASSERT_EQUAL(result2, EXIT_SUCCESS);

  free_trienode(test_dict);
  unlink("test_dict_misspell.txt");
  set_dict(NULL);
}

void test_search_with_stripping() {
  TrieNode *test_dict =
      create_test_dictionary("test_dict_strip.txt", "hello\nworld\ntest\n");

  set_dict(test_dict);

  // case 1: "-4hello!" should strip to "hello" and be found
  int result1 = search_word("-4hello!", 8, 1, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result1, EXIT_SUCCESS);

  // case 2: "123world!!!" should strip to "world" and be found
  int result2 = search_word("123world!!!", 11, 2, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result2, EXIT_SUCCESS);

  // case 3: "!!!test456" should strip to "test" and be found
  int result3 = search_word("!!!test456", 10, 3, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result3, EXIT_SUCCESS);

  // case 4: "---wrld---" should strip to "wrld" and NOT be found
  printf("\n[Expected output below: misspelling for 'wrld']\n");
  int result4 = search_word("---wrld---", 10, 4, 0, "testfile.txt");
  CU_ASSERT_EQUAL(result4, EXIT_SUCCESS);

  free_trienode(test_dict);
  unlink("test_dict_strip.txt");
  set_dict(NULL);
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);

  CU_pSuite pSuite = CU_add_suite("searcher_test_suite", NULL, NULL);

  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Tests strip_non_letters function
  CU_add_test(pSuite, "test_strip_prefix", test_strip_prefix);
  CU_add_test(pSuite, "test_strip_suffix", test_strip_suffix);
  CU_add_test(pSuite, "test_strip_both", test_strip_both);
  CU_add_test(pSuite, "test_strip_middle_preserved",
              test_strip_middle_preserved);
  CU_add_test(pSuite, "test_strip_no_letters", test_strip_no_letters);
  CU_add_test(pSuite, "test_strip_all_letters", test_strip_all_letters);

  // Tests search_word function
  CU_add_test(pSuite, "test_search_valid_word", test_search_valid_word);
  CU_add_test(pSuite, "test_search_misspelled_word",
              test_search_misspelled_word);
  CU_add_test(pSuite, "test_search_with_stripping", test_search_with_stripping);

  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_run_tests();

  int result = CU_get_error();

  CU_cleanup_registry();

  return result;
}

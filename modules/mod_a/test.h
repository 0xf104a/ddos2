#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

struct test{
  const char* name;
  bool (*perform)(void);
};

typedef struct test test_t;

void tests_begin(void);
void test_add(const char* name, bool (*test_fun)(void));
void test_all(void);

#endif 

#include "Core/ContainersTest.h"

#include <stdio.h>
#include <cmocka.h>

int main(void)
{
  int result = 0;

  printf("Running Queue Tests...\n");
  result += cmocka_run_group_tests(queue_tests, nullptr, nullptr);

  return result;
}

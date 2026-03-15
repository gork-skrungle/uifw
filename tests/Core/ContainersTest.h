#pragma once

#include "uifw/Core/Containers/Queue.h"

#include <cmocka.h>

static void test_queueInit(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  assert_non_null(queue.memory);
  assert_int_equal(queue.element_size, sizeof(int));
  assert_int_equal(queue.element_count, 0);

  ui_queueDestroy(&queue);
}

static void test_queueEmpty(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  assert_true(ui_queueEmpty(&queue));

  int value = 42;
  ui_queuePush(&queue, &value);

  assert_false(ui_queueEmpty(&queue));

  int popped;
  ui_queuePop(&queue, &popped);

  assert_true(ui_queueEmpty(&queue));

  ui_queueDestroy(&queue);
}

static void test_queuePushAndPop(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  int values[] = {1, 2, 3, 4, 5};
  int popped;

  for (int i = 0; i < 5; i++) {
    ui_queuePush(&queue, &values[i]);
  }

  for (int i = 0; i < 5; i++) {
    ui_queuePop(&queue, &popped);
    assert_int_equal(popped, values[i]);
  }

  assert_true(ui_queueEmpty(&queue));

  ui_queueDestroy(&queue);
}

static void test_queuePeek(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  int value = 100;
  ui_queuePush(&queue, &value);

  int peeked;
  ui_queuePeek(&queue, &peeked);
  assert_int_equal(peeked, 100);

  assert_int_equal(queue.element_count, 1);

  ui_queueDestroy(&queue);
}

static void test_queuePushPopPush(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  int value1 = 10;
  int value2 = 20;
  int value3 = 30;
  int popped;

  ui_queuePush(&queue, &value1);
  ui_queuePush(&queue, &value2);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 10);

  ui_queuePush(&queue, &value3);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 20);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 30);

  assert_true(ui_queueEmpty(&queue));

  ui_queueDestroy(&queue);
}

static void test_queueWithStruct(void **state)
{
  (void)state;

  typedef struct
  {
    int x;
    int y;
  } Point;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(Point));

  Point p1 = {1, 2};
  Point p2 = {3, 4};
  Point p3 = {5, 6};
  Point popped;

  ui_queuePush(&queue, &p1);
  ui_queuePush(&queue, &p2);
  ui_queuePush(&queue, &p3);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped.x, 1);
  assert_int_equal(popped.y, 2);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped.x, 3);
  assert_int_equal(popped.y, 4);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped.x, 5);
  assert_int_equal(popped.y, 6);

  ui_queueDestroy(&queue);
}

static void test_queueDestroy(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(double));

  double value = 3.14;
  ui_queuePush(&queue, &value);

  assert_false(ui_queueEmpty(&queue));
  assert_non_null(queue.memory);

  ui_queueDestroy(&queue);

  assert_true(ui_queueEmpty(&queue));
  assert_null(queue.memory);
  assert_int_equal(queue.element_size, 0);
  assert_int_equal(queue.element_count, 0);
}

static void test_queueMultiplePeeks(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(char));

  char value = 'A';
  ui_queuePush(&queue, &value);

  char peeked1, peeked2, peeked3;
  ui_queuePeek(&queue, &peeked1);
  ui_queuePeek(&queue, &peeked2);
  ui_queuePeek(&queue, &peeked3);

  assert_int_equal(peeked1, 'A');
  assert_int_equal(peeked2, 'A');
  assert_int_equal(peeked3, 'A');

  assert_int_equal(queue.element_count, 1);

  ui_queueDestroy(&queue);
}

static void test_queueLargeNumberOfElements(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  const int count = 1000;
  int pushed, popped;

  for (int i = 0; i < count; i++) {
    pushed = i;
    ui_queuePush(&queue, &pushed);
  }

  for (int i = 0; i < count; i++) {
    ui_queuePop(&queue, &popped);
    assert_int_equal(popped, i);
  }

  assert_true(ui_queueEmpty(&queue));

  ui_queueDestroy(&queue);
}

static void test_queueInterleavedOperations(void **state)
{
  (void)state;

  ui_Queue queue;
  ui_queueInit(&queue, sizeof(int));

  int value, popped, peeked;

  value = 1;
  ui_queuePush(&queue, &value);

  value = 2;
  ui_queuePush(&queue, &value);

  ui_queuePeek(&queue, &peeked);
  assert_int_equal(peeked, 1);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 1);

  value = 3;
  ui_queuePush(&queue, &value);

  ui_queuePeek(&queue, &peeked);
  assert_int_equal(peeked, 2);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 2);

  ui_queuePop(&queue, &popped);
  assert_int_equal(popped, 3);

  ui_queueDestroy(&queue);
}

const struct CMUnitTest queue_tests[] = {
  cmocka_unit_test(test_queueInit),
  cmocka_unit_test(test_queueEmpty),
  cmocka_unit_test(test_queuePushAndPop),
  cmocka_unit_test(test_queuePeek),
  cmocka_unit_test(test_queuePushPopPush),
  cmocka_unit_test(test_queueWithStruct),
  cmocka_unit_test(test_queueDestroy),
  cmocka_unit_test(test_queueMultiplePeeks),
  cmocka_unit_test(test_queueLargeNumberOfElements),
  cmocka_unit_test(test_queueInterleavedOperations),
};

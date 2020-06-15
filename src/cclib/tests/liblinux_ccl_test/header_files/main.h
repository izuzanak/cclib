
#ifndef TESTS_LIBLINUX_CCL_TEST_HEADER_FILES_MAIN_H
#define TESTS_LIBLINUX_CCL_TEST_HEADER_FILES_MAIN_H

#include "ccl_linux.h"
#include "ccl_regex.h"
#include "ccl_test.h"

// === test execution functions ================================================

constexpr int test_cnt = 8;

void test_fd();
void test_socket_address();
void test_socket();
void test_socket_udp();
void test_socket_tcp();
void test_pid();
void test_poll();
void test_epoll();

#endif


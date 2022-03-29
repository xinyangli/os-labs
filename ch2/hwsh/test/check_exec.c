#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "exec.h"

#define EXEC_TEST_SIZE 4

char *test_argv[EXEC_TEST_SIZE][100] = {
    {
        "echo",
        "test",
    }, {
        "echo",
        "some command",
    }, {
        "grep",
        "command"
    }, {
        "cut",
        "-d ",
        "-f1"
    }
};

Task task[EXEC_TEST_SIZE] = {
    {test_argv[0], 2},
    {test_argv[1], 2},
    {test_argv[2], 2},
    {test_argv[3], 3}
};

START_TEST(exec_test_1) {
  // Single task
  char buf[256];
  int pipefd[2];
  int status;
  int save_out = dup(STDOUT_FILENO);
  pipe(pipefd);
  pid_t pid = check_fork();
  ck_assert_uint_ge(pid, 0);
  if(pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    exec(task, 1);
    close(pipefd[1]);
    exit(0);
  } else {
    close(pipefd[1]);
    read(pipefd[0], buf, 4);
    buf[4] = '\0';
    close(pipefd[0]);
    wait(&status);
    ck_assert(WIFEXITED(status) && !WEXITSTATUS(status) == 1);
  }
  write(save_out, buf, 5);
  ck_assert_str_eq(buf, "test");
}
END_TEST

START_TEST(exec_test_2) {
  // Multiple tasks
  char buf[256];
  int pipefd[2];
  int status;
  int save_out = dup(STDOUT_FILENO);
  pipe(pipefd);
  pid_t pid = check_fork();
  ck_assert_uint_ge(pid, 0);
  if(pid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    exec(task, 2);
    close(pipefd[1]);
    exit(0);
  } else {
    close(pipefd[1]);
    read(pipefd[0], buf, 4);
    buf[4] = '\0';
    close(pipefd[0]);
    wait(&status);
    ck_assert(WIFEXITED(status) && !WEXITSTATUS(status) == 1);
  }
  mark_point();
  write(save_out, buf, 5);
  ck_assert_str_eq(buf, "some");
}
END_TEST

Suite *exec_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Exec");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, exec_test_1);
  tcase_add_test(tc_core, exec_test_2);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = exec_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
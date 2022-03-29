TESTS = check_analyzer check_exec
check_PROGRAMS = check_analyzer check_exec

check_analyzer_SOURCES = check_analyzer.c $(top_srcdir)/src/analyzer.c
check_analyzer_CPPFLAGS = -I$(top_srcdir)/include
check_analyzer_CFLAGS = @CHECK_CFLAGS@
check_analyzer_LDADD = @CHECK_LIBS@

check_exec_SOURCES = check_exec.c $(top_srcdir)/src/exec.c
check_exec_CPPFLAGS = -I$(top_srcdir)/include
check_exec_CFLAGS = @CHECK_CFLAGS@
check_exec_LDADD = @CHECK_LIBS@

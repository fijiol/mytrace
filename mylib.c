#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static pid_t (*real_fork)(void) = NULL;

// TODO: make everything length-safe

void replace(char* res, const char* from, const char* what, const char* to) {
	res[0] = '\0';

	char *s = strstr(from, what);
						//printf("substr suffix: %p %s\n", s, s);
	if (NULL == s) return;

	size_t i = s - from;
						//printf("pos: %ld\n", i);
	size_t from_s = strlen(from);
						//printf("from: %ld\n", from_s);
	size_t what_s = strlen(what);
						//printf("what: %ld\n", what_s);
	size_t to_s = strlen(to);
						//printf("to: %ld\n", to_s);

	strncpy(res,               from,             i);
	strncpy(res+i,               to,          to_s);
						//printf(">> rest: %ld\n", from_s-what_s);
	strncpy(res+i+to_s, from+i+what_s, from_s-what_s);
	res[from_s-what_s+to_s] = '\0';

						//printf("result: %s\n", res);

	//Success?
	return;
}

pid_t fork(void) {
	pid_t pid;
	real_fork = dlsym(RTLD_NEXT, "fork");
	pid = real_fork();
						//printf(">> forked %d\n", pid);

	char* fork_cmd = getenv("MYTRACE_FORK_CMD");
	if (fork_cmd) {
		char fork_cmd_wp[4096];
		char pid_str[32];

		sprintf(pid_str, "%d", pid);
						//printf("pid str: %s\n", pid_str);
		replace(fork_cmd_wp, fork_cmd, "%pid", pid_str);

						//printf(">> %s\n", fork_cmd_wp);
						//int exit_status = 0;
		int exit_status = system(fork_cmd_wp);
						//printf(">> exit code: %d\n", exit_status);

						//if (0)
		if (-1 == exit_status) {
			printf(">> shell returned error on: %s\n", fork_cmd_wp);
		}
	}

	return pid;
}


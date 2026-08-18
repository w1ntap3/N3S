#include "main.h"
#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct {
	const char *cmd_name;
	int (*cmd_handler)(int, char **);
} cmd_table[] = {{"health", cli_health}};

// static const struct commands
int main(int argc, char *argv[]) {
	// Not enough arguments
	if (argc < 2) {
		fprintf(stderr, "Usage: nsfs --help\n");
		return 1;
	}
	// Global help
	if (strcmp(argv[1], "--help") == 0) {
		printf("You can use a lot of commands\n");
		return 0;
	}
	if (strcmp(argv[1], "--version") == 0) {
		printf("NSFS CLI Version: %s\n", NSFS_CLI_VERSION);
		return 0;
	}
	const char *cmd = argv[1];
	const size_t table_entries = sizeof(cmd_table) / sizeof(cmd_table[0]);
	for (size_t i = 0; i < table_entries; i++) {
		if (strcmp(cmd, cmd_table[i].cmd_name) == 0) {
			// Do not pass the program and subcommands (therefore
			// argv + 2) to the subcommand handlers, these are
			// trivial anyways
			int result =
				cmd_table[i].cmd_handler(argc - 2, argv + 2);
			return result;
		}
	}

	fprintf(stderr, "Error: No such command as \"%s\"\n", argv[1]);
	return 1;
}

int cli_health(int argc, char *argv[]) {
	printf("NSFS CLI is successfully accepting commands.\n");
	return 0;
}

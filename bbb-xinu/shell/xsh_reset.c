/* xsh_reset.c - xsh_reset */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_reset - warm reset of BB
 *------------------------------------------------------------------------
 */
shellcmd xsh_reset(int nargs, char *args[]) {

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tPerform a warm reset of registers\n");
		printf("Options (one per invocation):\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	/* Check argument count */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

	if (nargs == 1) {
		volatile uint32 *prm_rstctrl = (volatile uint32 *)0X44E00F00;
		*prm_rstctrl = 0x01;
		return 0;
	}
}

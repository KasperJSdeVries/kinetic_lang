
#include <stdio.h>
#include <stdlib.h>

#define REGISTER_COUNT 4

static char *reg_names[REGISTER_COUNT] = {"%r8", "%r9", "%r10", "%r11"};

static int free_registers[REGISTER_COUNT];

void registers_freeall(void) {
	free_registers[0] = 1;
	free_registers[1] = 1;
	free_registers[2] = 1;
	free_registers[3] = 1;
}

static int register_alloc(void) {
	for (int i = 0; i < REGISTER_COUNT; i++) {
		if (free_registers[i]) {
			free_registers[i] = 0;
			return i;
		}
	}
	fprintf(stderr, "No more registers available\n");
	exit(1);
}

static void register_free(int r) {
	if (free_registers[r] != 0) {
		fprintf(stderr, "Trying to free already free register %d\n", r);
		exit(1);
	}
	free_registers[r] = 1;
}

void cgpreamble(FILE *out_file) {
	registers_freeall();
	fputs(
	    "\t.text\n"
	    ".LC0:\n"
	    "\t.string\t\"%d\\n\"\n"
	    "printint:\n"
	    "\tpushq\t%rbp\n"
	    "\tmovq\t%rsp, %rbp\n"
	    "\tsubq\t$16, %rsp\n"
	    "\tmovl\t%edi, -4(%rbp)\n"
	    "\tmovl\t-4(%rbp), %eax\n"
	    "\tmovl\t%eax, %esi\n"
	    "\tleaq	.LC0(%rip), %rdi\n"
	    "\tmovl	$0, %eax\n"
	    "\tcall	printf@PLT\n"
	    "\tnop\n"
	    "\tleave\n"
	    "\tret\n"
	    "\n"
	    "\t.globl\tmain\n"
	    "\t.type\tmain, @function\n"
	    "main:\n"
	    "\tpushq\t%rbp\n"
	    "\tmovq	%rsp, %rbp\n",
	    out_file
	);
}

void cgpostamble(FILE *out_file) {
	fputs(
	    "\tmovl	$0, %eax\n"
	    "\tpopq	%rbp\n"
	    "\tret\n",
	    out_file
	);
}

int cgload(int value, FILE *out_file) {
	int r = register_alloc();
	fprintf(out_file, "\tmovq\t$%d, %s\n", value, reg_names[r]);
	return r;
}

int cgadd(int r1, int r2, FILE *out_file) {
	fprintf(out_file, "\taddq\t%s, %s\n", reg_names[r1], reg_names[r2]);
	register_free(r1);
	return r2;
}

int cgmul(int r1, int r2, FILE *out_file) {
	fprintf(out_file, "\timulq\t%s, %s\n", reg_names[r1], reg_names[r2]);
	register_free(r1);
	return r2;
}

int cgsub(int r1, int r2, FILE *out_file) {
	fprintf(out_file, "\tsubq\t%s, %s\n", reg_names[r2], reg_names[r1]);
	register_free(r2);
	return r1;
}

int cgdiv(int r1, int r2, FILE *out_file) {
	fprintf(out_file, "\tmovq\t%s, %%rax\n", reg_names[r1]);
	fprintf(out_file, "\tcqo\n");
	fprintf(out_file, "\tidivq\t%s\n", reg_names[r2]);
	fprintf(out_file, "\tmovq\t%%rax, %s\n", reg_names[r1]);
	register_free(r2);
	return r1;
}

void cgprintint(int r, FILE *out_file) {
	fprintf(out_file, "\tmovq\t%s, %%rdi\n", reg_names[r]);
	fprintf(out_file, "\tcall\tprintint\n");
	register_free(r);
}

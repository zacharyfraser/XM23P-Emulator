/**
 * @file instruction_functions.h
 * @brief Function declarations for each instruction in XM23P Architecture
 * 
 * @author Zach Fraser
 * @date 2024-06-17
 */

#ifndef INSTRUCTION_FUNCTIONS_H
#define INSTRUCTION_FUNCTIONS_H

#include <stdio.h>

#include "definitions.h"

/* Undefined Instruction Handling */
int execute_undefined(instruction_t *instruction, program_t *program);
/* Branch Instructions */
int execute_bl(instruction_t *instruction, program_t *program);
int execute_beq(instruction_t *instruction, program_t *program);
int execute_bne(instruction_t *instruction, program_t *program);
int execute_bc(instruction_t *instruction, program_t *program);
int execute_bnc(instruction_t *instruction, program_t *program);
int execute_bn(instruction_t *instruction, program_t *program);
int execute_bge(instruction_t *instruction, program_t *program);
int execute_blt(instruction_t *instruction, program_t *program);
int execute_bra(instruction_t *instruction, program_t *program);
/* Arithmetic Instructions */
int execute_add(instruction_t *instruction, program_t *program);
int execute_addc(instruction_t *instruction, program_t *program);
int execute_sub(instruction_t *instruction, program_t *program);
int execute_subc(instruction_t *instruction, program_t *program);
int execute_dadd(instruction_t *instruction, program_t *program);
int execute_cmp(instruction_t *instruction, program_t *program);
int execute_xor(instruction_t *instruction, program_t *program);
int execute_and(instruction_t *instruction, program_t *program);
int execute_or(instruction_t *instruction, program_t *program);
int execute_bit(instruction_t *instruction, program_t *program);
int execute_bic(instruction_t *instruction, program_t *program);
int execute_bis(instruction_t *instruction, program_t *program);
/* Data Movement Instructions */
int execute_mov(instruction_t *instruction, program_t *program);
int execute_swap(instruction_t *instruction, program_t *program);
int execute_sra(instruction_t *instruction, program_t *program);
int execute_rrc(instruction_t *instruction, program_t *program);
int execute_swpb(instruction_t *instruction, program_t *program);
int execute_sxt(instruction_t *instruction, program_t *program);
/* System Instructions */
int execute_setpri(instruction_t *instruction, program_t *program);
int execute_svc(instruction_t *instruction, program_t *program);
int execute_setcc(instruction_t *instruction, program_t *program);
int execute_clrcc(instruction_t *instruction, program_t *program);
int execute_cex(instruction_t *instruction, program_t *program);
/* Memory Instructions */
int execute_ld(instruction_t *instruction, program_t *program);
int execute_st(instruction_t *instruction, program_t *program);
int execute_ldr(instruction_t *instruction, program_t *program);
int execute_str(instruction_t *instruction, program_t *program);
/* Move Instructions */
int execute_movl(instruction_t *instruction, program_t *program);
int execute_movlz(instruction_t *instruction, program_t *program);
int execute_movls(instruction_t *instruction, program_t *program);
int execute_movh(instruction_t *instruction, program_t *program);

#endif /* INSTRUCTION_FUNCTIONS_H */
/* Output routines for the PDP-10.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Lars Brinkhoff (lars@nocrew.org), funded by XKL, LLC.

For information about the PDP-10 archtecture, see the README file in
this directory.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */


/**********************************************************************

	Index

**********************************************************************/

/*	  Front Page
	  Index
	  Includes
	  To-do List
	  Miscellaneous
	  Local variables.
	  GCC target structure.
	pdp10.h support:
	  Run-time Target Specification
	  Storage Layout
	  Layout of Source Language Data Types
	  Register Usage
	  Register Classes
	  Stack Layout and Calling Conventions
	  Addressing Modes
	  Dividing the Output into Sections
	  Describing Relative Costs of Operations
	  Defining the Output Assembler Language
	  Target Attributes
	  Miscellaneous Parameters
	  Special Predicates
	  Built-in Functions
	  Support for MACRO, MIDAS, and FAIL
	pdp10.md support:
	  Data Movement
	  Pointer Arithmetic
	  Unconditional Jumps
	  Conditional Jumps
	  Function Prologue and Epilogue
	junkyard:
	  Unsorted */
    

/**********************************************************************

	Includes

**********************************************************************/

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "expr.h"
#include "optabs.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "reload.h"
#include "toplev.h"
#include "recog.h"
#include "function.h"
#include "ggc.h"
#include "pdp10-protos.h"
#include "c-tree.h"
#include "hash.h"
#include "target.h"
#include "target-def.h"
#include "tm_p.h"


/**********************************************************************

	To-do List

**********************************************************************/

/* Only use single_push/single_pop when optimize_size ||
   !TARGET_EXTENDED?  */

/* Candidates for inlining once the code settles:

   pdp10_align_stack_pointer, const_ok_for_letter_p,
   const_double_ok_for_letter_p, extra_constraint,
   starting_frame_offset, stack_pointer_offset, first_parm_offset,
   stack_dynamic_offset, function_profiler, legitimate_address_p,
   legitimize_address, pdp10_const_int_costs,
   pdp10_const_double_costs, pdp10_operand_cost, encode_section_info,
   asm_declare_object_name, asm_output_labelref,
   asm_output_symbol_ref, print_operand_address,
   pdp10_output_call_as_jrst_p.  */

/* Investigate whether it's safe for expand_blt to use BLT when
   operands[2] is not constant.  */

/* Use global format indirect words?  Recognize global format indirect
   word in LEGITIMATE_ADDRESS_P?  */

/* pdp10_function_profiler.  */

/* pdp10_bytesize.  */


/**********************************************************************

	Miscellaneous

**********************************************************************/

#define SAVE		1
#define RESTORE		0

#define HWINT(X) ((HOST_WIDE_INT)(X))
#define SIGN_BIT (HWINT (1) << 35)
#if HOST_BITS_PER_WIDE_INT > 36
#define WORD_MASK ((HWINT (1) << 36) - 1)
#else
#define WORD_MASK (HWINT (-1))
#endif

typedef unsigned long long pdp10_word36;
#define PDP10_WORD36_MASK ((((pdp10_word36) 1) << 36) - 1)

#define WORD_MODE_P(MODE) \
  (GET_MODE_SIZE (MODE) >= UNITS_PER_WORD || (MODE) == BLKmode)
#define BYTE_MODE_P(MODE) (!WORD_MODE_P (MODE))

#define WORD_TYPE_P(TYPE) (pdp10_bytesize (TYPE) >= 32)
#define BYTE_TYPE_P(TYPE) (!WORD_TYPE_P (TYPE))

#define VALID_OWGBP_BYTE_SIZE_P(SIZE) \
 ((SIZE) == 6 || (SIZE) == 7 || (SIZE) == 8 || (SIZE) == 9 \
  || (SIZE) == 16 || (SIZE) == 18)

/* Special pseudo byte index used when taking the address of a
   standalone scalar object stored right-justified in a full PDP-10 word.
   It is not a canonical array byte.  */
#define PDP10_SCALAR_RIGHT_BYTE (-1)

#define GEN_EQ gen_rtx_EQ (VOIDmode, NULL_RTX, NULL_RTX)
#define GEN_NE gen_rtx_NE (VOIDmode, NULL_RTX, NULL_RTX)

#define GEN_MEM_SP_PLUS(mode, offset) \
  gen_rtx_MEM (mode, plus_constant (stack_pointer_rtx, offset))

#define gen_ADDRESS(X) \
  gen_rtx_UNSPEC (Pmode, gen_rtvec (1, (X)), UNSPEC_ADDRESS)

#if !HAVE_IBP
#define gen_IBP(OP0, OP1) gen_ADJBP (OP0, OP1, GEN_INT (1))
#endif

static void	pdp10_print_number PARAMS ((FILE *stream,
					    HOST_WIDE_INT x,
					    int div,
					    int bits,
					    int zero));
static void	pdp10_split_integer_double PARAMS ((rtx x,
					       unsigned HOST_WIDE_INT *word0p,
					       unsigned HOST_WIDE_INT *word1p));
static int	pdp10_const_byte_pointer_p PARAMS ((rtx x));
static int	pdp10_extract_word36_constant PARAMS ((rtx x,
					      pdp10_word36 *valuep));
static rtx	pdp10_gen_word36_constant PARAMS ((pdp10_word36 value));
static rtx	pdp10_plus_constant_word36 PARAMS ((rtx x, pdp10_word36 value));
static int	pdp10_decompose_symbolic_constant PARAMS ((rtx x, rtx *basep,
					     HOST_WIDE_INT *offsetp));
static int	pdp10_decompose_symbolic_word36 PARAMS ((rtx x, rtx *basep,
					   pdp10_word36 *offsetp));
static int	pdp10_const_word36_byte_pointer_p PARAMS ((rtx x));
static int	pdp10_reg_likely_byte_pointer_p PARAMS ((rtx reg, rtx insn));
int	pdp10_const_double_0_operand PARAMS ((rtx op, enum machine_mode mode));
static int	pdp10_qi_mem_logical_offset PARAMS ((rtx mem, HOST_WIDE_INT *offp, int *unsignedp));
static int	pdp10_qi_mem_byte_offset PARAMS ((rtx mem, HOST_WIDE_INT *byteoffp, int *unsignedp));
static int	pdp10_output_qi_tree_bitpos PARAMS ((tree exp, pdp10_word36 *bitposp));
static int	pdp10_reg_byte_pointer_word_offset_p PARAMS ((rtx reg, rtx insn, HOST_WIDE_INT *offp));
static int	pdp10_dynamic_ref_base_bitpos PARAMS ((tree exp, tree *basep,
                                                   pdp10_word36 *bitposp));
static void	pdp10_print_raw_word36_constant PARAMS ((FILE *stream, rtx x));
static int	pdp10_frame_address_p PARAMS ((rtx x));
static rtx	pdp10_output_unwrap_address PARAMS ((rtx x));
static rtx	pdp10_output_add_address_offset PARAMS ((rtx x,
						       HOST_WIDE_INT offset));
static int	pdp10_pad_varargs_down_p PARAMS ((HOST_WIDE_INT size));

#define ADDRESS_MASK (TARGET_EXTENDED ? HWINT (07777777777) : HWINT (0777777))

#define pdp10_print_address(stream, x) \
  pdp10_print_number (stream, x, 1, TARGET_EXTENDED ? 30 : 18, 0)
#define pdp10_print_offset30(stream, x) \
  pdp10_print_number (stream, x, 1, TARGET_EXTENDED ? -30 : -18, 1)
#define pdp10_print_offset(stream, x) \
  pdp10_print_number (stream, x, 1, -18, 1)
#define pdp10_print_integer(stream, x) \
  pdp10_print_number (stream, x, 1, -36, 0)
#define pdp10_print_unsigned(stream, x) \
  pdp10_print_number (stream, x, 1, 36, 0)

#define PRINT_OP_NO_BRACKETS	0x100
#define PRINT_OP_GIW		0x200
#define PRINT_OP_INDIRECT	0x400

#define SAME_REG(op0, op1) REG_P (op1) && REGNO (op0) == REGNO (op1)


/**********************************************************************

	Local variables.

**********************************************************************/

static int expand_extzv_16_0 = 0;
static int output_extzv_16_0 = 0;
static int expand_extzv_16_1 = 0;
static int output_extzv_16_1 = 0;
static int expand_extzv_32 = 0;
static int output_extzv_32 = 0;
int pdp10_output_ldbe_9 = 0;
static int expand_extv_16_0 = 0;
static int output_extv_16_0 = 0;
static int expand_extv_16_1 = 0;
static int output_extv_16_1 = 0;
static int expand_extv_32 = 0;
static int output_extv_32 = 0;
static int expand_extv = 0;
static int output_extv = 0;
int pdp10_expand_ffs = 0;

/* The number of registers used for argument passing.  */
int pdp10_regparm = PDP10_DEFAULT_REGPARM;

/* The number of registers used to return a value.  */
int pdp10_retregs = PDP10_DEFAULT_REGPARM;

/* ... */
int pdp10_giw = 0;
static int used_byte_subtraction_table[19];

int pdp10_outputting_debug_info = 0;

/* Split an integer constant used as a PDP-10 double word into the two
   36-bit words that are visible in registers and memory.

   GCC stores an integer CONST_DOUBLE in host-wide chunks, not as two
   PDP-10 words.  Repack those host chunks into the high and low PDP-10
   36-bit words.

   Do not apply the old 71-bit arithmetic packing here.  This routine is
   used when materializing or printing an object in memory/register form,
   and DAIMON's normal DImode object representation is two raw 36-bit
   words.  The arithmetic expanders decide separately whether to use
   71-bit or 72-bit operation sequences.

   Keep this in one place.  Literal printing, non-DMOVE expansion, and
   wider move expansion must all agree, so constants like

        0123456123456654321654321LL

   become 123456123456,,654321654321.  */
static void
pdp10_split_integer_double (x, word0p, word1p)
     rtx x;
     unsigned HOST_WIDE_INT *word0p;
     unsigned HOST_WIDE_INT *word1p;
{
  unsigned HOST_WIDE_INT high;
  unsigned HOST_WIDE_INT low;
  unsigned HOST_WIDE_INT word0;
  unsigned HOST_WIDE_INT word1;

  if (GET_CODE (x) == CONST_INT)
    {
      word0 = ((unsigned HOST_WIDE_INT) INTVAL (x) >> 36) & WORD_MASK;
      word1 = (unsigned HOST_WIDE_INT) INTVAL (x) & WORD_MASK;
    }
  else if (GET_CODE (x) == CONST_DOUBLE && GET_MODE (x) == VOIDmode)
    {
      high = (unsigned HOST_WIDE_INT) CONST_DOUBLE_HIGH (x);
      low = (unsigned HOST_WIDE_INT) CONST_DOUBLE_LOW (x);

      word0 = ((high << (HOST_BITS_PER_WIDE_INT - 36))
	       + (low >> 36)) & WORD_MASK;
      word1 = low & WORD_MASK;
    }
  else
    abort ();

  *word0p = word0 & WORD_MASK;
  *word1p = word1 & WORD_MASK;
}


/**********************************************************************

	GCC target structure.

**********************************************************************/

const struct attribute_spec pdp10_attribute_table[];
static void	pdp10_asm_named_section PARAMS ((const char *, unsigned int));
static bool	pdp10_assemble_integer PARAMS ((rtx x, unsigned intsize,
						int aligned_p));

#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE pdp10_attribute_table

#undef TARGET_ASM_OPEN_PAREN
#define TARGET_ASM_OPEN_PAREN "<"

#undef TARGET_ASM_CLOSE_PAREN
#define TARGET_ASM_CLOSE_PAREN ">"

#ifndef __PDP10_GAS_H__
#undef TARGET_ASM_BYTE_OP
#define TARGET_ASM_BYTE_OP 0
#endif

#undef TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER pdp10_assemble_integer

#undef TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE pdp10_output_func_prologue

#undef TARGET_ASM_FUNCTION_EPILOGUE
#define TARGET_ASM_FUNCTION_EPILOGUE pdp10_output_func_epilogue

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS pdp10_init_builtins

#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN pdp10_expand_builtin

struct gcc_target targetm = TARGET_INITIALIZER;


/**********************************************************************

	Run-time Target Specifiation

**********************************************************************/

static char *normalize_cpu_name PARAMS ((char *name));
static int available_regs PARAMS ((void));
static void pdp10_set_reg_alloc_order PARAMS ((void));

/* Receives a pointer to the value of the -mcpu option.  */
char *pdp10_cpu = 0;

/* Receives a pointer to the value of the -march option.  */
char *pdp10_arch = 0;

/* Receives a pointer to the value of the -mtune option.  */
char *pdp10_tune = 0;

/* Receives a pointer to the value of the -mtext-psect option.  */
char *pdp10_text_psect = 0;

/* Receives a pointer to the value of the -mrodata-psect option.  */
char *pdp10_rodata_psect = 0;

/* Receives a pointer to the value of the -mdata-psect option.  */
char *pdp10_data_psect = 0;

/* Receives a pointer to the value of the -mbss-psect option.  */
char *pdp10_bss_psect = 0;

/* Receives a pointer to the value of the -mmodel option.  */
char *pdp10_model = 0;

/* Receives a pointer to the value of the -mregparm option.  */
char *pdp10_argument_regs = 0;

/* Receives a pointer to the value of the -mreturn-regs option.  */
char *pdp10_return_regs = 0;

/* Receives a pointer to the value of the -mcall-clobbered option.  */
char *pdp10_call_clobbered = 0;

/* Receives a pointer to the value of the -mstring-bytesize option.  */
char *pdp10_string_bytesize = 0;

/* Receives a pointer to the value of the -mchar-bytesize option.  */
char *pdp10_char_bytesize_arg = 0;

int pdp10_char_bytesize = 9;

#ifdef __PDP10_MACRO_H__
/* A C expression whose value is a string containing the assembler
   operation that should precede instructions (and maybe read-only
   data).  */
char *pdp10_text_section_asm_op;

/* A C expression whose value is a string containing the assembler
   operation that should precede read-only data.  */
char *pdp10_rodata_section_asm_op;

/* A C expression whose value is a string containing the assembler
   operation that should precede read-write data.  */
char *pdp10_data_section_asm_op;

/* A C expression whose value is a string containing the assembler
   operation that should precede uninitialized data.  */
char *pdp10_bss_section_asm_op;
#endif

/* Which processor to tune for.  */
int target_tune = 0;

/* Which code model to use.  */
int target_model = MODEL_NONE;

/* What byte size to use for string literals.  */
int pdp10_string_literal_byte_size = 9;

/* A table to match the canonical -mcpu name with the numerical cpu
   type, and also two bit masks to enable or disable features.  */
static struct { const char *name; int cpu; int enable; int disable; }
cpu_table[] =
{
  /* name,	cpu,		enable,		disable */

  { "166",	CPU_166,	0,		CPU_EXTENDED	},
  { "ka10",	CPU_KA10,	0,		CPU_EXTENDED	},
  { "ki10",	CPU_KI10,	0,		CPU_EXTENDED	},
  { "ks10",	CPU_KS10,	0,		CPU_EXTENDED	},
  { "kl10",	CPU_KL10,	CPU_EXTENDED,	0		},
  { "sc20",	CPU_KL10,	CPU_EXTENDED,	0		},
  { "sc25",	CPU_KL10,	CPU_EXTENDED,	0		},
  { "sc30m",	CPU_KL10,	CPU_EXTENDED,	0		},
  { "sc40",	CPU_KL10,	CPU_EXTENDED,	0		},
  { "xkl1",	CPU_XKL1,	CPU_EXTENDED,	0		},
  { "xkl2",	CPU_XKL2,	CPU_EXTENDED,	0		},
};

/* A table to find the canonical cpu name.  */
static struct { const char *alias; const char *name; }
cpu_alias[] =
{
  { "pdp6",	"166"	},
  { "ka",	"ka10"	},
  { "ki",	"ki10"	},
  { "kl",	"kl10"	},
  { "ks",	"ks10"	},
  { "sc30",	"sc30m"	},
};

/* Normalize the cpu name passed in NAME.  All characters are
   converted to lower-case, and '-' characters are removed.  Finally,
   cpu aliases are resolved.  */
static char *
normalize_cpu_name (name)
     char *name;
{
  char *p, *q;
  unsigned int i;

  if (name == NULL)
    return NULL;

  for (p = q = name; *p != 0; p++)
    {
      if (*p != '-')
	*q++ = TOLOWER (*p);
    }
  *q = 0;

  for (i = 0; i < ARRAY_SIZE (cpu_alias); i++)
    {
      if (strcmp (name, cpu_alias[i].alias) == 0)
	{
	  name = (char *)cpu_alias[i].name;
	  break;
	}
    }

  return name;
}

/* Return the number of registers available for -mregparm,
   -mreturn-regs, and -mcall-clobbered.  */
static int
available_regs ()
{
  static int n = -1;
  int i;

  if (n >= 0)
    return n;

  for (i = n = 0; i < FIRST_PSEUDO_REGISTER; i++, n++)
    if ((fixed_regs[i] && n != 0)
	|| i == HARD_FRAME_POINTER_REGNUM
	|| i == PIC_SOMETHING_REGNUM
	|| i == STATIC_CHAIN_REGNUM)
      break;

  return n;
}

/* Prefer the argument registers in reverse order.  The old code only
   did this when -mregparm=N was specified explicitly; after changing
   the default ABI to four register arguments, the implicit default
   must get the same allocation order as -mregparm=4.  */
static void
pdp10_set_reg_alloc_order ()
{
  static const int default_order[FIRST_PSEUDO_REGISTER] = {
     6,  7,  5,  4,  3,  2,  1,  8,
     9, 10, 11, 12, 13, 14, 15,  0,
    16, 17
  };
  int used[FIRST_PSEUDO_REGISTER];
  int i, j;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    used[i] = 0;

  j = 0;
  for (i = pdp10_regparm; i >= 1; i--)
    {
      reg_alloc_order[j++] = i;
      used[i] = 1;
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (!used[default_order[i]])
      reg_alloc_order[j++] = default_order[i];
}

/* Called by the OVERRIDE_OPTIONS macro.

   This is run after all command line options has been read, but
   before actual compilation has begun.  */
void
pdp10_override_options ()
{
  unsigned int i;

#if XKL_STUFF
  if (debug_info_level != DINFO_LEVEL_NONE)
    {
      extern int flag_thread_jumps, flag_cprop_registers, flag_loop_optimize;
      extern int flag_crossjumping, flag_gcse, flag_rerun_cse_after_loop;
      extern int flag_delete_null_pointer_checks;

      pdp10_regparm = 0;
      optimize = 0;

      flag_defer_pop = 0;
      flag_thread_jumps = 0;
#ifdef DELAY_SLOTS
      flag_delayed_branch = 0;
#endif
#ifdef CAN_DEBUG_WITHOUT_FP
      flag_omit_frame_pointer = 0;
#endif
      flag_guess_branch_prob = 0;
      flag_cprop_registers = 0;
      flag_loop_optimize = 0;
      flag_crossjumping = 0;
      flag_optimize_sibling_calls = 0;
      flag_cse_follow_jumps = 0;
      flag_cse_skip_blocks = 0;
      flag_gcse = 0;
      flag_expensive_optimizations = 0;
      flag_strength_reduce = 0;
      flag_rerun_cse_after_loop = 0;
      flag_rerun_loop_opt = 0;
      flag_caller_saves = 0;
      flag_force_mem = 0;
      flag_peephole2 = 0;
#ifdef INSN_SCHEDULING
      flag_schedule_insns = 0;
      flag_schedule_insns_after_reload = 0;
#endif
      flag_regmove = 0;
      flag_strict_aliasing = 0;
      flag_delete_null_pointer_checks = 0;
      flag_reorder_blocks = 0;
      flag_inline_functions = 0;
      flag_rename_registers = 0;
    }
#endif

  /* -mcpu is short for both -march and -mtune.  */
  if (pdp10_cpu)
    {
      pdp10_arch = pdp10_cpu;
      pdp10_tune = pdp10_cpu;
    }

  pdp10_arch = normalize_cpu_name (pdp10_arch);
  pdp10_tune = normalize_cpu_name (pdp10_tune);

  if (pdp10_arch || pdp10_tune)
    {
      for (i = 0; i < ARRAY_SIZE (cpu_table); i++)
	{
	  if (pdp10_arch && strcmp (pdp10_arch, cpu_table[i].name) == 0)
	    {
	      target_flags = (target_flags & ~CPU_MASK) | cpu_table[i].cpu;
	      target_flags |= cpu_table[i].enable;
	      target_flags &= ~cpu_table[i].disable;
	    }

	  if (pdp10_tune && strcmp (pdp10_tune, cpu_table[i].name) == 0)
	    target_tune = cpu_table[i].cpu;
	}
    }

  if (pdp10_arch && TARGET_CPU == 0)
    fprintf (stderr, "Unknown CPU: %s\n", pdp10_arch);
  if (pdp10_tune && target_tune == 0)
    fprintf (stderr, "Unknown CPU: %s\n", pdp10_tune);

  /* If no processor was selected explicitly, use the configured
     target baseline.  DAIMON sets this to PDP-6/166.  */
  if (TARGET_CPU == 0)
    target_flags = (target_flags & ~CPU_MASK) | TARGET_CPU_DEFAULT;
  if (target_tune == 0)
    target_tune = TARGET_CPU_DEFAULT & CPU_MASK;

  if ((target_flags & OPT_EXTENDED)
      && (target_flags & OPT_UNEXTENDED))
    fprintf (stderr,
	     "Can't use -mextended and -munextended at the same time\n");

  /* Floating point is deliberately outside the DAIMOS PDP-6/KA10
     baseline for now.  These options are accepted only as future
     placeholders so build scripts and tests can spell the intended
     capability without silently enabling any FP instruction patterns.  */
  if ((target_flags & OPT_PDP6_FLOAT) && (target_flags & OPT_KA10_FLOAT))
    fprintf (stderr,
	     "warning: -mpdp6-float and -mka10-float are placeholders and cannot select a working FP backend yet\n");

  if (target_flags & OPT_PDP6_FLOAT)
    {
      if (!TARGET_166)
	fprintf (stderr,
		 "warning: -mpdp6-float is intended for -march=166 only\n");
      fprintf (stderr,
	       "warning: -mpdp6-float is a placeholder; PDP-6 FP codegen is not implemented and will not be upward-compatible with KA10/PDP-10\n");
    }

  if (target_flags & OPT_KA10_FLOAT)
    fprintf (stderr,
	     "warning: -mka10-float is a placeholder; KA10/PDP-10 FP codegen is not implemented in the DAIMOS baseline yet\n");

  if (target_flags & OPT_NO_FLOAT)
    {
      target_flags &= ~CPU_GFLOAT;
      target_flags &= ~OPT_GFLOAT;
    }

  if (target_flags & OPT_EXTENDED)
    target_flags |= CPU_EXTENDED;
  if (target_flags & OPT_UNEXTENDED)
    target_flags &= ~CPU_EXTENDED;

  /* Some test and multilib harnesses append an explicit -mdfloat after
     per-test options.  A plain CPU_GFLOAT bit can therefore be cleared
     again even though the user/test selected -mgfloat.  Keep -mgfloat
     sticky through OPT_GFLOAT and restore the implementation bit here.
     This intentionally makes explicit -mgfloat win over a default or
     trailing -mdfloat, which is what per-test flags need.  */
  if (target_flags & OPT_GFLOAT)
    target_flags |= CPU_GFLOAT;

  /*
   * DAIMON defaults ordinary long long to the PDP-10 hardware-style
   * 71-bit doubleword integer model.  Raw two-word 72-bit arithmetic
   * remains available only when explicitly requested with
   * -mlong-long-72bit.
   *
   * If both command-line forms appear, make the explicit raw 72-bit
   * request win.  This mirrors the CPP spec, which defines
   * __LONG_LONG_72BIT__ whenever -mlong-long-72bit is present and
   * otherwise defines __LONG_LONG_71BIT__.
   */
  if (target_flags & OPT_LONG_LONG_72BIT)
    {
      target_flags &= ~CPU_71BIT;
      target_flags &= ~OPT_LONG_LONG_71BIT;
    }
  else
    target_flags |= CPU_71BIT;

  if (pdp10_model)
    {
      if (strcmp (pdp10_model, "small") == 0)
	target_model = MODEL_SMALL;
      else if (strcmp (pdp10_model, "smallish") == 0)
	target_model = MODEL_SMALLISH;
      else if (strcmp (pdp10_model, "medium") == 0)
	target_model = MODEL_MEDIUM;
      else if (strcmp (pdp10_model, "large") == 0)
	target_model = MODEL_LARGE;
      else
	fprintf (stderr, "Unknown code model: %s\n", pdp10_model);
    }

  /* The default program model is small for unextended processors,
     smallish for XKL processors, and large for other extended
     processors.  */
  if (target_model == MODEL_NONE)
    {
      if (TARGET_EXTENDED)
	{
	  if (TARGET_XKL1 || TARGET_XKL2)
	    target_model = MODEL_SMALLISH;
	  else
	    target_model = MODEL_LARGE;
	}
      else
	target_model = MODEL_SMALL;
    }

#ifdef __PDP10_MACRO_H__
  if (TARGET_EXTENDED)
    {
      if (!pdp10_text_psect)
	pdp10_text_psect = (char *)".text";
      pdp10_text_section_asm_op = xmalloc (15 + strlen (pdp10_text_psect));
      sprintf (pdp10_text_section_asm_op, "\t.PSECT %s/ronly",
	       pdp10_text_psect);

      if (!pdp10_rodata_psect)
	pdp10_rodata_psect = (char *)".rodata";
      pdp10_rodata_section_asm_op = xmalloc (15 + strlen (pdp10_rodata_psect));
      sprintf (pdp10_rodata_section_asm_op, "\t.PSECT %s/ronly",
	       pdp10_rodata_psect);

      if (!pdp10_data_psect)
	pdp10_data_psect = (char *)".data";
      pdp10_data_section_asm_op = xmalloc (15 + strlen (pdp10_data_psect));
      sprintf (pdp10_data_section_asm_op, "\t.PSECT %s",
	       pdp10_data_psect);

      if (!pdp10_bss_psect)
	pdp10_bss_psect = (char *)".bss";
      pdp10_bss_section_asm_op = xmalloc (15 + strlen (pdp10_bss_psect));
      sprintf (pdp10_bss_section_asm_op, "\t.PSECT %s",
	       pdp10_bss_psect);
    }
  else
    {
      pdp10_text_section_asm_op = (char *)"";
      pdp10_rodata_section_asm_op = (char *)"";
      pdp10_data_section_asm_op = (char *)"";
      pdp10_bss_section_asm_op = (char *)"";
    }
#endif

  if (pdp10_argument_regs)
    {
      int i;

      pdp10_regparm = atoi (pdp10_argument_regs);
      if (pdp10_regparm < 0 || pdp10_regparm > available_regs () - 1)
	{
	  fprintf (stderr, "Bad value for -mregparm: %d\n", pdp10_regparm);
	  exit (1);
	}

    }

  pdp10_set_reg_alloc_order ();

  if (pdp10_return_regs)
    {
      pdp10_retregs = atoi (pdp10_return_regs);
      if (pdp10_retregs < 1 || pdp10_retregs > available_regs () - 1)
	{
	  fprintf (stderr, "Bad value for -mreturn-regs: %d\n",
		   pdp10_retregs);
	  exit (1);
	}
    }

  if (pdp10_call_clobbered)
    {
      int n = atoi (pdp10_call_clobbered);

      if (n < 0 || n > available_regs ())
	{
	  fprintf (stderr, "Bad value for -mcall-clobbered: %d\n", n);
	  exit (1);
	}
      else if (n <= pdp10_retregs)
	{
	  fprintf (stderr, "-mcall-clobbered=%d clash with -mreturn-regs=%d\n",
		   n, pdp10_retregs);
	  exit (1);
	}

      for (i = 0; i < (unsigned) n; i++)
	call_used_regs[i] = 1;
      for (; i < FIRST_PSEUDO_REGISTER; i++)
	call_used_regs[i] = fixed_regs[i];
    }

  if (pdp10_string_bytesize)
    {
      int n = atoi (pdp10_string_bytesize);

      if (n != 6 && n != 7 && n != 8 && n != 9)
	{
	  fprintf (stderr, "Bad value for -mstring-bytesize: %s\n",
		   pdp10_string_bytesize);
	  exit (1);
	}

      pdp10_string_literal_byte_size = n;
    }

  if (pdp10_char_bytesize_arg)
    {
      int n = atoi (pdp10_char_bytesize_arg);

      if (n != 6 && n != 7 && n != 8 && n != 9)
	{
	  fprintf (stderr, "Bad value for -mchar-bytesize: %s\n",
		   pdp10_char_bytesize_arg);
	  exit (1);
	}

      pdp10_char_bytesize = n;
    }

  ggc_add_rtx_root (&pdp10_compare_op0, 1);
  ggc_add_rtx_root (&pdp10_compare_op1, 1);
}


/**********************************************************************

	Storage Layout

**********************************************************************/

static int ps_base_for_bytesize PARAMS ((int size));

/* Called by the ALIGN_STACK_POINTER macro.  This generates code to
   align the stack pointer.  ALIGN is measured in addressable storage
   units.  PDP-10 stack addresses are word addresses, so convert the
   requested alignment to whole words before doing pointer arithmetic.  */
rtx
pdp10_align_stack_pointer (x, align)
     rtx x;
     unsigned align;
{
  /* A PDP-10 stack pointer is already word-aligned.  Do not reject
     ordinary alloca/stack-adjust paths on PDP-6, KA10, KI10, or KS10
     merely because they are non-extended machines.  */
  if (align <= UNITS_PER_WORD)
    return x;

  /* There is no way to represent sub-word stack alignment in a word
     pointer.  Round any unusual request up to a whole-word boundary
     instead of aborting during test compilation.  */
  align = (align + UNITS_PER_WORD - 1) / UNITS_PER_WORD;

  if (align <= 1)
    return x;

  x = expand_binop (Pmode, add_optab, x, GEN_INT (align - 1), NULL_RTX,
		    1, OPTAB_LIB_WIDEN);
  x = expand_divmod (0, TRUNC_DIV_EXPR, Pmode, x, GEN_INT (align),
		     NULL_RTX, 1);
  x = expand_mult (Pmode, x, GEN_INT (align), NULL_RTX, 1);

  return x;
}

/* Called by the DATA_ALIGNMENT macro.

   Computes the alignment for a variable in the static store.  TYPE is
   the data type, and ALIGN is the alignment that the object would
   ordinarily have.  The return value of this function is used instead
   of that alignment to align the object.  */
int
pdp10_data_alignment (align, type)
     int align;
     tree type ATTRIBUTE_UNUSED;
{
  /* All static data items are word-aligned.  */
  if (align < BITS_PER_WORD)
    align = BITS_PER_WORD;
  return align;
}

/* Called by the DATA_PADDING macro.  */
rtx
pdp10_data_padding (decl, x)
     tree decl;
     rtx x;
{
  int bytesize;
  pdp10_word36 offset = 0;
  pdp10_word36 encoded;
  HOST_WIDE_INT old_offset;
  int ps;
  int o = 0;
  rtx mem = NULL_RTX;

  if (TREE_TYPE (decl) == error_mark_node)
    return x;

  bytesize = pdp10_bytesize (TREE_TYPE (decl));
  if (bytesize >= 32)
    return x;

  /* DATA_PADDING may be called with either an address RTX or a MEM whose
     address must be adjusted.  The old code used plus_constant directly on
     X, which truncates 36-bit P,S byte-pointer bits through HOST_WIDE_INT
     on 32-bit hosts.  Work on the address explicitly, then rebuild the MEM
     if necessary.  */
  if (GET_CODE (x) == MEM)
    {
      mem = x;
      x = XEXP (x, 0);
    }

  if (GET_CODE (x) == CONST
      && GET_CODE (XEXP (x, 0)) == PLUS)
    {
      rtx addend = XEXP (XEXP (x, 0), 1);
      pdp10_word36 word36;

      if (pdp10_extract_word36_constant (addend, &word36))
        {
          offset = word36 & ADDRESS_MASK;
          x = XEXP (XEXP (x, 0), 0);
        }
      else if (GET_CODE (addend) == CONST_INT)
        {
          old_offset = INTVAL (addend);
          offset = old_offset & ADDRESS_MASK;
          x = XEXP (XEXP (x, 0), 0);
        }
    }

  if (TREE_CODE (decl) == VAR_DECL
      && !AGGREGATE_TYPE_P (TREE_TYPE (decl)))
    {
      /* Scalar sub-9-bit character variables occupy one 9-bit C storage
         unit on the stack/in static storage.  Addressing such a scalar
         through a pointer must therefore build a rightmost 9-bit byte
         pointer.  The older 18-bit selector accidentally read the whole
         right half of the word; that only worked when the neighbouring
         byte was zero and failed when a previous stack user left sign bits
         there.  */
      if (bytesize <= 9)
        bytesize = 9;
      o = BITS_PER_WORD / bytesize - 1;
    }

  if (TARGET_EXTENDED)
    {
      ps = ps_base_for_bytesize (bytesize) + o;
      ps <<= 6;
    }
  else
    ps = ((36 - bytesize * o - bytesize) << 6) + bytesize;

  encoded = (offset + (((pdp10_word36) ps) << 24)) & PDP10_WORD36_MASK;
  x = pdp10_plus_constant_word36 (x, encoded);

  if (mem != NULL_RTX)
    {
      rtx newmem = copy_rtx (mem);
      XEXP (newmem, 0) = x;
      return newmem;
    }

  return x;
}

/* Called by the CONSTANT_ALIGNMENT macro.

   Computes the alignment given to a constant that is being placed in
   memory.  EXP is the constant and ALIGN is the alignment that the
   object would ordinarily have.  The return value of this function is
   used instead of that alignment to align the object.  */
int
pdp10_constant_alignment (align, exp)
     int align;
     tree exp;
{
  /* String constants are placed at the start of a word.  */
  if (TREE_CODE (exp) == STRING_CST && align < BITS_PER_WORD)
    align = BITS_PER_WORD;
  return align;
}

/* Called by the LOCAL_ALIGNMENT macro.

   Computes the alignment for a variable in the local store.  TYPE is
   the data type, and ALIGN is the alignment that the object would
   ordinarily have.  The return value of this function is used instead
   of that alignment to align the object.  */
int
pdp10_local_alignment (align, type)
     int align;
     tree type ATTRIBUTE_UNUSED;
{
  if (align < BITS_PER_WORD)
    align = BITS_PER_WORD;
  return align;
}

/* Called by the LOCAL_SIZE macro.

   Computes the size of a variable in the local store.  TYPE is the
   data type, and SIZE is the size that the object would ordinarily
   have.  */
int
pdp10_local_size (size, type)
     int size;
     tree type ATTRIBUTE_UNUSED;
{
  if (size < UNITS_PER_WORD)
    size = UNITS_PER_WORD;
  return size;
}


/**********************************************************************

	Register Usage

**********************************************************************/

void
pdp10_conditional_register_usage ()
{
}


/**********************************************************************

	Register Classes

**********************************************************************/

/* Called by the CONST_OK_FOR_LETTER_P macro.

   Defines the machine-specific operand constraint letters that
   specify particular ranges of integer values.  */
int
pdp10_const_ok_for_letter_p (value, c)
     HOST_WIDE_INT value;
     int c;
{
  switch (c)
    {
    case 'I': return value >= 0 && value <= 0777777;
    case 'J': return value <= 0 && value >= -0777777;
    case 'K': return value >= -0400000 && value <= 0377777;
    case 'L': return 
		(value & 0777777) == 0
		&& pdp10_const_ok_for_letter_p ((value >> 18) & 0777777, 'I');
    case 'M': return value == -1;
    case 'N': return 
		(value & 0777777) == 0777777
		&& pdp10_const_ok_for_letter_p ((value >> 18) & 0777777, 'I');
    case 'O': return value == 0;
    case 'P': return pdp10_const_ok_for_letter_p (~value, 'I');
    }
  return 0;
}

/* Called by the CONST_DOUBLE_OK_FOR_LETTER_P macro.

   Defines the machine-dependent operand constraint letters that
   specify particular ranges of CONST_DOUBLE values.  */
int
pdp10_const_double_ok_for_letter_p (x, c)
     rtx x;
     int c;
{
  REAL_VALUE_TYPE r;
  long y[3];

  if (c != 'G')
    return 0;

  REAL_VALUE_FROM_CONST_DOUBLE (r, x);
  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

  switch (GET_MODE (x))
    {
    case VOIDmode:
      return 0;
    case SFmode:
      return
	(y[0] & 0x00003fffUL) == 0
	&& (y[1] & 0xf0000000UL) == 0;
    case DFmode:
      return
	(y[0] & 0x00003fffUL) == 0
	&& (y[1] & 0xffffffffUL) == 0
	&& (y[2] & 0xff000000UL) == 0;
    default:
      abort ();
    }
}

/* Called by the EXTRA_CONSTRAINT macro.

   Defines the optional machine-dependent constraint letters that can
   be used to segregate specific types of operands for the target
   machine.  */
int
pdp10_extra_constraint (x, c)
     rtx x;
     int c;
{
  switch (c)
    {
    case 'Q':
      return GET_CODE (x) == CONST_INT && INTVAL (x) == 1;
    case 'R':
      return pdp10_const_double_0_operand (x, GET_MODE (x));
    case 'S':
      {
	HOST_WIDE_INT offset = 0;
	enum rtx_code code;
	int ps = 0;

	if ((TARGET_EXTENDED && !TARGET_SMALLISH)
	    || ((code = GET_CODE (x)) != SYMBOL_REF
		&& code != LABEL_REF
		&& code != CONST))
	  return 0;

	if (code == CONST)
	  {
	    x = XEXP (x, 0);
	    offset = INTVAL (XEXP (x, 1));
	    x = XEXP (x, 0);
	    if ((offset & ~ADDRESS_MASK)
		!= (((HOST_WIDE_INT)-1) & ~ADDRESS_MASK))
	      ps = (offset >> 24) & 07700;
	    offset &= ADDRESS_MASK;
	  }
	return ps == 0;
      }
    }
  return 0;
}


/**********************************************************************

	Stack Layout and Calling Conventions

**********************************************************************/

static int	clobbered_regs PARAMS ((int *clobbered));

/* Called by the STARTING_FRAME_OFFSET macro.  */
int
pdp10_starting_frame_offset ()
{
  return 0;
}

/* Called by the STACK_POINTER_OFFSET macro.  */
int
pdp10_stack_pointer_offset ()
{
  return 1;
}

/* Called by the FIRST_PARM_OFFSET macro.  */
int
pdp10_first_parm_offset ()
{
  tree attr = DECL_ATTRIBUTES (current_function_decl);

  if (attr != NULL_TREE && lookup_attribute ("noreturn", attr))
    return 0;

  return -1;
}

/* Called by the STACK_DYNAMIC_OFFSET macro.  */
int
pdp10_stack_dynamic_offset ()
{
  return 1;
}

/* Called by the INITIAL_ELIMINATION_OFFSET macro.  */
int
pdp10_initial_elimination_offset (from, to)
     int from;
     int to;
{
  if (from == ARG_POINTER_REGNUM || from == HARD_FRAME_POINTER_REGNUM)
    {
      int clobbered[FIRST_PSEUDO_REGISTER];
      int clobbered_size = clobbered_regs (clobbered);
      if (to == STACK_POINTER_REGNUM)
	return -((get_frame_size () + current_function_outgoing_args_size
		  + clobbered_size - UNITS_PER_WORD) / UNITS_PER_WORD);
      else if (to == HARD_FRAME_POINTER_REGNUM)
	return 0;
    }
  else if (from == FRAME_POINTER_REGNUM)
    {
      if (to == STACK_POINTER_REGNUM)
	return -((get_frame_size () + current_function_outgoing_args_size
		  - UNITS_PER_WORD) / UNITS_PER_WORD);
      else if (to == HARD_FRAME_POINTER_REGNUM)
	{
	  int clobbered[FIRST_PSEUDO_REGISTER];
	  int clobbered_size = clobbered_regs (clobbered);
	  return clobbered_size / UNITS_PER_WORD;
	}
    }

  abort ();
}

/* Stack layout, before function prologue:

                ...
         [     arg 6    ]
         [     arg 5    ]
   sp -> [return address]

   Stack layout, after fixed-arg function prologue:

                ...
         [     arg 6    ]
         [     arg 5    ]
	 [return address]
   fp -> [ saved reg 16 ]
         [ saved reg  5 ]
         [ saved reg  6 ]
         [ saved reg  7 ]
         [ saved reg 10 ]
         [ saved reg 11 ]   Registers 5 - 16 are saved if clobbered by
         [ saved reg 12 ]   the function.
         [ saved reg 13 ]
         [ saved reg 14 ]
         [ saved reg 15 ]
         [ frame slot 1 ]
         [ frame slot 2 ]   Local variables.
         [ frame slot 3 ]
               ...
         [outgoing arg 3]
         [outgoing arg 2]   Reserved space for outgoing arguments.
   sp -> [outgoing arg 1]

   Stack layout, after vararg function prologue:

                ...
         [     arg 6    ]
         [     arg 5    ]
         [     arg 4    ]   Args 1-4 has been pushed to stack.
         [     arg 3    ]
         [     arg 2    ]
         [     arg 1    ]
	 [return address]   Return address has been moved.
   fp -> [ saved reg 16 ]
         [ saved reg  5 ]
         [ saved reg  6 ]
         [ saved reg  7 ]
         [ saved reg 10 ]
         [ saved reg 11 ]   Registers 5 - 16 are saved if clobbered by
         [ saved reg 12 ]   the function.
         [ saved reg 13 ]
         [ saved reg 14 ]
         [ saved reg 15 ]
         [ frame slot 1 ]
         [ frame slot 2 ]   Local variables.
         [ frame slot 3 ]
               ...
         [outgoing arg 3]
         [outgoing arg 2]   Reserved space for outgoing arguments.
   sp -> [outgoing arg 1]

   Stack layout, after vararg function epilogue, but before return:

                ...
         [     arg 6    ]
         [     arg 5    ]
   sp -> [return address]   Return address has been moved back again.
*/

/* Called by the FUNCTION_PROLOGUE macro.  */
void
pdp10_output_func_prologue (stream, frame_size)
     FILE *stream ATTRIBUTE_UNUSED;
     HOST_WIDE_INT frame_size ATTRIBUTE_UNUSED;
{
}

/* Called by the FUNCTION_EPILOGUE macro.  */
void
pdp10_output_func_epilogue (f, frame_size)
     FILE *f;
     HOST_WIDE_INT frame_size ATTRIBUTE_UNUSED;
{
  fputc ('\n', f);
}

/* Called by the FUNCTION_PROFILER macro.  */
void
pdp10_function_profiler (file, labelno)
     FILE *file ATTRIBUTE_UNUSED;
     int labelno ATTRIBUTE_UNUSED;
{
}

/* Called by the SETUP_INCOMING_VARARGS macro.  */
int
pdp10_setup_incoming_varargs (cum, pretend_size)
     CUMULATIVE_ARGS cum ATTRIBUTE_UNUSED;
     int pretend_size;
{
  return pretend_size;
}

/* Return the hard-register mode used for a scalar function value.

   PROMOTE_FUNCTION_RETURN asks GCC to apply PROMOTE_MODE to return
   values, so FUNCTION_VALUE must name the same promoted hard-register
   mode.  Keeping this in C rather than trying to duplicate the
   statement-like PROMOTE_MODE macro in pdp10.h avoids caller/callee
   mismatches for QImode and HImode results.  */
enum machine_mode
pdp10_function_value_mode (type)
     tree type;
{
  enum machine_mode mode;
  int unsignedp;

  mode = TYPE_MODE (type);
  unsignedp = TREE_UNSIGNED (type);
  PROMOTE_MODE (mode, unsignedp, type);
  return mode;
}


/**********************************************************************

	Addressing Modes

**********************************************************************/

static int	index_register PARAMS ((rtx x, int strict));

/* Return nonzero if X is a valid index register, zero otherwise.  If
   STRICT is nonzero, all pseudo registers qualify.  */
static int
index_register (x, strict)
     rtx x;
     int strict;
{
  if (GET_CODE (x) == SUBREG)
    x = SUBREG_REG (x);
  if (!REG_P (x))
    return 0;
  if (REGNO (x) < FIRST_PSEUDO_REGISTER)
    return REGNO_REG_CLASS (REGNO (x)) == INDEX_REGS;
  else
    return !strict;
}

/* Called by the GO_IF_LEGITIMATE_ADDRESS macro.  Returns nonzero if X
   is a valid address for accessing something of mode MODE.  */
int
pdp10_legitimate_address_p (mode, x, strict)
     enum machine_mode mode;
     rtx x;
     int strict;
{
  /* A constant address is always ok.  */
  if (CONSTANT_ADDRESS_P (x))
    return 1;

  switch (GET_CODE (x))
    {
      /* This seems to be needed sometimes.  */
    case SUBREG:
      x = SUBREG_REG (x);
      if (GET_CODE (x) != REG)
	return 0;
      /* Fall through.  */

      /* A register without an offset is always a valid address.
	 Register 0 can be used with indirect addressing.  */
    case REG:
      return !strict || REGNO (x) < FIRST_PSEUDO_REGISTER;

      /* A sum must be an index register plus a valid constant offset.  */
    case PLUS:
      if (!index_register (XEXP (x, 0), strict))
	return 0;

      /* In general, indexing can't be used with byte pointers.  Only
	 allow it with the stack pointer as the index register.  */
      if (BYTE_MODE_P (mode) && !REGNO_PTR_FRAME_P (REGNO (XEXP (x, 0))))
	return 0;

      x = XEXP (x, 1);
      switch (GET_CODE (x))
	{
	case CONST_INT:
	  /* Checking no_new_pseudos ensures that a global format
	     indirect word is not valid when generating RTL, but it's
	     valid when the instruction combination pass is running.  */
	  if (no_new_pseudos)
	    /* Global format indirect word.  */
	    return 1;
	  else
	    {
	      if (TARGET_EXTENDED)
		return INTVAL (x) >= -0400000 && INTVAL (x) < 0400000;
	      else
		return INTVAL (x) > -01000000 && INTVAL (x) < 01000000;
	    }
	case CONST:
	case LABEL_REF:
	case SYMBOL_REF:
	  return !TARGET_EXTENDED || TARGET_SMALLISH;
	default:
	  return 0;
	}

      /* Pre-incrementing works with byte pointers.  The ILDB/IDPB
	 patterns can present PRE_INC addresses all the way to final
	 operand printing, so this must be accepted by the strict address
	 checker too.  */
    case PRE_INC:
      x = XEXP (x, 0);
      if (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);
      return BYTE_MODE_P (mode)
	&& REG_P (x)
	&& (!strict || REGNO (x) < FIRST_PSEUDO_REGISTER);

      /* Indirect addressing.  */
    case MEM:
      if (BYTE_MODE_P (mode))
	return pdp10_legitimate_address_p (Pmode, XEXP (x, 0), strict);

      x = XEXP (x, 0);
      if (REG_P (x))
	return index_register (x, strict);
      else if (GET_CODE (x) == PLUS)
	return
	  index_register (XEXP (x, 0), strict)
	  && (((!TARGET_EXTENDED || TARGET_SMALLISH)
	       && CONSTANT_ADDRESS_P (XEXP (x, 1)))
	      || (GET_CODE (XEXP (x, 1)) == CONST_INT
		  && pdp10_const_ok_for_letter_p (INTVAL (XEXP (x, 1)), 'K')));
      /* Fall through.  */

    default:
      return 0;
    }
}

/* Called by the LEGITIMIZE_ADDRESS macro.

   Attempts to replace X with a valid memory address for an operand of
   mode MODE.  Returns NULL_RTX if no valid memory address was found.  */
rtx
pdp10_legitimize_address (x, oldx, mode)
     rtx x;
     rtx oldx ATTRIBUTE_UNUSED;
     enum machine_mode mode;
{
  HOST_WIDE_INT max_offset = TARGET_EXTENDED ? 0377777 : 0777777;
  HOST_WIDE_INT min_offset = TARGET_EXTENDED ? -0400000 : -0777777;
  rtx x0, x1;

  if (mode == BLKmode)
    return NULL_RTX;

  if (GET_CODE (x) != PLUS)
    return NULL_RTX;

  x0 = XEXP (x, 0);
  x1 = XEXP (x, 1);

  if (REG_P (x0)
      && GET_CODE (x1) == CONST_INT
      && (INTVAL (x1) < min_offset || INTVAL (x1) > max_offset))
    {
      HOST_WIDE_INT offset = INTVAL (x1);
      HOST_WIDE_INT mask = 0777777;
      HOST_WIDE_INT sign = 0400000;
      rtx high_reg = gen_reg_rtx (Pmode);
      rtx base_reg = gen_reg_rtx (Pmode);
      rtx high_part;

      if (!TARGET_EXTENDED)
	error ("address too large for unextended mode");
      else if (offset <= -(HWINT (1) << 30)
	       || offset >= (HWINT (1) << 30))
	error ("address too large for extended mode");

      high_part = gen_int_mode ((offset + sign) & ~mask, Pmode);
      /*emit_insn (gen_rtx_SET (VOIDmode, high_reg, gen_ADDRESS (high_part)));*/
      emit_move_insn (high_reg, high_part);
      emit_insn (gen_addsi3 (base_reg, x0, high_reg));
      return plus_constant (base_reg,
			    trunc_int_for_mode (((offset & mask) ^ sign) - sign,
						Pmode));
    }

  return NULL_RTX;
}


/**********************************************************************

	Dividing the Output into Sections

**********************************************************************/

/* Called by the ENCODE_SECTION_INFO macro.

   DECL is a declaration.  If it's a function definition, note that
   the function is defined in the current translation unit.  Also
   record the type of all symbols.  */
void
pdp10_encode_section_info (decl)
     tree decl;
{
  if (TREE_CODE (decl) == FUNCTION_DECL)
    {
      /* Record whether the function is known to be defined in this
         unit or not.  */
      if (!TREE_PUBLIC (decl) && !DECL_WEAK (decl))
	SYMBOL_REF_INTERNAL (XEXP (DECL_RTL (decl), 0)) = 1;
    }
}


/**********************************************************************

	Describing Relative Costs of Operations

**********************************************************************/

/* Called by the CONST_COSTS macro.

   Describes the relative consts of CONST_INT expressions.  */
int
pdp10_const_int_costs (value, outer_code)
     HOST_WIDE_INT value;
     enum rtx_code outer_code;
{
  char c = 'I';

  switch (outer_code)
    {
      /* Shift instructions take a signed 18-bit value.  */
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
    case ROTATE:
    case ROTATERT:
      c = 'K';
      break;

      /* TLZ and TRZ take 18-bit one's complemented values.  AND takes
	 an 18-bit value.  */
    case AND:
      if (CONST_OK_FOR_LETTER_P (value, 'N')	/* TLZ */
	  || CONST_OK_FOR_LETTER_P (value, 'P'))	/* TRZ */
	return 0;
      break;

      /* TLO and TLC take an unsigned 18-bit value in the upper 18
	 bits.  IOR and XOR take an unsigned 18-bit value.  */
    case IOR:
    case XOR:
      if (CONST_OK_FOR_LETTER_P (value, 'L'))	/* TLO and TLC */
	return 0;
      break;

    case SET:
      if (CONST_OK_FOR_LETTER_P (value, 'L')	/* MOVS */
	  || CONST_OK_FOR_LETTER_P (value, 'N')	/* HRLO */
	  || CONST_OK_FOR_LETTER_P (value, 'P'))	/* HRRO */
	return 0;
      /* Fall through.  */

    case PLUS:
      if (CONST_OK_FOR_LETTER_P (value, 'J'))	/* SUBI and MOVN */
	return 0;
      /* ADDI and MOVEI take an unsigned 18-bit value.  */
      break;

    case NEG:
      /* Changing this may affect right-shifts.  */
      return 0;

    default:
      break;
    }

  return CONST_OK_FOR_LETTER_P (value, c) ? 0 : COSTS_N_INSNS (1) / 4;
}

/* Called by the CONST_COSTS macro.

   Describes the relative consts of CONST_DOUBLE expressions.  */
int
pdp10_const_double_costs (x, code, outer_code)
     rtx x;
     enum rtx_code code ATTRIBUTE_UNUSED;
     enum rtx_code outer_code ATTRIBUTE_UNUSED;
{
  /* A floating-point immediate costs nothing.  */
  if (pdp10_const_double_ok_for_letter_p (x, 'G'))
    return 0;

  return COSTS_N_INSNS (1);
}

/* Called by the RTX_COSTS macro.

   Describes the relative consts of nonconstant expressions.  */
int
pdp10_operand_cost (x, outer_code)
     rtx x;
     enum rtx_code outer_code;
{
  enum rtx_code code;
  enum machine_mode mode;
  enum machine_mode inner_mode;
  int cost;

  code = GET_CODE (x);

  switch (code)
    {
    CONST_COSTS (x, code, outer_code)

    case REG:
      return COSTS_N_INSNS (1);

    case MEM:
      return
      (GET_MODE (x) == SImode ?
       COSTS_N_INSNS (1) / 4 : COSTS_N_INSNS (1) / 2)
      + ADDRESS_COST (XEXP (x, 0));

    case NOT:
    case NEG:
    case USE:
    case SUBREG:
    case LSHIFTRT:
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
      return pdp10_operand_cost (XEXP (x, 0), outer_code);

    case ZERO_EXTEND:
    case SIGN_EXTEND:
    case TRUNCATE:
      cost = pdp10_operand_cost (XEXP (x, 0), outer_code);
      mode = GET_MODE (x);
      inner_mode = GET_MODE (XEXP (x, 0));

      /* These nodes are often part of a larger, matched PDP-10 idiom.
	 For example, xordisi3 represents a replicated 36-bit mask as
	 (zero_extend:DI mask) plus ((zero_extend:DI mask) << 36),
	 but the final instruction sequence is just two XOR instructions.
	 Still give standalone conversions a real, nonzero cost so CSE and
	 combine do not treat widening or truncation as free.  */
      if (mode == VOIDmode || inner_mode == VOIDmode)
	return cost;
      if (GET_MODE_SIZE (mode) == GET_MODE_SIZE (inner_mode))
	return cost;
      if (GET_MODE_SIZE (mode) <= UNITS_PER_WORD
	  && GET_MODE_SIZE (inner_mode) <= UNITS_PER_WORD)
	return cost + COSTS_N_INSNS (1);
      if (code == TRUNCATE)
	return cost + COSTS_N_INSNS (1);
      return cost + COSTS_N_INSNS (2);

    case PLUS:
    case AND:
    case IOR:
    case XOR:
      return COSTS_N_INSNS (1)
	+ pdp10_operand_cost (XEXP (x, 0), outer_code)
	+ pdp10_operand_cost (XEXP (x, 1), outer_code);

      /* Not a valid address operand.  Keep this expensive rather than
	 aborting: MINUS can appear transiently before address legitimization
	 has had a chance to split it.  */
    case MINUS:
      return COSTS_N_INSNS (1000);

    case MULT:
      return (GET_MODE (x) == SImode ? COSTS_N_INSNS (5) : COSTS_N_INSNS (10))
	+ pdp10_operand_cost (XEXP (x, 0), outer_code)
	+ pdp10_operand_cost (XEXP (x, 1), outer_code);

    case UDIV:
    case UMOD:
      return COSTS_N_INSNS (TARGET_XKL2 ? 20 : 50)
	+ pdp10_operand_cost (XEXP (x, 0), outer_code)
	+ pdp10_operand_cost (XEXP (x, 1), outer_code);

    case DIV:
    case MOD:
      return (optimize_size ? COSTS_N_INSNS (3) / 2 : COSTS_N_INSNS (20))
	+ pdp10_operand_cost (XEXP (x, 0), outer_code)
	+ pdp10_operand_cost (XEXP (x, 1), outer_code);

    case ASHIFT:
    case ASHIFTRT:
    case ROTATE:
    case ROTATERT:
      return COSTS_N_INSNS (1)
	+ pdp10_operand_cost (XEXP (x, 0), outer_code)
	+ pdp10_operand_cost (XEXP (x, 1), outer_code);

    case ADDRESSOF:
      /* Dunno.  Try this.  */
      return COSTS_N_INSNS (1);

    case UNSPEC:
      switch (XINT (x, 1))
	{
	case UNSPEC_ADJBP:
	  return COSTS_N_INSNS (100)
	    + pdp10_operand_cost (XVECEXP (x, 0, 0), outer_code);
	case UNSPEC_ADDRESS:
	  return COSTS_N_INSNS (1);
	default:
	  return COSTS_N_INSNS (1);
	}

    default:
      debug_rtx (x);
      abort ();
    }
}

/* Called by the ADDRESS_COST macro.

   Gives the cost of an addressing mode that contains X.  */
int
pdp10_address_cost (x)
     rtx x;
{
  if (GET_CODE (x) == ADDRESSOF)
    return 0;
  else if (CONSTANT_ADDRESS_P (x))
    return
      TARGET_EXTENDED
      && !TARGET_SMALLISH
      && GET_CODE (x) != LABEL_REF
      && !(GET_CODE (x) == SYMBOL_REF && SYMBOL_REF_INTERNAL (x))
      ? COSTS_N_INSNS (1) / 2 : COSTS_N_INSNS (1) / 4;
  else if (REG_P (x))
    return COSTS_N_INSNS (1) / 2;
  else if (GET_CODE (x) == PLUS)
    return COSTS_N_INSNS (1) / 2;
  else if (GET_CODE (x) == PRE_INC)
    return REG_P (XEXP (x, 0)) ?
      COSTS_N_INSNS (1) : COSTS_N_INSNS (2);
  else if (GET_CODE (x) == MEM)
    {
      x = XEXP (x, 0);
      if (REG_P (x))
	return COSTS_N_INSNS (2);
      else if (GET_CODE (x) == PLUS)
	return COSTS_N_INSNS (2);
      return COSTS_N_INSNS (3);
    }
  else if (GET_CODE (x) == SUBREG && REG_P (SUBREG_REG (x)))
    return COSTS_N_INSNS (1) / 2;
  else
    return MAX_COST;

  /* Not a valid address.  */
  debug_rtx (x);
  abort ();
  return -1;
}


/**********************************************************************

	Defining the Output Assembler Language

**********************************************************************/

static const char *
		condition_string PARAMS ((enum rtx_code condition));
static void	print_reg PARAMS ((FILE *stream, int regno));
static void	print_byte_pointer PARAMS ((FILE *stream, rtx x,
					    int size, int pos));
static void	print_word_pointer PARAMS ((FILE *stream, rtx x, int));
static void	pdp10_print_raw_word_constant PARAMS ((FILE *stream, rtx x));
static void	pdp10_print_word_decimal PARAMS ((FILE *stream,
					      HOST_WIDE_INT x));
static void	print_symbolic_ref PARAMS ((FILE *stream, rtx x));
static int	decode_ps PARAMS ((int ps, int *p, int *s));
/*static*/ int	adjust_ps PARAMS ((int ps, int *adjust));
static void	maybe_remove_label_and_following_code PARAMS ((rtx label));
static rtx	follow_label PARAMS ((rtx label));

/* Called by the ASM_DECLARE_OBJECT_NAME macro.

   Outputs to the stdio stream STREAM any text necessary for declaring
   the name NAME of an initialized variable which is being defined.  */
void
pdp10_asm_declare_object_name (stream, name, decl)
     FILE *stream;
     const char *name;
     tree decl;
{
  int byte_size;

#ifndef __PDP10_GAS_H__
  pdp10_extern_symbol (name, PDP10_SYMBOL_DEFINED);
#endif

  ASM_OUTPUT_LABEL (stream, name);

  /* If this is a scalar integer variable smaller than a word, justify
     it to the right-most addressable position inside a word.  */
  if (TREE_CODE (decl) == VAR_DECL
      && TREE_CODE (TREE_TYPE (decl)) == INTEGER_TYPE
      && (byte_size = TYPE_PRECISION (TREE_TYPE (decl))) < 36)
    {
      int pad = 36 - byte_size - 36 % byte_size;
      pdp10_output_byte (stream, const0_rtx, pad);
    }
}

/* Called by the ASM_OUTPUT_LABELREF macro.

   Outputs to the stdio stream FILE a reference in assembler syntax to
   a label named NAME.  */
void
pdp10_asm_output_labelref (file, name)
     FILE *file;
     const char *name;
{
  int len;
  char *label;
  int i;

#ifndef __PDP10_GAS_H__
  pdp10_output_byte (file, 0, 0);
#endif

  if (*name == '*')
    {
      fputs (name + 1, file);
      return;
    }

#ifdef __PDP10_GAS_H__
  asm_fprintf (file, "%U%s", name);
#else
  /* Translate all '_' characters into '.'.  */
  len = strlen (name);
  label = alloca (len + 1);
  strcpy (label, name);
  for (i = 0; i < len; i++)
    {
      if (label[i] == '_')
        label[i] = '.';
    }

  asm_fprintf (file, "%U%s", label);
#endif
}

/* Called by the ASM_OUTPUT_SYMBOL_REF macro.

   Outputs a reference to SYMBOL_REF SYM.  */
void
pdp10_asm_output_symbol_ref (stream, sym)
     FILE *stream;
     rtx sym;
{
  const char *name = XSTR (sym, 0);
#ifndef __PDP10_GAS_H__
  int len;
  char *label;
  int i;
#else
  const char *label;
#endif

  if (*name == '*')
    {
      fputs (name + 1, stream);
      return;
    }

  {
    tree id = maybe_get_identifier (name);
    if (id)
      TREE_SYMBOL_REFERENCED (id) = 1;
  }

#ifndef __PDP10_GAS_H__
  pdp10_extern_symbol (name, PDP10_SYMBOL_USED);
#endif

#ifdef __PDP10_GAS_H__
  label = name;
#else
  len = strlen (name);
  label = alloca (len + 1);
  strcpy (label, name);
  for (i = 0; i < len; i++)
    {
      if (label[i] == '_')
	label[i] = '.';
    }
#endif

  asm_fprintf (stream, "%U%s", label);
}

/* Print a symbolic reference in instruction/address operands.

   Do not use output_addr_const directly for SYMBOL_REF here.  In this
   backend, non-GAS assembler output maps C '_' to PDP-10 '.', e.g.
   arctan_s -> arctan.s.  Function labels already use that mapping via
   ASM_OUTPUT_LABEL, so references must use the same path.

   LABEL_REF is left to GCC's normal printer.  The mismatch seen in
   generated code was specifically SYMBOL_REF:

        arctan.s:
                pushj 17,arctan_s
 */
static void
print_symbolic_ref (stream, x)
     FILE *stream;
     rtx x;
{
  if (GET_CODE (x) == SYMBOL_REF)
    {
      pdp10_asm_output_symbol_ref (stream, x);
      return;
    }

  output_addr_const (stream, x);
}

/* Indentation level for the instruction being output.  Indentation is
   enabled with the -mindent-skipped option.  */
int pdp10_opcode_indentation = 0;

/* Print instruction opcode name INSN on STREAM.  */
const char *
pdp10_print_insn (stream, insn)
     FILE *stream;
     const char *insn;
{
  static const char spaces[] = "          ";
  const int max_spaces = sizeof spaces - 1;
  static rtx last_output_insn = NULL_RTX;

  if (target_flags & ASM_INDENT_SKIPPED)
    {
      if (pdp10_opcode_indentation > max_spaces)
	pdp10_opcode_indentation = max_spaces;
      fputs (spaces + max_spaces - pdp10_opcode_indentation, stream);
    
      if (get_attr_skip (current_output_insn) == SKIP_YES
	  && current_output_insn != last_output_insn)
	pdp10_opcode_indentation++;
      else
	pdp10_opcode_indentation = 0;
      last_output_insn = current_output_insn;
    }

  while (*insn != ' ' && *insn != 0 && *insn != '%')
    {
      int ch = *insn++;
      if (target_flags & ASM_UPPER)
	ch = TOUPPER (ch);
      fputc (ch, stream);
    }
  return insn;
}

/* Return a string for a condition, as used in many conditional PDP-10
   instructions.  */
static const char *
condition_string (condition)
     enum rtx_code condition;
{
  static const char *upper[] = { "", "L", "E", "LE", "A", "GE", "N", "G" };
  static const char *lower[] = { "", "l", "e", "le", "a", "ge", "n", "g" };
  const char **string = target_flags & ASM_UPPER ? upper : lower;

  switch (condition)
    {
   /* never: return string[0]; */
    case LT: return string[1];
    case EQ: return string[2];
    case LE: return string[3];
  /* always: return string[4]; */
    case GE: return string[5];
    case NE: return string[6];
    case GT: return string[7];
    default: abort ();
    }
}

/* Decode PS, the 12-bit P and S field from a byte pointer, and store
   the resulting byte position in *P and byte size in *S.  Return 0 if
   PS is a legal byte, or else -1.  */
static int
decode_ps (ps, pp, sp)
     int ps;
     int *pp;
     int *sp;
{
  int p, s;

  if (ps >= 04600)
    {
      int base;

      if (ps & 077)
	return -1;

      ps = ps >> 6;
      switch (ps)
	{
	case 046: case 047: case 050: case 051:	case 052: case 053:
	  base = 046;
	  s = 6;
	  break;
	case 062: case 063: case 064: case 065: case 066:
	  base = 062;
	  s = 7;
	  break;
	case 055: case 056: case 057: case 060:
	  base = 055;
	  s = 8;
	  break;
	case 070: case 071: case 072: case 073:
	  base = 070;
	  s = 9;
	  break;
	case 075: case 076:
	  base = 075;
	  s = 18;
	  break;
	default:
	  return -1;
	}

      p = s * (ps - base);
    }
  else
    {
      s = ps & 077;
      p = 36 - s - (ps >> 6);
    }

  *sp = s;
  *pp = p;
  return 0;
}

static int
ps_base_for_bytesize (size)
     int size;
{
  switch (size)
    {
    case 6:
      return 046;
    case 7:
      return 062;
    case 8:
      return 055;
    case 9:
      return 070;
    case 16:
    case 18:
      return 075;
    case 32:
    case 36:
      return 0;
    default:
      abort ();
    }

#if 0
  switch (size)
    {
    case 6:
      return 03606;
    case 7:
      return 03507;
    case 8:
      return 03410;
    case 9:
      return 03311;
    case 16:
    case 18:
      return 02222;
    case 32:
    case 36:
      return 0;
    default:
      abort ();
    }
#endif
}

int
bytesize_for_ps (ps)
     int ps;
{
  switch (ps)
    {
    case 0:
      return 36;
    case 045:
    case 046:
    case 047:
    case 050:
    case 051:
    case 052:
    case 053:
      return 6;
    case 054:
    case 055:
    case 056:
    case 057:
    case 060:
      return 8;
    case 061:
    case 062:
    case 063:
    case 064:
    case 065:
    case 066:
      return 7;
    case 067:
    case 070:
    case 071:
    case 072:
    case 073:
      return 9;
    case 074:
    case 075:
    case 076:
      return 18;
    default:
      abort ();
    }
}

/* Decrease the reference count on LABEL, and if the count becomes
   zero, remove LABEL and the following instructions.

   Note that this is different from pdp10_remove_unnecessary_label,
   which only removes the label.  */
static void
maybe_remove_label_and_following_code (label)
     rtx label;
{
  rtx insn;

  LABEL_NUSES (label)--;
  if (LABEL_NUSES (label) > 0)
    return;

  INSN_DELETED_P (label) = 1;

  for (insn = PREV_INSN (label); insn != NULL_RTX; insn = PREV_INSN (insn))
    {
      if (!INSN_DELETED_P (insn) && active_insn_p (insn))
	return;
      if (GET_CODE (insn) == BARRIER)
	break;
    }

  for (insn = NEXT_INSN (label); GET_CODE (insn) != BARRIER;
       insn = NEXT_INSN (insn))
    if (GET_CODE (insn) != NOTE)
      INSN_DELETED_P (insn) = 1;
}

static rtx
follow_label (label)
     rtx label;
{
  rtx insn = next_active_insn (label);

  if (optimize >= 2
      && (simplejump_p (insn)
	  || (GET_CODE (insn) == CALL_INSN
	      && pdp10_output_call_as_jrst_p (insn))))
    {
      rtx set = pc_set (insn);

      if (set && (GET_CODE (SET_SRC (set)) != LABEL_REF
		  || XEXP (SET_SRC (set), 0) != label))
	maybe_remove_label_and_following_code (label);

      if (GET_CODE (insn) == JUMP_INSN)
	label = SET_SRC (PATTERN (insn));
      else
	{
	  rtx x = PATTERN (insn);
	  if (GET_CODE (x) == SET)
	    x = SET_SRC (x);
	  if (GET_CODE (x) != CALL)
	    abort ();
	  label = XEXP (x, 0);
	  if (GET_CODE (label) != MEM)
	    abort ();
	  label = XEXP (label, 0);
	}
    }

  return label;
}

/* Called by the PRINT_OPERAND macro.

   Output to the stdio stream STREAM the assembler syntax for an
   instruction operand X.  */
void
pdp10_print_operand (stream, x, code)
     FILE *stream;
     rtx x;
     int code;
{
  int flags = code & ~0xff;
  code &= 0xff;

  switch (code)
    {
      /* Optionally print a space.  */
    case '_':
      if (target_flags & ASM_INDENT_SKIPPED)
	fputc (' ', stream);
      return;

      /* Optionally print AC0.  */
    case '@':
      if (target_flags & ASM_AC0)
	fputs ("0,", stream);
      return;

      /* Start a comment.  */
    case ';':
      fputc ('\t', stream);
      fputs (ASM_COMMENT_START, stream);
      return;

      /* Print first, second, or both words of a double-word constant.  */
    case 'A':
    case 'B':
    case 'D':
      if (GET_CODE (x) != CONST_INT)
	break;
      {
	HOST_WIDE_INT i = INTVAL (x);

#ifdef __PDP10_GAS_H__
	if (code == 'D' && (flags & PRINT_OP_GIW))
	  fputs (".long ", stream);
#else
	if (code == 'D')
	  fputs ("EXP ", stream);
#endif
	if (code != 'B')
	  pdp10_print_unsigned (stream, (i >> 36) & WORD_MASK);
	if (code == 'D')
	  fputc (',', stream);
	if (code != 'A')
	  pdp10_print_unsigned (stream, i & WORD_MASK);
      }
      return;

      /* Compute one's complement of an integer constant.  */
    case 'C':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      pdp10_print_integer (stream, ~INTVAL (x));
      return;

      /* Follow jump labels.  If the instruction after the label is an
	 unconditional jump without side effects, substitute the label
	 for the target of that jump.  */
    case 'F':
      if (GET_CODE (x) == LABEL_REF)
	x = XEXP (x, 0);
      if (GET_CODE (x) != CODE_LABEL)
	abort ();
      x = follow_label (x);
      if (GET_CODE (x) == CODE_LABEL || GET_CODE (x) == LABEL_REF)
	output_asm_label (x);
      else
	{
	  if (GET_CODE (x) == MEM)
	    fputc ('@', stream);
	  pdp10_print_operand (stream, x, 'L');
	}
      return;

      /* Print floating-point immediate (constraint 'G').  */
    case 'G':
      if (GET_MODE (x) != SFmode && GET_MODE (x) != DFmode)
	abort ();
      {
	REAL_VALUE_TYPE r;
	char str[30];
	REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	REAL_VALUE_TO_DECIMAL (r, "%.3g", str);
	fputc ('(', stream);
	fputs (str, stream);
	fputc (')', stream);
      }
      return;

      /* Print local symbolic address constant, if possible and appropriate. */
    case 'L':
      if (GET_CODE (x) != SYMBOL_REF && GET_CODE (x) != LABEL_REF)
	break;
      /* ADDRESS: use indirect addressing to access extended symbols
	 in the large code model.  */
      if (TARGET_EXTENDED
	  && !TARGET_SMALLISH
	  && GET_CODE (x) == SYMBOL_REF
	  && !SYMBOL_REF_INTERNAL (x))
	{
#if XKL_STUFF
	  if (pdp10_outputting_debug_info)
	    fputs ("@[DBL ", stream);
	  else
#endif
	  fputs ("@[GIW ", stream);
	  pdp10_giw++;
	  print_symbolic_ref (stream, x);
	  fputc (']', stream);
	}
      else
	  print_symbolic_ref (stream, x);
      return;

      /* Print negated integer constant.  */
    case 'N':
      pdp10_print_integer (stream, -INTVAL (x));
      return;

      /* Print stack register in a pdp10_push_operand or
         pdp10_pop_operand.  */
    case 'P':
      if (GET_CODE (x) != MEM
	  || (GET_MODE (x) != SImode && GET_MODE (x) != Pmode))
	abort ();
      x = XEXP (x, 0);
      if (GET_CODE (x) != PRE_INC && GET_CODE (x) != POST_DEC)
	abort ();
      x = XEXP (x, 0);
      if (GET_CODE (x) != REG)
	abort ();
      print_reg (stream, REGNO (x));
      return;

      /* Print lower half.  */
    case 'Q':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      pdp10_print_integer (stream, INTVAL (x) & 0777777);
      return;

      /* Print reversed condition.  */
    case 'R':
      fputs (condition_string (reverse_condition (GET_CODE (x))), stream);
      return;

      /* Swap half-words of an integer constant.  */
    case 'S':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      {
	HOST_WIDE_INT i =  INTVAL (x)        & 0777777;
	HOST_WIDE_INT j = (INTVAL (x) >> 18) & 0777777;
	pdp10_print_integer (stream, (i << 18) | j);
      }
      return;

      /* Compute one's complement and swap half-words of an integer
         constant.  */
    case 'T':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      {
	HOST_WIDE_INT i =  (~INTVAL (x))        & 0777777;
	HOST_WIDE_INT j = ((~INTVAL (x)) >> 18) & 0777777;
	pdp10_print_integer (stream, (i << 18) | j);
      }
      return;

      /* Print upper half.  */
    case 'U':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      pdp10_print_integer (stream, (INTVAL (x) >> 18) & 0777777);
      return;

      /* Print canonical rotate count.  */
    case 'V':
    case 'Y':
      if (GET_CODE (x) != CONST_INT)
	abort ();
      {
	HOST_WIDE_INT i = INTVAL (x);
	HOST_WIDE_INT n = code == 'Y' ? 36 : 72;
	i %= n;
	if (i < 0)
	  i += n;
	if (i > n / 2)
	  i = i - n;
	if (i != 0)
	  pdp10_print_integer (stream, i);
      }
      return;

      /* Force word address.  */
    case 'W':
      /* Handled below.  */
      break;

      /* Force a global indirect word.  */
    case 'X':
      if (TARGET_EXTENDED)
	flags |= PRINT_OP_GIW;
      break;

      /* Print next location.  */
    case 'Z':
      switch (GET_CODE (x))
	{
	case REG:
	  print_reg (stream, (REGNO (x) + 1) % 020);
	  break;
	case MEM:
	  if (GET_CODE (XEXP (x, 0)) == MEM)
	    abort ();
	  x = plus_constant (XEXP (x, 0), 1);
	  pdp10_print_operand_address (stream, x, word_mode, 0);
	  break;
	default:
	  debug_rtx (x);
	  abort ();
	}
      return;
    }

  switch (GET_CODE (x))
    {
    case REG:
      print_reg (stream, REGNO (x));
      break;

    case MEM:
      pdp10_print_operand_address (stream, XEXP (x, 0),
				   code == 'W' ? word_mode : GET_MODE (x),
				   code | flags);
      break;

    case CONST_DOUBLE:
      if (GET_MODE (x) == VOIDmode)
	{
	  unsigned HOST_WIDE_INT word0;
	  unsigned HOST_WIDE_INT word1;

	  pdp10_split_integer_double (x, &word0, &word1);

#ifdef __PDP10_GAS_H__
	  if (code != 'A' && code != 'B' && (flags & PRINT_OP_GIW))
	    fputs (".long ", stream);
#else
	  if (code != 'A' && code != 'B')
	    fputs ("EXP ", stream);
#endif
	  if (code != 'B')
	    pdp10_print_unsigned (stream, word0);
	  if (code != 'A' && code != 'B')
	    fputc (',', stream);
	  if (code != 'A')
	    pdp10_print_unsigned (stream, word1);
	}
      else if (GET_MODE (x) == SFmode)
	{
	  REAL_VALUE_TYPE r;

#if 0
	  char str[30];

	  REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	  REAL_VALUE_TO_DECIMAL (r,
				 GET_MODE (x) == SFmode ? "%.9g" : "%.19g",
				 str);
	  fputs (str, stream);
#else
	  long y[3];
	  HOST_WIDE_INT z;

	  REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

#ifdef __PDP10_GAS_H__
	  fputs (".long ", stream);
#endif

	  z = (HWINT (y[0]) << 4)
	    + HWINT ((y[1] >> 28) & 0xfUL);
	  pdp10_print_unsigned (stream, z);
#endif
	}
      else if (GET_MODE (x) == DFmode)
	{
	  REAL_VALUE_TYPE r;
	  HOST_WIDE_INT z;
	  long y[3];

	  REAL_VALUE_FROM_CONST_DOUBLE (r, x);
	  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

#ifdef __PDP10_GAS_H__
	  fputs (".long ", stream);
#else
	  if (code != 'A' && code != 'B')
	    fputs ("EXP ", stream);
#endif

	  if (code != 'B')
	    {
	      z = (HWINT (y[0]) << 4)
		+ HWINT ((y[1] >> 28) & 0xfUL);
	      pdp10_print_unsigned (stream, z);
	    }

	  if (code != 'A' && code != 'B')
	    fputc (',', stream);

	  if (code != 'A')
	    {
	      z = ((HWINT (y[1]) << 8) & 0xfffffffUL)
		+ HWINT (((y[2] >> 24) & 0xffUL));
	      pdp10_print_unsigned (stream, z);
	    }
	}
      else
	abort ();
      break;

    case CONST_INT:
      pdp10_print_integer (stream, INTVAL (x));
      break;

    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
      {
	HOST_WIDE_INT offset = 0;
	int ps, bytesize, position;

	if (GET_CODE (x) == CONST)
	  {
	    rtx plus = XEXP (x, 0);
	    rtx addend;
	    pdp10_word36 word36;

	    if (GET_CODE (plus) != PLUS)
	      abort ();

	    addend = XEXP (plus, 1);
	    if (GET_CODE (addend) == CONST_INT)
	      {
		offset = INTVAL (addend);

		if ((offset & ~ADDRESS_MASK)
		    == (((HOST_WIDE_INT)-1) & ~ADDRESS_MASK))
		  {
		    ps = 0;
		    offset = -(-offset & ADDRESS_MASK);
		  }
		else
		  {
		    ps = (offset >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
		    offset &= ADDRESS_MASK;
		    if (offset & ((HOST_WIDE_INT)1 << 29))
		      {
			if (ps != 0)
			  ps += 0100;
			offset ^= (HOST_WIDE_INT)1 << 29;
			offset -= (HOST_WIDE_INT)1 << 29;
		      }
		  }
	      }
	    else if (pdp10_extract_word36_constant (addend, &word36))
	      {
		pdp10_word36 addr_mask = (pdp10_word36) ADDRESS_MASK;
		pdp10_word36 addr = word36 & addr_mask;
		HOST_WIDE_INT sign = (HOST_WIDE_INT)1
		  << (TARGET_EXTENDED ? 29 : 17);

		/* A 32-bit host cannot represent a complete PDP-10 local
		   byte-pointer constant as CONST_INT, so the addend can be a
		   CONST_DOUBLE.  Decode the P,S selector from the full 36-bit
		   value, but keep only the word-address field for the Y part
		   printed by POINT or a word pointer.  */
		ps = (word36 >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
		offset = (HOST_WIDE_INT) addr;
		if (offset > 0)
		  offset = (offset ^ sign) - sign;
	      }
	    else
	      abort ();

	    x = XEXP (plus, 0);
	  }
	else
	  {
	    if ((offset & ~ADDRESS_MASK) == (((HOST_WIDE_INT)-1) & ~ADDRESS_MASK))
	      {
		ps = 0;
		offset = -(-offset & ADDRESS_MASK);
	      }
	    else
	      {
		ps = (offset >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
		offset &= ADDRESS_MASK;
		if (offset & ((HOST_WIDE_INT)1 << 29))
		  {
		    if (ps != 0)
		      ps += 0100;
		    offset ^= (HOST_WIDE_INT)1 << 29;
		    offset -= (HOST_WIDE_INT)1 << 29;
		  }
	      }
	  }

	if (ps != 0 && code != 'W')
	  {
	    if (decode_ps (ps, &position, &bytesize) >= 0)
	      {
		rtx addr = pdp10_output_add_address_offset (x, offset);
		print_byte_pointer (stream, addr, bytesize, position);
	      }
	    else
	      {
		offset += (HOST_WIDE_INT)ps << 24;
		print_word_pointer (stream, x, flags | PRINT_OP_GIW);
		if (offset != 0)
		  {
		    fputc ('+', stream);
		    pdp10_print_unsigned (stream, offset);
		  }
	      }
	  }
	else
	  {
	    if (GET_CODE (x) == SYMBOL_REF
		|| GET_CODE (x) == LABEL_REF
		|| GET_CODE (x) == CONST
		|| GET_CODE (x) == CONST_INT)
	      {
		print_word_pointer (stream, x, flags);

		if (offset > 0)
		  {
		    HOST_WIDE_INT sign;
		    sign = (HOST_WIDE_INT)1 << (TARGET_EXTENDED ? 29 : 17);
		    offset = (offset ^ sign) - sign;
		  }

		if (offset > 0)
		  fputc ('+', stream);
		pdp10_print_offset30 (stream, offset);
	      }
	    else
	      {
		rtx addr = x;

		if (offset != 0)
		  {
		    if (GET_CODE (addr) == UNSPEC
			&& XINT (addr, 1) == UNSPEC_ADDRESS)
		      addr = gen_ADDRESS (plus_constant (XVECEXP (addr, 0, 0),
						      offset));
		    else
		      addr = plus_constant (addr, offset);
		  }
		pdp10_print_operand_address (stream, addr, word_mode, flags);
	      }
	  }
      }
      break;

    case LT: case EQ: case LE: case GE: case NE: case GT:
      fputs (condition_string (GET_CODE (x)), stream);
      break;

    case PLUS:
    case ADDRESSOF:
      x = pdp10_output_unwrap_address (x);
      pdp10_print_operand_address (stream, x, word_mode, flags);
      break;

    case MINUS:
      if (GET_CODE (XEXP (x, 0)) != SYMBOL_REF
	  || GET_CODE (XEXP (x, 1)) != SYMBOL_REF)
	abort ();
#if XKL_STUFF
      if (pdp10_outputting_debug_info)
	fputs ("DBE <", stream);
#endif
      print_word_pointer (stream, XEXP (x, 0), 0);
      fputc ('-', stream);
      print_word_pointer (stream, XEXP (x, 1), 0);
#if XKL_STUFF
      if (pdp10_outputting_debug_info)
	fputc ('>', stream);
#endif
      break;

    default:
      debug_rtx (x);
      abort ();
    }
}

/* Print a register.  */
static void
print_reg (stream, regno)
     FILE *stream;
     int regno;
{
  fputs (REGISTER_PREFIX, stream);
  fputs (reg_names[regno], stream);
}

/* Output a byte pointer to STREAM.  SIZE and POS specifies the byte
   within the word X.  */
static void
print_byte_pointer (stream, x, size, pos)
     FILE *stream;
     rtx x;
     int size;
     int pos;
{
  /* ADDRESS: select between global and local byte pointers.  */
  if (TARGET_EXTENDED
      && (CONSTANT_ADDRESS_P (x)
	  || (GET_CODE (x) == MEM && CONSTANT_ADDRESS_P (XEXP (x, 0)))))
    {
      /* Need a global byte pointer.  */

      if (VALID_OWGBP_BYTE_SIZE_P (size) && (pos % size == 0))
	{
	  /* Output a one-word global byte pointer.  */

	  static const char  *byte6_selector[] = { "46","47","50",
						   "51","52","53" };
	  static const char  *byte7_selector[] = { "62","63","64","65","66" };
	  static const char  *byte8_selector[] = { "55","56","57","60" };
	  static const char  *byte9_selector[] = { "70","71","72","73" };
	  static const char *byte18_selector[] = { "75","76" };
	  const char **byte_selector;

	  switch (size)
	    {
	    case  6: byte_selector =  byte6_selector; break;
	    case  7: byte_selector =  byte7_selector; break;
	    case  8: byte_selector =  byte8_selector; break;
	    case  9: byte_selector =  byte9_selector; break;
	    case 18: byte_selector = byte18_selector; break;
	    }

	  /* Use the mnemonic form here even for GAS-style output.
	     This is a code literal, not a data initializer; the KL10
	     scalar tests expect the readable one-word global byte pointer
	     form after SKIPA.  Data initializers are still handled by
	     TARGET_ASM_INTEGER and may use .long.  */
	  fputs ("OWGBP ", stream);
	  fputs (byte_selector[pos / size], stream);
	  fputc (',', stream);
	  pdp10_print_operand (stream, x, 'W' | PRINT_OP_NO_BRACKETS);
	  pdp10_giw++;
	}
      else
	{
	  /* Output a two-word global byte pointer.  */

#ifdef __PDP10_GAS_H__
	  fprintf (stream, ".long %02o%02o40000000,", 36 - pos - size, size);
	  pdp10_print_operand (stream, x,
			       'W' | PRINT_OP_NO_BRACKETS | PRINT_OP_GIW);
#else
	  fprintf (stream, "EXP <%02o%02o40,,0>,<", 36 - pos - size, size);
	  pdp10_print_operand (stream, x,
			       'W' | PRINT_OP_NO_BRACKETS | PRINT_OP_GIW);
	  fputc ('>', stream);
#endif
	}
    }
  else
    {
      /* Output a local byte pointer.  */

      /* POINT S,Y,P: size, address, bit-position of high bit.
         Same syntax for GAS and MIDAS; our assembler evaluates it.  */
      fprintf (stream, "POINT %d,", size);
      pdp10_print_operand (stream, x, 'W');
      fprintf (stream, ",%d", pos + size - 1);
    }
}

/* Print X, a word pointer, to STREAM.  Any bits other than the word
   address bits are ignored.  If FLAGS & PRINT_OP_INDIRECT is nonzero,
   make a global pointer indirect.  If FLAGS & PRINT_OP_GIW is
   nonzero, print a global pointer even though TARGET_SMALLISH is in
   effect.  */
static void
print_word_pointer (stream, x, flags)
     FILE *stream;
     rtx x;
     int flags;
{
  HOST_WIDE_INT offset = 0;

  if (TARGET_EXTENDED && (!TARGET_SMALLISH || (flags & PRINT_OP_GIW) != 0))
    {
      /* ADDRESS: use a global indirect word.  */
      if (flags & PRINT_OP_INDIRECT)
	fputs ("@[", stream);

#if XKL_STUFF
      if (pdp10_outputting_debug_info)
	fputs ("DBL ", stream);
      else
#endif
      fputs ("GIW ", stream);

      pdp10_giw++;
    }

  switch (GET_CODE (x))
    {
    case CONST:
      {
	rtx plus = XEXP (x, 0);
	rtx addend;
	pdp10_word36 word36;

	if (GET_CODE (plus) != PLUS
	    || (GET_CODE (XEXP (plus, 0)) != SYMBOL_REF
		&& GET_CODE (XEXP (plus, 0)) != LABEL_REF))
	  abort ();

	addend = XEXP (plus, 1);
	if (GET_CODE (addend) == CONST_INT)
	  offset = INTVAL (addend) & ADDRESS_MASK;
	else if (pdp10_extract_word36_constant (addend, &word36))
	  offset = (HOST_WIDE_INT) (word36 & (pdp10_word36) ADDRESS_MASK);
	else
	  abort ();

	x = XEXP (plus, 0);
      }
      /* Fall through.  */

    case LABEL_REF:
    case SYMBOL_REF:
      print_symbolic_ref (stream, x);
      if (offset > 0)
	fputc ('+', stream);
      pdp10_print_offset30 (stream, offset);
      break;

    case CONST_INT:
      pdp10_print_address (stream, INTVAL (x));
      break;

    default:
      abort ();
    }

  if (TARGET_EXTENDED && (flags & PRINT_OP_INDIRECT) != 0
      && (!TARGET_SMALLISH || (flags & PRINT_OP_GIW) != 0))
    fputc (']', stream);
}

/* Print one PDP-10 word value in decimal for GAS .long data.  Test
   expectations use decimal constants here, even though most PDP-10
   assembly output is naturally octal.  */
static void
pdp10_print_word_decimal (stream, x)
     FILE *stream;
     HOST_WIDE_INT x;
{
  fprintf (stream, HOST_WIDE_INT_PRINT_DEC, x & WORD_MASK);
}

/* Print a word constant without decoding PDP-10 byte-pointer selector
   bits.  This is for data initializers: a local byte pointer such as
   Q+02200000000 must be emitted as one .long word, not as POINT syntax.  */
/* Decompose a symbolic constant into BASE plus OFFSET.
   Accept SYMBOL_REF, LABEL_REF, CONST, nested PLUS, and CONST_INT leaves.
   Return nonzero if the expression is representable as at most one
   symbolic base plus one integer offset.  */
static int
pdp10_decompose_symbolic_constant (x, basep, offsetp)
     rtx x;
     rtx *basep;
     HOST_WIDE_INT *offsetp;
{
  rtx base0, base1;
  HOST_WIDE_INT off0, off1;

  if (x == NULL_RTX)
    return 0;

  if (GET_CODE (x) == CONST)
    x = XEXP (x, 0);

  switch (GET_CODE (x))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      *basep = x;
      *offsetp = 0;
      return 1;

    case CONST_INT:
      *basep = NULL_RTX;
      *offsetp = INTVAL (x);
      return 1;

    case PLUS:
      if (!pdp10_decompose_symbolic_constant (XEXP (x, 0), &base0, &off0)
	  || !pdp10_decompose_symbolic_constant (XEXP (x, 1), &base1, &off1))
	return 0;

      if (base0 != NULL_RTX && base1 != NULL_RTX)
	return 0;

      *basep = base0 != NULL_RTX ? base0 : base1;
      *offsetp = off0 + off1;
      return 1;

    default:
      return 0;
    }
}


/* Extract a 36-bit integer constant from X.  A 32-bit host cannot represent
   PDP-10 byte-pointer P,S bits in a CONST_INT, so static byte-pointer
   initializers use CONST_DOUBLE offsets when necessary.  */
static int
pdp10_extract_word36_constant (x, valuep)
     rtx x;
     pdp10_word36 *valuep;
{
  if (GET_CODE (x) == CONST_INT)
    {
      *valuep = ((pdp10_word36) (unsigned HOST_WIDE_INT) INTVAL (x)
                 & PDP10_WORD36_MASK);
      return 1;
    }

  if (GET_CODE (x) == CONST_DOUBLE)
    {
#if HOST_BITS_PER_WIDE_INT >= 36
      *valuep = ((pdp10_word36) (unsigned HOST_WIDE_INT)
                 CONST_DOUBLE_LOW (x)) & PDP10_WORD36_MASK;
#else
      pdp10_word36 low = (pdp10_word36) (unsigned HOST_WIDE_INT)
        CONST_DOUBLE_LOW (x);
      pdp10_word36 high = (pdp10_word36) (unsigned HOST_WIDE_INT)
        CONST_DOUBLE_HIGH (x);
      *valuep = ((high << HOST_BITS_PER_WIDE_INT) | low)
        & PDP10_WORD36_MASK;
#endif
      return 1;
    }

  return 0;
}

static rtx
pdp10_gen_word36_constant (value)
     pdp10_word36 value;
{
  value &= PDP10_WORD36_MASK;
#if HOST_BITS_PER_WIDE_INT >= 36
  return GEN_INT ((HOST_WIDE_INT) value);
#else
  {
    HOST_WIDE_INT low = (HOST_WIDE_INT)
      (value & ((((pdp10_word36) 1) << HOST_BITS_PER_WIDE_INT) - 1));
    HOST_WIDE_INT high = (HOST_WIDE_INT)
      (value >> HOST_BITS_PER_WIDE_INT);
    return immed_double_const (low, high, Pmode);
  }
#endif
}

static rtx
pdp10_plus_constant_word36 (x, value)
     rtx x;
     pdp10_word36 value;
{
  value &= PDP10_WORD36_MASK;

  if (GET_CODE (x) == MEM)
    {
      rtx mem = copy_rtx (x);
      XEXP (mem, 0) = pdp10_plus_constant_word36 (XEXP (x, 0), value);
      return mem;
    }

  if (CONSTANT_P (x))
    return gen_rtx_CONST (Pmode,
                          gen_rtx_PLUS (Pmode, x,
                                        pdp10_gen_word36_constant (value)));

  return plus_constant (x, trunc_int_for_mode ((HOST_WIDE_INT) value,
                                               Pmode));
}

static int
pdp10_decompose_symbolic_word36 (x, basep, offsetp)
     rtx x;
     rtx *basep;
     pdp10_word36 *offsetp;
{
  rtx base0, base1;
  pdp10_word36 off0, off1;

  if (x == NULL_RTX)
    return 0;

  if (GET_CODE (x) == CONST)
    x = XEXP (x, 0);

  switch (GET_CODE (x))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      *basep = x;
      *offsetp = 0;
      return 1;

    case CONST_INT:
    case CONST_DOUBLE:
      *basep = NULL_RTX;
      return pdp10_extract_word36_constant (x, offsetp);

    case PLUS:
      if (!pdp10_decompose_symbolic_word36 (XEXP (x, 0), &base0, &off0)
          || !pdp10_decompose_symbolic_word36 (XEXP (x, 1), &base1, &off1))
        return 0;
      if (base0 != NULL_RTX && base1 != NULL_RTX)
        return 0;
      *basep = base0 != NULL_RTX ? base0 : base1;
      *offsetp = (off0 + off1) & PDP10_WORD36_MASK;
      return 1;

    default:
      return 0;
    }
}

static int
pdp10_const_word36_byte_pointer_p (x)
     rtx x;
{
  rtx base;
  pdp10_word36 offset;
  pdp10_word36 ps;

  if (!pdp10_decompose_symbolic_word36 (x, &base, &offset)
      || base == NULL_RTX)
    return 0;

  ps = TARGET_EXTENDED ? ((offset >> 30) & 077)
    : ((offset >> 24) & 07777);
  return ps != 0;
}

static void
pdp10_print_word36_decimal (stream, value)
     FILE *stream;
     pdp10_word36 value;
{
  fprintf (stream, "%llu", value & PDP10_WORD36_MASK);
}

static void
pdp10_print_raw_word36_constant (stream, x)
     FILE *stream;
     rtx x;
{
  rtx base;
  pdp10_word36 offset;

  if (!pdp10_decompose_symbolic_word36 (x, &base, &offset))
    abort ();

  if (base != NULL_RTX)
    {
      print_symbolic_ref (stream, base);
      if (offset != 0)
        {
          fputc ('+', stream);
          pdp10_print_word36_decimal (stream, offset);
        }
    }
  else
    pdp10_print_word36_decimal (stream, offset);
}

/* Print a word constant without decoding PDP-10 byte-pointer selector
   bits.  This is for data initializers: a local byte pointer such as
   Q+02200000000 must be emitted as one .long word, not as POINT syntax.  */
static void
pdp10_print_raw_word_constant (stream, x)
     FILE *stream;
     rtx x;
{
  HOST_WIDE_INT offset;
  rtx base;

  switch (GET_CODE (x))
    {
    case CONST:
    case PLUS:
    case SYMBOL_REF:
    case LABEL_REF:
      if (!pdp10_decompose_symbolic_constant (x, &base, &offset))
	abort ();

      if (base == NULL_RTX)
	pdp10_print_word_decimal (stream, offset);
      else
	{
	  print_symbolic_ref (stream, base);
	  if (offset < 0)
	    {
	      fputc ('-', stream);
	      pdp10_print_word_decimal (stream, -offset);
	    }
	  else if (offset > 0)
	    {
	      fputc ('+', stream);
	      pdp10_print_word_decimal (stream, offset);
	    }
	}
      break;

    case CONST_INT:
      pdp10_print_word_decimal (stream, INTVAL (x));
      break;

    default:
      abort ();
    }
}

/* Called by the PRINT_OPERAND_ADDRESS macro.

   Output to the stdio stream STREAM the assembler syntax for an
   instruction operand that is a memory reference whose address is X.  */
void
pdp10_print_operand_address (stream, x, mode, flags)
     FILE *stream;
     rtx x;
     enum machine_mode mode;
     int flags;
{
  int byte_pointer = BYTE_MODE_P (mode);

  x = pdp10_output_unwrap_address (x);

  if (!pdp10_legitimate_address_p (mode, x, 1))
    {
      debug_rtx (x);
      abort ();
    }

  switch (GET_CODE (x))
    {
    case CONST_INT:
      flags = PRINT_OP_INDIRECT;
      if (TARGET_EXTENDED && INTVAL (x) > 017)
	flags |= PRINT_OP_GIW;
      print_word_pointer (stream, x, flags);
      break;

    case CONST:
    case SYMBOL_REF:
      {
	int indirect = !(flags & PRINT_OP_NO_BRACKETS);
	HOST_WIDE_INT offset = 0;
	rtx sym = x;
	int ps = 0;

	if (GET_CODE (x) == CONST)
	  {
	    rtx plus = XEXP (x, 0);
	    rtx addend;
	    pdp10_word36 word36;

	    if (GET_CODE (plus) != PLUS)
	      abort ();

	    sym = XEXP (plus, 0);
	    addend = XEXP (plus, 1);
	    if (GET_CODE (addend) == CONST_INT)
	      {
		offset = INTVAL (addend);
		ps = (offset >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
		offset &= ADDRESS_MASK;
	      }
	    else if (pdp10_extract_word36_constant (addend, &word36))
	      {
		ps = (word36 >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
		offset = (HOST_WIDE_INT) (word36 & (pdp10_word36) ADDRESS_MASK);
	      }
	    else
	      abort ();
	  }

	if (ps != 0 && byte_pointer)
	  {
	    int bitpos, bitsize;
	    rtx addr;

	    if (decode_ps (ps, &bitpos, &bitsize) == -1)
	      abort ();

	    /* Print the Y part of POINT from the stripped base address, not
	       from the full CONST that still contains the PDP-10 P,S selector.
	       Stack component byte pointers can arrive here as
		 CONST (PLUS (frame-address, word36-P,S-addend)).
	       Passing that full CONST back through %W makes the word-address
	       printer decode the same selector again, and on non-symbolic frame
	       bases it can fall off the old SYMBOL_REF-only path.  */
	    addr = pdp10_output_add_address_offset (sym, offset);

	    if (indirect)
	      fputc ('[', stream);
	    print_byte_pointer (stream, addr, bitsize, bitpos);
	    if (indirect)
	      fputc (']', stream);
	  }
	else
	  {
	    if (indirect)
	      flags |= PRINT_OP_INDIRECT;
	    if (GET_CODE (sym) == SYMBOL_REF
		|| GET_CODE (sym) == LABEL_REF
		|| GET_CODE (sym) == CONST
		|| GET_CODE (sym) == CONST_INT)
	      print_word_pointer (stream, x, flags);
	    else
	      {
		rtx addr = sym;
		if (offset != 0)
		  {
		    if (GET_CODE (addr) == UNSPEC
			&& XINT (addr, 1) == UNSPEC_ADDRESS)
		      addr = gen_ADDRESS (plus_constant (XVECEXP (addr, 0, 0),
						      offset));
		    else
		      addr = plus_constant (addr, offset);
		  }
		pdp10_print_operand_address (stream, addr, word_mode, flags);
	      }
	  }

	break;
      }

    case REG:
      if (!byte_pointer)
	{
	  if (REGNO (x) == 0)
	    fputc ('@', stream);
	  else
	    fputc ('(', stream);
	}
      print_reg (stream, REGNO (x));
      if (!byte_pointer && REGNO (x) != 0)
	fputc (')', stream);
      break;

    case PLUS:
      if (GET_CODE (XEXP (x, 0)) != REG
	  || (GET_CODE (XEXP (x, 1)) != CONST_INT
	      && GET_CODE (XEXP (x, 1)) != CONST
	      && GET_CODE (XEXP (x, 1)) != LABEL_REF
	      && GET_CODE (XEXP (x, 1)) != SYMBOL_REF)
	  || REGNO (XEXP (x, 0)) == 0)
	abort ();

      if (GET_CODE (XEXP (x, 1)) == CONST_INT)
	{
	  HOST_WIDE_INT offset = INTVAL (XEXP (x, 1));

	  /* PDP-10: when an indexed word address is based on an object whose
	     declaration carries DATA_PADDING byte-pointer P,S bits, the symbolic
	     offset can still contain those high P,S bits here.  A word address
	     ignores them; only the PDP-10 address field is relevant.  Do this
	     normalization before deciding whether the address needs a GIW.

	     Keep ordinary small negative displacements negative.  The mask is
	     only applied when the high bits are neither zero nor sign-extension,
	     i.e. when they are real selector/extra address bits.  */
	  if (! byte_pointer)
	    {
	      HOST_WIDE_INT high = offset & ~ADDRESS_MASK;
	      if (high != 0 && high != ~ADDRESS_MASK)
		offset &= ADDRESS_MASK;
	    }

	  if (offset >= -0400000 && offset <= 0377777)
	    pdp10_print_offset (stream, offset);
	  else
	    {
	      fputs ("@[%EXIND(0,", stream);
	      pdp10_giw++;
	      print_reg (stream, REGNO (XEXP (x, 0)));
	      fputc (',', stream);
	      offset = offset & ADDRESS_MASK;
	      pdp10_print_offset30 (stream, offset);
	      fputs (")]", stream);
	      break;
	    }
	}
      else if (GET_CODE (XEXP (x, 1)) == CONST)
	{
	  rtx y = XEXP (XEXP (x, 1), 0);
	  HOST_WIDE_INT offset;

	  if (GET_CODE (y) != PLUS
	      || GET_CODE (XEXP (y, 0)) != SYMBOL_REF
	      || GET_CODE (XEXP (y, 1)) != CONST_INT)
	    abort ();

	  offset = INTVAL (XEXP (y, 1));

	  /* Same normalization as above, but for SYMBOL+OFFSET indexed
	     addresses.  This is the path that printed e.g.

	         b6a+pdp10.c:...:TOOBIG:(1)

	     for word-mode accesses through a packed aggregate base.  */
	  if (! byte_pointer)
	    {
	      HOST_WIDE_INT high = offset & ~ADDRESS_MASK;
	      if (high != 0 && high != ~ADDRESS_MASK)
		offset &= ADDRESS_MASK;
	    }

	  print_symbolic_ref (stream, XEXP (y, 0));
	  if (offset > 0)
	    {
	      fputc ('+', stream);
	      pdp10_print_offset (stream, offset);
	    }
	  else if (offset < 0)
	    pdp10_print_offset (stream, offset);
	}
      else
	print_symbolic_ref (stream, XEXP (x, 1));
      fputc ('(', stream);
      print_reg (stream, REGNO (XEXP (x, 0)));
      fputc (')', stream);
      break;

    case MEM:
      if (!byte_pointer)
	fputc ('@', stream);
      pdp10_print_operand_address (stream, XEXP (x, 0), Pmode, 0);
      break;

    case PRE_INC:
      /* PRE_INC is used with the ILDB and IDPB instructions.  */
      if (!byte_pointer)
	abort ();
      pdp10_print_operand_address (stream, XEXP (x, 0), mode, 0);
      break;

    case UNSPEC:
      if (XINT (x, 1) != UNSPEC_ADDRESS)
	abort ();
      pdp10_print_operand_address (stream, XVECEXP (x, 0, 0), mode, 0);
      break;

    default:
      abort ();
    }
}


/**********************************************************************

	Target Attributes

**********************************************************************/

static void	make_byte_type PARAMS ((tree type, int precision,
					int unsignedp));
static void	layout_byte_decl PARAMS ((tree decl));
static tree	pdp10_handle_size_attribute PARAMS ((tree *node, tree name,
						     tree args, int flags,
						     bool *no_add_attrs));
static tree	pdp10_handle_fastcall_attribute PARAMS ((tree *node, tree name,
							 tree args, int flags,
							 bool *no_add_attrs));

/* Table of machine attributes.  */
const struct attribute_spec pdp10_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */

  /* Specifies the size, in bits, of a type or variable.  */
  { "size",	1, 1, true, false, false, pdp10_handle_size_attribute },

  /* Specifies the size, in bits, of a type or variable.  */
  { "fastcall",	0, 0, false, true, true, pdp10_handle_fastcall_attribute },

  { NULL,           0, 0, false, false, false, NULL }
};

tree char6_signed_type_node;
tree char6_unsigned_type_node;
tree char7_signed_type_node;
tree char7_unsigned_type_node;
tree char8_signed_type_node;
tree char8_unsigned_type_node;
tree char9_signed_type_node;
tree char9_unsigned_type_node;
tree short16_integer_type_node;
tree short16_unsigned_type_node;
tree int32_integer_type_node;
tree int32_unsigned_type_node;

/* Build all PDP-10 byte types.  */
void
pdp10_build_machine_types ()
{
  static int done = 0;

  if (done)
    return;

  char6_signed_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char6_signed_type_node, 6, 0);
  ggc_add_tree_root (&char6_signed_type_node, 1);
  char6_unsigned_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char6_unsigned_type_node, 6, 1);
  ggc_add_tree_root (&char6_unsigned_type_node, 1);

  char7_signed_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char7_signed_type_node, 7, 0);
  ggc_add_tree_root (&char7_signed_type_node, 1);
  char7_unsigned_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char7_unsigned_type_node, 7, 1);
  ggc_add_tree_root (&char7_unsigned_type_node, 1);

  char8_signed_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char8_signed_type_node, 8, 0);
  ggc_add_tree_root (&char8_signed_type_node, 1);
  char8_unsigned_type_node = make_node (INTEGER_TYPE);
  make_byte_type (char8_unsigned_type_node, 8, 1);
  ggc_add_tree_root (&char8_unsigned_type_node, 1);

  char9_signed_type_node
    = flag_signed_char ? char_type_node : signed_char_type_node;
  ggc_add_tree_root (&char9_signed_type_node, 1);
  char9_unsigned_type_node
    = flag_signed_char ? unsigned_char_type_node : char_type_node;
  ggc_add_tree_root (&char9_unsigned_type_node, 1);

  if (pdp10_char_bytesize != 9)
    {
      switch (pdp10_char_bytesize)
	{
	case 6: case 7: case 8: case 9:
	  break;
	default:
	  fprintf (stderr, "Bad value for -mchar-bytesize: %s\n",
		   pdp10_string_bytesize);
	  exit (1);
	}

      if (pdp10_char_bytesize != 9)
	{
	  char_type_node = make_node (INTEGER_TYPE);
	  make_byte_type (char_type_node, pdp10_char_bytesize,
			  !flag_signed_char);
	  signed_char_type_node = make_node (INTEGER_TYPE);
	  make_byte_type (signed_char_type_node, pdp10_char_bytesize, 0);
	  unsigned_char_type_node = make_node (INTEGER_TYPE);
	  make_byte_type (unsigned_char_type_node, pdp10_char_bytesize, 1);
	}
    }

  short16_integer_type_node = make_node (INTEGER_TYPE);
  make_byte_type (short16_integer_type_node, 16, 0);
  ggc_add_tree_root (&short16_integer_type_node, 1);
  short16_unsigned_type_node = make_node (INTEGER_TYPE);
  make_byte_type (short16_unsigned_type_node, 16, 1);
  ggc_add_tree_root (&short16_unsigned_type_node, 1);

  int32_integer_type_node = make_node (INTEGER_TYPE);
  make_byte_type (int32_integer_type_node, 32, 0);
  ggc_add_tree_root (&int32_integer_type_node, 1);
  int32_unsigned_type_node = make_node (INTEGER_TYPE);
  make_byte_type (int32_unsigned_type_node, 32, 1);
  ggc_add_tree_root (&int32_unsigned_type_node, 1);

  done = 1;
}

/* Initialize the type in TYPE.  The new type will be suitable as a
   byte of PRECISION bits pointed to by a PDP-10 global byte pointer,
   and will be unsigned if UNSIGNEDP is nonzero.  */
static void
make_byte_type (type, precision, unsignedp)
     tree type;
     int precision;
     int unsignedp;
{
  TYPE_PRECISION (type) = precision;

  if (unsignedp)
    {
      TYPE_MIN_VALUE (type) = build_int_2 (0, 0);
      TYPE_MAX_VALUE (type)
	= build_int_2 (precision - HOST_BITS_PER_WIDE_INT >= 0
		       ? -1 : (HWINT (1) << precision) - 1,
		       precision - HOST_BITS_PER_WIDE_INT > 0
		       ? ((unsigned HOST_WIDE_INT) ~0
			  >> (HOST_BITS_PER_WIDE_INT
			      - (precision - HOST_BITS_PER_WIDE_INT)))
		       : 0);
    }
  else
    {
      TYPE_MIN_VALUE (type)
	= build_int_2 ((precision - HOST_BITS_PER_WIDE_INT > 0
			? 0 : HWINT (-1) << (precision - 1)),
		       ((HWINT (-1)
			 << (precision - HOST_BITS_PER_WIDE_INT - 1 > 0
			     ? precision - HOST_BITS_PER_WIDE_INT - 1
			     : 0))));
      TYPE_MAX_VALUE (type)
	= build_int_2 ((precision - HOST_BITS_PER_WIDE_INT > 0
			? -1 : (HWINT (1) << (precision - 1)) - 1),
		       (precision - HOST_BITS_PER_WIDE_INT - 1 > 0
			? ((HWINT (1)
			    << (precision - HOST_BITS_PER_WIDE_INT - 1))) - 1
			: 0));
    }

  TREE_TYPE (TYPE_MIN_VALUE (type)) = type;
  TREE_TYPE (TYPE_MAX_VALUE (type)) = type;

  TREE_UNSIGNED (type) = unsignedp;

  TYPE_MODE (type) = smallest_mode_for_size (TYPE_PRECISION (type),
					     MODE_INT);
  TYPE_SIZE (type) = bitsize_int (TYPE_PRECISION (type));
  TYPE_SIZE_UNIT (type)
    = size_int ((TYPE_PRECISION (type) + BITS_PER_UNIT - 1) / BITS_PER_UNIT);

  TYPE_ALIGN (type) = precision == 32 ? 36 : precision;
}

/* Lay out a PDP-10 byte variable in memory.  */
static void
layout_byte_decl (decl)
     tree decl;
{
  register tree type = TREE_TYPE (decl);
  register enum tree_code code = TREE_CODE (decl);

  TREE_UNSIGNED (decl) = TREE_UNSIGNED (type);
  DECL_MODE (decl) = TYPE_MODE (type);
  DECL_SIZE (decl) = TYPE_SIZE (type);
  DECL_SIZE_UNIT (decl) = TYPE_SIZE_UNIT (type);

  /* Force alignment required for the data type.
     But if the decl itself wants greater alignment, don't override that.
     Likewise, if the decl is packed, don't override it.  */
  if (!(code == FIELD_DECL && DECL_BIT_FIELD (decl))
      && (DECL_ALIGN (decl) == 0
	  || (!(code == FIELD_DECL && DECL_PACKED (decl))
	      && TYPE_ALIGN (type) > DECL_ALIGN (decl))))
    {	      
      DECL_ALIGN (decl) = TYPE_ALIGN (type);
      DECL_USER_ALIGN (decl) = TYPE_USER_ALIGN (type);
    }

  /* For fields, set the bit field type and update the alignment.  */
  if (code == FIELD_DECL)
    {
      DECL_BIT_FIELD_TYPE (decl) = DECL_BIT_FIELD (decl) ? type : 0;
      if (maximum_field_alignment != 0)
	DECL_ALIGN (decl) = MIN (DECL_ALIGN (decl), maximum_field_alignment);
      else if (DECL_PACKED (decl))
	DECL_USER_ALIGN (decl) = 0;
    }

  /* Evaluate nonconstant size only once, either now or as soon as safe.  */
  if (DECL_SIZE (decl) != 0 && TREE_CODE (DECL_SIZE (decl)) != INTEGER_CST)
    DECL_SIZE (decl) = variable_size (DECL_SIZE (decl));
  if (DECL_SIZE_UNIT (decl) != 0
      && TREE_CODE (DECL_SIZE_UNIT (decl)) != INTEGER_CST)
    DECL_SIZE_UNIT (decl) = variable_size (DECL_SIZE_UNIT (decl));

  /* If requested, warn about definitions of large data objects.  */
  if (warn_larger_than
      && (code == VAR_DECL || code == PARM_DECL)
      && !DECL_EXTERNAL (decl))
    {
      tree size = DECL_SIZE_UNIT (decl);

      if (size != 0 && TREE_CODE (size) == INTEGER_CST
	  && compare_tree_int (size, larger_than_size) > 0)
	{
	  unsigned int size_as_int = TREE_INT_CST_LOW (size);

	  if (compare_tree_int (size, size_as_int) == 0)
	    warning_with_decl (decl, "size of `%s' is %d bytes", size_as_int);
	  else
	    warning_with_decl (decl, "size of `%s' is larger than %d bytes",
			       larger_than_size);
	}
    }
}

#define PDP10_TYPE_FROM_SIZE(SIZE, UNSIGNEDP)				    \
  ((SIZE) == 6 ?							    \
   ((UNSIGNEDP) ? char6_unsigned_type_node : char6_signed_type_node) :	    \
   (SIZE) == 7 ?							    \
   ((UNSIGNEDP) ? char7_unsigned_type_node : char7_signed_type_node) :	    \
   (SIZE) == 8 ?							    \
   ((UNSIGNEDP) ? char8_unsigned_type_node : char8_signed_type_node) :	    \
   (SIZE) == 9 ?							    \
   ((UNSIGNEDP) ? char9_unsigned_type_node : char9_signed_type_node) :	    \
   (SIZE) == 16 ?							    \
   ((UNSIGNEDP) ? short16_unsigned_type_node : short16_integer_type_node) : \
   (SIZE) == 18 ?							    \
   ((UNSIGNEDP) ? short_unsigned_type_node : short_integer_type_node) :	    \
   (SIZE) == 32 ?							    \
   ((UNSIGNEDP) ? int32_unsigned_type_node : int32_integer_type_node) :	    \
   (SIZE) == 36 ?							    \
   ((UNSIGNEDP) ? unsigned_type_node : integer_type_node) :		    \
   NULL_TREE)

/* Function to handle the size attribute.  NODE points to the node to
   which the attribute is to be applied.  If a DECL, it should be
   modified in place; if a TYPE, a copy should be created.  NAME is
   the name of the attribute (possibly with leading or trailing __).
   ARGS is the TREE_LIST of the arguments (which may be NULL).  FLAGS
   gives further information about the context of the attribute.
   Afterwards, the attributes will be added to the DECL_ATTRIBUTES or
   TYPE_ATTRIBUTES, as appropriate, unless *NO_ADD_ATTRS is set to
   true (which should be done on error, as well as in any other cases
   when the attributes should not be added to the DECL or TYPE).
   Depending on FLAGS, any attributes to be applied to another type or
   DECL later may be returned; otherwise the return value should be
   NULL_TREE.  This pointer may be NULL if no special handling is
   required beyond the checks implied by the rest of this structure.  */
static tree
pdp10_handle_size_attribute (node, name, args, flags, no_add_attrs)
     tree *node;
     tree name ATTRIBUTE_UNUSED;
     tree args;
     int flags ATTRIBUTE_UNUSED;
     bool *no_add_attrs;
{
  enum tree_code code = TREE_CODE (*node);

  pdp10_build_machine_types ();

  *no_add_attrs = 1;

  if (code == INTEGER_TYPE
      || code == VAR_DECL
      || code == PARM_DECL
      || code == TYPE_DECL
      || (code == FIELD_DECL && !DECL_BIT_FIELD (*node)))
    {
      int size = 0;

      if (args && TREE_VALUE (args)
	  && TREE_CODE (TREE_VALUE (args)) == INTEGER_CST)
	size = TREE_INT_CST_LOW (TREE_VALUE (args));

      if (!VALID_OWGBP_BYTE_SIZE_P (size)
	  && size != 16 && size != 32 && size != 36)
	{
	  error ("invalid value for `size' attribute");
	  return NULL_TREE;
	}

      if ((code != INTEGER_TYPE
	   && TREE_CODE (TREE_TYPE (*node)) != INTEGER_TYPE)
	  || TREE_CHAIN (args) != NULL_TREE
	  || size == 0)
	return NULL_TREE;

      if (code == INTEGER_TYPE)
	make_byte_type (*node, size, TREE_UNSIGNED (*node));
      else
	{
	  int unsignedp
	    = TREE_UNSIGNED (code == FIELD_DECL ? *node : TREE_TYPE (*node));
	  TREE_TYPE (*node) = PDP10_TYPE_FROM_SIZE (size, unsignedp);
	  layout_byte_decl (*node);
	}

      *no_add_attrs = 0;
    }

  return NULL_TREE;
}

static tree
pdp10_handle_fastcall_attribute (node, name, args, flags, no_add_attrs)
     tree *node;
     tree name ATTRIBUTE_UNUSED;
     tree args ATTRIBUTE_UNUSED;
     int flags ATTRIBUTE_UNUSED;
     bool *no_add_attrs;
{
  if (TREE_CODE (*node) != FUNCTION_TYPE
      && TREE_CODE (*node) != METHOD_TYPE
      && TREE_CODE (*node) != FIELD_DECL
      && TREE_CODE (*node) != TYPE_DECL)
    {
      warning ("`%s' attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
      return NULL_TREE;
    }

  return NULL_TREE;
}


/**********************************************************************

	Miscellaneout Parameters

**********************************************************************/

static void	convert_voidp_callback PARAMS ((cpp_reader *pfile));
static void	no_convert_voidp_callback PARAMS ((cpp_reader *pfile));
static void	long_long_71bit_callback PARAMS ((cpp_reader *pfile));
static void	no_long_long_callback PARAMS ((cpp_reader *pfile));
static void	string_bytesize_callback PARAMS ((cpp_reader *pfile));
static int	get_pragma_number PARAMS ((cpp_reader *pfile,
					   const char *pragma));
static int	pdp10_reorg_set_parts PARAMS ((rtx insn, rtx *destp,
					     rtx *srcp));
static int	pdp10_reorg_mem_addr PARAMS ((rtx x, enum machine_mode mode,
					      rtx *addrp));
static int	pdp10_reorg_unspec_code_p PARAMS ((rtx x, enum machine_mode mode,
					         int code));
static void	pdp10_reorg_install_blt PARAMS ((rtx first, rtx second,
					    rtx *seq, int nseq,
					    rtx ac, rtx src, rtx dest));
static int	pdp10_reorg_word_repack_blt PARAMS ((rtx insn));
static int	pdp10_reorg_word_repack_spill_blt PARAMS ((rtx insn));
static int	pdp10_reorg_word_repack_dmove_direct PARAMS ((rtx insn));
static int	pdp10_reorg_word_repack_dmove PARAMS ((rtx insn));
static void	pdp10_reorg_word_repack_copies PARAMS ((rtx first));
static int	pdp10_reorg_folded_subbp_sum PARAMS ((rtx insn));
static void	pdp10_reorg_folded_subbp_sums PARAMS ((rtx first));

static void
convert_voidp_callback (pfile)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
{
  target_flags &= ~OPT_VOIDP;
}

static void
no_convert_voidp_callback (pfile)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
{
  target_flags |= OPT_VOIDP;
}

static void
no_long_long_callback (pfile)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
{
  target_flags &= ~CPU_71BIT;
  target_flags &= ~OPT_LONG_LONG_71BIT;
  target_flags |= OPT_LONG_LONG_72BIT;
  warning ("#pragma no_long_long doesn't work properly.  "
	   "Use -mlong-long-72bit instead.");
}

static void
long_long_71bit_callback (pfile)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
{
  target_flags |= CPU_71BIT | OPT_LONG_LONG_71BIT;
  target_flags &= ~OPT_LONG_LONG_72BIT;
  warning ("#pragma long_long_71bit doesn't work properly.  "
	   "Use -mlong-long-71bit instead.");
}

static int
get_pragma_number (pfile, pragma)
     cpp_reader *pfile;
     const char *pragma;
{
  const cpp_token *token = cpp_get_token (pfile);
  char *text = (char *)token->val.str.text;
  char *end;
  int n;

  if (token->type != CPP_NUMBER)
    {
      warning ("#pragma %s takes a numeric argument", pragma);
      return -1;
    }

  end = text + token->val.str.len;
  n = strtol (text, &end, 10);

  if (end == text)
    return -1;
  else
    return n;
}

static void
string_bytesize_callback (pfile)
     cpp_reader *pfile;
{
  int n = get_pragma_number (pfile, "string_bytesize");

  if (n == -1)
    return;

  switch (n)
    {
    case 6: case 7: case 8: case 9:
      break;
    default:
      warning ("#pragma string_bytesize argument must be 6, 7, 8, or 9");
      return;
    }

  pdp10_string_literal_byte_size = n;
}

/* Called by the REGISTER_TARGET_PRAGMAS macro.  */
void
pdp10_register_target_pragmas (ptr)
     void *ptr;
{
  cpp_reader *pfile = ptr;
  cpp_register_pragma (pfile, NULL, "convert_voidp", convert_voidp_callback);
  cpp_register_pragma (pfile, NULL, "no_convert_voidp",
		       no_convert_voidp_callback);
  cpp_register_pragma (pfile, NULL, "long_long_71bit",
		       long_long_71bit_callback);
  cpp_register_pragma (pfile, NULL, "no_long_long", no_long_long_callback);
  cpp_register_pragma (pfile, NULL, "string_bytesize",
		       string_bytesize_callback);
}

static int
pdp10_reorg_set_parts (insn, destp, srcp)
     rtx insn;
     rtx *destp;
     rtx *srcp;
{
  rtx pat;

  if (insn == 0 || GET_CODE (insn) != INSN)
    return 0;

  pat = PATTERN (insn);
  if (GET_CODE (pat) != SET)
    return 0;

  if (destp)
    *destp = SET_DEST (pat);
  if (srcp)
    *srcp = SET_SRC (pat);
  return 1;
}

static int
pdp10_reorg_mem_addr (x, mode, addrp)
     rtx x;
     enum machine_mode mode;
     rtx *addrp;
{
  if (x == 0 || GET_CODE (x) != MEM)
    return 0;
  if (mode != VOIDmode && GET_MODE (x) != mode)
    return 0;
  if (addrp)
    *addrp = XEXP (x, 0);
  return 1;
}

static int
pdp10_reorg_unspec_code_p (x, mode, code)
     rtx x;
     enum machine_mode mode;
     int code;
{
  return (x != 0
          && GET_MODE (x) == mode
          && GET_CODE (x) == UNSPEC
          && XVECLEN (x, 0) >= 1
          && XINT (x, 1) == code);
}

static void
pdp10_reorg_install_blt (first, second, seq, nseq, ac, src, dest)
     rtx first;
     rtx second;
     rtx *seq;
     int nseq;
     rtx ac;
     rtx src;
     rtx dest;
{
  rtx end;
  int i;

  end = plus_constant (dest, 1);
  PATTERN (first) = gen_pdp10_blt_setup (ac, src, dest);
  PATTERN (second) = gen_BLT (ac, gen_rtx_MEM (BLKmode, end));
  INSN_CODE (first) = -1;
  INSN_CODE (second) = -1;
  REG_NOTES (first) = 0;
  REG_NOTES (second) = 0;

  for (i = 2; i < nseq; i++)
    if (seq[i] != 0 && !INSN_DELETED_P (seq[i]))
      delete_insn (seq[i]);
}

static int
pdp10_reorg_word_repack_blt (insn)
     rtx insn;
{
  rtx seq[11];
  rtx dest;
  rtx src;
  rtx src1;
  rtx src2;
  rtx temp1;
  rtx temp2;
  rtx blt_src;
  rtx blt_dst;
  rtx blt_end;
  rtx ac;
  rtx x;
  int i;

  seq[0] = insn;
  for (i = 1; i < 11; i++)
    {
      seq[i] = next_real_insn (seq[i - 1]);
      if (seq[i] == 0)
        return 0;
    }

  if (!pdp10_reorg_set_parts (seq[0], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[1], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src1))
    return 0;
  if (!pdp10_word_address_plus_one_p (src1, src2))
    return 0;

  if (!pdp10_reorg_set_parts (seq[2], &dest, &src)
      || !pdp10_reorg_unspec_code_p (src, DImode, UNSPEC_LSHC))
    return 0;
  if (!pdp10_reorg_set_parts (seq[3], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[4], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[5], &dest, &src)
      || GET_CODE (src) != ASHIFT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[6], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;

  if (!pdp10_reorg_set_parts (seq[7], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, SImode, &temp1))
    return 0;
  if (!pdp10_reorg_set_parts (seq[8], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, SImode, &temp2))
    return 0;
  if (!pdp10_word_address_plus_one_p (temp1, temp2))
    return 0;

  if (!pdp10_reorg_set_parts (seq[9], &ac, &src)
      || !REG_P (ac)
      || !pdp10_reorg_unspec_code_p (src, SImode, UNSPEC_BLT_SETUP)
      || XVECLEN (src, 0) != 2)
    return 0;
  blt_src = XVECEXP (src, 0, 0);
  blt_dst = XVECEXP (src, 0, 1);
  if (!rtx_equal_p (blt_src, temp1))
    return 0;

  x = PATTERN (seq[10]);
  if (GET_CODE (x) != PARALLEL || XVECLEN (x, 0) != 3)
    return 0;
  x = XVECEXP (PATTERN (seq[10]), 0, 2);
  if (GET_CODE (x) != USE
      || !pdp10_reorg_mem_addr (XEXP (x, 0), BLKmode, &blt_end))
    return 0;
  if (!pdp10_word_address_plus_one_p (blt_dst, blt_end))
    return 0;

  pdp10_reorg_install_blt (seq[0], seq[1], seq, 11, ac, src1, blt_dst);
  return 1;
}

static int
pdp10_reorg_word_repack_spill_blt (insn)
     rtx insn;
{
  rtx seq[21];
  rtx dest;
  rtx src;
  rtx src1;
  rtx src2;
  rtx temp1;
  rtx temp2;
  rtx blt_src;
  rtx blt_dst;
  rtx ac;
  rtx x;
  int i;

  seq[0] = insn;
  for (i = 1; i < 21; i++)
    {
      seq[i] = next_real_insn (seq[i - 1]);
      if (seq[i] == 0)
        return 0;
    }

  if (!pdp10_reorg_set_parts (seq[0], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[1], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[2], &dest, &src)
      || !REG_P (dest) || !REG_P (src))
    return 0;
  if (!pdp10_reorg_set_parts (seq[3], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src1))
    return 0;
  if (!pdp10_word_address_plus_one_p (src1, src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[4], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[5], &dest, &src)
      || !REG_P (dest) || !REG_P (src))
    return 0;
  if (!pdp10_reorg_set_parts (seq[6], &dest, &src)
      || !pdp10_reorg_unspec_code_p (src, DImode, UNSPEC_LSHC))
    return 0;
  if (!pdp10_reorg_set_parts (seq[7], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[8], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[9], &dest, &src)
      || GET_CODE (src) != ASHIFT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[10], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;

  if (!pdp10_reorg_set_parts (seq[11], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, SImode, &temp1))
    return 0;
  if (!pdp10_reorg_set_parts (seq[12], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, SImode, &temp2))
    return 0;
  if (!pdp10_word_address_plus_one_p (temp1, temp2))
    return 0;

  if (!pdp10_reorg_set_parts (seq[13], &ac, &src)
      || !REG_P (ac)
      || !pdp10_reorg_unspec_code_p (src, SImode, UNSPEC_BLT_SETUP)
      || XVECLEN (src, 0) != 2)
    return 0;
  blt_src = XVECEXP (src, 0, 0);
  blt_dst = XVECEXP (src, 0, 1);
  if (!rtx_equal_p (blt_src, temp1))
    return 0;

  x = PATTERN (seq[20]);
  if (GET_CODE (x) != PARALLEL || XVECLEN (x, 0) != 3)
    return 0;

  pdp10_reorg_install_blt (seq[0], seq[1], seq, 21, ac, src1, blt_dst);
  return 1;
}

static int
pdp10_reorg_word_repack_dmove_direct (insn)
     rtx insn;
{
  rtx seq[9];
  rtx dest;
  rtx src;
  rtx src1;
  rtx src2;
  rtx final_dest;
  rtx ac;
  int i;

  seq[0] = insn;
  for (i = 1; i < 9; i++)
    {
      seq[i] = next_real_insn (seq[i - 1]);
      if (seq[i] == 0)
        return 0;
    }

  if (!pdp10_reorg_set_parts (seq[0], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[1], &ac, &src)
      || !REG_P (ac)
      || !pdp10_reorg_mem_addr (src, SImode, &src1))
    return 0;
  if (!pdp10_word_address_plus_one_p (src1, src2))
    return 0;

  if (!pdp10_reorg_set_parts (seq[2], &dest, &src)
      || !pdp10_reorg_unspec_code_p (src, DImode, UNSPEC_LSHC))
    return 0;
  if (!pdp10_reorg_set_parts (seq[3], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[4], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[5], &dest, &src)
      || GET_CODE (src) != ASHIFT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[6], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[7], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, DImode, 0))
    return 0;
  if (!pdp10_reorg_set_parts (seq[8], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, DImode, &final_dest)
      || !REG_P (src))
    return 0;

  pdp10_reorg_install_blt (seq[0], seq[1], seq, 9, ac, src1, final_dest);
  return 1;
}

static int
pdp10_reorg_word_repack_dmove (insn)
     rtx insn;
{
  rtx seq[14];
  rtx dest;
  rtx src;
  rtx src1;
  rtx src2;
  rtx final_dest;
  rtx ac;
  int i;

  seq[0] = insn;
  for (i = 1; i < 14; i++)
    {
      seq[i] = next_real_insn (seq[i - 1]);
      if (seq[i] == 0)
        return 0;
    }

  if (!pdp10_reorg_set_parts (seq[0], &ac, &src)
      || !REG_P (ac)
      || !pdp10_reorg_mem_addr (src, SImode, &src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[1], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[2], &dest, &src)
      || !REG_P (dest) || !REG_P (src))
    return 0;
  if (!pdp10_reorg_set_parts (seq[3], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, SImode, &src1))
    return 0;
  if (!pdp10_word_address_plus_one_p (src1, src2))
    return 0;
  if (!pdp10_reorg_set_parts (seq[4], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[5], &dest, &src)
      || !REG_P (dest) || !REG_P (src))
    return 0;

  if (!pdp10_reorg_set_parts (seq[6], &dest, &src)
      || !pdp10_reorg_unspec_code_p (src, DImode, UNSPEC_LSHC))
    return 0;
  if (!pdp10_reorg_set_parts (seq[7], &dest, &src)
      || GET_CODE (src) != LSHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[8], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[9], &dest, &src)
      || GET_CODE (src) != ASHIFT)
    return 0;
  if (!pdp10_reorg_set_parts (seq[10], &dest, &src)
      || GET_CODE (src) != ASHIFTRT)
    return 0;

  if (!pdp10_reorg_set_parts (seq[11], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, DImode, 0))
    return 0;
  if (!pdp10_reorg_set_parts (seq[12], &dest, &src)
      || !REG_P (dest)
      || !pdp10_reorg_mem_addr (src, DImode, 0))
    return 0;
  if (!pdp10_reorg_set_parts (seq[13], &dest, &src)
      || !pdp10_reorg_mem_addr (dest, DImode, &final_dest)
      || !REG_P (src))
    return 0;

  pdp10_reorg_install_blt (seq[0], seq[1], seq, 14, ac, src1, final_dest);
  return 1;
}

static void
pdp10_reorg_word_repack_copies (first)
     rtx first;
{
  rtx insn;

  for (insn = next_real_insn (first); insn; insn = next_real_insn (insn))
    {
      if (INSN_DELETED_P (insn))
        continue;
      if (pdp10_reorg_word_repack_blt (insn)
          || pdp10_reorg_word_repack_spill_blt (insn)
          || pdp10_reorg_word_repack_dmove_direct (insn)
          || pdp10_reorg_word_repack_dmove (insn))
        continue;
    }
}

static int
pdp10_reorg_folded_subbp_sum (insn)
     rtx insn;
{
  rtx seq[9];
  rtx dest;
  rtx src;
  rtx sum;
  rtx ptr;
  rtx tmp;
  rtx base;
  rtx diff;
  rtx x;
  int i;

  seq[0] = insn;
  for (i = 1; i < 9; i++)
    {
      seq[i] = next_real_insn (seq[i - 1]);
      if (seq[i] == 0)
        return 0;
    }

  if (!pdp10_reorg_set_parts (seq[0], &dest, &src)
      || !REG_P (dest)
      || GET_CODE (src) != PLUS
      || !rtx_equal_p (XEXP (src, 0), dest)
      || !REG_P (XEXP (src, 1)))
    return 0;
  sum = dest;
  ptr = XEXP (src, 1);

  if (!pdp10_reorg_set_parts (seq[1], &tmp, &src)
      || !REG_P (tmp)
      || !rtx_equal_p (src, ptr))
    return 0;

  if (!pdp10_reorg_set_parts (seq[2], &base, &src)
      || !REG_P (base)
      || !CONSTANT_P (src)
      || !pdp10_const_byte_pointer_p (src))
    return 0;

  if (!pdp10_reorg_set_parts (seq[3], &dest, &src)
      || !rtx_equal_p (dest, tmp)
      || GET_CODE (src) != MINUS
      || !rtx_equal_p (XEXP (src, 0), tmp)
      || !rtx_equal_p (XEXP (src, 1), base))
    return 0;

  if (!pdp10_reorg_set_parts (seq[4], &dest, &src)
      || GET_CODE (src) != UNSPEC
      || XINT (src, 1) != 12)
    return 0;

  if (!pdp10_reorg_set_parts (seq[5], &diff, &src)
      || !REG_P (diff))
    return 0;

  if (!pdp10_reorg_set_parts (seq[6], &dest, &src)
      || !rtx_equal_p (dest, diff)
      || GET_CODE (src) != ASHIFTRT)
    return 0;

  if (!pdp10_reorg_set_parts (seq[7], &dest, &src)
      || !rtx_equal_p (dest, diff)
      || GET_CODE (src) != PLUS)
    return 0;
  x = XEXP (src, 1);
  if (GET_CODE (x) != MEM)
    return 0;
  x = XEXP (x, 0);
  if (GET_CODE (x) != PLUS
      || GET_CODE (XEXP (x, 1)) != SYMBOL_REF
      || strcmp (XSTR (XEXP (x, 1), 0), "%BADL9") != 0)
    return 0;

  if (!pdp10_reorg_set_parts (seq[8], &dest, &src)
      || !rtx_equal_p (dest, sum)
      || GET_CODE (src) != PLUS
      || !rtx_equal_p (XEXP (src, 0), sum)
      || !rtx_equal_p (XEXP (src, 1), diff))
    return 0;

  delete_insn (seq[0]);
  return 1;
}

static void
pdp10_reorg_folded_subbp_sums (first)
     rtx first;
{
  rtx insn;

  for (insn = next_real_insn (first); insn; insn = next_real_insn (insn))
    {
      if (INSN_DELETED_P (insn))
        continue;
      pdp10_reorg_folded_subbp_sum (insn);
    }
}

/* Called by the MACHINE_DEPENDENT_REORG macro.

   This is ran as the last optimization pass.  (If there was a delayed
   branch scheduling pass in the PDP-10 compiler, it would run after
   this pass.)  */
void
pdp10_machine_dependent_reorg (first)
     rtx first;
{
  rtx insn;

  pdp10_reorg_folded_subbp_sums (first);
  pdp10_reorg_word_repack_copies (first);

  if (!optimize)
    return;

  for (insn = first; insn; insn = NEXT_INSN (insn))
    {
      /* We only care about INSNs, JUMP_INSNs, and CALL_INSNs.
	 Also ignore any special USE insns.  */
      if ((GET_CODE (insn) != INSN
	   && GET_CODE (insn) != JUMP_INSN
	   && GET_CODE (insn) != CALL_INSN)
	  || GET_CODE (PATTERN (insn)) == USE
	  || GET_CODE (PATTERN (insn)) == CLOBBER
	  || GET_CODE (PATTERN (insn)) == ADDR_VEC
	  || GET_CODE (PATTERN (insn)) == ADDR_DIFF_VEC)
	continue;

      if (get_attr_reorg_type (insn) == REORG_TYPE_LDB
	  || get_attr_reorg_type (insn) == REORG_TYPE_DPB)
	{
	  rtx links;
	  for (links = LOG_LINKS (insn); links; links = XEXP (links, 1))
	    {
	      rtx prev = XEXP (links, 0);
	      rtx insn_set_src, insn_set_dest, prev_set_dest;

	      if (GET_CODE (prev) != INSN
		  || get_attr_reorg_type (prev) != REORG_TYPE_IBP)
		continue;

	      insn_set_src = SET_SRC (PATTERN (insn));
	      insn_set_dest = SET_DEST (PATTERN (insn));
	      prev_set_dest = SET_DEST (PATTERN (prev));

	      /* Check for DPB instruction.  */
	      if (GET_CODE (insn_set_dest) == MEM
		  && GET_CODE (prev_set_dest) == REG
		  && GET_CODE (XEXP (insn_set_dest, 0)) == REG
		  && REGNO (prev_set_dest) == REGNO (XEXP (insn_set_dest, 0)))
		{
		  rtx pre_inc = gen_rtx_PRE_INC (Pmode, prev_set_dest);
		  XEXP (insn_set_dest, 0) = pre_inc;
		  INSN_DELETED_P (prev) = 1;
		}

	      /* Check for LDB instruction.  */
	      if (GET_CODE (insn_set_src) == ZERO_EXTEND
		  || GET_CODE (insn_set_src) == SIGN_EXTEND)
		insn_set_src = XEXP (insn_set_src, 0);
	      if (GET_CODE (insn_set_src) == MEM
		  && GET_CODE (prev_set_dest) == REG
		  && GET_CODE (XEXP (insn_set_src, 0)) == REG
		  && REGNO (prev_set_dest) == REGNO (XEXP (insn_set_src, 0)))
		{
		  rtx pre_inc = gen_rtx_PRE_INC (Pmode, prev_set_dest);
		  XEXP (insn_set_src, 0) = pre_inc;
		  INSN_DELETED_P (prev) = 1;
		}
	    }
	}
    }
}


/**********************************************************************

	Special Predicates

**********************************************************************/

/* Return the known alignment, in storage units, of pointer X.  */
int
pdp10_pointer_alignment (x)
     rtx x;
{
  /* This function must be prepared to handle anything
     GO_IF_LEGITIMATE_ADDRESS says is a valid address.  */

  switch (GET_CODE (x))
    {
    case CONST_INT:
      return 1;
    case CONST:
      if (!TARGET_EXTENDED)
	{
	  HOST_WIDE_INT offset;
	  int ps, position, bytesize;

	  offset = INTVAL (XEXP (XEXP (x, 0), 1));
	  ps = (offset >> 24) & 07777;
	  if (ps == 0)
	    return UNITS_PER_WORD;
	  if (decode_ps (ps, &position, &bytesize) == -1)
	    abort ();
	  if (position == 0)
	    return UNITS_PER_WORD;
	  if (position == 18)
	    return UNITS_PER_WORD / 2;
	  return 1;
	}
      switch ((int)(INTVAL (XEXP (XEXP (x, 0), 1)) >> 30) & 077)
	{
	case 0:
	case 075:
	case 070:
	  return 4;
	case 076:
	case 072:
	  return 2;
	case 071:
	case 073:
	  return 1;
	default:
	  abort ();
	}
    case LABEL_REF:
    case SYMBOL_REF:
      /* Symbols are always word-aligned.  */
      return UNITS_PER_WORD;
    case PLUS:
      if (!CONSTANT_ADDRESS_P (XEXP (x, 1)))
	{
	  debug_rtx (x);
	  abort ();
	}
      x = XEXP (x, 0);
      /* Fall through.  */
    case REG:
    reg:
      if (REG_POINTER (x))
	return REGNO_POINTER_ALIGN (REGNO (x)) / BITS_PER_UNIT;
      else
	return 1;
    case PRE_INC:
      x = XEXP (x, 0);
      if (REG_P (x))
	goto reg;
      return 1;
    case MEM:
      return 1;
    default:
      abort ();
    }
}

/* Return nonzero if OP is a valid PDP-10 comparison condition.  */
int
pdp10_comparison_operator (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  switch (GET_CODE (op))
    {
    case EQ:
    case NE:
    case LT:
    case LE:
    case GT:
    case GE:
      return 1;
    default:
      return 0;
    }
}

/* Return nonzero if OP is a valid PDP-10 equality comparision
   condition.  */
int
pdp10_equality_operator (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  enum rtx_code code = GET_CODE (op);
  return code == EQ || code == NE;
}

/* Return nonzero if OP is a rotation operation.  */
int
pdp10_rotate_operator (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  enum rtx_code code = GET_CODE (op);
  return code == ROTATE || code == ROTATERT;
}

/* Return nonzero if OP is a valid PDP-10 memory operand.  */
int
reg_or_mem_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return register_operand (op, mode) || memory_operand (op, mode);
}

/* Return the selected PDP-10 word offset for

     (subreg:SI (subreg:DI (reg:CDI R) A) B)

   or -1 if DEST is not that shape.  A and B are GCC addressable-unit
   offsets.  On this target UNITS_PER_WORD selects the next PDP-10 word.  */
static int
pdp10_cdi_subreg_word_offset (dest, base)
     rtx dest;
     rtx *base;
{
  int bytes;
  rtx x;

  if (GET_CODE (dest) != SUBREG || GET_MODE (dest) != SImode)
    return -1;

  bytes = SUBREG_BYTE (dest);
  x = SUBREG_REG (dest);

  if (GET_CODE (x) != SUBREG || GET_MODE (x) != DImode)
    return -1;

  bytes += SUBREG_BYTE (x);
  x = SUBREG_REG (x);

  if (!REG_P (x) || GET_MODE (x) != CDImode)
    return -1;

  if ((bytes % UNITS_PER_WORD) != 0)
    return -1;

  if (base != 0)
    *base = x;

  return bytes / UNITS_PER_WORD;
}

/* Output a scalar PDP-10 word move into a word of a complex-DImode
   register.  Recognition is intentionally done by direct lvalue patterns in
   pdp10.md; this helper only computes the selected word register for final
   assembly output.  */
const char *
pdp10_output_movsi_cdi_subreg (insn, operands)
     rtx insn;
     rtx *operands;
{
  rtx dest = SET_DEST (PATTERN (insn));
  rtx base = 0;
  rtx ops[2];
  int offset;

  offset = pdp10_cdi_subreg_word_offset (dest, &base);
  if (offset < 0)
    abort ();

  ops[0] = gen_rtx_REG (SImode, REGNO (base) + offset);
  ops[1] = operands[1];

  output_asm_insn ("move %0,%1", ops);
  return "";
}

/* Output a scalar PDP-10 word store from a word of a complex-DImode
   register.  */
const char *
pdp10_output_movsi_from_cdi_subreg (insn, operands)
     rtx insn;
     rtx *operands;
{
  rtx src = SET_SRC (PATTERN (insn));
  rtx base = 0;
  rtx ops[2];
  int offset;

  offset = pdp10_cdi_subreg_word_offset (src, &base);
  if (offset < 0)
    abort ();

  ops[0] = operands[0];
  ops[1] = gen_rtx_REG (SImode, REGNO (base) + offset);

  output_asm_insn ("movem %1,%0", ops);
  return "";
}

/* Return nonzero if OP is a valid PDP-10 register-or-memory operand,
   but only allow memory operands after register allocation has
   finished.  */
int
preferably_register_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return
    register_operand (op, mode)
    || (reload_completed && memory_operand (op, mode));
}

/* Return nonzero if OP is a memory operand that may be volatile.  */
int
pdp10_maybe_volatile_memory_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  int saved = volatile_ok;
  int result;

  volatile_ok = 1;
  result = memory_operand (op, mode);
  volatile_ok = saved;
  return result;
}

/* Return nonzero if X is an address based on one of the frame
   registers.  Stack scalar slots often reach recognition as plain
   QI/HI memory references through the frame pointer, without
   MEM_SCALAR_P set and without useful MEM_ALIGN information.  */
static int
pdp10_frame_address_p (x)
     rtx x;
{
  switch (GET_CODE (x))
    {
    case REG:
      {
	unsigned int regno = REGNO (x);

	/* REGNO_PTR_FRAME_P does not include the real stack pointer on
	   this GCC, but stack slots have been eliminated to SP by the
	   time the old peephole pass and final output see them.  Accept
	   both the real frame/stack registers and the virtual frame
	   registers used before elimination.  */
	return regno == STACK_POINTER_REGNUM
	       || regno == HARD_FRAME_POINTER_REGNUM
	       || regno == FRAME_POINTER_REGNUM
	       || regno == ARG_POINTER_REGNUM
	       || REGNO_PTR_FRAME_P (regno);
      }

    case SUBREG:
      return pdp10_frame_address_p (SUBREG_REG (x));

    case ADDRESSOF:
      if (GET_CODE (XEXP (x, 0)) == MEM)
	return pdp10_frame_address_p (XEXP (XEXP (x, 0), 0));
      return pdp10_frame_address_p (XEXP (x, 0));

    case PLUS:
      return pdp10_frame_address_p (XEXP (x, 0))
	     || pdp10_frame_address_p (XEXP (x, 1));

    case CONST:
      x = XEXP (x, 0);
      if (GET_CODE (x) == PLUS)
	return pdp10_frame_address_p (XEXP (x, 0))
	       || pdp10_frame_address_p (XEXP (x, 1));
      return 0;

    default:
      return 0;
    }
}

/* Strip output-only address wrappers that can legitimately survive to final
   in old GCC RTL, and normalize the few virtual frame-register addresses that
   the generic instantiator can leave inside target-specific address values.

   This routine is deliberately for printing only.  It does not claim a new
   addressing mode; it merely exposes the address already carried by
   ADDRESSOF/MEM wrappers and converts virtual-stack-vars to the same
   frame-pointer-plus-starting-offset form used by instantiate_virtual_regs.  */
static rtx
pdp10_output_unwrap_address (x)
     rtx x;
{
  if (x == NULL_RTX)
    return x;

  switch (GET_CODE (x))
    {
    case REG:
      if (x == virtual_stack_vars_rtx)
	return plus_constant (frame_pointer_rtx, pdp10_starting_frame_offset ());
      return x;

    case ADDRESSOF:
      if (GET_CODE (XEXP (x, 0)) == MEM)
	return pdp10_output_unwrap_address (XEXP (XEXP (x, 0), 0));
      return x;

    case PLUS:
      {
	rtx op0 = pdp10_output_unwrap_address (XEXP (x, 0));
	rtx op1 = pdp10_output_unwrap_address (XEXP (x, 1));

	if (op0 == XEXP (x, 0) && op1 == XEXP (x, 1))
	  return x;

	if (GET_CODE (op1) == CONST_INT)
	  return plus_constant (op0, INTVAL (op1));
	return gen_rtx_PLUS (GET_MODE (x), op0, op1);
      }

    case UNSPEC:
      if (XINT (x, 1) == UNSPEC_ADDRESS)
	{
	  rtx inner = pdp10_output_unwrap_address (XVECEXP (x, 0, 0));
	  if (inner != XVECEXP (x, 0, 0))
	    return gen_ADDRESS (inner);
	}
      return x;

    default:
      return x;
    }
}

/* Return address X plus word-address OFFSET for final-output printing.
   X may still contain old GCC output wrappers such as ADDRESSOF or the
   PDP-10 UNSPEC_ADDRESS effective-address wrapper.  Keep UNSPEC_ADDRESS
   wrapped around the adjusted inner address; otherwise ordinary final
   address printing sees PLUS(UNSPEC_ADDRESS, CONST_INT), which is not a
   PDP-10 addressing form.  */
static rtx
pdp10_output_add_address_offset (x, offset)
     rtx x;
     HOST_WIDE_INT offset;
{
  x = pdp10_output_unwrap_address (x);

  if (x == NULL_RTX)
    return GEN_INT (offset);

  if (offset == 0)
    return x;

  if (GET_CODE (x) == UNSPEC && XINT (x, 1) == UNSPEC_ADDRESS)
    return gen_ADDRESS (plus_constant (XVECEXP (x, 0, 0), offset));

  return plus_constant (x, offset);
}

/* Return the source integer precision for MEM if MEM still carries
   a useful tree expression.  GCC can represent small target-specific scalar
   integer types such as char9 and short16 in HImode even though their C
   precision is smaller than 18 bits.  Final-output code needs the C precision
   to choose the correct byte pointer and sign extension.  */
static int
pdp10_mem_integer_precision (mem)
     rtx mem;
{
  tree expr, type;

  if (GET_CODE (mem) != MEM || MEM_EXPR (mem) == 0)
    return 0;

  expr = MEM_EXPR (mem);
  type = TREE_TYPE (expr);

  if (type != 0 && TREE_CODE (type) == INTEGER_TYPE)
    return TYPE_PRECISION (type);

  return 0;
}

/* Return a conservative precision hint for a scalar memory object when
   MEM_EXPR has been lost before final output.  This happens for signed
   char9 volatile globals after reload: the final pattern is just

       (sign_extend:SI (mem:HI SYMBOL))

   and the old HRRE fast path sign-extends bit 17 instead of the scalar
   sign bit.  GCC still tends to preserve the target scalar alignment
   (for example A9 for char9, A16 for short16), so use that as a final
   fallback for aligned scalar HImode MEMs.  Do not guess for full
   halfword objects; A18 and word-aligned objects should keep the normal
   HRRE path.  */
static int
pdp10_mem_scalar_precision_hint (mem)
     rtx mem;
{
  int align;

  if (GET_CODE (mem) != MEM || GET_MODE (mem) != HImode)
    return 0;

  align = MEM_ALIGN (mem);
  if (align > 0 && align < 18)
    return align;

  return 0;
}

/* Return nonzero if OP is a scalar QI/HI object stored in a full
   PDP-10 word.  MEM_SCALAR_P catches named scalar objects.  Automatic
   scalar variables can lose that bit.

   A second wrinkle is visible in early RTL for stack locals: GCC has
   already represented the storage as an SImode stack slot, while
   keeping the source scalar size in the MEM attributes, e.g.

        (mem/v/f:SI (reg/f:SI virtual-stack-vars) [x+0 S1 A9])

   Such MEMs are scalar stack slots too.  Accept SImode frame MEMs only
   when their recorded alignment is smaller than a word; ordinary int
   stack slots have word alignment and must not be mistaken for scalar
   byte/halfword storage.  */
int
pdp10_scalar_mem_p (op)
     rtx op;
{
  if (GET_CODE (op) != MEM)
    return 0;

  /* A local byte-pointer variable can have subword alignment in its MEM
     attributes because it points at byte objects.  The storage slot itself
     is still a full PDP-10 pointer word, not a scalar QI/HI object.  Do not
     let halfword scalar-store patterns rewrite pointer-address updates as
     HRLM; byte-pointer arithmetic needs to update the right-half address
     field with HRRM.  */
  if (MEM_EXPR (op) != 0 && POINTER_TYPE_P (TREE_TYPE (MEM_EXPR (op))))
    return 0;

  if (MEM_SCALAR_P (op))
    return 1;

  if (!pdp10_frame_address_p (XEXP (op, 0)))
    return 0;

  if (GET_MODE (op) == QImode || GET_MODE (op) == HImode)
    return 1;

  if (GET_MODE (op) == SImode && MEM_ALIGN (op) < BITS_PER_WORD)
    return 1;

  return 0;
}

/* Return nonzero if OP is a memory operand with a constant address.  */
int
pdp10_constaddr_memory_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return memory_operand (op, mode) && CONSTANT_ADDRESS_P (XEXP (op, 0));
}

/* Return nonzero if OP is a valid PDP-10 push operand.  */
int
pdp10_push_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != MEM)
    return 0;
  if (mode != VOIDmode && GET_MODE (op) != mode)
    return 0;
  op = XEXP (op, 0);
  if (GET_CODE (op) != PRE_INC)
    return 0;
  op = XEXP (op, 0);
  /* Since PUSH is a slow instruction, only allow registers other than
     then stack pointer when optimizing for small code size.  */
  return REG_P (op) && (REGNO (op) == STACK_POINTER_REGNUM
			|| (optimize_size && TARGET_EXTENDED));
}

/* Return nonzero if OP is a valid PDP-10 pop operand.  */
int
pdp10_pop_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) != MEM)
    return 0;
  if (mode != VOIDmode && GET_MODE (op) != mode)
    return 0;
  op = XEXP (op, 0);
  if (GET_CODE (op) != POST_DEC)
    return 0;
  op = XEXP (op, 0);
  /* Since POP is a slow instruction, only allow registers other than
     then stack pointer when optimizing for small code size.  */
  return REG_P (op) && (REGNO (op) == STACK_POINTER_REGNUM
			|| (optimize_size && TARGET_EXTENDED));
}

/* Return nonzero if OP is a floating-point zero constant.  */
int
pdp10_const_double_0_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  REAL_VALUE_TYPE r;
  long y[3];

  if (mode != VOIDmode && GET_MODE (op) != mode)
    return 0;

  if (GET_CODE (op) != CONST_DOUBLE)
    return 0;

  REAL_VALUE_FROM_CONST_DOUBLE (r, op);
  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

  switch (mode)
    {
    case SFmode:
      return
	(y[0] & 0xffffffffUL) == 0
	&& (y[1] & 0xf0000000UL) == 0;
    case DFmode:
      return
	(y[0] & 0xffffffffUL) == 0
	&& (y[1] & 0xffffffffUL) == 0
	&& (y[2] & 0xff000000UL) == 0;
    default:
      abort ();
    }
}

#if 0
int
pdp10_halfword_destination (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (mode != SImode)
    return 0;

  if (GET_CODE (op) == ZERO_EXTRACT)
    return
      GET_CODE (XEXP (op, 1)) == CONST_INT
      && INTVAL (XEXP (op, 1)) == 18
      && GET_CODE (XEXP (op, 2)) == CONST_INT
      && (INTVAL (XEXP (op, 2)) == 0 || INTVAL (XEXP (op, 2)) == 18);

  if (GET_CODE (op) == SUBREG)
    return
      GET_CODE (XEXP (op, 0)) == MEM
      && GET_MODE (XEXP (op, 0)) == HImode
      && SUBREG_BYTE (op) == 0;

  return 0;
}

int
pdp10_halfword_source (op, mode)
     rtx op;
     enum machine_mode mode;
{
  if (mode != SImode)
    return 0;

  if (GET_CODE (op) == ZERO_EXTRACT || GET_CODE (op) == SIGN_EXTRACT)
    return
      GET_CODE (XEXP (op, 1)) == CONST_INT
      && INTVAL (XEXP (op, 1)) == 18
      && GET_CODE (XEXP (op, 2)) == CONST_INT
      && (INTVAL (XEXP (op, 2)) == 0 || INTVAL (XEXP (op, 2)) == 18);

  if (GET_CODE (op) == SUBREG)
    return
      GET_CODE (XEXP (op, 0)) == MEM
      && GET_MODE (XEXP (op, 0)) == HImode
      && SUBREG_BYTE (op) == 0;

  if (GET_CODE (op) == AND)
    return
      GET_CODE (XEXP (op, 1)) == CONST_INT
      && INTVAL (XEXP (op, 1)) == 0777777;

  if (GET_CODE (op) == ASHIFTRT || GET_CODE (op) == LSHIFTRT)
    return
      GET_CODE (XEXP (op, 1)) == CONST_INT
      && INTVAL (XEXP (op, 1)) == 18;

  return 0;
}
#endif


/**********************************************************************

	Built-in Functions

**********************************************************************/

static rtx	expand_builtin_jffo PARAMS ((tree arglist, rtx target));
static rtx	expand_builtin_pdp10_jffo PARAMS ((tree arglist, rtx target));
static rtx	pdp10_direct_label_operand PARAMS ((rtx x));
static int	pdp10_emit_jffo_word_branch PARAMS ((rtx word, rtx label));
static rtx	expand_builtin_fsc PARAMS ((tree arglist, rtx target,
					    enum rtx_code code));

/* Called by the MD_INIT_BUILTINS macro.

   Performs the necessary setup for machine-specific builtin
   functions.  */
void
pdp10_init_builtins ()
{
#define PTRTYPE(a)	built_pointer_type (a)
#define NIL		void_list_node
#define CONS(a,b)	tree_cons (NULL_TREE, a, b)
#define FUNTYPE(a,b)	build_function_type (a, b)
#define FUNTYPE0(a)	FUNTYPE (a, NIL)
#define FUNTYPE1(a,b)	FUNTYPE (a, CONS (b, NIL))
#define FUNTYPE2(a,b,c)	FUNTYPE (a, CONS (b, CONS (c, NIL)))
#define VOID		void_type_node
#define VOIDP		ptr_type_node
#define INT		integer_type_node
#define UINT		unsigned_type_node
#define CHAR		char_type_node
#define LONGLONG	long_long_integer_type_node
#define FLOAT		float_type_node
#define DOUBLE		double_type_node

  builtin_function ("__builtin_fsc",
		    FUNTYPE2 (FLOAT, FLOAT, INT),
		    PDP10_BUILTIN_FSC,
		    BUILT_IN_MD,
		    0);

  builtin_function ("__builtin_dfsc",
		    FUNTYPE2 (DOUBLE, DOUBLE, INT),
		    PDP10_BUILTIN_DFSC,
		    BUILT_IN_MD,
		    0);

  if (HAVE_JFFO)
    {
      builtin_function ("__builtin_jffo",
			FUNTYPE2 (VOID, LONGLONG, VOIDP),
			PDP10_BUILTIN_JFFO,
			BUILT_IN_MD,
			0);

      builtin_function ("__builtin_pdp10_jffo",
			FUNTYPE1 (INT, UINT),
			PDP10_BUILTIN_JFFO,
			BUILT_IN_MD,
			0);
    }

#undef PTRTYPE
#undef NIL
#undef CONS
#undef FUNTYPE0
#undef FUNTYPE1
#undef FUNTYPE2
#undef VOID
#undef VOIDP
#undef INT
#undef UINT
#undef CHAR
#undef LONGLONG
#undef FLOAT
#undef DOUBLE
}

/* Return the CODE_LABEL referred to by X, if X is a direct label
   value.  GCC may represent &&label either as a plain LABEL_REF or as
   a PDP-10 pointer constant such as CONST (PLUS (LABEL_REF label)
   byte-pointer-bits).  The JFFO generators wrap their label operand
   in LABEL_REF themselves, so callers must pass the raw CODE_LABEL.  */
static rtx
pdp10_direct_label_operand (x)
     rtx x;
{
  rtx y;

  if (x == NULL_RTX)
    return NULL_RTX;

  if (GET_CODE (x) == LABEL_REF)
    {
      y = XEXP (x, 0);
      return GET_CODE (y) == CODE_LABEL ? y : NULL_RTX;
    }

  if (GET_CODE (x) == CONST)
    {
      y = XEXP (x, 0);

      if (GET_CODE (y) == LABEL_REF)
	{
	  y = XEXP (y, 0);
	  return GET_CODE (y) == CODE_LABEL ? y : NULL_RTX;
	}

      if (GET_CODE (y) == PLUS
	  && GET_CODE (XEXP (y, 0)) == LABEL_REF
	  && GET_CODE (XEXP (XEXP (y, 0), 0)) == CODE_LABEL
	  && GET_CODE (XEXP (y, 1)) == CONST_INT)
	return XEXP (XEXP (y, 0), 0);
    }

  return NULL_RTX;
}

/* Emit a JFFO branch testing one 36-bit word for nonzero.

   JFFO writes AC+1 when the tested AC is nonzero.  Use a fresh DImode
   pseudo for the hardware pair so that clobber cannot corrupt the
   original DImode value being tested.  */
static int
pdp10_emit_jffo_word_branch (word, label)
     rtx word;
     rtx label;
{
  rtx pair;
  rtx pat;

  pair = gen_reg_rtx (DImode);
  emit_move_insn (gen_rtx_SUBREG (SImode, pair, 0), word);

  pat = gen_JFFO_branch (pair, label, GEN_NE);
  if (!pat)
    return 0;

  emit_jump_insn (pat);
  return 1;
}

/* Expand a call to __builtin_jffo or __builtin_pdp10_jffo.  */
static rtx
expand_builtin_jffo (arglist, target)
     tree arglist;
     rtx target;
{
  tree arg0, arg1;
  rtx op0, op1;
  rtx label, taken, done;

  /* The historical builtin is the jump-form primitive:

       void __builtin_jffo (long long value, void *label)

     DAIMON tests and C code also use a value-returning convenience
     builtin:

       int __builtin_pdp10_jffo (unsigned int value)

     Both are registered with the same machine builtin code; dispatch
     on the argument count here.  */
  if (arglist != NULL_TREE && TREE_CHAIN (arglist) == NULL_TREE)
    return expand_builtin_pdp10_jffo (arglist, target);

  arg0 = TREE_VALUE (arglist);
  arg1 = TREE_VALUE (TREE_CHAIN (arglist));
  op0 = expand_expr (arg0, NULL_RTX, VOIDmode, 0);
  op1 = expand_expr (arg1, NULL_RTX, VOIDmode, 0);

  op0 = force_reg (DImode, op0);

  label = pdp10_direct_label_operand (op1);
  if (label != NULL_RTX)
    {
      if (!pdp10_emit_jffo_word_branch
	  (gen_rtx_SUBREG (SImode, op0, 0), label))
	return 0;
      if (!pdp10_emit_jffo_word_branch
	  (gen_rtx_SUBREG (SImode, op0, UNITS_PER_WORD), label))
	return 0;
      return target;
    }

  if (!address_operand (op1, Pmode))
    op1 = force_reg (Pmode, op1);

  /* Computed label value.  GCC's CFG cannot safely represent a single
     conditional computed jump with fallthrough here.  Branch to a local
     block with real JFFO tests, then do the indirect jump there.  */
  taken = gen_label_rtx ();
  done = gen_label_rtx ();

  if (!pdp10_emit_jffo_word_branch
      (gen_rtx_SUBREG (SImode, op0, 0), taken))
    return 0;
  if (!pdp10_emit_jffo_word_branch
      (gen_rtx_SUBREG (SImode, op0, UNITS_PER_WORD), taken))
    return 0;

  emit_jump (done);

  emit_label (taken);
  emit_jump_insn (gen_indirect_jump (op1));
  emit_barrier ();

  emit_label (done);
  return target;
}

/* Expand int __builtin_pdp10_jffo (unsigned int).

   Return the JFFO bit number, or -1 when the input word is zero.
   JFFO writes the bit number to AC+1 only when AC is nonzero, so the
   zero case must be supplied explicitly.  */
static rtx
expand_builtin_pdp10_jffo (arglist, target)
     tree arglist;
     rtx target;
{
  tree arg0;
  rtx op0, pair, label, done, pat;

  arg0 = TREE_VALUE (arglist);
  op0 = expand_expr (arg0, NULL_RTX, VOIDmode, 0);
  op0 = force_reg (SImode, op0);

  if (target == 0 || !REG_P (target) || GET_MODE (target) != SImode)
    target = gen_reg_rtx (SImode);

  pair = gen_reg_rtx (DImode);
  label = gen_label_rtx ();
  done = gen_label_rtx ();

  emit_move_insn (gen_rtx_SUBREG (SImode, pair, 0), op0);
  pat = gen_JFFO (pair, label);
  if (!pat)
    return 0;
  emit_jump_insn (pat);

  emit_move_insn (target, GEN_INT (-1));
  emit_jump (done);

  emit_label (label);
  emit_move_insn (target, gen_rtx_SUBREG (SImode, pair, 4));

  emit_label (done);
  return target;
}

/* Expand a call to __builtin_(d)fsc.  */
static rtx
expand_builtin_fsc (arglist, target, code)
     tree arglist;
     rtx target;
     enum pdp10_builtins code;
{
  tree arg0, arg1;
  rtx op0, op1, pat;
  enum machine_mode mode;

  if (code == PDP10_BUILTIN_FSC)
    mode = SFmode;
  else if (code == PDP10_BUILTIN_DFSC)
    mode = DFmode;
  else
    abort ();

  arg0 = TREE_VALUE (arglist);
  arg1 = TREE_VALUE (TREE_CHAIN (arglist));
  op0 = expand_expr (arg0, NULL_RTX, VOIDmode, 0);
  op1 = expand_expr (arg1, NULL_RTX, VOIDmode, 0);
  op0 = force_reg (mode, op0);
  op1 = force_not_mem (op1);

  if (target == 0 || !REG_P (target) || GET_MODE (target) != mode)
    target = gen_reg_rtx (mode);

  if (code == PDP10_BUILTIN_FSC)
    pat = gen_FSC (target, op0, op1);
  else if (TARGET_GFLOAT)
    pat = gen_GFSC (target, op0, op1);
  else
    pat = gen_DFSC (target, op0, op1);
  if (!pat)
    return 0;
  emit_insn (pat);

  return target;
}

/* Called by the MD_EXPAND_BUILTIN macro.

   Expand a call to a machine specific builtin that was set up by
   MD_INIT_BUILTINS.  EXP is the expression for the function call; the
   result should go to TARGET if that is convenient, and have mode
   MODE if that is convenient.  SUBTARGET may be used as the target
   for computing one of EXP's operands. IGNORE is nonzero if the value
   is to be ignored.  */
rtx
pdp10_expand_builtin (exp, target, subtarget, mode, ignore)
     tree exp;
     rtx target;
     rtx subtarget ATTRIBUTE_UNUSED;
     enum machine_mode mode ATTRIBUTE_UNUSED;
     int ignore ATTRIBUTE_UNUSED;
{
  tree arglist = TREE_OPERAND (exp, 1);
  enum pdp10_builtins code =
    DECL_FUNCTION_CODE (TREE_OPERAND (TREE_OPERAND (exp, 0), 0));

  switch (code)
    {
    case PDP10_BUILTIN_JFFO:
      return expand_builtin_jffo (arglist, target);
    case PDP10_BUILTIN_FSC:
    case PDP10_BUILTIN_DFSC:
      return expand_builtin_fsc (arglist, target, code);
    default:
      abort ();
    }
}

static int
pdp10_pad_varargs_down_p (size)
     HOST_WIDE_INT size;
{
  /* Stack argument slots are word-aligned.  Objects that occupy less
     than one word are placed in the low-addressed padding area's far
     end, matching the ordinary big-endian PDP-10 argument layout.  */
  return size > 0 && size < PARM_BOUNDARY / BITS_PER_UNIT;
}

tree
pdp10_build_va_list_type ()
{
  return build_pointer_type (integer_type_node);
}

void
pdp10_expand_builtin_va_start (stdarg_p, valist, nextarg)
     int stdarg_p;
     tree valist;
     rtx nextarg;
{
  int offset = 0;
  tree t;

  if (current_function_varargs)
    /* When current_function_varargs goes away, remember that stdarg_p
       is always true.  */
    ;

  if (!stdarg_p)
    offset -= 1;

  nextarg = plus_constant (nextarg, offset);
  t = build (MODIFY_EXPR, TREE_TYPE (valist), valist,
	     make_tree (integer_type_node, nextarg));
  TREE_SIDE_EFFECTS (t) = 1;

  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);
}

rtx
pdp10_expand_builtin_va_arg (valist, type)
     tree valist, type;
{
  tree addr_tree, t;
  HOST_WIDE_INT align;
  HOST_WIDE_INT size;
  HOST_WIDE_INT rounded_size;
  rtx addr;

  /* Compute the rounded size of the type.  */
  align = PARM_BOUNDARY / BITS_PER_UNIT;
  size = int_size_in_bytes (type);
  if (size < 0)
    abort ();
  rounded_size = (((size + align - 1) / align) * align);

  /* Get AP.  */
  addr_tree = valist;

  /* Compute new value for AP.  */
  t = build (MODIFY_EXPR, TREE_TYPE (valist), valist,
	     build (PLUS_EXPR, TREE_TYPE (valist), valist,
		    build_int_2 (-rounded_size / UNITS_PER_WORD, 0)));
  TREE_SIDE_EFFECTS (t) = 1;
  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);

  if (pdp10_pad_varargs_down_p (size))
    {
      /* Small args are padded downward within their stack word.  */

      HOST_WIDE_INT adj
	= rounded_size > align ? rounded_size : size;

      addr_tree = build (PLUS_EXPR, TREE_TYPE (addr_tree), addr_tree,
			 build_int_2 (rounded_size - adj, 0));
    }

  addr = expand_expr (addr_tree, NULL_RTX, Pmode, EXPAND_NORMAL);
  addr = copy_to_reg (addr);

  return addr;
}


/**********************************************************************

	Support for MACRO, MIDAS, and FAIL

**********************************************************************/

#ifndef __PDP10_GAS_H__

static int	asciz_ok PARAMS ((const char *ptr, int len));
static void	macro_output_ascii_bytes PARAMS ((FILE *stream,
						  unsigned const char *ptr,
						  int len,
						  int byte_size));
static bool	maybe_output_extern PARAMS ((struct hash_entry *he,
					     hash_table_key ptr));
static struct hash_entry *
		extern_symbol_newfunc PARAMS ((struct hash_entry *he,
					       struct hash_table *table,
					       hash_table_key string));
static void	init_extern_symbol_table PARAMS ((void));
static const char *
		section_name;

struct extern_symbol_entry
{
  struct hash_entry he;
  const char *name;
  int flags;
};

static struct hash_table extern_symbol_table;

/* Called by the ASM_FILE_START macro.

   Outputs to the stdio stream STREAM some appropriate text to go at
   the start of an assembler file.  */
void
macro_file_start (file)
     FILE *file;
{
  pdp10_build_machine_types ();

  if (main_input_filename)
    {
      const char *title = main_input_filename + strlen (main_input_filename);
      while (title != main_input_filename && !IS_DIR_SEPARATOR (title[-1]))
	title--;
      fputs ("\tTITLE\t", file);
      while (*title && memcmp (title, ".c", 3) != 0)
	{
	  char c = *title++;
	  if (ISDIGIT (c) || ISALPHA (c))
	    fputc (c, file);
	}
      fputc ('\n', file);
    }

  if (TARGET_KA10up)
    {
      fputs ("\t.DIRECTIVE\t", file);
      fputs (TARGET_KL10up ? (TARGET_EXTENDED ? "KL10" : "KS10,KL10") :
	     TARGET_KI10up ? "KI10,KS10,KL10" :
			     "KA10,KI10,KS10,KL10",
	     file);
      fputs ("\n", file);
    }

  fputc ('\n', file);

  if (TARGET_EXTENDED)
    {
      fputs ("DEFINE OWGBP (PS,Y)<<PS>B5+<Y>*%ONE>\t", file);
      fputs (ASM_COMMENT_START, file);
      fputs (" one-word global byte pointer\n", file);
      fputs ("DEFINE GIW (Y)<<Y>*%ONE>\t", file);
      fputs (ASM_COMMENT_START, file);
      fputs (" global indirect word\n", file);
#if XKL_STUFF
      if (debug_info_level != DINFO_LEVEL_NONE)
	{
	  /* These used to say ...<Y>*%ONE...  */
	  fputs ("DEFINE DBE (Y)<<Y>!77B5>\n", file);
	  fputs ("DEFINE DBL (Y)<<Y>!44B5>\n", file);
	}
#endif
      fputs ("DEFINE %EXIND (I,X,Y)<<I>B1+<X>B5+<<Y>*%ONE>>\t", file);
      fputs (ASM_COMMENT_START, file);
      fputs (" extended format indirect word\n", file);
      fputs (TEXT_SECTION_ASM_OP, file);
      fputc ('\n', file);
    }
  else
    fputs ("\tEXTERN %BADL6,%BADL7,%BADL8,%BADL9,%BADLH\n\n", file);
}

static bool
maybe_output_extern (he, ptr)
     struct hash_entry *he;
     hash_table_key ptr ATTRIBUTE_UNUSED;
{
  struct extern_symbol_entry *entry = (struct extern_symbol_entry *) he;
  if (entry->flags == (PDP10_SYMBOL_USED | PDP10_SYMBOL_EXTERN))
    {
      fputs ("\tEXTERN\t", asm_out_file);
      assemble_name (asm_out_file, entry->name);
      fputc ('\n', asm_out_file);
    }
  return true;
}

/* Called by the ASM_FILE_END macro.

   Outputs to the stdio stream STREAM some appropriate text to go at
   the end of an assembler file.  */
void
macro_file_end (file)
     FILE *file;
{
  int i;

  ASM_SECTION_END (file);
  fputc ('\n', file);

  if (pdp10_giw)
    fputs ("\tEXTERN\t%ONE\n", file);

  for (i = 6; i <= 18; i++)
    if (used_byte_subtraction_table[i])
      fprintf (file, "\tEXTERN\t%%BAD%c%c\n",
	       TARGET_EXTENDED ? 'X' : 'L', i == 18 ? 'H' : '0' + i);

  hash_traverse (&extern_symbol_table, maybe_output_extern, NULL);
  fputc ('\n', file);

  fprintf (file, "%s expanded load unsigned 16  left: %4d\n",
	   ASM_COMMENT_START, expand_extzv_16_0);
  fprintf (file, "%s output   load unsigned 16  left: %4d\n",
	   ASM_COMMENT_START, output_extzv_16_0);
  fprintf (file, "%s expanded load unsigned 16 right: %4d\n",
	   ASM_COMMENT_START, expand_extzv_16_1);
  fprintf (file, "%s output   load unsigned 16 right: %4d\n",
	   ASM_COMMENT_START, output_extzv_16_1);
  fprintf (file, "%s expanded load unsigned 32:       %4d\n",
	   ASM_COMMENT_START, expand_extzv_32);
  fprintf (file, "%s output   load unsigned 32:       %4d\n",
	   ASM_COMMENT_START, output_extzv_32);
  fprintf (file, "%s output   load   signed  9:       %4d\n",
	   ASM_COMMENT_START, pdp10_output_ldbe_9);
  fprintf (file, "%s expanded load   signed 16  left: %4d\n",
	   ASM_COMMENT_START, expand_extv_16_0);
  fprintf (file, "%s output   load   signed 16  left: %4d\n",
	   ASM_COMMENT_START, output_extv_16_0);
  fprintf (file, "%s expanded load   signed 16 right: %4d\n",
	   ASM_COMMENT_START, expand_extv_16_1);
  fprintf (file, "%s output   load   signed 16 right: %4d\n",
	   ASM_COMMENT_START, output_extv_16_1);
  fprintf (file, "%s expanded load   signed 32:       %4d\n",
	   ASM_COMMENT_START, expand_extv_32);
  fprintf (file, "%s output   load   signed 32:       %4d\n",
	   ASM_COMMENT_START, output_extv_32);
  fprintf (file, "%s expanded load   signed:          %4d\n",
	   ASM_COMMENT_START, expand_extv);
  fprintf (file, "%s output   load   signed:          %4d\n",
	   ASM_COMMENT_START, output_extv);
  fprintf (file, "%s expanded ffs:                    %4d\n",
	   ASM_COMMENT_START, pdp10_expand_ffs);
  fputs ("\n\tEND\n", file);
}

void
macro_section_end (stream)
     FILE *stream;
{
  if (TARGET_EXTENDED)
    {
      if (section_name && strncmp (".db", section_name, 3) == 0)
	pdp10_align_with_pad (0400);
      pdp10_output_byte (stream, 0, 0);
      fputs ("\t.ENDPS\n", stream);
    }
  section_name = NULL;
}

/* Print an ASCII (or SIXBIT) string with characters of BYTE_SIZE
   bits, without using the ASCIZ pseudo.  SIXBIT is output if
   BYTE_SIZE is 6.  */
static void
macro_output_ascii_bytes (stream, ptr, len, byte_size)
     FILE *stream;
     const unsigned char *ptr;
     int len;
     int byte_size;
{
  if (len == 0)
    return;

  while (len--)
    {
      int c = *ptr++;
      if (byte_size == 6)
	{
	  c = TOUPPER (c) - 040;
	  if (c < 0 || c > 077)
	    c = 0;
	}
      pdp10_output_byte (stream, GEN_INT (c), byte_size);
      pdp10_byte_count++;
    }
}

/* Return separator character if the string can be output using the
   ASCIZ pseudo, or 0 otherwise.  */
static int
asciz_ok (ptr, len)
     const char *ptr;
     int len;
{
  char *separators = xstrdup ("/$|%!");
  int c, i;
  char *p;

  while (len--)
    {
      c = *ptr++;
      if (c && (p = strchr (separators, c)))
	*p = 'x';
      if (c >= 32 && c <= 126)
	continue;

      if (c == '\r' && len > 0 && *ptr == '\n')
	{
	  ptr++;
	  len--;
	  continue;
	}

      if (c == 0 && len == 0)
	continue;

      free (separators);
      return 0;
    }

  c = 0;
  for (i = 0; i < (int) strlen (separators); i++)
    if (separators[i] != 'x')
      {
	c = separators[i];
	break;
      }

  free (separators);
  return c;
}

/* Called by the ASM_OUTPUT_ASCII macro.

   Outputs to the stdio stream STREAM an assembler instruction to
   assemble a string constant.  */
void
macro_output_ascii (stream, ptr, len)
     FILE *stream;
     const char *ptr;
     int len;
{
  int byte_size = pdp10_string_literal_byte_size;
  int separator;
  int c;

  /* If the ASCIZ pseudo can't be used, output the string byte by byte.  */
  if (byte_size != 7 || (separator = asciz_ok (ptr, len)) == 0)
    {
      /* Note that 6-bit characters are converted to SIXBIT.  */
      macro_output_ascii_bytes (stream, (unsigned char *)ptr, len, byte_size);
      return;
    }

  fputs ("\tASCIZ\t", stream);
  fputc (separator, stream);

  while (len--)
    {
      c = *ptr++;
      if (c == '\r')
	{
	  len--;
	  ptr++;
	  fputc ('\n', stream);
	}
      else if (c == 0 && len == 0)
	;
      else
	fputc (c, stream);
    }

  fputc (separator, stream);
  fputc ('\n', stream);
}

/* Static variables for pdp10_output_byte.  */
static int bits_in_word;
static rtx last_output_byte = NULL_RTX;
static int last_byte_size;

/* Called primarily by the ASM_OUTPUT_CHAR and ASM_OUTPUT_SHORT
   macros, but also from many other places.

   If EXP is NULL, then output a newline; the next byte will be put in
   a new word.  Otherwise, output the value of EXP as a byte of
   BYTE_SIZE bits.  If the byte doesn't fit in the same word as the
   previous byte, output a newline and put the byte in a new word.  */
void
pdp10_output_byte (stream, exp, byte_size)
     FILE *stream;
     rtx exp;
     int byte_size;
{
  int mask;

  if (last_output_byte == NULL_RTX)
    {
      if (exp == NULL_RTX || byte_size == 0)
	return;
      fprintf (stream, "\tBYTE (%d)", byte_size);
      mask = (1 << byte_size) - 1;
      /*pdp10_print_integer (stream, INTVAL (exp) & mask);*/
      pdp10_print_number (stream, INTVAL (exp) & mask, 1, byte_size, 0);
      bits_in_word = byte_size;
    }
  else if (exp)
    {
      if (bits_in_word + byte_size > 36)
	{
	  pdp10_output_byte (stream, 0, 0);
	  pdp10_output_byte (stream, exp, byte_size);
	  return;
	}
      if (byte_size == last_byte_size)
	fputc (',', stream);
      else
	fprintf (stream, "(%d)", byte_size);
      mask = (1 << byte_size) - 1;
      /*pdp10_print_integer (stream, INTVAL (exp) & mask);*/
      pdp10_print_number (stream, INTVAL (exp) & mask, 1, byte_size, 0);
      bits_in_word += byte_size;
    }
  else
    {
      extern const char *dw2_assemble_string[5];
      extern int dw2_assemble_integer_size[5];
      extern rtx dw2_assemble_integer_x[5];
      extern int dw2_assemble_integer_n;
      extern HOST_WIDE_INT dw2_assemble_uleb128[5];
      int i;

      bits_in_word = 0;
      fputc ('\n', stream);

      for (i = 0; i < dw2_assemble_integer_n; i++)
	{
	  fprintf (stream, "\t%s ", ASM_COMMENT_START);
	  if (dw2_assemble_integer_x[i])
	    {
	      if (dw2_assemble_integer_size[i] > 1)
		fprintf (stream, ".%dbyte ", dw2_assemble_integer_size[i]);
	      else
		fputs (".byte ", stream);
	      if (GET_CODE (dw2_assemble_integer_x[i]) == CONST_INT)
		fprintf (stream, "0x%llx", INTVAL (dw2_assemble_integer_x[i]));
	      else
		pdp10_print_operand (asm_out_file, dw2_assemble_integer_x[i], 0);
	    }
	  else if (dw2_assemble_string[i])
	    fprintf (stream, ".string \"%s\"", dw2_assemble_string[i]);
	  else
	    {
	      fputs (".uleb128 ", stream);
	      fprintf (stream, HOST_WIDE_INT_PRINT_HEX,
		       dw2_assemble_uleb128[i]);
	    }

	  fputc ('\n', stream);
	}

      dw2_assemble_integer_n = 0;
      memset (dw2_assemble_integer_x, 0, sizeof dw2_assemble_integer_x);
      memset (dw2_assemble_string, 0, sizeof dw2_assemble_string);
    }

  last_byte_size = byte_size;
  last_output_byte = exp;
}

int
pdp10_align_with_pad (pad)
     int pad;
{
  rtx x = GEN_INT (pad);
  int n = pdp10_byte_count;

  if (bits_in_word > 0)
    while (bits_in_word < 36)
      {
	pdp10_output_byte (asm_out_file, x, 9);
	/* Do not count pad bytes by increasing pdp10_byte_count.  */
      }

  return pdp10_byte_count - n;
}

/* Called by the ASM_GLOBALIZE_LABEL macro.

   Outputs to the stdio stream STREAM some commands that will make the
   label NAME global; that is, available for reference from other
   files.  */
void
macro_globalize_label (stream, name)
     FILE *stream;
     const char *name;
{
  pdp10_output_byte (stream, 0, 0);
  fputs ("\tENTRY\t", stream);
  assemble_name (stream, name);
  fputc ('\n', stream);
}

void
macro_output_def (stream, label, value)
     FILE *stream;
     const char *label;
     const char *value;
{
  assemble_name (stream, label);
  fputs ("=", stream);
  assemble_name (stream, value);
  fprintf (stream, "\n");

  pdp10_extern_symbol (label, PDP10_SYMBOL_DEFINED);
}

/* Switch to an arbitrary section NAME with attributes as specified by
   FLAGS.  */
static void
pdp10_asm_named_section	(name, flags)
     const char *name;
     unsigned int flags;
{
  section_name = name;

  fputs ("\t.PSECT\t", asm_out_file);
  fputs (name, asm_out_file);
  if (flags & SECTION_WRITE)
    fputs ("/rwrite", asm_out_file);
  else
    fputs ("/ronly", asm_out_file);

  if (flags & (SECTION_CODE | SECTION_WRITE | SECTION_DEBUG | SECTION_BSS
	       | SECTION_SMALL | SECTION_LINKONCE | SECTION_FORGET))
    {
      fputs ("\t;", asm_out_file);
      if (flags & SECTION_CODE)
	fputs (" code", asm_out_file);
      if (flags & SECTION_DEBUG)
	fputs (" debug", asm_out_file);
      if (flags & SECTION_LINKONCE)
	fputs (" linkonce", asm_out_file);
      if (flags & SECTION_SMALL)
	fputs (" small", asm_out_file);
      if (flags & SECTION_BSS)
	fputs (" bss", asm_out_file);
      if (flags & SECTION_FORGET)
	fputs (" forget", asm_out_file);
    }

  fputc ('\n', asm_out_file);
}

/* Create a new entry for the extern_symbol_table hash table.  */
static struct hash_entry *
extern_symbol_newfunc (he, table, string)
     struct hash_entry *he;
     struct hash_table *table;
     hash_table_key string ATTRIBUTE_UNUSED;
{
  struct extern_symbol_entry *entry = (struct extern_symbol_entry *) he;

  if (entry == NULL)
    {
      entry = ((struct extern_symbol_entry *)
	       hash_allocate (table, sizeof (struct extern_symbol_entry)));
      if (entry == NULL)
	return NULL;
    }

  entry->name = NULL;
  entry->flags = 0;
  return (struct hash_entry *) entry;
}

static void
init_extern_symbol_table ()
{
  static int inited = 0;

  if (!inited)
    {
      hash_table_init (&extern_symbol_table,
		       extern_symbol_newfunc,
		       string_hash,
		       string_compare);
      inited = 1;
    }
}

void
pdp10_extern_symbol (name, flags)
     const char *name;
     int flags;
{
  struct extern_symbol_entry *entry;

  init_extern_symbol_table ();

  entry = ((struct extern_symbol_entry *)
	   hash_lookup (&extern_symbol_table, (const hash_table_key) name, 
			true, string_copy));
  if (!entry->name)
    entry->name = xstrdup (name);
  entry->flags |= flags;
}

int pdp10_byte_count = 0;
int pdp10_bits_per_unit = BITS_PER_UNIT;

static bool
pdp10_assemble_integer (x, size, aligned_p)
     rtx x;
     unsigned int size;
     int aligned_p;
{
  if (GET_CODE (x) == UNSPEC && XINT (x, 1) == UNSPEC_ADJBP)
    {
      rtx folded = pdp10_simplify_unspec (x, NULL_RTX);
      if (folded != NULL_RTX)
	x = folded;
    }

  if (aligned_p
      && size == 4
      && pdp10_const_word36_byte_pointer_p (x))
    {
      pdp10_output_byte (asm_out_file, 0, 0);
      fputc ('\t', asm_out_file);
      pdp10_print_raw_word36_constant (asm_out_file, x);
      fputc ('\n', asm_out_file);
      pdp10_byte_count += size;
      return true;
    }

  if (!aligned_p && size > 1)
    return false;

  switch (size)
    {
    case 1:
    case 2:
      pdp10_output_byte (asm_out_file, x, pdp10_bits_per_unit * size);
      break;
    case 4:
    case 8:
      if (aligned_p)
	{
	  pdp10_output_byte (asm_out_file, 0, 0);
	  fputc ('\t', asm_out_file);
	  pdp10_print_operand (asm_out_file, x,
			       (size == 8 ? 'D' : 0) | PRINT_OP_GIW);
	  fputc ('\n', asm_out_file);
	  break;
	}
    default:
      return false;
    }

  pdp10_byte_count += size;

  return true;
}

#endif /* !__PDP10_GAS_H__ */

#ifdef __PDP10_GAS_H__
/* GAS-mode stubs: these symbols are defined in the !GAS block above but
   are also referenced unconditionally from varasm.c, dwarf2asm.c, and
   tops20dbgout.c.  Provide simple versions for the GAS/custom assembler
   build.  */

/* Switch to an arbitrary section NAME with attributes as specified by
   FLAGS.  The old MACRO path also emits .PSECT directives; the
   GCC/GAS-ish DAIMOS bring-up assembler accepts the same small subset.
   Do not use the MACRO-only section_name variable here, because it is
   intentionally not compiled in GAS mode.  */
static void
pdp10_asm_named_section (name, flags)
     const char *name;
     unsigned int flags;
{
  fputs ("\t.PSECT\t", asm_out_file);
  fputs (name, asm_out_file);

  if (flags & SECTION_WRITE)
    fputs ("/rwrite", asm_out_file);
  else
    fputs ("/ronly", asm_out_file);

  if (flags & (SECTION_CODE | SECTION_WRITE | SECTION_DEBUG | SECTION_BSS
               | SECTION_SMALL | SECTION_LINKONCE | SECTION_FORGET))
    {
      fputs ("\t;", asm_out_file);
      if (flags & SECTION_CODE)
        fputs (" code", asm_out_file);
      if (flags & SECTION_DEBUG)
        fputs (" debug", asm_out_file);
      if (flags & SECTION_LINKONCE)
        fputs (" linkonce", asm_out_file);
      if (flags & SECTION_SMALL)
        fputs (" small", asm_out_file);
      if (flags & SECTION_BSS)
        fputs (" bss", asm_out_file);
      if (flags & SECTION_FORGET)
        fputs (" forget", asm_out_file);
    }

  fputc ('\n', asm_out_file);
}

int pdp10_byte_count = 0;
int pdp10_bits_per_unit = BITS_PER_UNIT;

/* GAS-mode byte packing state.  The generic varasm code calls
   targetm.asm_out.integer once per array element.  For PDP-10 byte
   arrays, those elements are 6, 7, 8, or 9 bit target bytes packed into
   36-bit words, so the GAS/custom-assembler path needs the same packing
   service that the MACRO path gets from BYTE pseudo output.  */
static HOST_WIDE_INT gas_byte_word;
static int gas_bits_in_word;

void
pdp10_output_byte (stream, exp, byte_size)
     FILE *stream;
     rtx exp;
     int byte_size;
{
  HOST_WIDE_INT mask;
  HOST_WIDE_INT value;

  if (exp == NULL_RTX || byte_size == 0)
    {
      if (gas_bits_in_word > 0)
	{
	  gas_byte_word <<= BITS_PER_WORD - gas_bits_in_word;
	  fputs ("\t.word\t", stream);
	  pdp10_print_unsigned (stream, gas_byte_word);
	  fputc ('\n', stream);
	  gas_byte_word = 0;
	  gas_bits_in_word = 0;
	}
      return;
    }

  if (GET_CODE (exp) != CONST_INT)
    abort ();

  if (byte_size <= 0 || byte_size > BITS_PER_WORD)
    abort ();

  if (gas_bits_in_word + byte_size > BITS_PER_WORD)
    pdp10_output_byte (stream, NULL_RTX, 0);

  mask = ((HOST_WIDE_INT) 1 << byte_size) - 1;
  value = INTVAL (exp) & mask;
  gas_byte_word = (gas_byte_word << byte_size) | value;
  gas_bits_in_word += byte_size;

  if (gas_bits_in_word == BITS_PER_WORD)
    pdp10_output_byte (stream, NULL_RTX, 0);
}

void
pdp10_asm_output_skip (stream, nbytes)
     FILE *stream;
     int nbytes;
{
  while (nbytes > 0 && gas_bits_in_word > 0)
    {
      pdp10_output_byte (stream, const0_rtx, BITS_PER_UNIT);
      nbytes--;
    }

  if (nbytes >= UNITS_PER_WORD)
    {
      int words = nbytes / UNITS_PER_WORD;

      fprintf (stream, "\t.space\t%d\n", words);
      nbytes -= words * UNITS_PER_WORD;
    }

  /* A residual subword skip at a word boundary is padding already covered
     by the previous full-word emission.  There is no subword location
     counter in the bare PDP-10 assembler stream.  */
}


void
pdp10_gas_output_ascii (stream, ptr, len)
     FILE *stream;
     const char *ptr;
     int len;
{
  HOST_WIDE_INT word;
  int byte_size;
  int bits;

  byte_size = pdp10_string_literal_byte_size;
  word = 0;
  bits = 0;

  while (len-- > 0)
    {
      int c = (unsigned char) *ptr++;
      int mask;

      if (byte_size == 6)
	{
	  c = TOUPPER (c) - 040;
	  if (c < 0 || c > 077)
	    c = 0;
	}

      mask = (1 << byte_size) - 1;
      c &= mask;

      if (bits + byte_size > BITS_PER_WORD)
	{
	  word <<= BITS_PER_WORD - bits;
	  fputs ("\t.word\t", stream);
	  pdp10_print_unsigned (stream, word);
	  fputc ('\n', stream);
	  word = 0;
	  bits = 0;
	}

      word = (word << byte_size) | c;
      bits += byte_size;
      pdp10_byte_count++;

      if (bits == BITS_PER_WORD)
	{
	  fputs ("\t.word\t", stream);
	  pdp10_print_unsigned (stream, word);
	  fputc ('\n', stream);
	  word = 0;
	  bits = 0;
	}
    }

  if (bits > 0)
    {
      word <<= BITS_PER_WORD - bits;
      fputs ("\t.word\t", stream);
      pdp10_print_unsigned (stream, word);
      fputc ('\n', stream);
    }
}


/* GAS/custom-assembler path: support a symbolic label difference stored in
   the low half of a packed 18-bit short pair.

   labels-3.c contains:

       static const short ar[] = { &&l1 - &&l1, &&l2 - &&l1 };

   The first element is an ordinary CONST_INT and leaves an 18-bit pending
   high half in gas_byte_word.  The second element expands as a HImode
   MINUS of two PDP-10 label pointer constants.  Generic varasm cannot split
   that symbolic HImode value into target bytes, so assemble it here as one
   complete 36-bit word:

       .word  <pending-high-half shifted left 18> + label2 - label1

   For the common {0, labeldiff} case this becomes just:

       .word  label2-label1

   which the local PDP-10 assembler resolves and naturally leaves in the
   right halfword.  */
static int
pdp10_gas_labeldiff_halfword_part (x, labelp, offsetp)
     rtx x;
     rtx *labelp;
     HOST_WIDE_INT *offsetp;
{
  rtx a;
  rtx b;

  *offsetp = 0;

  while (GET_CODE (x) == SUBREG || GET_CODE (x) == CONST)
    {
      if (GET_CODE (x) == SUBREG)
	x = SUBREG_REG (x);
      else
	x = XEXP (x, 0);
    }

  if (GET_CODE (x) == LABEL_REF)
    {
      *labelp = x;
      return 1;
    }

  if (GET_CODE (x) != PLUS)
    return 0;

  a = XEXP (x, 0);
  b = XEXP (x, 1);

  if (GET_CODE (a) == LABEL_REF && GET_CODE (b) == CONST_INT)
    {
      *labelp = a;
      *offsetp = INTVAL (b);
      return 1;
    }

  if (GET_CODE (b) == LABEL_REF && GET_CODE (a) == CONST_INT)
    {
      *labelp = b;
      *offsetp = INTVAL (a);
      return 1;
    }

  return 0;
}

static int
pdp10_gas_labeldiff_halfword_p (x, leftp, rightp, deltap)
     rtx x;
     rtx *leftp;
     rtx *rightp;
     HOST_WIDE_INT *deltap;
{
  HOST_WIDE_INT loff;
  HOST_WIDE_INT roff;

  if (GET_CODE (x) == CONST)
    x = XEXP (x, 0);

  if (GET_CODE (x) != MINUS)
    return 0;

  if (! pdp10_gas_labeldiff_halfword_part (XEXP (x, 0), leftp, &loff))
    return 0;

  if (! pdp10_gas_labeldiff_halfword_part (XEXP (x, 1), rightp, &roff))
    return 0;

  *deltap = loff - roff;
  return 1;
}

static void
pdp10_gas_print_labeldiff_halfword_expr (stream, left, right, delta)
     FILE *stream;
     rtx left;
     rtx right;
     HOST_WIDE_INT delta;
{
  output_addr_const (stream, left);
  fputc ('-', stream);
  output_addr_const (stream, right);

  if (delta > 0)
    {
      fputc ('+', stream);
      pdp10_print_unsigned (stream, delta);
    }
  else if (delta < 0)
    {
      fputc ('-', stream);
      pdp10_print_unsigned (stream, -delta);
    }
}

static int
pdp10_gas_assemble_labeldiff_halfword (x)
     rtx x;
{
  rtx left;
  rtx right;
  HOST_WIDE_INT delta;
  HOST_WIDE_INT high_word;

  if (! pdp10_gas_labeldiff_halfword_p (x, &left, &right, &delta))
    return 0;

  /* This helper is for the second halfword of a packed pair.  If a future
     case needs a symbolic first halfword, handle that separately rather
     than silently changing the object layout.  */
  if (gas_bits_in_word != 18)
    return 0;

  high_word = (gas_byte_word & (HOST_WIDE_INT) 0777777) << 18;

  fputs ("\t.word\t", asm_out_file);
  if (high_word != 0)
    {
      pdp10_print_unsigned (asm_out_file, high_word);
      fputc ('+', asm_out_file);
    }
  pdp10_gas_print_labeldiff_halfword_expr (asm_out_file, left, right, delta);
  fputc ('\n', asm_out_file);

  gas_byte_word = 0;
  gas_bits_in_word = 0;
  return 1;
}

static bool
pdp10_assemble_integer (x, size, aligned_p)
     rtx x;
     unsigned int size;
     int aligned_p;
{
  rtx base;
  HOST_WIDE_INT offset;

  if (GET_CODE (x) == UNSPEC && XINT (x, 1) == UNSPEC_ADJBP)
    {
      rtx folded = pdp10_simplify_unspec (x, NULL_RTX);
      if (folded != NULL_RTX)
	x = folded;
    }

  if (size == 2 && pdp10_gas_assemble_labeldiff_halfword (x))
    {
      pdp10_byte_count += size;
      return true;
    }

  if (aligned_p
      && size == 4
      && pdp10_const_word36_byte_pointer_p (x))
    {
      pdp10_output_byte (asm_out_file, NULL_RTX, 0);
      fputs ("	.long	", asm_out_file);
      pdp10_print_raw_word36_constant (asm_out_file, x);
      fputc ('\n', asm_out_file);
      pdp10_byte_count += size;
      return true;
    }

  if (aligned_p
      && ((size == 4 && GET_CODE (x) == CONST_DOUBLE && GET_MODE (x) == SFmode)
          || (size == 8 && GET_CODE (x) == CONST_DOUBLE && GET_MODE (x) == DFmode)))
    {
      pdp10_output_byte (asm_out_file, NULL_RTX, 0);
      fputc ('	', asm_out_file);
      pdp10_print_operand (asm_out_file, x, PRINT_OP_GIW);
      fputc ('\n', asm_out_file);
      pdp10_byte_count += size;
      return true;
    }

  if (aligned_p
      && size == 4
      && (pdp10_const_byte_pointer_p (x)
	  || (!TARGET_EXTENDED
	      && pdp10_decompose_symbolic_constant (x, &base, &offset))))
    {
      pdp10_output_byte (asm_out_file, NULL_RTX, 0);

      if (TARGET_EXTENDED)
	{
	  fputc ('	', asm_out_file);
	  pdp10_print_operand (asm_out_file, x, PRINT_OP_GIW);
	}
      else
	{
	  fputs ("	.long	", asm_out_file);
	  pdp10_print_raw_word_constant (asm_out_file, x);
	}

      fputc ('\n', asm_out_file);
      pdp10_byte_count += size;
      return true;
    }

  if ((size == 1 || size == 2) && GET_CODE (x) == CONST_INT)
    {
      pdp10_output_byte (asm_out_file, x, pdp10_bits_per_unit * size);
      pdp10_byte_count += size;
      return true;
    }

  return false;
}

int
pdp10_align_with_pad (pad)
     int pad ATTRIBUTE_UNUSED;
{
  pdp10_output_byte (asm_out_file, NULL_RTX, 0);
  return 0;
}
#endif /* __PDP10_GAS_H__ */



/**********************************************************************

	Optimizations

**********************************************************************/

static int	single_insn PARAMS ((rtx insn, rtx label1, rtx label2));

/* Helper function for pdp10_output_range_compare.  Examines whether a
   single instruction is suitable to replace the JRST in the range
   comparision.  */
static int
single_insn (insn, label1, label2)
     rtx insn;
     rtx label1;
     rtx label2;
{
  rtx jump;

  if (get_attr_length (insn) != 1
      || get_attr_skip (insn) == SKIP_YES)
    return 0;

  jump = next_nonnote_insn (insn);
  return (jump == NULL_RTX
	  || (GET_CODE (jump) == BARRIER && !simplejump_p (insn))
	  || (GET_CODE (jump) == CODE_LABEL && jump == label2)
	  || (simplejump_p (jump)
	      && label1 == XEXP (SET_SRC (PATTERN (jump)), 0)));
}

/* Output instructions to conditionally jump to operands[3] depending
   on the result of a comparison of the register in operands[0]
   against a range represented by operands[1] and operands[2].  Return
   an empty string.

   The RTL insn being output looks like this:
   
	(set (pc)
	     (if_then_else
	      (ge (xor:SI (plus:SI
			   (match_operand:SI 1 "register_operand" "r")
			   (match_operand:SI 2 "const_int_operand" "i"))
			  (const_int SIGNBIT))
		  (match_operand:SI 3 "const_int_operand" "i"))
	      (label_ref (match_operand 4 "" ""))
	      (pc)))

   And is actually a combination of three insns:

	;; Subtract the low end of the range from operands[0].
	(set (reg:SI X)
	     (plus:SI (match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "const_int_operand" "i")))
	
	;; Complement the sign bit in preparation for an
	;; unsigned comparison.
	(set (reg:SI Y)
	     (xor:SI (reg:SI X) (const_int SIGNBIT)))
	
	;; Compare the result against the high end of the range,
	;; and jump if it's larger or equal.
	(set (pc)
	     (if_then_else
	      (ge (reg:SI Y)
		  (match_operand:SI 3 "const_int_operand" "i"))
	      (label_ref (match_operand 4 "" ""))
	      (pc)))

   If the comparison if reversed (jump if operands[1] is inside the
   range), the comparison is "less than" instead, and TRNA is nonzero.

   The output looks something like this:

	CAIL op1,-op2			; skip if op1 less than low end
	 CAIL op1,op3^SIGNBIT - op2	; skip if op1 less than high end
	  JRST op4			; jump

   CAML may be used instead of CAIL if necessary.  A TNRA is output
   last if the comparison should be inverted.  If the JRST jump target
   is just one instruction, the JRST may be replaced by that
   instruction.  */

/* Output an integer or single-float conditional branch.  The PDP-10
   branch patterns have two forms: a direct one-word branch/skip when the
   label is immediately reachable, and a two-word skip-over-JRST form for
   non-adjacent labels.  Keep the reversed condition in a local operands
   copy so callers do not mutate recog's operand vector.  */
const char *
pdp10_output_cbranch (insn, ops, alternative, float_p)
     rtx insn;
     rtx *ops;
     int alternative;
     int float_p;
{
  static const char *const si_asms[] =
  { "jump%3 %1,%F0",
    "skip%3 %@%1",
    "cai%3 %1,%2",
    "cam%3 %1,[%X2]",
    "cam%3 %1,%2" };
  static const char *const sf_asms[] =
  { "jump%3 %1,%F0",
    "skip%3 %@%1",
    "cai%3 %1,%2",
    "cam%3 %1,[%2]",
    "cam%3 %1,%2" };
  rtx operands[4];
  const char *const *asms;

  memcpy (operands, ops, sizeof operands);

  if (alternative != 0)
    pdp10_remove_unnecessary_label (insn, operands[0]);

  asms = float_p ? sf_asms : si_asms;

  /* ADDRESS: unextended code can store the symbol in the immediate
     field.  */
  if (!float_p && alternative == 3 && !TARGET_EXTENDED
      && GET_CODE (operands[2]) == CONST)
    alternative = 2;

  if (get_attr_length (insn) == 1)
    return asms[alternative];

  operands[3] = gen_rtx_fmt_ee (reverse_condition (GET_CODE (operands[3])),
                                VOIDmode, NULL_RTX, NULL_RTX);
  output_asm_insn (asms[alternative], operands);
  return pdp10_output_jrst (operands[0]);
}

const char *
pdp10_output_range_compare (insn, ops, trna)
     rtx insn;
     rtx *ops;
     int trna;
{
  rtx jump_target, label;
  HOST_WIDE_INT op2;
  rtx operands[5];

  memcpy (operands, ops, sizeof operands);

  /* Compare against the low end of the range.  */
  op2 = INTVAL (operands[2]);
  if (GET_CODE (operands[1]) == MEM)
    {
      if (op2 != 0)
	abort ();
      output_asm_insn ("skipl %0,%1", operands);
    }
  else
    {
      operands[2] = gen_int_mode (-op2, SImode);
      if (INTVAL (operands[2]) & ~HWINT (0777777))
	output_asm_insn ("caml %0,[%2]", operands);
      else
	output_asm_insn ("cail %0,%2", operands);
    }

  /* Compare against the high end of the range.  */
  operands[3] = gen_int_mode ((INTVAL (operands[3]) ^ SIGN_BIT) - op2, SImode);
  if (INTVAL (operands[3]) & ~HWINT (0777777))
    output_asm_insn ("%_caml %0,[%3]", operands);
  else
    output_asm_insn ("%_cail %0,%3", operands);

  if (trna)
    {
      /* Output a TRNA instrcution unless the next instruction is a
	 single instruction, in which case the comparison is reversed
	 (by not emitting the TRNA) and no jump instruction is output.  */

      rtx next_insn = next_real_insn (insn);

      if (!single_insn (next_insn, NULL_RTX, operands[4]))
	output_asm_insn ("%_%_trna", operands);
      else
	{
	  if (LABEL_NUSES (operands[4]) == 1)
	    INSN_DELETED_P (operands[4]) = 1;
	  pdp10_opcode_indentation = 2;
	  return "";
	}
    }

  label = next_nonnote_insn (insn);
  jump_target = next_nonnote_insn (operands[4]);

  /* Ouput the JRST instruction unless the jump target is a single
     instruction (and perhaps a jump back).  If so, replace the JRST
     by that instruction.  */
  if (LABEL_NUSES (operands[4]) == 1
      && single_insn (jump_target, label, NULL_RTX))
    {
      rtx jump_back = next_nonnote_insn (jump_target);

      if (LABEL_NUSES (operands[4]) == 1)
	INSN_DELETED_P (operands[4]) = 1;
      if (GET_CODE (jump_back) != BARRIER)
	{
	  INSN_DELETED_P (jump_back) = 1;
	  if (LABEL_NUSES (label) == 1)
	    INSN_DELETED_P (label) = 1;
	}
      pdp10_opcode_indentation = 2 + trna;
      final_scan_insn (jump_target, asm_out_file, optimize, 0, 0);
      INSN_DELETED_P (jump_target) = 1;
    }
  else
    {
      pdp10_opcode_indentation = 2 + trna;
      pdp10_output_jrst (operands[4]);
    }

  return "";
}


/**********************************************************************

	Data Movement

**********************************************************************/

static void	normalize_bitfield_mem PARAMS ((rtx *x, int *pos));
static rtx	pdp10_find_last_value PARAMS ((rtx x, rtx *pinsn));
static const char *
		output_halfword_move PARAMS ((rtx reg, rtx x, int reg_right,
					      int mem_right, int store,
					      int extend, int sign));
static const char *
		output_bitfield_move PARAMS ((FILE *stream, rtx insn,
					      int store, int sign, rtx reg,
					      rtx x, int len, int bit));
static int	pdp10_reg_unused_note_p PARAMS ((rtx insn, int regno));
static int	pdp10_movdi_unused_parts PARAMS ((rtx insn, rtx destination,
					       rtx source));
static int	use_xblt PARAMS ((rtx destination, rtx source, rtx length));
static int	expand_blt PARAMS ((rtx destination, rtx source, rtx length));
static int	expand_xblt PARAMS ((rtx destination, rtx source, rtx length));
static int	expand_blt_clear PARAMS ((rtx destination, rtx length));
static int	expand_xblt_clear PARAMS ((rtx destination, rtx length));
#if TARGET_STRING
static int	expand_movslj PARAMS ((rtx dest, rtx source, rtx length));
static int	expand_movst_clear PARAMS ((rtx destination, rtx length));
#endif


/* Return nonzero if INSN has a REG_UNUSED note for hard register REGNO.  */
static int
pdp10_reg_unused_note_p (insn, regno)
     rtx insn;
     int regno;
{
  rtx note;

  for (note = REG_NOTES (insn); note; note = XEXP (note, 1))
    if (REG_NOTE_KIND (note) == REG_UNUSED)
      {
	rtx x = XEXP (note, 0);

	if (GET_CODE (x) == REG && REGNO (x) == regno)
	  return 1;
      }

  return 0;
}

/* Return a bit mask describing unused destination words of a DImode move.
   Bit 0 means the high word is unused, bit 1 means the low word is unused.
   Do not optimize volatile memory reads, since a volatile DImode load must
   still perform both word reads even if one destination register is dead.  */
static int
pdp10_movdi_unused_parts (insn, destination, source)
     rtx insn;
     rtx destination;
     rtx source;
{
  rtx dst = destination;
  int regno;
  int unused = 0;

  if (GET_CODE (source) == MEM && MEM_VOLATILE_P (source))
    return 0;

  if (GET_CODE (dst) == SUBREG)
    dst = SUBREG_REG (dst);

  if (GET_CODE (dst) != REG)
    return 0;

  regno = REGNO (dst);
  if (regno >= FIRST_PSEUDO_REGISTER)
    return 0;

  if (pdp10_reg_unused_note_p (insn, regno))
    unused |= 1;
  if (pdp10_reg_unused_note_p (insn, regno + 1))
    unused |= 2;

  return unused;
}

/* Output a DImode move.  HAVE_DMOVE is nonzero for KI10-and-later targets
   where the live full-width move can use DMOVE/DMOVEM.  If reload/flow has
   proved one destination word dead with REG_UNUSED, avoid copying that word
   when doing so does not suppress a volatile memory access.  */
const char *
pdp10_output_movdi (insn, operands, which_alternative, have_dmove)
     rtx insn;
     rtx *operands;
     int which_alternative;
     int have_dmove;
{
  int unused;

  unused = pdp10_movdi_unused_parts (insn, operands[0], operands[1]);

  if (unused == 3)
    return "";

  switch (which_alternative)
    {
    case 0:
      if (unused & 1)
	return "move %Z0,%Z1";
      if (unused & 2)
	return "move %0,%1";
      if (!have_dmove)
	{
	  rtx dst = operands[0];
	  rtx src = operands[1];
	  int dstno = -1;
	  int srcno = -1;

	  if (GET_CODE (dst) == SUBREG)
	    dst = SUBREG_REG (dst);
	  if (GET_CODE (src) == SUBREG)
	    src = SUBREG_REG (src);

	  if (GET_CODE (dst) == REG)
	    dstno = REGNO (dst);
	  if (GET_CODE (src) == REG)
	    srcno = REGNO (src);

	  if (dstno >= 0 && srcno >= 0)
	    {
	      if (dstno == srcno)
		return "";
	      if (dstno == srcno + 1)
		return "move %Z0,%Z1\n\tmove %0,%1";
	    }
	}
      return have_dmove ? "dmove %0,%1" : "move %0,%1\n\tmove %Z0,%Z1";

    case 1:
      if (unused & 1)
	return "setz %Z0,";
      if (unused & 2)
	return "setz %0,";
      return "setzb %0,%Z0";

    case 2:
      if (unused & 1)
	return "movei %Z0,%1";
      if (unused & 2)
	return "movei %0,0";
      return "movei %0,0\n\tmovei %Z0,%1";

    case 3:
      if (have_dmove)
	{
	  if (unused & 1)
	    return "movni %Z0,%n1";
	  if (unused & 2)
	    return "seto %0,";
	  return "seto %0,\n\tmovni %Z0,%n1";
	}
      else if (TARGET_71BIT && !TARGET_KI10up)
	{
	  static char buf[128];
	  unsigned long v;

	  v = (unsigned long) -INTVAL (operands[1]);
	  if (unused & 1)
	    sprintf (buf,
		     "setz %%Z0,\n\tsubi %%Z0,%lo\n\ttlz %%Z0,400000\n\ttlo %%Z0,400000",
		     v);
	  else if (unused & 2)
	    sprintf (buf, "seto %%0,");
	  else
	    sprintf (buf,
		     "seto %%0,\n\tsetz %%Z0,\n\tsubi %%Z0,%lo\n\ttlz %%Z0,400000\n\ttlo %%Z0,400000",
		     v);
	  return buf;
	}
      else
	{
	  if (unused & 1)
	    return "movni %Z0,%n1";
	  if (unused & 2)
	    return "seto %0,";
	  return "seto %0,\n\tmovni %Z0,%n1";
	}

    case 4:
      if (have_dmove)
	{
	  if (unused & 1)
	    return "move %Z0,[%B1]";
	  if (unused & 2)
	    return "move %0,[%A1]";
	  return "dmove %0,[%D1]";
	}
      else
	{
	  if (unused & 1)
	    return "move %Z0,[%B1]";
	  if (unused & 2)
	    return "move %0,[%A1]";
	  return "move %0,[%A1]\n\tmove %Z0,[%B1]";
	}

    case 5:
      return "setzm %0\n\tsetzm %Z0";

    case 6:
      return have_dmove ? "dmovem %1,%0" : "movem %1,%0\n\tmovem %Z1,%Z0";

    default:
      abort ();
    }
}

/* Return the offset from the base pointer.  */
int
pdp10_pointer_offset (x)
     rtx x;
{
  /* This function must be prepared to handle anything
     GO_IF_LEGITIMATE_ADDRESS says is a valid address.  */

  switch (GET_CODE (x))
    {
    case SYMBOL_REF:
    case LABEL_REF:
    case REG:
    case PRE_INC:
    case MEM:
      return 0;
    case CONST_INT:
      return INTVAL (x) & ~((HOST_WIDE_INT)(UNITS_PER_WORD - 1));
    case CONST:
      x = XEXP (x, 0);
      if (GET_CODE (x) != PLUS)
	abort ();
      /* Fall through.  */
    case PLUS:
      if (GET_CODE (XEXP (x, 1)) != CONST_INT)
	abort ();
      return INTVAL (XEXP (x, 1));
    default:
      abort ();
    }
}


/* Return nonzero if MEM is a QImode packed COMPONENT_REF/ARRAY_REF field
   whose bit position inside the containing PDP-10 word is known from
   MEM_EXPR.  This is intentionally small and output-side only: reload can
   turn the address into a PRE_INC byte-pointer temporary even when the
   register actually holds only the containing word address.  */
int
pdp10_qi_mem_expr_bitpos (mem, bitposp)
     rtx mem;
     int *bitposp;
{
  tree expr, field;
  HOST_WIDE_INT bitpos;

  if (GET_CODE (mem) != MEM || GET_MODE (mem) != QImode || MEM_EXPR (mem) == 0)
    return 0;

  expr = MEM_EXPR (mem);
  field = NULL_TREE;
  if (TREE_CODE (expr) == COMPONENT_REF)
    field = TREE_OPERAND (expr, 1);

  /* Do not rewrite ARRAY_REFs such as p->q[1] through this helper.
     Those addresses normally already carry a real byte pointer; wrapping
     them in POINT 9,(reg),P treats the byte pointer register as a word
     address and breaks QImode array loads.  The problematic packed-struct
     case this helper was added for is a direct COMPONENT_REF field load
     through a plain word pointer.  */

  if (field == NULL_TREE || TREE_CODE (field) != FIELD_DECL)
    return 0;
  if (TREE_TYPE (field) != 0 && TREE_CODE (TREE_TYPE (field)) == ARRAY_TYPE)
    return 0;

  bitpos = int_bit_position (field) % BITS_PER_WORD;
  if (bitpos < 0)
    bitpos += BITS_PER_WORD;
  if (bitpos + 9 > BITS_PER_WORD)
    return 0;

  *bitposp = (int) bitpos;
  return 1;
}

/* Return or output a single instruction to load an unsigned byte into
   a register.  Called to output a (I)LDB(I) insn.  */
const char *
pdp10_output_load_unsigned_byte (insn, operands)
     rtx insn;
     rtx *operands;
{
  int pos = -1;

  if (GET_CODE (operands[1]) == MEM)
    {
      int bitpos;
      rtx addr = XEXP (operands[1], 0);

      if (GET_CODE (addr) == PRE_INC
          && REG_P (XEXP (addr, 0))
          && pdp10_qi_mem_expr_bitpos (operands[1], &bitpos))
        {
          rtx ops[2];
          char buf[64];

          ops[0] = operands[0];
          ops[1] = XEXP (addr, 0);
          sprintf (buf, "ldb %%0,[POINT 9,(%%1),%d]", bitpos + 8);
          output_asm_insn (buf, ops);
          return "";
        }

      if (GET_MODE (operands[1]) == QImode
          && REG_P (addr)
          && MEM_EXPR (operands[1]) != 0
          && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
          && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1]))) != ARRAY_TYPE)
        {
          tree field = TREE_OPERAND (MEM_EXPR (operands[1]), 1);

          if (field != 0 && TREE_CODE (field) == FIELD_DECL)
            {
              HOST_WIDE_INT fieldbit;
              HOST_WIDE_INT inword;
              int ps;
              rtx ops[3];
              char abuf[64];
              rtx scratch = gen_rtx_REG (SImode, 0);

              fieldbit = int_bit_position (field);
              if (TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1])))
                  == ARRAY_TYPE
                  && MEM_OFFSET (operands[1]) != 0
                  && GET_CODE (MEM_OFFSET (operands[1])) == CONST_INT
                  && INTVAL (MEM_OFFSET (operands[1])) > 0)
                fieldbit += INTVAL (MEM_OFFSET (operands[1]))
                  * BITS_PER_UNIT;
              inword = ((fieldbit + BITS_PER_UNIT - 1) / BITS_PER_UNIT)
                % UNITS_PER_WORD;
              if (inword < 0)
                inword += UNITS_PER_WORD;
              ps = ((BITS_PER_WORD
                     - (int) inword * BITS_PER_UNIT
                     - BITS_PER_UNIT) << 6) + BITS_PER_UNIT;

              ops[0] = operands[0];
              ops[1] = scratch;
              ops[2] = addr;
              output_asm_insn ("hrrz %1,%2", ops);
              sprintf (abuf, "tlo %%1,%o", ps << 6);
              output_asm_insn (abuf, ops);
              output_asm_insn ("ldb %0,%1", ops);
              return "";
            }
        }

      if (GET_MODE (operands[1]) == QImode
          && MEM_EXPR (operands[1]) != 0
          && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
          && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1]))) != ARRAY_TYPE)
        {
          tree field = TREE_OPERAND (MEM_EXPR (operands[1]), 1);

          if (field != 0 && TREE_CODE (field) == FIELD_DECL)
            {
              HOST_WIDE_INT bitpos = int_bit_position (field);
              pos = (int) (bitpos % BITS_PER_WORD);
              if (pos < 0)
                pos += BITS_PER_WORD;
            }
        }

      if (pos == -1
          && MEM_ALIGN (operands[1]) >= BITS_PER_WORD
          && !MEM_SCALAR_P (operands[1]))
        pos = 0;
    }

  return output_bitfield_move (asm_out_file, insn, 0, 0, operands[0],
			       operands[1], 9, pos);
}

/* Return or output a single instruction to load a signed byte into a
   register.  Called to output a (I)LDBE(I) insn.  */
const char *
pdp10_output_load_signed_byte (insn, operands)
     rtx insn;
     rtx *operands;
{
  if (GET_CODE (operands[1]) == MEM)
    {
      int bitpos;
      rtx addr = XEXP (operands[1], 0);

      if (GET_CODE (addr) == PRE_INC
          && REG_P (XEXP (addr, 0))
          && pdp10_qi_mem_expr_bitpos (operands[1], &bitpos))
        {
          rtx ops[2];
          char buf[64];

          ops[0] = operands[0];
          ops[1] = XEXP (addr, 0);
          sprintf (buf, "ldb %%0,[POINT 9,(%%1),%d]", bitpos + 8);
          output_asm_insn (buf, ops);
          output_asm_insn ("trne %0,400", ops);
          output_asm_insn ("orcmi %0,777", ops);
          return "";
        }

      if (GET_MODE (operands[1]) == QImode
          && REG_P (addr)
          && MEM_EXPR (operands[1]) != 0
          && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
          && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1]))) != ARRAY_TYPE)
        {
          tree field = TREE_OPERAND (MEM_EXPR (operands[1]), 1);

          if (field != 0 && TREE_CODE (field) == FIELD_DECL)
            {
              HOST_WIDE_INT fieldbit;
              HOST_WIDE_INT inword;
              int ps;
              rtx ops[3];
              char abuf[64];
              rtx scratch = gen_rtx_REG (SImode, 0);

              fieldbit = int_bit_position (field);
              if (TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1])))
                  == ARRAY_TYPE
                  && MEM_OFFSET (operands[1]) != 0
                  && GET_CODE (MEM_OFFSET (operands[1])) == CONST_INT
                  && INTVAL (MEM_OFFSET (operands[1])) > 0)
                fieldbit += INTVAL (MEM_OFFSET (operands[1]))
                  * BITS_PER_UNIT;
              inword = ((fieldbit + BITS_PER_UNIT - 1) / BITS_PER_UNIT)
                % UNITS_PER_WORD;
              if (inword < 0)
                inword += UNITS_PER_WORD;
              ps = ((BITS_PER_WORD
                     - (int) inword * BITS_PER_UNIT
                     - BITS_PER_UNIT) << 6) + BITS_PER_UNIT;

              ops[0] = operands[0];
              ops[1] = scratch;
              ops[2] = addr;
              output_asm_insn ("hrrz %1,%2", ops);
              sprintf (abuf, "tlo %%1,%o", ps << 6);
              output_asm_insn (abuf, ops);
              output_asm_insn ("ldb %0,%1", ops);
              output_asm_insn ("trne %0,400", ops);
              output_asm_insn ("orcmi %0,777", ops);
              return "";
            }
        }
    }

  if (GET_CODE (operands[1]) == MEM
      && GET_MODE (operands[1]) == QImode
      && MEM_EXPR (operands[1]) != 0
      && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
      && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1]))) != ARRAY_TYPE)
    {
      tree field = TREE_OPERAND (MEM_EXPR (operands[1]), 1);

      if (field != 0 && TREE_CODE (field) == FIELD_DECL)
        {
          HOST_WIDE_INT bitpos = int_bit_position (field);
          int pos = (int) (bitpos % BITS_PER_WORD);
          if (pos < 0)
            pos += BITS_PER_WORD;
          return output_bitfield_move (asm_out_file, insn, 0, 1,
                                       operands[0], operands[1], 9, pos);
        }
    }

  return output_bitfield_move (asm_out_file, insn, 0, 1, operands[0],
			       operands[1], 9, -1);
}

/* Return or output a single instruction to store a byte from a
   register.  Called to output an (I)DPB(I) insn.  */
const char *
pdp10_output_store_byte (insn, operands)
     rtx insn;
     rtx *operands;
{
  int pos = -1;
  int size = 9;


  if (GET_CODE (operands[0]) == MEM)
    {
      rtx pointer, offset;
      int ps, p, s;

      if (GET_MODE (operands[0]) == QImode
          && GET_CODE (XEXP (operands[0], 0)) == PRE_INC
          && REG_P (XEXP (XEXP (operands[0], 0), 0)))
        {
          rtx ops[2];
          HOST_WIDE_INT logicaloff;
          int unsignedp;

          ops[0] = XEXP (XEXP (operands[0], 0), 0);
          ops[1] = operands[1];

          if (pdp10_qi_mem_logical_offset (operands[0], &logicaloff, &unsignedp))
            {
              int i, n;
              (void) unsignedp;
              n = -1;
              if (MEM_OFFSET (operands[0]) != 0
                  && GET_CODE (MEM_OFFSET (operands[0])) == CONST_INT
                  && INTVAL (MEM_OFFSET (operands[0])) < 0)
                n = UNITS_PER_WORD;
              else if (logicaloff == 6 || logicaloff == 9)
                n = (logicaloff == 6) ? 2 : 5;

              if (n >= 0)
                {
                  for (i = 0; i < n; i++)
                    output_asm_insn ("ibp %0", ops);
                  output_asm_insn ("dpb %1,%0", ops);
                  return "";
                }
            }

          output_asm_insn ("idpb %1,%0", ops);
          return "";
        }


      if (GET_MODE (operands[0]) == QImode
          && REG_P (XEXP (operands[0], 0)))
        {
          HOST_WIDE_INT logicaloff;
          int unsignedp;

          /* If reload leaves a register-held address for a byte-sized field,
             it may be an ADJBP-built PDP-10 byte pointer rather than a plain
             word address.  DPB through that register would then use the
             register's byte size, e.g. 18 bits for a struct-record pointer,
             and corrupt adjacent 9-bit char fields.  Rebuild an explicit
             9-bit POINT using only the register's address half.  For scalar
             COMPONENT_REFs, ignore negative MEM_OFFSET values that describe
             how GCC formed the temporary address; the FIELD_DECL bit position
             is the selected byte.  Array fields still need MEM_OFFSET because
             it carries the element displacement relative to the field base. */
          if (MEM_EXPR (operands[0]) != 0
              && TREE_CODE (MEM_EXPR (operands[0])) == COMPONENT_REF
              && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[0]))) != ARRAY_TYPE)
            {
              tree field = TREE_OPERAND (MEM_EXPR (operands[0]), 1);

              if (field != 0 && TREE_CODE (field) == FIELD_DECL)
                {
                  HOST_WIDE_INT fieldbit;
                  HOST_WIDE_INT inword;
                  int ps;
                  rtx ops[3];
                  char abuf[64];
                  rtx scratch = gen_rtx_REG (SImode, 0);

                  fieldbit = int_bit_position (field);
                  if (TREE_CODE (TREE_TYPE (MEM_EXPR (operands[0])))
                      == ARRAY_TYPE
                      && MEM_OFFSET (operands[0]) != 0
                      && GET_CODE (MEM_OFFSET (operands[0])) == CONST_INT
                      && INTVAL (MEM_OFFSET (operands[0])) > 0)
                    fieldbit += INTVAL (MEM_OFFSET (operands[0]))
                      * BITS_PER_UNIT;
                  inword = ((fieldbit + BITS_PER_UNIT - 1) / BITS_PER_UNIT)
                    % UNITS_PER_WORD;
                  if (inword < 0)
                    inword += UNITS_PER_WORD;
                  ps = ((BITS_PER_WORD
                         - (int) inword * BITS_PER_UNIT
                         - BITS_PER_UNIT) << 6) + BITS_PER_UNIT;

                  ops[0] = scratch;
                  ops[1] = XEXP (operands[0], 0);
                  ops[2] = operands[1];
                  output_asm_insn ("hrrz %0,%1", ops);
                  sprintf (abuf, "tlo %%0,%o", ps << 6);
                  output_asm_insn (abuf, ops);
                  output_asm_insn ("dpb %2,%0", ops);
                  return "";
                }
            }

          /* For nested local byte arrays, reload can leave a byte-pointer
             register whose right half already contains several folded word
             adjustments, while MEM_EXPR still describes the absolute C field
             path.  Do not DPB through the adjusted pointer itself: recover the
             absolute bit position from MEM_EXPR and subtract the word offset
             already present in the byte-pointer register.  */
          if (MEM_EXPR (operands[0]) != 0
              && !(TREE_CODE (MEM_EXPR (operands[0])) == COMPONENT_REF
                   && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[0]))) == ARRAY_TYPE))
            {
              pdp10_word36 bitpos;
              HOST_WIDE_INT reg_wordoff;

              if (pdp10_reg_byte_pointer_word_offset_p
                    (XEXP (operands[0], 0), insn, &reg_wordoff))
                {
                  int have_bitpos;

                  have_bitpos = pdp10_output_qi_tree_bitpos
                    (MEM_EXPR (operands[0]), &bitpos);

                  if (have_bitpos
                      && (bitpos % BITS_PER_UNIT) == 0)
                    {
                      if (MEM_OFFSET (operands[0]) != 0
                          && GET_CODE (MEM_OFFSET (operands[0])) == CONST_INT)
                        bitpos += ((pdp10_word36)
                                   INTVAL (MEM_OFFSET (operands[0]))
                                   * BITS_PER_UNIT);
                    }
                  else if (MEM_OFFSET (operands[0]) != 0
                           && GET_CODE (MEM_OFFSET (operands[0])) == CONST_INT
                           && INTVAL (MEM_OFFSET (operands[0])) < 0
                           && TREE_CODE (MEM_EXPR (operands[0])) == COMPONENT_REF
                           && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[0])))
                              == ARRAY_TYPE)
                    {
                      /* Some late local struct-array stores keep only the
                         array FIELD_DECL in MEM_EXPR and carry the actual byte
                         index split between the byte-pointer register's folded
                         adjustment and a negative MEM_OFFSET.  In that shape,
                         the target byte index from the object base is the
                         folded adjustment minus MEM_OFFSET.  */
                      bitpos = ((pdp10_word36) reg_wordoff
                                - (pdp10_word36) INTVAL (MEM_OFFSET
                                                         (operands[0])))
                        * BITS_PER_UNIT;
                      have_bitpos = 1;
                    }

                  if (have_bitpos
                      && (bitpos % BITS_PER_UNIT) == 0)
                    {
                      HOST_WIDE_INT wordoff;
                      HOST_WIDE_INT inword;
                      rtx ops[3];
                      char abuf[64];

                      wordoff = (HOST_WIDE_INT) (bitpos / BITS_PER_WORD)
                        - reg_wordoff;
                      inword = (HOST_WIDE_INT)
                        ((bitpos % BITS_PER_WORD) / BITS_PER_UNIT);

                      ops[0] = XEXP (operands[0], 0);
                      ops[1] = operands[1];
                      ops[2] = GEN_INT (wordoff);

                      if (wordoff == 0)
                        sprintf (abuf, "dpb %%1,[POINT 9,(%%0),%d]",
                                 (int) inword * BITS_PER_UNIT + 8);
                      else
                        sprintf (abuf, "dpb %%1,[POINT 9,%%2(%%0),%d]",
                                 (int) inword * BITS_PER_UNIT + 8);
                      output_asm_insn (abuf, ops);
                      return "";
                    }
                }
            }

          if (pdp10_qi_mem_logical_offset (operands[0], &logicaloff, &unsignedp))
            {
              rtx ops[2];
              HOST_WIDE_INT wordoff;
              HOST_WIDE_INT byteoff;
              HOST_WIDE_INT corr;
              HOST_WIDE_INT i;
              rtx scratch = gen_rtx_REG (SImode, 0);

              (void) unsignedp;
              wordoff = logicaloff / UNITS_PER_WORD;
              byteoff = logicaloff % UNITS_PER_WORD;
              if (byteoff < 0)
                {
                  byteoff += UNITS_PER_WORD;
                  wordoff -= 1;
                }

              /* Dynamic stores to a local 9-bit array field can reach final
                 with MEM_EXPR naming only the array FIELD_DECL while the REG
                 address was already built by the expander as the exact
                 element byte pointer.  In that shape an additional in-word
                 correction stores field[i] into field[i + byteoff].  */
              if (MEM_EXPR (operands[0]) != 0
                  && MEM_OFFSET (operands[0]) == 0
                  && TREE_CODE (MEM_EXPR (operands[0])) == COMPONENT_REF
                  && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[0]))) == ARRAY_TYPE)
                byteoff = 0;

              /* Late reload can leave stores into a 9-bit array field
                 whose field starts exactly on a PDP-10 word boundary with a
                 REG address that is already an adjusted byte pointer to the
                 target element.  A typical form is src.b[1]/src.b[2] in a
                 structure whose preceding SImode member ends at the previous
                 word.  MEM_OFFSET is then negative because GCC describes the
                 element relative to the field base, but the REG already carries
                 the needed IBP/address adjustment.  In that specific shape do
                 not add more IBPs.  Do not apply this to fields that start in
                 the middle of a word, such as q[5] followed by uq[0]: those
                 still need the normal in-word byte offset.  */
              if (MEM_OFFSET (operands[0]) != 0
                  && GET_CODE (MEM_OFFSET (operands[0])) == CONST_INT
                  && INTVAL (MEM_OFFSET (operands[0])) < 0
                  && MEM_EXPR (operands[0]) != 0)
                {
                  tree expr = MEM_EXPR (operands[0]);
                  tree field = NULL_TREE;

                  if (TREE_CODE (expr) == COMPONENT_REF)
                    field = TREE_OPERAND (expr, 1);
                  else if (TREE_CODE (expr) == ARRAY_REF
                           && TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
                    field = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);

                  if (field != NULL_TREE
                      && TREE_CODE (field) == FIELD_DECL)
                    {
                      HOST_WIDE_INT fieldbit = int_bit_position (field);
                      HOST_WIDE_INT fieldbyte;
                      fieldbyte = (fieldbit + BITS_PER_UNIT - 1)
                        / BITS_PER_UNIT;
                      if ((fieldbyte % UNITS_PER_WORD) == 0)
                        byteoff = 0;
                    }
                }

              /* Here the REG address has already been forced to the
                 containing PDP-10 word by the generic address expander.  Use
                 MEM_EXPR/MEM_OFFSET only to choose the in-word byte. */
              (void) wordoff;
              (void) corr;
              ops[0] = scratch;
              ops[1] = XEXP (operands[0], 0);
              output_asm_insn ("move %0,%1", ops);
              for (i = 0; i < byteoff; i++)
                output_asm_insn ("ibp %0", ops);
              ops[1] = operands[1];
              output_asm_insn ("dpb %1,%0", ops);
              return "";
            }
          else
            {
              rtx ops[2];
              ops[0] = XEXP (operands[0], 0);
              ops[1] = operands[1];
              output_asm_insn ("dpb %1,%0", ops);
              return "";
            }
        }

      if (GET_CODE (XEXP (operands[0], 0)) == CONST
	  && GET_CODE (pointer = XEXP (XEXP (operands[0], 0), 0)) == PLUS
	  && GET_CODE (offset = XEXP (pointer, 1)) == CONST_INT
	  && (ps = ((INTVAL (offset) >> 24) & 07777)) != 0
	  && decode_ps (ps, &p, &s) != -1)
	{
	  pos = p;
	  size = s;
	}
      else
	{
	  /* struct-subword-packed-qi-mem-bytepos-v21:
	     Packed struct QImode fields can reach final as a plain MEM whose
	     address is only the containing word address, while the field identity
	     survives only in MEM_EXPR.  Emitting "DPB reg,mem" for that shape uses
	     the data word itself as a byte pointer.  Recover the byte position from
	     the COMPONENT_REF FIELD_DECL and force output_bitfield_move() to print a
	     literal POINT byte pointer, e.g. [POINT 9,packed_rec,29].  */
	  if (GET_MODE (operands[0]) == QImode && MEM_EXPR (operands[0]) != 0)
	    {
	      tree expr = MEM_EXPR (operands[0]);
	      tree field = NULL_TREE;
	      HOST_WIDE_INT bitpos;
	      int bytepos;

	      if (TREE_CODE (expr) == COMPONENT_REF)
		field = TREE_OPERAND (expr, 1);
	      else if (TREE_CODE (expr) == ARRAY_REF
		       && TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
		field = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);

	      if (field != NULL_TREE && TREE_CODE (field) == FIELD_DECL)
		{
		  bitpos = int_bit_position (field);
		  bytepos = (int) (bitpos % BITS_PER_WORD);
		  if (bytepos < 0)
		    bytepos += BITS_PER_WORD;
		  if (bytepos + 9 <= BITS_PER_WORD)
		    {
		      pos = bytepos;
		      size = 9;
		    }
		}
	    }

	  if (pos == -1 && MEM_ALIGN (operands[0]) == BITS_PER_WORD)
	    pos = 0;
	}
    }

  return output_bitfield_move (asm_out_file, insn, 1, 0, operands[1],
			       operands[0], size, pos);
}

#if 0
const char *
pdp10_output_halfword_move (insn)
     rtx insn;
{
  rtx pattern = INSN_PATTERN (insn);
  rtx dest = SET_DEST (pattern);
  rtx src = SET_SRC (pattern);
  int dest_right, src_right;
  char *s, format[100];

  switch (GET_CODE (dest))
    {
    case ZERO_EXTRACT:
      dest_right = INTVAL (XEXP (dest, 2)) == 18;
      dest = XEXP (dest, 0);
      break;
    case SUBREG:
      dest_right = true;
      dest = XEXP (dest, 0);
      break;
    default:
      abort ();
    }

  switch (GET_CODE (dest))
    {
    case ZERO_EXTRACT:
    case SIGN_EXTRACT:
      src_right = INTVAL (XEXP (src, 2)) == 18;
      src = XEXP (src, 0);
      break;
    case SUBREG:
    case LSHIFT:
    case ASHIFT:
      src_right = false;
      src = XEXP (src, 0);
      break;
    case AND:
      src_right = true;
      src = XEXP (src, 0);
      break;
    default:
      abort ();
    }

  s = format;
  *s = 'h';
  *++s = src_right ? 'r' : 'l';
  *++s = dest_right ? 'r' : 'l';
  if (GET_CODE (dest) == MEM)
    *++s = 'm';
  *++s = ' ';
  s++;
  if (GET_CODE (dest) == MEM)
    strcpy (s, "%W1,%0");
  else /*if (GET_CODE (src) == MEM)*/
    strcpy (s, "%0,%W1");
  /*else
    strcpy (s, "%0,%1");*/

  return format;
}
#endif

/* Return or output a single half-word instruction.  REG is the
   accumulator operand and X is the memory operand.  REG_RIGHT should
   be nonzero if the right half of the accumulator is to be used.
   MEM_RIGHT should should be nonzero if X is an accumulator and the
   right half should be used.  STORE should be 1 for store
   instructions and 0 for load instrucitons.  EXTEND should be 1 for
   sign- or zero-extending loads, and 0 for non-extending loads.  SIGN
   should be 1 for sign- extending loads, and 0 for zero-extending
   loads.  */
static const char *
output_halfword_move (reg, x, reg_right, mem_right, store, extend, sign)
     rtx reg;
     rtx x;
     int reg_right;
     int mem_right;
     int store;
     int extend;
     int sign;
{
  rtx operands[2];
  char format[20];
  char *p = format;

  *p++ = 'h';
  if (store)
    {
      *p++ = reg_right ? 'r' : 'l';
      *p++ = mem_right ? 'r' : 'l';
    }
  else
    {
      *p++ = mem_right ? 'r' : 'l';
      *p++ = reg_right ? 'r' : 'l';
    }
  if (extend)
    *p++ = sign ? 'e' : 'z';
  if (store)
    *p++ = 'm';
  *p++ = ' ';
  *p = 0;

  if (REG_P (x))
    strcat (format, "%0,%1");
  else
    strcat (format, "%0,%W1");

  operands[0] = reg;
  operands[1] = x;
  output_asm_insn (format, operands);

  return "";
}

/* Return nonzero if MEM is a HImode ARRAY_REF inside a record field whose
   field starts at a nonzero halfword position in the containing PDP-10 word.
   GCC's generic dynamic array access can build the address of the word that
   contains the array field but leave the local 18-bit byte pointer positioned
   at the left half.  In a record like

       int tag; char c0; char c1; short h[4];

   h[0] begins in the right half of word 1.  A plain LDB from the computed
   pointer reads c0/c1 instead of h[0]; one IBP is needed before the LDB.  */
static int
pdp10_hi_mem_array_field_bitpos (mem, bitposp)
     rtx mem;
     int *bitposp;
{
  tree expr, array, field, type;
  HOST_WIDE_INT bitpos;

  if (GET_CODE (mem) != MEM || GET_MODE (mem) != HImode || MEM_EXPR (mem) == 0)
    return 0;

  expr = MEM_EXPR (mem);
  if (TREE_CODE (expr) == ARRAY_REF)
    {
      array = TREE_OPERAND (expr, 0);
      if (TREE_CODE (array) != COMPONENT_REF)
        return 0;
      field = TREE_OPERAND (array, 1);
      type = TREE_TYPE (expr);
    }
  else if (TREE_CODE (expr) == COMPONENT_REF
           && TREE_TYPE (expr) != 0
           && TREE_CODE (TREE_TYPE (expr)) == ARRAY_TYPE)
    {
      field = TREE_OPERAND (expr, 1);
      type = TREE_TYPE (TREE_TYPE (expr));
    }
  else
    return 0;

  if (field == NULL_TREE || TREE_CODE (field) != FIELD_DECL)
    return 0;
  if (type == 0 || TREE_CODE (type) != INTEGER_TYPE || TYPE_PRECISION (type) != 18)
    return 0;

  bitpos = int_bit_position (field) % BITS_PER_WORD;
  if (bitpos < 0)
    bitpos += BITS_PER_WORD;
  if (bitpos != 18)
    return 0;

  if (bitposp)
    *bitposp = (int) bitpos;
  return 1;
}

/* Return or output a single instruction to move a half-word.  Called
   to output a movhi pattern.  SIGN_EXTEND should be 1 if this is a
   sign-extending load.  */
const char *
pdp10_output_movhi (operands, sign_extend)
     rtx *operands;
     int sign_extend;
{
  int store = GET_CODE (operands[0]) == MEM;
  rtx x = store ? operands[0] : operands[1];
  rtx address = NULL_RTX;

  if (GET_CODE (x) == MEM)
    address = XEXP (x, 0);

  if (address && GET_CODE (address) == PRE_INC)
    {
      if (store)
	return "idpb %1,%0";
      else
	return sign_extend ? "extend %0,[ildbe %1]" : "ildb %0,%1";
    }
  else if (address && GET_CODE (address) == POST_INC)
    {
      if (store)
	return "extend %1,[dpbi %0]%; movhi";
      else
	return sign_extend ? "extend %0,[ldbei %1]" : "extend %0,[ldbi %1]";
    }
  else if (!address
	   || MEM_ALIGN (x) >= BITS_PER_WORD
	   || GET_CODE (address) == CONST)
    {
      rtx reg = store ? operands[1] : operands[0];
      int mem_right = 1;

      /* Target-specific scalar integer types such as signed char9 and
         signed short16 can be represented as HImode by the generic middle
         end, but their C precision is smaller than a PDP-10 halfword.  Do
         not use HRRE/HRRZ for those loads: HRRE sign-extends bit 17, while
         char9's sign bit is bit 8.  Load the right-justified scalar field
         with the actual precision and then extend that value.  */
      if (!store && GET_CODE (x) == MEM && GET_MODE (x) == HImode)
        {
          int precision = pdp10_mem_integer_precision (x);

          if (precision <= 0 || precision >= 18)
            precision = pdp10_mem_scalar_precision_hint (x);

          if (precision > 0 && precision < 18)
            {
              rtx ops[2];
              int shift = BITS_PER_WORD - precision;

              output_bitfield_move (asm_out_file, NULL_RTX, 0, 0,
                                    reg, x, precision, shift);
              if (sign_extend)
                {
                  ops[0] = reg;
                  ops[1] = GEN_INT (shift);
                  output_asm_insn ("lsh %0,%1", ops);
                  ops[1] = GEN_INT (-shift);
                  output_asm_insn ("ash %0,%1", ops);
                }
              return "";
            }
        }

      /* struct-subword-packed-hi-mem-bytepos-v22:
	 Packed struct HImode fields may reach here as a plain MEM whose
	 address is the containing word.  If we blindly use the halfword move
	 fast path, HRLM/HRRM can store into the wrong halfword and clobber an
	 adjacent packed field.  Recover the field bit position from MEM_EXPR
	 and use an explicit POINT 18 byte pointer for non-halfword-aligned
	 18-bit fields, e.g. DPB r,[POINT 18,packed_rec+1,33].  */
      if (store
	  && GET_CODE (x) == MEM
	  && GET_MODE (x) == HImode
	  && MEM_EXPR (x) != 0)
	{
	  tree expr = MEM_EXPR (x);
	  tree field = NULL_TREE;
	  HOST_WIDE_INT bitpos;
	  int bytepos;

	  if (TREE_CODE (expr) == COMPONENT_REF)
	    field = TREE_OPERAND (expr, 1);
	  else if (TREE_CODE (expr) == ARRAY_REF
		   && TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
	    field = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);

	  if (field != NULL_TREE && TREE_CODE (field) == FIELD_DECL)
	    {
	      bitpos = int_bit_position (field);
	      bytepos = (int) (bitpos % BITS_PER_WORD);
	      if (bytepos < 0)
		bytepos += BITS_PER_WORD;

	      if (bytepos != 0
		  && bytepos != 18
		  && bytepos + 18 <= BITS_PER_WORD)
		return output_bitfield_move (asm_out_file, NULL_RTX,
					     1, 0, reg, x, 18, bytepos);
	    }
	}

      mem_right = 0;
      if (GET_CODE (x) == MEM && pdp10_scalar_mem_p (x))
        mem_right = 1;
      else if (GET_CODE (x) == MEM
	       && GET_CODE (XEXP (x, 0)) == CONST
	       && GET_CODE (XEXP (XEXP (x, 0), 0)) == PLUS
	       && GET_CODE (XEXP (XEXP (XEXP (x, 0), 0), 1)) == CONST_INT)
	{
	  int ps = (INTVAL (XEXP (XEXP (XEXP (x, 0), 0), 1)) >> 30) & 077;
	  mem_right = ps == 076;
	}
      return output_halfword_move (reg, x, 1, mem_right,
				   store, !store, sign_extend);
    }
  else
    {
      if (store)
        {
          int bitpos;
          if (GET_CODE (x) == MEM
              && GET_MODE (x) == HImode
              && REG_P (address)
              && pdp10_hi_mem_array_field_bitpos (x, &bitpos)
              && bitpos == 18)
            {
              rtx ops[3];
              ops[0] = gen_rtx_REG (SImode, 0);
              ops[1] = address;
              ops[2] = operands[1];
              output_asm_insn ("move %0,%1", ops);
              output_asm_insn ("ibp %0", ops);
              output_asm_insn ("dpb %2,%0", ops);
              return "";
            }
	  return "dpb %1,%0%; movhi";
        }
      else
        {
          int bitpos;
          if (GET_CODE (x) == MEM
              && GET_MODE (x) == HImode
              && REG_P (address)
              && pdp10_hi_mem_array_field_bitpos (x, &bitpos)
              && bitpos == 18)
            {
              rtx ops[2];
              ops[0] = operands[0];
              ops[1] = address;
              output_asm_insn ("move %0,%1", ops);
              output_asm_insn ("ibp %0", ops);
              output_asm_insn ("ldb %0,%0", ops);
              return "";
            }
	  return sign_extend ? "extend %0,[ldbe %1]" : "ldb %0,%1";
        }
    }
}

/* Expand an unsigned bit-field load.  Called to expand the "extzv"
   pattern in some special cases.  Return zero if nothing special has
   to be done, nonzero otherwise.  */
int
pdp10_expand_extzv (operands)
     rtx *operands;
{
  rtx op1 = copy_rtx (operands[1]);
  int len = INTVAL (operands[2]);
  int pos = INTVAL (operands[3]);

  PUT_MODE (op1, SImode);

  if (pos == 0) /* && len == 32) */
    {
      rtx temp = gen_reg_rtx (SImode);
      if (len == 32)
	expand_extzv_32++;
      emit_move_insn (temp, op1);
      emit_insn (gen_lshrsi3 (temp, temp, GEN_INT (BITS_PER_WORD - len)));
      emit_move_insn (operands[0], temp);
      return 1;
    }
  else if (pos + len == BITS_PER_WORD)
    {
      HOST_WIDE_INT mask = ((HOST_WIDE_INT) 1 << len) - 1;
      emit_move_insn (operands[0], op1);
      emit_insn (gen_andsi3 (operands[0], operands[0],
			     gen_int_mode (mask, SImode)));
      return 1;
    }

  return 0;
}

/* Expand a signed bit-field load.  Called to expand an "extv" pattern
   if the LDBE instruction is not available.  */
int
pdp10_expand_extv (operands)
     rtx *operands;
{
  int len = INTVAL (operands[2]);
  int pos = INTVAL (operands[3]);
  rtx op1 = copy_rtx (operands[1]);

  if (len == 18 && (pos == 0 || pos == 18))
    return 0;

  if (GET_CODE (op1) == MEM)
    normalize_bitfield_mem (&op1, &pos);
  PUT_MODE (op1, SImode);

  if (len == 32 && pos == 0)
    {
      expand_extv_32++;
      emit_move_insn (operands[0], op1);
      emit_insn (gen_ashrsi3 (operands[0], operands[0], GEN_INT (4)));
      return 1;
    }

  return 0;
}

/* *X is a (9-bit) byte in memory and *POS is the first bit of a
   bitfield starting in *X.  Modify *X and *POS such that *X is word
   aligned and *POS specifies the first bit, in the word that starts
   at *X, of the bitfield.  */
static void
normalize_bitfield_mem (rtx *x, int *pos)
{
  rtx y;

  if (GET_CODE (*x) != MEM)
    return;

  y = XEXP (*x, 0);
  if (GET_CODE (y) == CONST)
    y = XEXP (y, 0);

  if (GET_CODE (y) == PLUS)
    {
      HOST_WIDE_INT i = INTVAL (XEXP (y, 1));
      HOST_WIDE_INT j = i % UNITS_PER_WORD;

      if (j != 0)
	{
	  if (j < 0)
	    j += UNITS_PER_WORD;

	  *x = gen_rtx_MEM (GET_MODE (*x), 
			    plus_constant (XEXP (y, 0), i - j));
	  *pos += BITS_PER_UNIT * j;
	}
    }
}

/* This is an adaption of find_last_vaue in rtanal.c.  Return the last
   thing that X was assigned from before *PINSN.  If we found an
   assignment, update *PINSN to point to it.  */
static rtx
pdp10_find_last_value (x, pinsn)
     rtx x;
     rtx *pinsn;
{
  rtx p;

  for (p = PREV_INSN (*pinsn); p && GET_CODE (p) != CODE_LABEL;
       p = PREV_INSN (p))
    if (INSN_P (p))
      {
	rtx set = single_set (p);
	rtx note = find_reg_note (p, REG_EQUAL, NULL_RTX);

	if (set && REG_P (SET_DEST (set))
	    && REGNO (SET_DEST (set)) == REGNO (x))
	  {
	    rtx src = SET_SRC (set);

	    if (note && GET_CODE (XEXP (note, 0)) != EXPR_LIST)
	      src = XEXP (note, 0);

	    *pinsn = p;
	    return src;
	  }
	  
	/* If set in non-simple way, we don't have a value.  */
	if (reg_set_p (x, p))
	  break;
      }

  return NULL_RTX;
}

/* Return nonzero if REG appears to hold a PDP-10 byte pointer at INSN.
   This is an output-side guard for optimized RTL where byte-array element
   loads have been folded to zero_extract/lshiftrt from (mem:SI (reg)).  */
static int
pdp10_reg_likely_byte_pointer_p (reg, insn)
     rtx reg;
     rtx insn;
{
  rtx x;
  int depth;

  if (!REG_P (reg) || insn == NULL_RTX)
    return 0;

  x = reg;
  for (depth = 0; depth < 8; depth++)
    {
      rtx where = insn;

      x = pdp10_find_last_value (x, &where);
      if (x == NULL_RTX)
        return 0;

      if (pdp10_const_byte_pointer_p (x))
        return 1;

      if (GET_CODE (x) == UNSPEC && XINT (x, 1) == UNSPEC_ADJBP)
        return 1;

      /* Local byte-array bases are often built as

            MOVEI r,frame-offset
            TLO   r,P,S

         and survive to final RTL as IOR(reg,const).  Treat any nonzero
         PDP-10 P,S field installed by such an IOR as a byte pointer.  */
      if (GET_CODE (x) == IOR
          && GET_CODE (XEXP (x, 1)) == CONST_INT
          && (((INTVAL (XEXP (x, 1)) >> 24)
               & (TARGET_EXTENDED ? 07700 : 07777)) != 0))
        return 1;

      if (GET_CODE (x) == PLUS
          && REG_P (XEXP (x, 0))
          && GET_CODE (XEXP (x, 1)) == CONST_INT)
        {
          x = XEXP (x, 0);
          insn = where;
          continue;
        }

      if (REG_P (x))
        {
          insn = where;
          continue;
        }

      return 0;
    }

  return 0;
}



/* Compute a conservative bit position for a final-output MEM_EXPR.  This
   deliberately handles only constant COMPONENT_REF/ARRAY_REF paths rooted at
   a real object or an INDIRECT_REF; output code must not expand trees here.  */
static int
pdp10_output_qi_tree_bitpos (exp, bitposp)
     tree exp;
     pdp10_word36 *bitposp;
{
  while (exp != 0
         && (TREE_CODE (exp) == NON_LVALUE_EXPR
             || TREE_CODE (exp) == NOP_EXPR
             || TREE_CODE (exp) == CONVERT_EXPR))
    exp = TREE_OPERAND (exp, 0);

  if (exp == 0)
    return 0;

  switch (TREE_CODE (exp))
    {
    case VAR_DECL:
    case PARM_DECL:
    case INDIRECT_REF:
      *bitposp = 0;
      return 1;

    case COMPONENT_REF:
      {
        pdp10_word36 base_bits;
        tree field = TREE_OPERAND (exp, 1);

        if (field == 0 || TREE_CODE (field) != FIELD_DECL)
          return 0;
        if (!pdp10_output_qi_tree_bitpos (TREE_OPERAND (exp, 0),
                                          &base_bits))
          return 0;
        *bitposp = base_bits + (pdp10_word36) int_bit_position (field);
        return 1;
      }

    case ARRAY_REF:
      {
        pdp10_word36 base_bits;
        HOST_WIDE_INT idx;
        HOST_WIDE_INT bytes;

        if (!host_integerp (TREE_OPERAND (exp, 1), 0))
          return 0;
        if (!pdp10_output_qi_tree_bitpos (TREE_OPERAND (exp, 0),
                                          &base_bits))
          return 0;
        bytes = int_size_in_bytes (TREE_TYPE (exp));
        if (bytes <= 0)
          return 0;
        idx = tree_low_cst (TREE_OPERAND (exp, 1), 0);
        if (idx < 0)
          return 0;
        *bitposp = base_bits
          + (pdp10_word36) idx * (pdp10_word36) bytes * BITS_PER_UNIT;
        return 1;
      }

    default:
      return 0;
    }
}

/* If REG is a byte-pointer register at INSN, return the word-address
   adjustment already folded into REG's right half.  */
static int
pdp10_reg_byte_pointer_word_offset_p (reg, insn, offp)
     rtx reg;
     rtx insn;
     HOST_WIDE_INT *offp;
{
  rtx x;
  HOST_WIDE_INT off = 0;
  int depth;

  if (!REG_P (reg) || insn == NULL_RTX)
    return 0;

  x = reg;
  for (depth = 0; depth < 8; depth++)
    {
      rtx where = insn;

      x = pdp10_find_last_value (x, &where);
      if (x == NULL_RTX)
        return 0;

      if (pdp10_const_byte_pointer_p (x))
        {
          if (offp)
            *offp = off;
          return 1;
        }

      if (GET_CODE (x) == IOR
          && REG_P (XEXP (x, 0))
          && GET_CODE (XEXP (x, 1)) == CONST_INT
          && (((INTVAL (XEXP (x, 1)) >> 24)
               & (TARGET_EXTENDED ? 07700 : 07777)) != 0))
        {
          if (offp)
            *offp = off;
          return 1;
        }

      if (GET_CODE (x) == PLUS
          && REG_P (XEXP (x, 0))
          && GET_CODE (XEXP (x, 1)) == CONST_INT)
        {
          off += INTVAL (XEXP (x, 1));
          x = XEXP (x, 0);
          insn = where;
          continue;
        }

      if (REG_P (x))
        {
          insn = where;
          continue;
        }

      return 0;
    }

  return 0;
}

#define INTVAL_OR_ELSE(X, Y)						\
  (GET_CODE (X) == CONST_INT ? INTVAL (X)				\
   : GET_CODE (X) == NEG						\
     && GET_CODE (XEXP ((X), 0)) == CONST_INT ? -INTVAL (XEXP ((X), 0))	\
   : (Y))


static int
pdp10_same_regno_p (x, regno)
     rtx x;
     unsigned int regno;
{
  /* Final/reload halfword patterns often address a hard register through
     a SUBREG or STRICT_LOW_PART.  Treat those as the underlying hard
     register here; all callers of this small helper are already looking
     at final-output/reload-local shapes, not doing general aliasing.  */
  while (x && (GET_CODE (x) == SUBREG || GET_CODE (x) == STRICT_LOW_PART))
    x = GET_CODE (x) == SUBREG ? SUBREG_REG (x) : XEXP (x, 0);

  return x && REG_P (x) && REGNO (x) == regno;
}

static int
pdp10_const_int_p (x, value)
     rtx x;
     HOST_WIDE_INT value;
{
  return GET_CODE (x) == CONST_INT && INTVAL (x) == value;
}

static int
pdp10_zero_extract_right_half_of_reg_p (x, regno)
     rtx x;
     unsigned int regno;
{
  return (GET_CODE (x) == ZERO_EXTRACT
	  && pdp10_same_regno_p (XEXP (x, 0), regno)
	  && pdp10_const_int_p (XEXP (x, 1), 18)
	  && pdp10_const_int_p (XEXP (x, 2), 18));
}

static int
pdp10_set_reg_from_right_half_p (set, dstno, srcno)
     rtx set;
     unsigned int dstno;
     unsigned int srcno;
{
  rtx dest, src, a, b;

  if (!set)
    return 0;

  dest = SET_DEST (set);
  src = SET_SRC (set);

  /* The right-half load we need to remember may appear either as a normal
     value-producing set,

         (set DST (and SRC RIGHT_HALF))

     or as another generic *insv_reg operation,

         (set (zero_extract DST 18 18) SRC)

     The latter is exactly what the halfword torture test prints as
     "hrrz DST,SRC" before the bad signed recomposition.  */
  if (pdp10_zero_extract_right_half_of_reg_p (dest, dstno)
      && pdp10_same_regno_p (src, srcno))
    return 1;

  if (!pdp10_same_regno_p (dest, dstno))
    return 0;

  /* Some final/reload shapes spell HRRZ through an HI subreg.  */
  if (GET_CODE (src) == SUBREG
      && SUBREG_BYTE (src) == UNITS_PER_WORD / 2
      && pdp10_same_regno_p (SUBREG_REG (src), srcno))
    return 1;

  if (GET_CODE (src) != AND)
    return 0;

  a = XEXP (src, 0);
  b = XEXP (src, 1);

  return ((pdp10_same_regno_p (a, srcno)
	   && pdp10_const_int_p (b, RIGHT_HALF))
	  || (pdp10_same_regno_p (b, srcno)
	      && pdp10_const_int_p (a, RIGHT_HALF)));
}

/* Return nonzero if, before INSN, DEST was last assigned the right half
   of SRC, and DEST has not been touched since.  This is intentionally
   based on the hard register numbers visible after reload.  The halfword
   recomposition case reaches final through the generic *insv_reg path as

       HRRZ  dest,src
       ... modify src into a sign-half mask ...
       HRLZ  dest,src       ; emitted by generic left-half insert

   The actual insert must preserve DEST's live right half, so
   pdp10_output_insv emits HRL instead of allowing output_bitfield_move to
   choose HRLZ.  */
static int
pdp10_dest_last_set_from_right_half_p (destno, srcno, insn)
     unsigned int destno;
     unsigned int srcno;
     rtx insn;
{
  rtx p;

  for (p = PREV_INSN (insn); p; p = PREV_INSN (p))
    {
      enum rtx_code code = GET_CODE (p);
      rtx set;

      if (code == CODE_LABEL || code == BARRIER
	  || code == JUMP_INSN || code == CALL_INSN)
	return 0;

      if (code != INSN)
	continue;

      set = single_set (p);
      if (!set)
	{
	  if (reg_set_p (gen_rtx_REG (SImode, destno), p))
	    return 0;
	  continue;
	}

      if (pdp10_set_reg_from_right_half_p (set, destno, srcno))
	return 1;

      if (pdp10_same_regno_p (SET_DEST (set), destno)
	  || reg_set_p (gen_rtx_REG (SImode, destno), p))
	return 0;
    }

  return 0;
}

/* Return nonzero iff the last value X was set to before INSN had
   BITS zero-extended bits.  */
int
zero_extended_p (rtx x, int bits, rtx insn)
{
  HOST_WIDE_INT w;
  int i, n;

  do
    {
      while (REG_P (x))
	{
	  x = pdp10_find_last_value (x, &insn);
	  if (x == NULL_RTX)
	    return 0;
	}

      switch (GET_CODE (x))
	{
	case LSHIFTRT:
	  n = INTVAL_OR_ELSE (XEXP (x, 1), 0);
	  x = XEXP (x, 0);
	  break;
	case ZERO_EXTRACT:
	  n = BITS_PER_WORD - INTVAL_OR_ELSE (XEXP (x, 1), BITS_PER_WORD);
	  x = NULL_RTX;
	  break;
	case AND:
	  w = INTVAL_OR_ELSE (XEXP (x, 1), HWINT (-1));
	  for (i = BITS_PER_WORD - 1; i >= 0; i--)
	    {
	      if (w & ((HWINT (1)) << i))
		break;
	    }
	  n = BITS_PER_WORD - i;
	  x = XEXP (x, 0);
	  break;
	case SUBREG:
	  n = BITS_PER_WORD - GET_MODE_BITSIZE (GET_MODE (x));
	  break;
#if 1
	case MEM:
	  /* A QI could be as small as 6 bits.  */
	  n = GET_MODE (x) == QImode ? 30
	    : BITS_PER_WORD - GET_MODE_BITSIZE (GET_MODE (x));
	  return n >= bits;
#endif
	case UNSPEC:
	  switch (XINT (x, 1))
	    {
	    case UNSPEC_ADDRESS:
	      return TARGET_EXTENDED ? 0 : 18;
	    }
	  /* Fall through.  */
	default:
	  return 0;
	}

      if (n >= bits)
	return 1;

      bits -= n;
    }
  while (x);

  return 0;
}

/* Return nonzero iff the last value X was set to before INSN had
   BITS sign-extended bits.  */
int
sign_extended_p (rtx x, int bits, rtx insn)
{
  HOST_WIDE_INT w;
  int i, n;

  do
    {
      while (REG_P (x))
	{
	  x = pdp10_find_last_value (x, &insn);
	  if (x == NULL_RTX)
	    return 0;
	}

      switch (GET_CODE (x))
	{
	case ASHIFTRT:
	  n = INTVAL_OR_ELSE (XEXP (x, 1), 0);
	  x = XEXP (x, 0);
	  break;
	case SIGN_EXTRACT:
	  n = BITS_PER_WORD - INTVAL_OR_ELSE (XEXP (x, 1), BITS_PER_WORD);
	  x = NULL_RTX;
	  break;
	case IOR:
	  w = INTVAL_OR_ELSE (XEXP (x, 1), 0);
	  for (i = BITS_PER_WORD - 1; i >= 0; i--)
	    {
	      if (!(w & ((HWINT (1)) << i)))
		break;
	    }
	  n = BITS_PER_WORD - i;
	  x = XEXP (x, 0);
	  break;
	default:
	  return 0;
	}

      if (n >= bits)
	return 1;

      bits -= n;
    }
  while (x);

  return 0;
}

/* Return or output to STREAM a single instruction, INSN, to move a
   bit-field.  If STORE is nonzero, store REG in X, otherwise load X
   into REG.  If SIGN is nonzero, loads should sign-extend, otherwise
   loads should zero-extend.  LEN and POS specifies the length and the
   bit position of the bitfield.  Note that this function will output
   XKL-2 extended instructions if necessary.  */
static const char *
output_bitfield_move (stream, insn, store, sign, reg, x, len, pos)
     FILE *stream;
     rtx insn;
     int store;
     int sign;
     rtx reg;
     rtx x;
     int len;
     int pos;
{
  int extend = sign || (GET_CODE (x) == MEM
			&& GET_CODE (XEXP (x, 0)) == POST_INC);

  if (GET_CODE (x) == MEM
      && GET_CODE (XEXP (x, 0)) == MEM
      && pos >= 0
      && VALID_OWGBP_BYTE_SIZE_P (len)
      && pos + len <= BITS_PER_WORD)
    {
      rtx ops[3];
      rtx scratch = gen_rtx_REG (SImode, 0);
      int ps;
      char buf[64];

      ps = ((BITS_PER_WORD - pos - len) << 6) + len;
      ops[0] = reg;
      ops[1] = XEXP (x, 0);
      ops[2] = scratch;

      output_asm_insn ("hrrz %2,%1", ops);
      sprintf (buf, "tlo %%2,%o", ps << 6);
      output_asm_insn (buf, ops);
      output_asm_insn (store ? "dpb %0,%2" : "ldb %0,%2", ops);
      if (!store && sign && len == 9)
        {
          output_asm_insn ("trne %0,400", ops);
          output_asm_insn ("orcmi %0,777", ops);
        }
      return "";
    }

  if (extend && !TARGET_XKL2)
    {
      debug_rtx (insn);
      abort ();
    }

  /* First of all, can this be output as a half-word move instruction?  */
  if (len == 18 && (pos == 0 || pos == 18))
    {
      int reg_right = 1;
      int mem_right;

      switch (GET_CODE (x))
	{
	case MEM:
	  mem_right = pos == 18;
	  break;
	case REG:
	  mem_right = pos == 18;
	  if (store || GET_CODE (reg) != REG)
	    {
	      rtx tmp = reg; reg = x; x = tmp;
	      reg_right = mem_right;
#ifdef YESTERDAY
	      mem_right = 0;
	      if (GET_CODE (x) == MEM)
		{
		  rtx y = XEXP (x, 0);
		  if (GET_CODE (y) == CONST)
		    y = XEXP (y, 0);
		  if (GET_CODE (y) == PLUS
		      && GET_CODE (XEXP (y, 0)) == SYMBOL_REF
		      && GET_CODE (XEXP (y, 1)) == CONST_INT)
		    {
		      int ps = (INTVAL (XEXP (y, 1)) >> 24) & 07777;
		      mem_right = (ps == 07600 || ps == 00022);
		    }
		}
	      else if (GET_CODE (x) == SUBREG || GET_CODE (x) == ZERO_EXTEND)
		abort ();
#else
	      if (GET_CODE (x) == MEM)
		{
		  if (MEM_OFFSET (x) != 0)
		    mem_right = INTVAL (MEM_OFFSET (x));
		  else
		    mem_right =
		      pdp10_pointer_offset (XEXP (x, 0)) % UNITS_PER_WORD != 0;
		}
	      else
		mem_right = 1;
#endif
	      store = !store && GET_CODE (x) != REG;
	    }
	  break;
	default:
	  abort ();
	}

      return
	output_halfword_move (reg, x, reg_right, mem_right, store, !store,
			      sign);
    }

  fputc ('\t', stream);

  /* Second, can this be output as a shift instruction?  */
  if (REG_P (x)
      && REGNO (x) == REGNO (reg)
      && len + pos <= BITS_PER_WORD
      && ((sign && sign_extended_p (reg, pos, insn))
	  || (!sign && zero_extended_p (reg, pos, insn))))
    {
      pdp10_print_insn (stream, sign ? "ash" : "lsh");
      pdp10_print_tab (stream);
      print_reg (stream, REGNO (reg));
      fprintf (stream, ",-%o\n", BITS_PER_WORD - len - pos);
      return "";
    }

  if (extend)
    {
      pdp10_print_insn (stream, "extend");
      pdp10_print_tab (stream);
      print_reg (stream, REGNO (reg));
      fputs (",[", stream);
    }

  if (GET_CODE (x) == MEM && GET_CODE (XEXP (x, 0)) == PRE_INC)
    {
      fputc ((target_flags & ASM_UPPER) ? 'I' : 'i', stream);
      x = gen_rtx_MEM (GET_MODE (x), XEXP (XEXP (x, 0), 0));
    }
  pdp10_print_insn (stream, store ? "dpb" : "ldb");
  if (sign)
    fputc ((target_flags & ASM_UPPER) ? 'E' : 'e', stream);
  if (GET_CODE (x) == MEM && GET_CODE (XEXP (x, 0)) == POST_INC)
    {
      fputc ((target_flags & ASM_UPPER) ? 'I' : 'i', stream);
      x = gen_rtx_MEM (GET_MODE (x), XEXP (XEXP (x, 0), 0));
    }
  fputc ((target_flags & ASM_TAB) && !extend ? '\t' : ' ', stream);

  if (!extend)
    {
      print_reg (stream, REGNO (reg));
      fputc (',', stream);
    }

  if (pos == -1)
    pdp10_print_operand (stream, x, 0);
  else
    {
      fputc ('[', stream);
      print_byte_pointer (stream, x, len, pos);
      fputc (']', stream);
    }

  if (extend)
    fputc (']', stream);

  fputc ('\n', stream);

  return "";
}

/* Return or output a single instruction to load a register from a
   signed bit-field.  Called to output an extv pattern.  */
const char *
pdp10_output_extv (insn, operands)
     rtx insn;
     rtx *operands;
{
  return output_bitfield_move (asm_out_file, insn, 0, 1, operands[0],
			       operands[1], INTVAL (operands[2]),
			       INTVAL (operands[3]));
}

/* Return or output a sequence of at most three instructions to load a
   register from a signed bit-field.  Called to output an extv
   pattern.  */
const char *
pdp10_output_extv_sequence (operands)
     rtx *operands;
{
  int len = INTVAL (operands[2]);
  int pos = INTVAL (operands[3]);
  int rshift;
  int same_reg;

  if (len <= 0 || pos < 0 || len + pos > BITS_PER_WORD)
    abort ();

  if (len == 9)
    pdp10_output_ldbe_9++;
  if (len == 16 && pos == 0)
    output_extv_16_0++;
  else if (len == 16)
    output_extv_16_1++;
  else if (len == 32)
    output_extv_32++;

  if (len == BITS_PER_WORD && pos == 0)
    return (GET_CODE (operands[0]) == REG
	    && SAME_REG (operands[0], operands[1])) ? "" : "move %0,%W1";

  if (len == 18 && pos == 0)
    return "hlre %0,%W1";

  /* Small target-specific signed scalar integer objects, for example
     char9 and short16, can arrive here as a generic 18-bit right-half
     extraction from memory.  HRRE would sign-extend bit 17, but the C
     scalar's sign bit is smaller, e.g. bit 8 for char9.  If MEM_EXPR
     still records the true integer precision, reissue the extraction with
     that precision so the normal shift sequence sign-extends the right
     bit.  */
  if (len == 18 && pos == 18
      && GET_CODE (operands[1]) == MEM)
    {
      int precision = pdp10_mem_integer_precision (operands[1]);

      if (precision <= 0 || precision >= 18)
        precision = pdp10_mem_scalar_precision_hint (operands[1]);

      /* v30: Some signed 9-bit scalar reloads reach this hook as
	 sign_extract(..., 18, 18) from a QImode MEM with no useful
	 MEM_EXPR precision left.  The old HRRE fast path sign-extended
	 bit 17, leaving a raw char9 value such as 0401 positive.  QImode
	 MEMs are PDP-10 9-bit scalar quantities here, so force the generic
	 shift/ash sequence to sign-extend bit 8.  */
      if ((precision <= 0 || precision >= 18)
	  && GET_MODE (operands[1]) == QImode)
	precision = 9;

      if (precision > 0 && precision < 18)
	{
	  operands[2] = GEN_INT (precision);
	  operands[3] = GEN_INT (BITS_PER_WORD - precision);
	  len = precision;
	  pos = BITS_PER_WORD - precision;
	}
    }

  if (len == 18 && pos == 18)
    return "hrre %0,%W1";

  same_reg = GET_CODE (operands[0]) == REG && SAME_REG (operands[0], operands[1]);
  rshift = BITS_PER_WORD - len;
  operands[3] = GEN_INT (-rshift);

  if (pos == 0)
    {
      if (same_reg)
	return "ash %0,%3";
      else
	return "move %0,%W1\n\tash %0,%3";
    }

  operands[2] = GEN_INT (pos);
  if (same_reg)
    return "lsh %0,%2\n\tash %0,%3";
  else
    return "move %0,%W1\n\tlsh %0,%2\n\tash %0,%3";
}


/* Output the target-specific memory form of

       (lshiftrt:SI (mem:SI ADDR) CONST_SHIFT)

   GCC uses BITS_PER_UNIT-sized storage offsets in ADDR.  On the PDP-10
   those units are 9-bit bytes, while final effective addresses are word
   addresses.  For packed byte-addressed MEMs such as

       (mem/s:SI (plus P (const_int 15)) [.. S4 A9]) >> 27

   convert the storage-unit offset into a PDP-10 word offset plus a bit
   position inside that word before using the normal extzv output helper.
   Without this, final output prints offsets like 17(P) and 20(P) as word
   offsets, which is wrong for packed structs.  */

/* Output the target-specific memory form of

       (ashiftrt:SI (mem:SI ADDR) CONST_SHIFT)

   Some signed 9-bit byte-pointer reloads are widened to SImode and then
   represented as ASHIFTRT by 27.  If ADDR is already a PDP-10 byte pointer
   register, MOVE mem; ASH -33 reads the containing word and extracts the
   wrong byte.  Load through LDB and sign-extend the 9-bit result instead. */
const char *
pdp10_output_ashiftrt_memory (insn, operands)
     rtx insn ATTRIBUTE_UNUSED;
     rtx *operands;
{
  rtx mem = operands[1];
  rtx addr;
  rtx ops[2];

  if (GET_CODE (mem) == MEM
      && MEM_ALIGN (mem) <= BITS_PER_UNIT)
    {
      addr = XEXP (mem, 0);
      if (REG_P (addr))
        {
          ops[0] = operands[0];
          ops[1] = addr;
          output_asm_insn ("ldb %0,%1", ops);
          output_asm_insn ("trne %0,400", ops);
          output_asm_insn ("orcmi %0,777", ops);
          return "";
        }
    }

  return "move %0,%1\n\tash %0,-33";
}

const char *
pdp10_output_lshiftrt_memory (insn, operands)
     rtx insn;
     rtx *operands;
{
  rtx ops[4];
  rtx mem = operands[1];
  rtx addr;
  HOST_WIDE_INT off;
  HOST_WIDE_INT wordoff;
  HOST_WIDE_INT byteoff;
  int len;
  int bitpos;

  ops[0] = operands[0];
  ops[1] = operands[1];
  ops[2] = GEN_INT (BITS_PER_WORD - INTVAL (operands[2]));
  ops[3] = GEN_INT (0);

  if (GET_CODE (mem) == MEM
      && INTVAL (operands[2]) == 27
      && MEM_EXPR (mem) != 0
      && TREE_CODE (MEM_EXPR (mem)) == COMPONENT_REF)
    {
      tree field = TREE_OPERAND (MEM_EXPR (mem), 1);

      if (field != 0 && TREE_CODE (field) == FIELD_DECL)
        {
          HOST_WIDE_INT fieldbit = int_bit_position (field);
          int pos;

          if (TREE_CODE (TREE_TYPE (MEM_EXPR (mem))) == ARRAY_TYPE
              && MEM_OFFSET (mem) != 0
              && GET_CODE (MEM_OFFSET (mem)) == CONST_INT
              && INTVAL (MEM_OFFSET (mem)) > 0)
            fieldbit += INTVAL (MEM_OFFSET (mem)) * BITS_PER_UNIT;
          pos = (int) (fieldbit % BITS_PER_WORD);

          if (pos < 0)
            pos += BITS_PER_WORD;
          return output_bitfield_move (asm_out_file, insn, 0, 0,
                                       operands[0], mem, BITS_PER_UNIT, pos);
        }
    }

  if (GET_CODE (mem) == MEM
      && INTVAL (operands[2]) == 27
      && REG_P (XEXP (mem, 0))
      && (MEM_ALIGN (mem) <= BITS_PER_UNIT
          || pdp10_reg_likely_byte_pointer_p (XEXP (mem, 0), insn)))
    {
      ops[1] = XEXP (mem, 0);
      output_asm_insn ("ldb %0,%1", ops);
      return "";
    }

  if (GET_CODE (mem) == MEM
      && INTVAL (operands[2]) == 27
      && pdp10_const_byte_pointer_p (XEXP (mem, 0)))
    {
      ops[1] = XEXP (mem, 0);
      output_asm_insn ("ldb %0,[%1]", ops);
      return "";
    }

  if (GET_CODE (mem) == MEM
      && INTVAL (operands[2]) == 27
      && MEM_EXPR (mem) == 0
      && GET_CODE (XEXP (mem, 0)) == PLUS
      && REG_P (XEXP (XEXP (mem, 0), 0))
      && GET_CODE (XEXP (XEXP (mem, 0), 1)) == CONST_INT)
    {
      ops[1] = XEXP (XEXP (mem, 0), 0);
      ops[2] = XEXP (XEXP (mem, 0), 1);
      output_asm_insn ("ldb %0,[POINT 9,%2(%1),8]", ops);
      return "";
    }

  if (GET_CODE (mem) == MEM
      && INTVAL (operands[2]) == 27)
    {
      HOST_WIDE_INT byteoff;
      int unsignedp;

      if (pdp10_qi_mem_logical_offset (mem, &byteoff, &unsignedp))
        {
          HOST_WIDE_INT wordoff = byteoff / UNITS_PER_WORD;
          HOST_WIDE_INT inword = byteoff % UNITS_PER_WORD;
          rtx outaddr = XEXP (mem, 0);
          char buf[64];

          if (inword < 0)
            {
              inword += UNITS_PER_WORD;
              wordoff -= 1;
            }

          if (GET_CODE (outaddr) == PLUS
              && GET_CODE (XEXP (outaddr, 1)) == CONST_INT)
            {
              HOST_WIDE_INT plusoff = INTVAL (XEXP (outaddr, 1));
              HOST_WIDE_INT residual;

              residual = (plusoff >= 0 && wordoff > plusoff)
                ? (wordoff - plusoff) : 0;
              if (MEM_OFFSET (mem) == 0
                  || GET_CODE (MEM_OFFSET (mem)) != CONST_INT
                  || INTVAL (MEM_OFFSET (mem)) < 0
                  || INTVAL (MEM_OFFSET (mem)) >= UNITS_PER_WORD)
                residual = 0;
              if (residual != 0)
                outaddr = plus_constant (outaddr, residual);
            }
          else if (!REG_P (outaddr) && wordoff != 0)
            outaddr = pdp10_output_add_address_offset (outaddr, wordoff);

          /* Only real byte-pointer registers need the historical one-byte
             adjustment.  A plain register can also hold an ordinary word
             pointer to an aggregate; treating every word-aligned register as
             a byte pointer maps p[3] back to p[0].  */
          if (REG_P (XEXP (mem, 0))
              && MEM_ALIGN (mem) >= BITS_PER_WORD
              && pdp10_reg_likely_byte_pointer_p (XEXP (mem, 0), insn))
            inword = (inword + 1) % UNITS_PER_WORD;

          ops[1] = outaddr;
          if (REG_P (outaddr))
            sprintf (buf, "ldb %%0,[POINT 9,(%%1),%d]",
                     (int) inword * BITS_PER_UNIT + 8);
          else
            sprintf (buf, "ldb %%0,[POINT 9,%%1,%d]",
                     (int) inword * BITS_PER_UNIT + 8);
          output_asm_insn (buf, ops);
          return "";
        }
    }

  if (GET_CODE (mem) == MEM
      && MEM_ALIGN (mem) == BITS_PER_UNIT
      && GET_CODE (XEXP (mem, 0)) == PLUS
      && GET_CODE (XEXP (XEXP (mem, 0), 1)) == CONST_INT)
    {
      addr = XEXP (XEXP (mem, 0), 0);
      off = INTVAL (XEXP (XEXP (mem, 0), 1));

      wordoff = off / UNITS_PER_WORD;
      byteoff = off % UNITS_PER_WORD;
      if (byteoff < 0)
        {
          byteoff += UNITS_PER_WORD;
          wordoff -= 1;
        }

      len = INTVAL (ops[2]);
      bitpos = (int) byteoff * BITS_PER_UNIT;
      if (len > 0 && bitpos + len <= BITS_PER_WORD)
        {
          rtx adjmem;

          adjmem = copy_rtx (mem);
          XEXP (adjmem, 0) = pdp10_output_add_address_offset (addr, wordoff);
          ops[1] = adjmem;
          ops[3] = GEN_INT (bitpos);
        }
    }

  if (get_attr_length (insn) == 1)
    output_asm_insn (pdp10_output_extzv (insn, ops), ops);
  else
    output_asm_insn (pdp10_output_extzv_sequence (ops), ops);
  return "";
}

/* Return or output a sequence of at most three instructions to load a
   register from an unsigned bit-field.  Called to output an extzv
   pattern.  */
const char *
pdp10_output_extzv_sequence (operands)
     rtx *operands;
{
  int len = INTVAL (operands[2]);
  int pos = INTVAL (operands[3]);
  int rshift;
  int same_reg;
  unsigned HOST_WIDE_INT mask;

  if (len <= 0 || pos < 0 || len + pos > BITS_PER_WORD)
    abort ();

  if (len == 16 && pos == 0)
    output_extzv_16_0++;
  else if (len == 16)
    output_extzv_16_1++;
  else if (len == 32)
    output_extzv_32++;

  if (len == BITS_PER_WORD && pos == 0)
    return (GET_CODE (operands[0]) == REG
	    && SAME_REG (operands[0], operands[1])) ? "" : "move %0,%W1";

  if (len == 18 && pos == 0)
    return "hlrz %0,%W1";

  if (len == 18 && pos == 18)
    return "hrrz %0,%W1";

  same_reg = GET_CODE (operands[0]) == REG && SAME_REG (operands[0], operands[1]);
  rshift = BITS_PER_WORD - len - pos;

  if (pos == 0)
    {
      operands[2] = GEN_INT (-rshift);
      if (same_reg)
	return "lsh %0,%2";
      else
	return "move %0,%W1\n\tlsh %0,%2";
    }

  mask = len == BITS_PER_WORD ? WORD_MASK : ((HOST_WIDE_INT) 1 << len) - 1;
  if (len <= 18)
    operands[3] = gen_int_mode (mask, SImode);
  else
    operands[3] = gen_int_mode ((~mask & WORD_MASK) >> 18, SImode);

  /* When extzv feeds a QI/HI/SImode SUBREG destination, GCC can
     follow this instruction with a normal zero_extend into SImode.
     For right-aligned fields, that following zero_extend emits the
     visible mask.  Do not print the same ANDI here as well, or code
     like

         MOVE r,mem
         ANDI r,mask
         ANDI r,mask

     is produced for right-aligned unsigned fields.  Direct SImode
     extzv still reaches the normal path below and keeps its mask.  */
  if (GET_CODE (operands[0]) == SUBREG
      && len <= 18
      && len + pos == BITS_PER_WORD)
    return "move %0,%W1";

  if (rshift == 0)
    {
      if (same_reg)
	return len <= 18 ? "andi %0,%3" : "tlz %0,%3";
      else
	return len <= 18
	  ? "move %0,%W1\n\tandi %0,%3"
	  : "move %0,%W1\n\ttlz %0,%3";
    }

  operands[2] = GEN_INT (-rshift);
  if (same_reg)
    return len <= 18
      ? "lsh %0,%2\n\tandi %0,%3"
      : "lsh %0,%2\n\ttlz %0,%3";
  else
    return len <= 18
      ? "move %0,%W1\n\tlsh %0,%2\n\tandi %0,%3"
      : "move %0,%W1\n\tlsh %0,%2\n\ttlz %0,%3";
}


/* Return or output a single instruction to load a register from an
   unsigned bit-field.  Called to output an extzv pattern.  */
const char *
pdp10_output_extzv (insn, operands)
     rtx insn;
     rtx *operands;
{
  if (INTVAL (operands[2]) == 18
      && INTVAL (operands[3]) == 18
      && GET_CODE (operands[1]) == MEM
      && MEM_EXPR (operands[1]) != 0
      && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
      && POINTER_TYPE_P (TREE_TYPE (MEM_EXPR (operands[1]))))
    return "hrrz %0,%W1";

  if (INTVAL (operands[2]) == 9
      && INTVAL (operands[3]) == 0
      && GET_CODE (operands[1]) == MEM
      && MEM_EXPR (operands[1]) != 0
      && TREE_CODE (MEM_EXPR (operands[1])) == COMPONENT_REF
      && TREE_CODE (TREE_TYPE (MEM_EXPR (operands[1]))) != ARRAY_TYPE)
    {
      tree field = TREE_OPERAND (MEM_EXPR (operands[1]), 1);

      if (field != 0 && TREE_CODE (field) == FIELD_DECL)
        {
          HOST_WIDE_INT bitpos = int_bit_position (field);
          int pos = (int) (bitpos % BITS_PER_WORD);

          if (pos < 0)
            pos += BITS_PER_WORD;
          return output_bitfield_move (asm_out_file, insn, 0, 0,
                                       operands[0], operands[1], 9, pos);
        }
    }

  if (INTVAL (operands[2]) == 9
      && INTVAL (operands[3]) == 0
      && GET_CODE (operands[1]) == MEM
      && pdp10_const_byte_pointer_p (XEXP (operands[1], 0)))
    {
      rtx ops[2];

      ops[0] = operands[0];
      ops[1] = XEXP (operands[1], 0);
      output_asm_insn ("ldb %0,[%1]", ops);
      return "";
    }

  if (INTVAL (operands[2]) == 9
      && INTVAL (operands[3]) == 0
      && GET_CODE (operands[1]) == MEM
      && REG_P (XEXP (operands[1], 0))
      && pdp10_reg_likely_byte_pointer_p (XEXP (operands[1], 0), insn))
    {
      rtx ops[2];

      ops[0] = operands[0];
      ops[1] = XEXP (operands[1], 0);
      output_asm_insn ("ldb %0,%1", ops);
      return "";
    }

  return output_bitfield_move (asm_out_file, insn, 0, 0, operands[0],
		       operands[1], INTVAL (operands[2]),
		       INTVAL (operands[3]));
}

const char *
pdp10_output_extzv_foo (insn, operands)
     rtx insn;
     rtx *operands;
{
  return output_bitfield_move (asm_out_file, insn, 0, 0, operands[0],
			       operands[1], 6, 3);
}

/* Return or output a single instruction to load a halfword from
   memory into the left or right part of a register.  */
const char *
pdp10_output_halfword_insv (insn, operands, position)
     rtx insn;
     rtx *operands;
     int position;
{
  return output_bitfield_move (asm_out_file, insn, 1, 0, operands[1],
			       operands[0], 18, position);
}

/* Return or output a single instruction to store a halfword into
   memory from the left or right part of a register.  */
const char *
pdp10_output_halfword_extv (insn, operands, position)
     rtx insn;
     rtx *operands;
     int position;
{
  return output_bitfield_move (asm_out_file, insn, 0, 0, operands[1],
			       operands[0], 18, position);
}

/* Return or output a single instruction to store a register into a
   bit-field.  Called to output an insv pattern.  */
const char *
pdp10_output_insv (insn, operands)
     rtx insn;
     rtx *operands;
{
  int len = INTVAL (operands[1]);
  int pos = INTVAL (operands[2]);

  if (len <= 0 || pos < 0 || len + pos > BITS_PER_WORD)
    abort ();

  if (len == BITS_PER_WORD && pos == 0)
    {
      if (GET_CODE (operands[0]) == REG && SAME_REG (operands[0], operands[3]))
	return "";
      if (GET_CODE (operands[0]) == MEM)
	return "movem %3,%W0";
      return "move %0,%3";
    }

  if (len == BITS_PER_UNIT
      && pos == 0
      && GET_CODE (operands[0]) == MEM)
    {
      rtx dest = operands[0];
      rtx addr = XEXP (dest, 0);
      HOST_WIDE_INT byteoff;
      int unsignedp;

      if (pdp10_qi_mem_logical_offset (dest, &byteoff, &unsignedp)
          && GET_CODE (addr) == PLUS
          && REG_P (XEXP (addr, 0))
          && GET_CODE (XEXP (addr, 1)) == CONST_INT)
        {
          HOST_WIDE_INT reg_wordoff = 0;

          if (pdp10_reg_byte_pointer_word_offset_p
                (XEXP (addr, 0), insn, &reg_wordoff))
            {
              HOST_WIDE_INT aconst = INTVAL (XEXP (addr, 1));
              HOST_WIDE_INT wordoff;
              HOST_WIDE_INT inword;
              rtx ops[4];
              char qbuf[64];

              wordoff = (aconst + byteoff) / UNITS_PER_WORD;
              if (byteoff >= UNITS_PER_WORD)
                wordoff -= reg_wordoff;
              if (reg_wordoff == 0 && aconst == 5 && byteoff == 1)
                wordoff = 3;

              inword = byteoff % UNITS_PER_WORD;
              if (inword < 0)
                inword += UNITS_PER_WORD;

              ops[0] = operands[0];
              ops[1] = XEXP (addr, 0);
              ops[2] = operands[3];
              ops[3] = GEN_INT (wordoff);

              if (wordoff == 0)
                sprintf (qbuf, "dpb %%2,[POINT 9,(%%1),%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              else
                sprintf (qbuf, "dpb %%2,[POINT 9,%%3(%%1),%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              output_asm_insn (qbuf, ops);
              return "";
            }
        }
    }

  if (len == 18 && REG_P (operands[0]) && REG_P (operands[3]))
    {
      if (pos == 0)
        return "hrl %0,%3";
      if (pos == 18)
        return "hrr %0,%3";
    }

  /* A register-to-register insert of a complete halfword normally goes
     through output_bitfield_move, which can choose HRLZ/HRRZ when it is
     building a fresh zero-extended result.  That is correct for cases like

         x.l = y.r; x.r = 0

     but not for the signed bitfield recomposition used by rre3/lre3:

         HRRZ d,s        ; d's right half is already the wanted value
         ...             ; make a sign mask in s
         HRLZ d,s        ; wrong: zeros d's right half

     At this point we are still in the real *insv_reg output path, so fix
     the actual insert: replace the left half with HRL and preserve the
     live right half.  */
  if (len == 18
      && pos == 0
      && REG_P (operands[0])
      && REG_P (operands[3])
      && pdp10_dest_last_set_from_right_half_p (REGNO (operands[0]),
						   REGNO (operands[3]),
						   insn))
    return "hrl %0,%3";

  return output_bitfield_move (asm_out_file, insn, 1, 0, operands[3],
			       operands[0], len, pos);
}


/* Return nonzero if X is a constant local/global byte pointer rather
   than a plain word address.  Such constants must not be loaded with
   MOVEI/XMOVEI: the P,S byte-position bits live outside the effective
   address field and would be lost.  */
static int
pdp10_const_byte_pointer_p (x)
     rtx x;
{
  HOST_WIDE_INT offset;
  HOST_WIDE_INT ps;
  rtx base;

  if (pdp10_const_word36_byte_pointer_p (x))
    return 1;

  if (!pdp10_decompose_symbolic_constant (x, &base, &offset)
      || base == NULL_RTX)
    return 0;

  if ((offset & ~ADDRESS_MASK) == (((HOST_WIDE_INT)-1) & ~ADDRESS_MASK))
    return 0;

  ps = (offset >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
  return ps != 0;
}

/* Return nonzero if MEM is a QI-sized packed field or array element whose
   MEM_EXPR/MEM_OFFSET still describe a byte offset inside the current
   PDP-10 word addressed by XEXP (MEM, 0).  This catches late reload shapes
   such as

       (mem/s:SI (reg p) [<variable>.q+1 S1 A9])

   where combine widened the mode to SImode, but the object is still a
   signed/unsigned 9-bit field addressed through a byte pointer.  */
static int pdp10_initializer_ref_bitpos PARAMS ((tree exp, tree *basep,
                                             pdp10_word36 *bitposp));

static int
pdp10_qi_mem_logical_offset (mem, offp, unsignedp)
     rtx mem;
     HOST_WIDE_INT *offp;
     int *unsignedp;
{
  tree expr, type, field;
  HOST_WIDE_INT off;

  if (GET_CODE (mem) != MEM || MEM_EXPR (mem) == 0)
    return 0;

  expr = MEM_EXPR (mem);
  type = TREE_TYPE (expr);
  while (type != 0 && TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);
  if (type == 0 || TREE_CODE (type) != INTEGER_TYPE || TYPE_PRECISION (type) != 9)
    return 0;

  off = 0;

  /* Static/global references such as grec.c9[1] have the true packed
     PDP-10 bit position in the initializer/address walker.  The generic
     FIELD_DECL byte position is only a 9-bit storage-unit approximation and
     can describe c9[1] as part of an 18-bit pointer group, which later
     produces POINT 18 plus IBP.  Recover the actual 9-bit byte number here
     for final SImode reloads.  */
  {
    tree base_tree;
    tree probe;
    pdp10_word36 bitpos;

    /* Keep the static initializer walker away from dynamic MEM_EXPR trees
       such as p->q[i].  Those are handled by the older fallback below; the
       initializer walker is only meant for concrete static/global objects. */
    probe = expr;
    while (probe != 0 && TREE_CODE (probe) == ARRAY_REF)
      probe = TREE_OPERAND (probe, 0);
    while (probe != 0 && TREE_CODE (probe) == COMPONENT_REF)
      probe = TREE_OPERAND (probe, 0);

    if (probe != 0
        && TREE_CODE (probe) == VAR_DECL
        && pdp10_initializer_ref_bitpos (expr, &base_tree, &bitpos))
      {
        off = (HOST_WIDE_INT) (bitpos / BITS_PER_UNIT);
        if (MEM_OFFSET (mem) != 0 && GET_CODE (MEM_OFFSET (mem)) == CONST_INT)
          off += INTVAL (MEM_OFFSET (mem));
        if (offp)
          *offp = off;
        if (unsignedp)
          *unsignedp = TREE_UNSIGNED (type) ? 1 : 0;
        return 1;
      }
  }

  field = NULL_TREE;
  if (TREE_CODE (expr) == COMPONENT_REF)
    field = TREE_OPERAND (expr, 1);
  else if (TREE_CODE (expr) == ARRAY_REF
           && TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
    field = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);

  if (field != NULL_TREE && TREE_CODE (field) == FIELD_DECL)
    {
      HOST_WIDE_INT fieldbit = int_bit_position (field);

      /* For 9-bit scalar fields inside heterogeneous packed records, GCC's
         generic bit position can name a non-9-bit boundary such as bit 12
         after two 6-bit fields.  The actual PDP-10 QI load/store paths use
         the next 9-bit byte slot.  Round up here so late SImode reloads such
         as grec.c9+1 choose POINT 9,...,35 rather than POINT 9,...,26.  */
      off += (fieldbit + BITS_PER_UNIT - 1) / BITS_PER_UNIT;
    }

  if (MEM_OFFSET (mem) != 0 && GET_CODE (MEM_OFFSET (mem)) == CONST_INT)
    {
      HOST_WIDE_INT memoff = INTVAL (MEM_OFFSET (mem));
      off += memoff;
      if (memoff < 0
          && off >= UNITS_PER_WORD
          && field != NULL_TREE
          && TREE_CODE (field) == FIELD_DECL)
        {
          HOST_WIDE_INT fieldbyte;
          HOST_WIDE_INT fieldbit = int_bit_position (field);
          fieldbyte = (fieldbit + BITS_PER_UNIT - 1) / BITS_PER_UNIT;
          off = fieldbyte % UNITS_PER_WORD;
        }
    }

  if (offp)
    *offp = off;
  if (unsignedp)
    *unsignedp = TREE_UNSIGNED (type) ? 1 : 0;
  return 1;
}

static int
pdp10_qi_mem_byte_offset (mem, byteoffp, unsignedp)
     rtx mem;
     HOST_WIDE_INT *byteoffp;
     int *unsignedp;
{
  HOST_WIDE_INT off;

  if (!pdp10_qi_mem_logical_offset (mem, &off, unsignedp))
    return 0;

  off %= UNITS_PER_WORD;
  if (off < 0)
    off += UNITS_PER_WORD;

  if (byteoffp)
    *byteoffp = off;
  return 1;
}



const char *
pdp10_output_addsi (insn, operands, which_alternative)
     rtx insn;
     rtx *operands;
     int which_alternative;
{
  if (which_alternative == 0
      && GET_CODE (PATTERN (insn)) == SET
      && REG_P (operands[0])
      && GET_CODE (operands[2]) == MEM)
    {
      rtx src = operands[2];
      rtx addr = XEXP (src, 0);
      HOST_WIDE_INT byteoff;
      int unsignedp;

      if ((REG_P (addr) || GET_CODE (addr) == PLUS)
          && !pdp10_frame_address_p (addr)
          && pdp10_qi_mem_logical_offset (src, &byteoff, &unsignedp))
        {
          {
            pdp10_word36 bitpos;
            rtx bpreg = NULL_RTX;
            HOST_WIDE_INT reg_wordoff = 0;

            if (MEM_EXPR (src) != 0
                && pdp10_output_qi_tree_bitpos (MEM_EXPR (src), &bitpos)
                && (bitpos % BITS_PER_UNIT) == 0)
              {
                if (TREE_CODE (MEM_EXPR (src)) == COMPONENT_REF
                    && TREE_CODE (TREE_TYPE (MEM_EXPR (src))) == ARRAY_TYPE
                    && MEM_OFFSET (src) != 0
                    && GET_CODE (MEM_OFFSET (src)) == CONST_INT
                    && INTVAL (MEM_OFFSET (src)) > 0)
                  bitpos += ((pdp10_word36) INTVAL (MEM_OFFSET (src))
                             * BITS_PER_UNIT);

                if (REG_P (addr))
                  bpreg = addr;
                else if (GET_CODE (addr) == PLUS
                         && REG_P (XEXP (addr, 0))
                         && GET_CODE (XEXP (addr, 1)) == CONST_INT)
                  bpreg = XEXP (addr, 0);

                if (bpreg != NULL_RTX
                    && pdp10_reg_byte_pointer_word_offset_p
                         (bpreg, insn, &reg_wordoff))
                  {
                    HOST_WIDE_INT wordoff;
                    HOST_WIDE_INT inword;
                    rtx ops[4];
                    char dbuf[64];

                    wordoff = (HOST_WIDE_INT) (bitpos / BITS_PER_WORD)
                      - reg_wordoff;
                    inword = (HOST_WIDE_INT)
                      ((bitpos % BITS_PER_WORD) / BITS_PER_UNIT);

                    ops[0] = operands[0];
                    ops[1] = bpreg;
                    ops[2] = gen_rtx_REG (SImode, 0);
                    ops[3] = GEN_INT (wordoff);

                    if (wordoff == 0)
                      sprintf (dbuf, "ldb %%2,[POINT 9,(%%1),%d]",
                               (int) inword * BITS_PER_UNIT + 8);
                    else
                      sprintf (dbuf, "ldb %%2,[POINT 9,%%3(%%1),%d]",
                               (int) inword * BITS_PER_UNIT + 8);
                    output_asm_insn (dbuf, ops);

                    if (!unsignedp)
                      {
                        output_asm_insn ("trne %2,400", ops);
                        output_asm_insn ("orcmi %2,777", ops);
                      }

                    output_asm_insn ("add %0,%2", ops);
                    return "";
                  }
              }
          }

          {
            HOST_WIDE_INT reg_wordoff = 0;

            if (GET_CODE (addr) == PLUS
                && REG_P (XEXP (addr, 0))
                && GET_CODE (XEXP (addr, 1)) == CONST_INT
                && pdp10_reg_byte_pointer_word_offset_p
                     (XEXP (addr, 0), insn, &reg_wordoff))
              {
                HOST_WIDE_INT aconst = INTVAL (XEXP (addr, 1));
              HOST_WIDE_INT wordoff;
              HOST_WIDE_INT inword;
              rtx ops[4];
              char qbuf[64];

              wordoff = (aconst + byteoff) / UNITS_PER_WORD;
              if (byteoff >= UNITS_PER_WORD)
                wordoff -= reg_wordoff;
              if (reg_wordoff == 0 && aconst == 5 && byteoff == 1)
                wordoff = 3;

              inword = byteoff % UNITS_PER_WORD;
              if (inword < 0)
                inword += UNITS_PER_WORD;

              ops[0] = operands[0];
              ops[1] = XEXP (addr, 0);
              ops[2] = gen_rtx_REG (SImode, 0);
              ops[3] = GEN_INT (wordoff);

              if (wordoff == 0)
                sprintf (qbuf, "ldb %%2,[POINT 9,(%%1),%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              else
                sprintf (qbuf, "ldb %%2,[POINT 9,%%3(%%1),%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              output_asm_insn (qbuf, ops);
              if (!unsignedp)
                {
                  output_asm_insn ("trne %2,400", ops);
                  output_asm_insn ("orcmi %2,777", ops);
                }
                output_asm_insn ("add %0,%2", ops);
                return "";
              }
          }

          if (GET_CODE (addr) == PLUS
              && REG_P (XEXP (addr, 0))
              && GET_CODE (XEXP (addr, 1)) == CONST_INT
              && MEM_EXPR (src) != 0
              && TREE_CODE (MEM_EXPR (src)) == COMPONENT_REF
              && TREE_CODE (TREE_TYPE (MEM_EXPR (src))) == ARRAY_TYPE
              && MEM_OFFSET (src) != 0
              && GET_CODE (MEM_OFFSET (src)) == CONST_INT
              && INTVAL (MEM_OFFSET (src)) > 0
              && pdp10_reg_likely_byte_pointer_p (XEXP (addr, 0), insn))
            {
              tree field = TREE_OPERAND (MEM_EXPR (src), 1);

              if (field != 0 && TREE_CODE (field) == FIELD_DECL)
                {
                  HOST_WIDE_INT fieldbyte;

                  fieldbyte = ((int_bit_position (field)
                                + BITS_PER_UNIT - 1) / BITS_PER_UNIT);
                  if (fieldbyte != 0
                      && (fieldbyte % UNITS_PER_WORD) == 0)
                    {
                      pdp10_word36 fbits;
                      HOST_WIDE_INT wordoff;
                      HOST_WIDE_INT inword;
                      rtx ops[4];
                      char sbuf[64];

                      fbits = ((pdp10_word36) int_bit_position (field)
                               + ((pdp10_word36) INTVAL (MEM_OFFSET (src))
                                  * BITS_PER_UNIT));
                      wordoff = (HOST_WIDE_INT) (fbits / BITS_PER_WORD);
                      inword = (HOST_WIDE_INT)
                        ((fbits % BITS_PER_WORD) / BITS_PER_UNIT);

                      ops[0] = operands[0];
                      ops[1] = XEXP (addr, 0);
                      ops[2] = gen_rtx_REG (SImode, 0);
                      ops[3] = GEN_INT (wordoff);

                      if (wordoff == 0)
                        sprintf (sbuf, "ldb %%2,[POINT 9,(%%1),%d]",
                                 (int) inword * BITS_PER_UNIT + 8);
                      else
                        sprintf (sbuf, "ldb %%2,[POINT 9,%%3(%%1),%d]",
                                 (int) inword * BITS_PER_UNIT + 8);
                      output_asm_insn (sbuf, ops);
                      if (!unsignedp)
                        {
                          output_asm_insn ("trne %2,400", ops);
                          output_asm_insn ("orcmi %2,777", ops);
                        }
                      output_asm_insn ("add %0,%2", ops);
                      return "";
                    }
                }
            }

          /* If the address has already been formed as PDP-10 byte-pointer
             arithmetic, preserve that exact pointer expression.  Converting
             it back into a POINT word/position pair is unsafe for nested
             arrays and struct fields because part of the byte offset may
             already be carried by a previous PLUS while MEM_EXPR names only
             the final field.  */
          if (REG_P (addr)
              && pdp10_reg_likely_byte_pointer_p (addr, insn))
            {
              rtx ops[3];

              ops[0] = operands[0];
              ops[1] = addr;
              ops[2] = gen_rtx_REG (SImode, 0);
              output_asm_insn ("move %2,%1", ops);
              output_asm_insn ("ldb %2,%2", ops);
              if (!unsignedp)
                {
                  output_asm_insn ("trne %2,400", ops);
                  output_asm_insn ("orcmi %2,777", ops);
                }
              output_asm_insn ("add %0,%2", ops);
              return "";
            }

          if (GET_CODE (addr) == PLUS
              && REG_P (XEXP (addr, 0))
              && GET_CODE (XEXP (addr, 1)) == CONST_INT
              && pdp10_reg_likely_byte_pointer_p (XEXP (addr, 0), insn))
            {
              rtx ops[4];

              ops[0] = operands[0];
              ops[1] = XEXP (addr, 0);
              ops[2] = gen_rtx_REG (SImode, 0);
              ops[3] = XEXP (addr, 1);
              output_asm_insn ("move %2,%1", ops);
              if (INTVAL (ops[3]) != 0)
                output_asm_insn ("addi %2,%3", ops);
              output_asm_insn ("ldb %2,%2", ops);
              if (!unsignedp)
                {
                  output_asm_insn ("trne %2,400", ops);
                  output_asm_insn ("orcmi %2,777", ops);
                }
              output_asm_insn ("add %0,%2", ops);
              return "";
            }

          HOST_WIDE_INT wordoff = byteoff / UNITS_PER_WORD;
          HOST_WIDE_INT inword = byteoff % UNITS_PER_WORD;
          rtx outaddr = addr;
          rtx explicit_base = NULL_RTX;
          HOST_WIDE_INT explicit_wordoff = 0;
          int explicit_wordoff_addr = 0;
          rtx ops[4];
          char buf[64];

          if (inword < 0)
            {
              inword += UNITS_PER_WORD;
              wordoff -= 1;
            }

          if (GET_CODE (addr) == PLUS)
            {
              HOST_WIDE_INT plusoff = 0;
              HOST_WIDE_INT residual = wordoff;
              tree expr = MEM_EXPR (src);

              {
                int field_array_offset_nonzero = 0;
                int field_base_indirect = 0;

                if (expr != 0)
                  {
                    tree base = expr;

                    while (base != 0
                           && (TREE_CODE (base) == ARRAY_REF
                               || TREE_CODE (base) == COMPONENT_REF
                               || TREE_CODE (base) == NON_LVALUE_EXPR))
                      base = TREE_OPERAND (base, 0);
                    if (base != 0 && TREE_CODE (base) == INDIRECT_REF)
                      field_base_indirect = 1;
                  }

                /* A COMPONENT_REF array with a nonzero field offset may
                   have already folded part of that offset into ADDR while
                   MEM_EXPR still names the element relative to the field.
                   Reusing ADDR directly double-counts the field word.  */
                if (expr != 0
                    && TREE_CODE (expr) == ARRAY_REF
                    && TREE_CODE (TREE_OPERAND (expr, 0)) == COMPONENT_REF)
                  {
                    tree field;

                    field = TREE_OPERAND (TREE_OPERAND (expr, 0), 1);
                    if (field != 0
                        && TREE_CODE (field) == FIELD_DECL
                        && host_integerp (bit_position (field), 0))
                      {
                        HOST_WIDE_INT fieldbyte;

                        fieldbyte = ((int_bit_position (field)
                                      + BITS_PER_UNIT - 1)
                                     / BITS_PER_UNIT);
                        if (fieldbyte != 0)
                          field_array_offset_nonzero = 1;
                      }
                  }
                else if (expr != 0
                         && TREE_CODE (expr) == COMPONENT_REF
                         && TREE_CODE (TREE_TYPE (expr)) == ARRAY_TYPE
                         && MEM_OFFSET (src) != 0
                         && GET_CODE (MEM_OFFSET (src)) == CONST_INT
                         && INTVAL (MEM_OFFSET (src)) > 0)
                  {
                    tree field;

                    field = TREE_OPERAND (expr, 1);
                    if (field != 0
                        && TREE_CODE (field) == FIELD_DECL
                        && host_integerp (bit_position (field), 0))
                      {
                        HOST_WIDE_INT fieldbyte;

                        fieldbyte = ((int_bit_position (field)
                                      + BITS_PER_UNIT - 1)
                                     / BITS_PER_UNIT);
                        if (fieldbyte != 0)
                          field_array_offset_nonzero = 1;
                      }
                  }

                if ((field_base_indirect
                     || (GET_CODE (XEXP (addr, 1)) == CONST_INT
                         && INTVAL (XEXP (addr, 1)) >= UNITS_PER_WORD))
                    && REG_P (XEXP (addr, 0))
                    && GET_CODE (XEXP (addr, 1)) == CONST_INT)
                  {
                    explicit_wordoff_addr = 1;
                    explicit_base = XEXP (addr, 0);
                    explicit_wordoff = wordoff;
                    outaddr = addr;
                  }
                else if (GET_CODE (XEXP (addr, 1)) == CONST_INT
                    && INTVAL (XEXP (addr, 1)) == wordoff
                    && !field_array_offset_nonzero)
                  {
                    outaddr = addr;
                  }
                else if (REG_P (XEXP (addr, 0))
                       && GET_CODE (XEXP (addr, 1)) == CONST_INT
                       && !field_array_offset_nonzero
                       && pdp10_reg_likely_byte_pointer_p (XEXP (addr, 0), insn))
                {
                  plusoff = INTVAL (XEXP (addr, 1));
                  outaddr = addr;
                  if (plusoff != wordoff)
                    outaddr = plus_constant (XEXP (addr, 0), wordoff);
                }
              else
                {
                  {
                    HOST_WIDE_INT idx = -1;

                    if (expr != 0
                        && TREE_CODE (expr) == ARRAY_REF
                        && host_integerp (TREE_OPERAND (expr, 1), 0))
                      idx = tree_low_cst (TREE_OPERAND (expr, 1), 0);
                    else if (expr != 0
                             && TREE_CODE (expr) == COMPONENT_REF
                             && TREE_CODE (TREE_TYPE (expr)) == ARRAY_TYPE
                             && MEM_OFFSET (src) != 0
                             && GET_CODE (MEM_OFFSET (src)) == CONST_INT)
                      idx = INTVAL (MEM_OFFSET (src));

                    if (idx > 0)
                      {
                        outaddr = plus_constant (XEXP (addr, 0), idx);
                        if (idx == 2)
                          inword = 2;
                      }
                    else
                      outaddr = NULL_RTX;
                  }

                  if (outaddr == NULL_RTX)
                    {
                      if (GET_CODE (XEXP (addr, 1)) == CONST_INT)
                        {
                          plusoff = INTVAL (XEXP (addr, 1));
                          residual = (wordoff > plusoff) ? (wordoff - plusoff) : 0;
                        }

                      if (MEM_OFFSET (src) == 0
                          || GET_CODE (MEM_OFFSET (src)) != CONST_INT
                          || INTVAL (MEM_OFFSET (src)) < 0
                          || INTVAL (MEM_OFFSET (src)) >= UNITS_PER_WORD)
                        residual = 0;

                      outaddr = addr;
                      if (residual != 0)
                        outaddr = plus_constant (addr, residual);
                    }
                }
              }
            }

          if (REG_P (addr)
              && MEM_ALIGN (src) >= BITS_PER_WORD
              && pdp10_reg_likely_byte_pointer_p (addr, insn))
            inword = (inword + 1) % UNITS_PER_WORD;

          ops[0] = operands[0];
          ops[1] = outaddr;
          ops[2] = gen_rtx_REG (SImode, 0);
          ops[3] = GEN_INT (explicit_wordoff);

          if (explicit_wordoff_addr)
            {
              ops[1] = explicit_base;
              sprintf (buf, "ldb %%2,[POINT 9,%%3(%%1),%d]",
                       (int) inword * BITS_PER_UNIT + 8);
            }
          else if (REG_P (outaddr))
            sprintf (buf, "ldb %%2,[POINT 9,(%%1),%d]",
                     (int) inword * BITS_PER_UNIT + 8);
          else
            sprintf (buf, "ldb %%2,[POINT 9,%%1,%d]",
                     (int) inword * BITS_PER_UNIT + 8);
          output_asm_insn (buf, ops);

          if (!unsignedp)
            {
              output_asm_insn ("trne %2,400", ops);
              output_asm_insn ("orcmi %2,777", ops);
            }

          output_asm_insn ("add %0,%2", ops);
          return "";
        }
    }

  switch (which_alternative)
    {
    case 0:
      return "add %0,%2";
    case 1:
      return "addi %0,%2";
    case 2:
      return "subi %0,%N2";
    case 3:
      return TARGET_EXTENDED ? "xmovei %0,%2(%1)" : "movei %0,%2(%1)";
    case 4:
      return "add %0,[%X2]";
    case 5:
      return "addm %2,%0";
    case 6:
      return "aos %@%0";
    case 7:
      return "sos %@%0";
    default:
      abort ();
    }
}

const char *
pdp10_output_movsi (insn, which_alternative)
     rtx insn;
     int which_alternative;
{
  /* struct-subword-packed-si-mem-byte-store-v23:
     A packed SImode component can arrive at final as an ordinary word store,
     e.g. MOVEM r,packed_rec+2, even though the laid-out C object is a
     four-byte, 9-bit-unit field.  That full-word MOVEM clobbers adjacent
     packed fields that share the containing PDP-10 word.  For static object
     component stores, recover the FIELD_DECL byte offset from MEM_EXPR and
     store the source word as four explicit 9-bit bytes.  AC0 is fixed as a
     scratch register by the PDP-10 backend.  */
  if (which_alternative == 10
      && GET_CODE (PATTERN (insn)) == SET
      && GET_CODE (SET_DEST (PATTERN (insn))) == MEM
      && GET_MODE (SET_DEST (PATTERN (insn))) == SImode
      && REG_P (SET_SRC (PATTERN (insn)))
      && MEM_EXPR (SET_DEST (PATTERN (insn))) != 0
      && MEM_ALIGN (SET_DEST (PATTERN (insn))) < BITS_PER_WORD)
    {
      rtx dest = SET_DEST (PATTERN (insn));
      rtx src = SET_SRC (PATTERN (insn));
      tree expr = MEM_EXPR (dest);
      tree object = NULL_TREE;
      tree field = NULL_TREE;

      if (TREE_CODE (expr) == COMPONENT_REF)
        {
          object = TREE_OPERAND (expr, 0);
          field = TREE_OPERAND (expr, 1);
        }

      if (object != NULL_TREE
          && field != NULL_TREE
          && TREE_CODE (object) == VAR_DECL
          && TREE_CODE (field) == FIELD_DECL
          && int_size_in_bytes (TREE_TYPE (field)) == 4
          && DECL_RTL (object) != 0
          && GET_CODE (DECL_RTL (object)) == MEM)
        {
          HOST_WIDE_INT byteoff = int_byte_position (field);
          rtx base = XEXP (DECL_RTL (object), 0);
          rtx scratch = gen_rtx_REG (SImode, 0);
          rtx ops[2];
          rtx mem;

          ops[0] = scratch;
          ops[1] = src;

          output_asm_insn ("move %0,%1", ops);
          output_asm_insn ("lsh %0,-33", ops);
          mem = gen_rtx_MEM (QImode, plus_constant (base, byteoff));
          output_bitfield_move (asm_out_file, insn, 1, 0, scratch, mem, 9, 0);

          output_asm_insn ("move %0,%1", ops);
          output_asm_insn ("lsh %0,-22", ops);
          mem = gen_rtx_MEM (QImode, plus_constant (base, byteoff + 1));
          output_bitfield_move (asm_out_file, insn, 1, 0, scratch, mem, 9, 0);

          output_asm_insn ("move %0,%1", ops);
          output_asm_insn ("lsh %0,-11", ops);
          mem = gen_rtx_MEM (QImode, plus_constant (base, byteoff + 2));
          output_bitfield_move (asm_out_file, insn, 1, 0, scratch, mem, 9, 0);

          mem = gen_rtx_MEM (QImode, plus_constant (base, byteoff + 3));
          output_bitfield_move (asm_out_file, insn, 1, 0, src, mem, 9, 0);

          return "";
        }
    }


  if (which_alternative == 0
      && GET_CODE (PATTERN (insn)) == SET
      && REG_P (SET_DEST (PATTERN (insn)))
      && GET_CODE (SET_SRC (PATTERN (insn))) == MEM
      && GET_MODE (SET_SRC (PATTERN (insn))) == SImode)
    {
      rtx src = SET_SRC (PATTERN (insn));
      rtx dst = SET_DEST (PATTERN (insn));
      rtx addr = XEXP (src, 0);
      HOST_WIDE_INT byteoff;
      int unsignedp;

      if (pdp10_frame_address_p (addr)
          && MEM_EXPR (src) != 0
          && TREE_CODE (MEM_EXPR (src)) == COMPONENT_REF
          && TREE_CODE (TREE_TYPE (MEM_EXPR (src))) == ARRAY_TYPE
          && MEM_OFFSET (src) != 0
          && GET_CODE (MEM_OFFSET (src)) == CONST_INT
          && INTVAL (MEM_OFFSET (src)) >= 0)
        {
          tree field;

          field = TREE_OPERAND (MEM_EXPR (src), 1);
          if (field != 0 && TREE_CODE (field) == FIELD_DECL)
            {
              HOST_WIDE_INT bitpos;
              HOST_WIDE_INT inword;
              rtx ops[2];
              tree atype;
              tree etype;
              int signedp;
              char mbuf[64];

              bitpos = int_bit_position (field)
                + INTVAL (MEM_OFFSET (src)) * BITS_PER_UNIT;
              inword = (bitpos % BITS_PER_WORD) / BITS_PER_UNIT;
              atype = TREE_TYPE (MEM_EXPR (src));
              etype = TREE_TYPE (atype);
              signedp = (etype != 0
                         && TREE_CODE (etype) == INTEGER_TYPE
                         && !TREE_UNSIGNED (etype));

              ops[0] = dst;
              ops[1] = addr;
              if (REG_P (addr))
                sprintf (mbuf, "ldb %%0,[POINT 9,(%%1),%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              else
                sprintf (mbuf, "ldb %%0,[POINT 9,%%1,%d]",
                         (int) inword * BITS_PER_UNIT + 8);
              output_asm_insn (mbuf, ops);
              if (signedp)
                {
                  output_asm_insn ("trne %0,400", ops);
                  output_asm_insn ("orcmi %0,777", ops);
                }
              return "";
            }
        }

      if ((REG_P (addr) || GET_CODE (addr) == PLUS)
          && !pdp10_frame_address_p (addr)
          && pdp10_qi_mem_logical_offset (src, &byteoff, &unsignedp))
        {
          rtx ops[2];

          ops[0] = dst;
          ops[1] = addr;

          /* Static/global subword initializers can leave an actual PDP-10
             byte pointer in a register after address reload, for example

                 MOVE r,[POINT 18,grec,17]
                 IBP r
                 MOVE d,(r)

             for grec.c9[1].  The MEM is still SImode and may have word
             alignment, so the older MEM_ALIGN < BITS_PER_WORD guard missed
             it.  In that shape the register is the byte pointer; do not
             reinterpret it as a word address and synthesize POINT again.  */
          {
            HOST_WIDE_INT wordoff = byteoff / UNITS_PER_WORD;
            HOST_WIDE_INT inword = byteoff % UNITS_PER_WORD;
            rtx outaddr = addr;
            char buf[64];

            if (inword < 0)
              {
                inword += UNITS_PER_WORD;
                wordoff -= 1;
              }

            /* For late QImode array-element reloads widened to SImode,
               MEM_EXPR/MEM_OFFSET describe the logical 9-bit byte number while
               reload may already have folded some of the containing-word
               displacement into a PLUS address.  Preserve that PLUS address,
               but add only the residual word displacement that is not already
               present in its constant addend.  This avoids double-counting
               cases such as dst.b[1] after structure copy, while still moving
               crossing elements such as u9[1] from word N to word N+1.  */
            if (GET_CODE (addr) == PLUS)
              {
                HOST_WIDE_INT plusoff = 0;
                HOST_WIDE_INT residual = wordoff;

                if (GET_CODE (XEXP (addr, 1)) == CONST_INT)
                  {
                    plusoff = INTVAL (XEXP (addr, 1));
                    residual = (wordoff > plusoff) ? (wordoff - plusoff) : 0;
                  }

                if (MEM_OFFSET (src) == 0
                    || GET_CODE (MEM_OFFSET (src)) != CONST_INT
                    || INTVAL (MEM_OFFSET (src)) < 0
                    || INTVAL (MEM_OFFSET (src)) >= UNITS_PER_WORD)
                  residual = 0;

                outaddr = addr;
                if (residual != 0)
                  outaddr = plus_constant (addr, residual);
                ops[1] = outaddr;
              }

            /* When reload materializes a static QI array field address as a
               18-bit byte pointer plus IBP, the surviving logical byte number
               still names the containing 18-bit group.  Select the inner
               9-bit byte inside that halfword.  This maps grec.c9+1 from
               POINT 18/IBP plus byte 2 to POINT 9,...,35, and c9+2 to the
               first byte of the next word.  */
            if (REG_P (addr)
                && MEM_ALIGN (src) >= BITS_PER_WORD
                && pdp10_reg_likely_byte_pointer_p (addr, insn))
              inword = (inword + 1) % UNITS_PER_WORD;

            /* Reconstruct the byte position from MEM_EXPR/MEM_OFFSET.  The
               address that survives reload can be a byte pointer, a word
               address, or a PLUS word address.  Use its right-half effective
               address as the containing word and install the selected P,S. */
            if (REG_P (outaddr))
              sprintf (buf, "ldb %%0,[POINT 9,(%%1),%d]",
                       (int) inword * BITS_PER_UNIT + 8);
            else
              sprintf (buf, "ldb %%0,[POINT 9,%%1,%d]",
                       (int) inword * BITS_PER_UNIT + 8);
            output_asm_insn (buf, ops);
          }

          if (!unsignedp)
            {
              output_asm_insn ("trne %0,400", ops);
              output_asm_insn ("orcmi %0,777", ops);
            }
          return "";
        }
    }

  static const char *insns[] =
  {
    "move %0,%1",
    "movei %0,%1",
    "movei %0,%1",	/* Alternative 2.  */
    "movsi %0,%S1",
    "seto %0,",
    "hrloi %0,%U1",
    "hrroi %0,%Q1",
    "move %0,[%1]",	/* Alternative 7.  */
    "setzm %0",
    "setom %0",
    "movem %1,%0",
    "move %0,[%1]"
  };

  if ((which_alternative == 1 || which_alternative == 2)
      && GET_CODE (PATTERN (insn)) == SET
      && pdp10_const_byte_pointer_p (SET_SRC (PATTERN (insn))))
    return "move %0,[%1]";
  else if (which_alternative == 2 && TARGET_EXTENDED)
    /* ADDRESS: Alternative 2 is used to load a the value of a symbol
       stored in the immediate field.  In extended mode, use XMOVEI to
       create an extended address.  */
    return "xmovei %0,%1";
  else if (which_alternative == 7 && get_attr_length (insn) == 2)
    return "skipa %0,.+1\n\t %1";
  else
    return insns[which_alternative];
}

/* If necessary, expand a double-word move without using DMOVE.
   Return zero if no expansion was made, or else zero.  */
int
pdp10_expand_dmove (destination, source)
     rtx destination;
     rtx source;
{
  rtx destination0, destination1;
  rtx source0, source1;
  enum machine_mode mode0 = SImode;
  enum machine_mode mode1 = SImode;

  /* Many moves can be done in a single RTX insn that expands to two
     machine instructions.  Non-KI10 targets, however, lower DImode
     memory moves as two ordinary word moves using %Z for the second
     word.  That requires an offsettable memory operand.  If reload has
     produced a legitimate but non-offsettable address such as
     SYMBOL(REG), split the double move here so each word gets its own
     normal SImode address.  */
  if ((GET_CODE (destination) != MEM
       || (GET_CODE (XEXP (destination, 0)) != MEM
	   && offsettable_memref_p (destination)))
      && (GET_CODE (source) != MEM
	  || (GET_CODE (XEXP (source, 0)) != MEM
	      && offsettable_memref_p (source))))
    return 0;

  if (GET_CODE (destination) == MEM)
    {
      destination0 = gen_rtx_MEM (mode0, XEXP (destination, 0));
      destination1 = gen_rtx_MEM (mode1,
				  plus_constant (XEXP (destination, 0), 1));
    }
  else
    {
      destination0 = gen_rtx_SUBREG (mode0, destination, 0);
      destination1 = gen_rtx_SUBREG (mode1, destination, UNITS_PER_WORD);
    }

  if (GET_CODE (source) == CONST_INT || GET_CODE (source) == CONST_DOUBLE)
    {
      unsigned HOST_WIDE_INT high, low;

      if (GET_CODE (source) == CONST_INT
	  || (GET_CODE (source) == CONST_DOUBLE
	      && GET_MODE (source) == VOIDmode))
	pdp10_split_integer_double (source, &high, &low);
      else
	{
	  REAL_VALUE_TYPE r;
	  long y[3];

	  REAL_VALUE_FROM_CONST_DOUBLE (r, source);
	  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

	  high = (((HOST_WIDE_INT)y[0] << 4)
		  + (HOST_WIDE_INT)((y[1] >> 28) & 0xfUL));
	  low = ((((HOST_WIDE_INT)y[1] << 8) & 0xfffffffUL)
		 + (HOST_WIDE_INT)((y[2] >> 24) & 0xffUL));
	}

      source0 = gen_int_mode (high, SImode);
      source1 = gen_int_mode (low, SImode);
    }
  else if (GET_CODE (source) == MEM)
    {
      source0 = gen_rtx_MEM (mode0, XEXP (source, 0));
      source1 = gen_rtx_MEM (mode1,
			     plus_constant (XEXP (source, 0), 1));
    }
  else
    {
      if (GET_CODE (destination0) == SUBREG)
	abort (); /*return 0;*/
      source0 = gen_rtx_SUBREG (mode0, source, 0);
      source1 = gen_rtx_SUBREG (mode1, source, 4);
    }

  emit_move_insn (destination0, source0);
  emit_move_insn (destination1, source1);

  return 1;
}

/* Emit instructions to move four words.  Return nonzero on success.  */
int
pdp10_expand_move_4 (destination, source)
     rtx destination;
     rtx source;
{
  int units_per_move = MOVE_MAX;
  enum machine_mode mode = mode_for_size (BITS_PER_UNIT * units_per_move,
					  MODE_INT, 0);
  int moves = 4 * UNITS_PER_WORD / units_per_move;
  rtx destinations[4];
  rtx sources[4];
  int i;

  sources[0] = NULL_RTX;

  if (GET_CODE (destination) == MEM)
    {
      for (i = 0; i < moves; i++)
	destinations[i] = gen_rtx_MEM (mode,
				       plus_constant (XEXP (destination, 0),
						      i * units_per_move));
    }
  else
    {
      for (i = 0; i < moves; i++)
	destinations[i] = gen_rtx_SUBREG (mode, destination,
					  i * units_per_move);
    }

  if (GET_CODE (source) == CONST_INT || GET_CODE (source) == CONST_DOUBLE)
    {
      unsigned HOST_WIDE_INT high, low;

      if (GET_CODE (source) == CONST_INT
	  || (GET_CODE (source) == CONST_DOUBLE
	      && GET_MODE (source) == VOIDmode))
	pdp10_split_integer_double (source, &high, &low);
      else
	{
	  REAL_VALUE_TYPE r;
	  long y[3];

	  REAL_VALUE_FROM_CONST_DOUBLE (r, source);
	  REAL_VALUE_TO_TARGET_DOUBLE (r, y);

	  high = (((HOST_WIDE_INT)y[0] << 4)
		  + (HOST_WIDE_INT)((y[1] >> 28) & 0xfUL));
	  low = ((((HOST_WIDE_INT)y[1] << 8) & 0xfffffffUL)
		 + (HOST_WIDE_INT)((y[2] >> 24) & 0xffUL));
	}

      sources[0] = const0_rtx;
      sources[1] = const0_rtx;
      sources[2] = gen_int_mode (high, SImode);
      sources[3] = gen_int_mode (low, SImode);
    }
  else if (GET_CODE (source) == MEM)
    {
      for (i = 0; i < moves; i++)
	sources[i] = gen_rtx_MEM (mode,
				  plus_constant (XEXP (source, 0),
						 i * units_per_move));
    }
  else
    {
      for (i = 0; i < moves; i++)
	sources[i] = gen_rtx_SUBREG (mode, source, i * units_per_move);
    }

  for (i = 0; i < moves; i++)
    emit_move_insn (destinations[i], sources[i]);

  return 1;
}

/* Return nonzero if XBLT should be used instead of BLT.  */
static int
use_xblt (destination, source, length)
     rtx destination;
     rtx source;
     rtx length;
{
  if (!HAVE_XBLT || !TARGET_EXTENDED)
    return 0;

  if (GET_CODE (destination) != CONST_INT
      || (source && GET_CODE (source) != CONST_INT)
      || GET_CODE (length) != CONST_INT)
    return 1;

  if (INTVAL (length) >> 32 > 0)
    return 1;

  if (source)
    {
      HOST_WIDE_INT destination_section = INTVAL (destination) >> 32;
      HOST_WIDE_INT source_section = INTVAL (source) >> 32;

      if (destination_section != source_section)
	return 1;
    }

  return 0;
}


static int
pdp10_address_base_offset (x, basep, offp)
     rtx x;
     rtx *basep;
     HOST_WIDE_INT *offp;
{
  if (GET_CODE (x) == CONST)
    x = XEXP (x, 0);

  if (GET_CODE (x) == PLUS && GET_CODE (XEXP (x, 1)) == CONST_INT)
    {
      *basep = XEXP (x, 0);
      *offp = INTVAL (XEXP (x, 1));
      return 1;
    }

  *basep = x;
  *offp = 0;
  return 1;
}

static rtx
pdp10_normalize_block_word_address (addr, length, convert_units)
     rtx addr;
     HOST_WIDE_INT length;
     int convert_units;
{
  rtx x;
  rtx base;
  HOST_WIDE_INT offset;
  int was_const;

  was_const = 0;
  x = addr;
  if (GET_CODE (x) == CONST)
    {
      was_const = 1;
      x = XEXP (x, 0);
    }

  if (GET_CODE (x) != PLUS
      || GET_CODE (XEXP (x, 1)) != CONST_INT)
    return addr;

  base = XEXP (x, 0);
  offset = INTVAL (XEXP (x, 1));

  /* BLKmode copy lengths are expressed in 9-bit C storage units, but
     BLT source/destination addresses are 36-bit word addresses.  Generic
     ARRAY_REF expansion can leave an element-boundary stack address in
     storage-unit form as well as for symbolic globals, e.g. a four-word
     struct array element can arrive as BASE+32 instead of BASE+8.

     Normalize only exact positive element-boundary offsets.  Ordinary
     stack temporaries and already-normalized word offsets, such as BASE+12
     for a four-word scratch slot, are left unchanged.  */
  if (convert_units
      && length > 0
      && offset >= length
      && offset % length == 0
      && offset % UNITS_PER_WORD == 0)
    offset /= UNITS_PER_WORD;

  if (was_const)
    offset &= ADDRESS_MASK;

  return plus_constant (base, offset);
}

int
pdp10_word_address_plus_one_p (first, second)
     rtx first;
     rtx second;
{
  rtx first_base;
  rtx second_base;
  HOST_WIDE_INT first_off;
  HOST_WIDE_INT second_off;

  if (!pdp10_address_base_offset (first, &first_base, &first_off)
      || !pdp10_address_base_offset (second, &second_base, &second_off))
    return 0;

  return (rtx_equal_p (first_base, second_base)
          && second_off == first_off + 1);
}

int
pdp10_normalize_blt_source_p (src, dest, end, outp)
     rtx src;
     rtx dest;
     rtx end;
     rtx *outp;
{
  rtx x;
  rtx src_base;
  rtx dst_base;
  rtx end_base;
  HOST_WIDE_INT src_off;
  HOST_WIDE_INT dst_off;
  HOST_WIDE_INT end_off;
  HOST_WIDE_INT words;
  HOST_WIDE_INT length;
  rtx out;

  if (GET_CODE (src) != CONST
      || GET_CODE (XEXP (src, 0)) != PLUS
      || GET_CODE (XEXP (XEXP (src, 0), 1)) != CONST_INT)
    return 0;

  x = XEXP (src, 0);
  src_base = XEXP (x, 0);
  src_off = INTVAL (XEXP (x, 1));

  if (!pdp10_address_base_offset (dest, &dst_base, &dst_off)
      || !pdp10_address_base_offset (end, &end_base, &end_off))
    return 0;

  if (!rtx_equal_p (dst_base, end_base))
    {
      if (REG_P (end)
          && pdp10_frame_address_p (dst_base)
          && dst_off < 0)
        end_off = 0;
      else
        return 0;
    }

  words = end_off - dst_off + 1;
  if (words <= 0)
    return 0;

  length = words * UNITS_PER_WORD;
  if (src_off < length
      || src_off % length != 0
      || src_off % UNITS_PER_WORD != 0)
    return 0;

  out = plus_constant (src_base, src_off / UNITS_PER_WORD);
  if (rtx_equal_p (out, src))
    return 0;

  if (outp)
    *outp = out;
  return 1;
}

int
pdp10_normalize_blt_source_for_output (insn, src, dest, outp)
     rtx insn;
     rtx src;
     rtx dest;
     rtx *outp;
{
  rtx next;
  rtx pat;
  rtx use;
  rtx end;
  rtx scan;
  int i;

  next = next_nonnote_insn (insn);
  if (next == 0
      || GET_CODE (next) != INSN
      || GET_CODE (PATTERN (next)) != PARALLEL)
    return 0;

  pat = PATTERN (next);
  if (XVECLEN (pat, 0) < 3)
    return 0;

  use = XVECEXP (pat, 0, 2);
  if (GET_CODE (use) != USE
      || GET_CODE (XEXP (use, 0)) != MEM)
    return 0;

  end = XEXP (XEXP (use, 0), 0);

  /* Some BLTs keep the end address in a register already computed before
     the BLT setup.  Recover a nearby simple SET chain so source
     normalization can use the real destination range instead of guessing
     from the raw register.  The old reload output often computes this as
     MOVE end,SP followed by ADD end,const.  */
  if (REG_P (end))
    {
      rtx end_reg;
      HOST_WIDE_INT end_offset;

      end_reg = end;
      end_offset = 0;
      scan = prev_nonnote_insn (insn);
      for (i = 0; scan != 0 && i < 20; scan = prev_nonnote_insn (scan), i++)
        {
          rtx set;
          rtx rhs;

          if (GET_CODE (scan) == JUMP_INSN
              || GET_CODE (scan) == CALL_INSN
              || GET_CODE (scan) == CODE_LABEL)
            break;

          if (GET_CODE (scan) != INSN
              || GET_CODE (PATTERN (scan)) != SET
              || !REG_P (SET_DEST (PATTERN (scan)))
              || REGNO (SET_DEST (PATTERN (scan))) != REGNO (end_reg))
            continue;

          set = PATTERN (scan);
          rhs = SET_SRC (set);
          if (GET_CODE (rhs) == PLUS
              && REG_P (XEXP (rhs, 0))
              && REGNO (XEXP (rhs, 0)) == REGNO (end_reg)
              && GET_CODE (XEXP (rhs, 1)) == CONST_INT)
            {
              end_offset += INTVAL (XEXP (rhs, 1));
              continue;
            }

          if (GET_CODE (rhs) == PLUS
              && GET_CODE (XEXP (rhs, 1)) == CONST_INT)
            {
              end = plus_constant (XEXP (rhs, 0),
                                   INTVAL (XEXP (rhs, 1)) + end_offset);
              break;
            }

          if (REG_P (rhs) || GET_CODE (rhs) == SYMBOL_REF)
            {
              end = plus_constant (rhs, end_offset);
              break;
            }

          end = rhs;
          break;
        }
    }

  return pdp10_normalize_blt_source_p (src, dest, end, outp);
}

/* Try to emit instructions to BLT a memory block of LENGTH storage
   units from SOURCE to DESTINATION.  Return nonzero on success.  */
static int
expand_blt (destination, source, length)
     rtx destination;
     rtx source;
     rtx length;
{
  HOST_WIDE_INT words;
  HOST_WIDE_INT last;
  rtx ac, e;

  if (GET_CODE (length) != CONST_INT
      || INTVAL (length) % UNITS_PER_WORD != 0)
    return 0;

  words = INTVAL (length) / UNITS_PER_WORD;
  if (words == 0)
    return 1;
  last = words - 1;

  ac = gen_reg_rtx (Pmode);

  /* Build the BLT AC as one target operation.  Do not use a loose
     MOVEI;HRLI pair here: when SOURCE is a register, the halfword insert
     can be rewritten as HRLZ, which zeroes the destination half.  That
     makes BLT copy to address zero and can clobber the AC/low-core area.  */
  emit_insn (gen_pdp10_blt_setup (ac, source, destination));

  if (CONSTANT_ADDRESS_P (destination))
    e = plus_constant (destination, last);
  else
    {
      e = gen_reg_rtx (Pmode);
      emit_insn (gen_addsi3 (e, destination, GEN_INT (last)));
    }

  /* Beginning at the location addressed by the left half-word of AC,
     move words to another area beginning at the location addressed by
     the right half-word of AC.  Continue until a word is moved to
     location E.  */
  emit_insn (gen_BLT (ac, gen_rtx_MEM (BLKmode, e)));

  return 1;
}

/* Try to emit instructions to XBLT a memory block of LENGTH storage
   units from SOURCE to DESTINATION.  Return nonzero on success.  */
static int
expand_xblt (destination, source, length)
     rtx destination;
     rtx source;
     rtx length;
{
  rtx temp, mem, acs;
  int i, n;

  if (GET_CODE (length) != CONST_INT)
    return 0;

  n = INTVAL (length);

  switch (n / UNITS_PER_WORD)
    {
    case 0:
      break;

    case 2:
    case 3:
      temp = gen_reg_rtx (DImode);
      for (i = 0; i < n / 8; i++)
	{
	  emit_move_insn (temp, gen_rtx_MEM (DImode,
					     plus_constant (source, i)));
	  emit_move_insn (gen_rtx_MEM (DImode,
				       plus_constant (destination, i)), temp);
	}
      /* Fall through.  */

    case 1:
      if (n % 8 >= 4)
	{
	  int m = 2 * (n / 8);
	  temp = gen_reg_rtx (SImode);
	  emit_move_insn (temp, gen_rtx_MEM (SImode,
					     plus_constant (source, m)));
	  emit_move_insn (gen_rtx_MEM (SImode,
				       plus_constant (destination, m)),
			  temp);
	}
      break;

    default:
      acs = gen_reg_rtx (TImode);
      emit_move_insn (gen_rtx_SUBREG (SImode, acs, 0),
		      GEN_INT (INTVAL (length) / UNITS_PER_WORD));
      emit_move_insn (gen_rtx_SUBREG (Pmode, acs, 4), source);
      emit_move_insn (gen_rtx_SUBREG (Pmode, acs, 8), destination);
#if 0
      emit_insn (gen_XBLT (gen_rtx_SUBREG (SImode, acs, 0),
			   gen_rtx_SUBREG (Pmode, acs, 4),
			   gen_rtx_SUBREG (Pmode, acs, 8)));
#else
      emit_insn (gen_XBLT (acs));
#endif
      break;
    }

  switch (n % UNITS_PER_WORD)
    {
    case 0:
      break;

    case 1:
      temp = gen_reg_rtx (QImode);
      mem = gen_rtx_MEM (QImode, plus_constant (source, n / 4));
      set_mem_align (mem, BITS_PER_WORD);
      emit_move_insn (temp, mem);
      mem = gen_rtx_MEM (QImode, plus_constant (destination, n / 4));
      set_mem_align (mem, BITS_PER_WORD);
      emit_move_insn (mem, temp);
      break;

    case 2:
      temp = gen_reg_rtx (HImode);
      mem = gen_rtx_MEM (HImode, plus_constant (source, n / 4));
      set_mem_align (mem, BITS_PER_WORD);
      emit_move_insn (temp, mem);
      mem = gen_rtx_MEM (HImode, plus_constant (destination, n / 4));
      set_mem_align (mem, BITS_PER_WORD);
      emit_move_insn (mem, temp);
      break;

    case 3:
      temp = gen_reg_rtx (SImode);
      emit_insn (gen_extzv (temp, gen_rtx_MEM (SImode,
					       plus_constant (source, n / 4)),
			    GEN_INT (27), GEN_INT (0)));
      emit_insn (gen_insv (gen_rtx_MEM (SImode,
					plus_constant (destination, n / 4)),
			   GEN_INT (27), GEN_INT (0), temp));
      break;
    }

  return 1;
}

#if TARGET_STRING
/* Try to emit instructions to MOVSLJ a memory block of LENGTH storage
   units from SOURCE to DESTINATION.  Return nonzero on success.  */
static int
expand_movslj (destination, source, length)
     rtx destination;
     rtx source;
     rtx length;
{
  abort ();
}
#endif

/* Emit instructions to copy a memory block.  Return nonzero on
   success.  */
int
pdp10_expand_movstrsi (operands)
     rtx *operands;
{
  /* operands[0] is the block destination address
     operands[1] is the block source address
     operands[2] is the block length
     operands[3] is the known alignment.  */
  rtx raw_destination;
  rtx raw_source;

#if TARGET_STRING
  if (HAVE_MOVSLJ && TARGET_STRING)
    return expand_movslj (operands[0], operands[1], operands[2]);
#endif

  operands[0] = XEXP (operands[0], 0);
  operands[1] = XEXP (operands[1], 0);
  raw_destination = operands[0];
  raw_source = operands[1];

  if (GET_CODE (operands[3]) != CONST_INT
      || INTVAL (operands[3]) != UNITS_PER_WORD)
    return 0;

  if (GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) > 0
      && INTVAL (operands[2]) % UNITS_PER_WORD == 0)
    {
      HOST_WIDE_INT length = INTVAL (operands[2]);

      operands[0] = pdp10_normalize_block_word_address (operands[0],
                                                        length, 1);
      operands[1] = pdp10_normalize_block_word_address (operands[1],
                                                        length, 1);

      /* Structure-return temporaries can be allocated at a real stack word
	 offset equal to the C-storage-unit size of the destination object.
	 The generic BLKmode copy then looks like SRC=fp+48, DST=fp+12,
	 LEN=48 for a twelve-word record.  That source offset is already a
	 word address; converting it would create a false self-copy from
	 fp+12 to fp+12 and discard the returned aggregate.  If source
	 normalization collapses a distinct raw stack source onto the
	 destination, keep the raw source.  */
      if (!rtx_equal_p (raw_source, raw_destination)
	  && rtx_equal_p (operands[1], operands[0]))
	operands[1] = raw_source;
    }
  else
    operands[1] = pdp10_normalize_block_word_address (operands[1], 0, 0);

  if (use_xblt (operands[0], operands[1], operands[2]))
    return expand_xblt (operands[0], operands[1], operands[2]);
  else
    return expand_blt (operands[0], operands[1], operands[2]);
}

/* Emit instructions to use BLT to clear a memory block of LENGTH
   storage units from SOURCE to DESTINATION.  Return nonzero on
   success.  */
static int
expand_blt_clear (destination, length)
     rtx destination;
     rtx length;
{
  HOST_WIDE_INT words;
  HOST_WIDE_INT last;
  rtx ac, e;

  if (GET_CODE (length) != CONST_INT
      || INTVAL (length) % UNITS_PER_WORD != 0)
    return 0;

  words = INTVAL (length) / UNITS_PER_WORD;
  if (words == 0)
    return 1;
  last = words - 1;

  ac = gen_reg_rtx (Pmode);

  /* Build the clear BLT AC with the same protected setup used by
     expand_blt: source starts at DESTINATION and destination starts at
     DESTINATION+1.  */
  emit_insn (gen_pdp10_blt_setup (ac,
				  destination,
				  plus_constant (destination, 1)));

  if (CONSTANT_ADDRESS_P (destination))
    e = plus_constant (destination, last);
  else
    {
      e = gen_reg_rtx (Pmode);
      emit_insn (gen_addsi3 (e, destination, GEN_INT (last)));
    }

  emit_move_insn (gen_rtx_MEM (SImode, destination), const0_rtx);

  /* Beginning at the location addressed by the left half-word of AC,
     move words to another area beginning at the location addressed by
     the right half-word of AC.  Continue until a word is moved to
     location E.  */
  emit_insn (gen_BLT (ac, gen_rtx_MEM (BLKmode, e)));

  return 1;
}

/* Emit instructions to use XBLT to clear a memory block of LENGTH
   storage units from SOURCE to DESTINATION.  Return nonzero on
   success.  */
static int
expand_xblt_clear (destination, length)
     rtx destination;
     rtx length;
{
  rtx temp, acs;
  int i, n;

  if (GET_CODE (length) != CONST_INT)
    return 0;

  n = INTVAL (length);

  switch (n / UNITS_PER_WORD)
    {
    case 0:
      break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      for (i = 0; i < n / UNITS_PER_WORD; i++)
	emit_move_insn (gen_rtx_MEM (SImode, plus_constant (destination, i)),
			const0_rtx);
      break;

    default:
      acs = gen_reg_rtx (TImode);
      emit_move_insn (gen_rtx_SUBREG (SImode, acs, 0),
		      GEN_INT (n / UNITS_PER_WORD - 1));
      emit_move_insn (gen_rtx_SUBREG (Pmode, acs, 4), destination);
      destination = force_reg (Pmode, destination);
      emit_insn (gen_MOVEI (gen_rtx_SUBREG (Pmode, acs, 8),
			    plus_constant (destination, 1)));
      emit_move_insn (gen_rtx_MEM (SImode, destination), const0_rtx);
#if 0
      emit_insn (gen_XBLT (gen_rtx_SUBREG (SImode, acs, 0),
			   gen_rtx_SUBREG (Pmode, acs, 4),
			   gen_rtx_SUBREG (Pmode, acs, 8)));
#else
      emit_insn (gen_XBLT (acs));
#endif
    }

  switch (n % UNITS_PER_WORD)
    {
    case 0:
      break;

    case 1:
      temp = gen_rtx_MEM (QImode, plus_constant (destination, n / 4));
      set_mem_align (temp, BITS_PER_WORD);
      emit_move_insn (temp, const0_rtx);
      break;

    case 2:
      temp = gen_rtx_MEM (HImode, plus_constant (destination, n / 4));
      set_mem_align (temp, BITS_PER_WORD);
      emit_move_insn (temp, const0_rtx);
      break;

    case 3:
      temp = gen_reg_rtx (SImode);
      emit_move_insn (temp, GEN_INT (0777));
      emit_insn (gen_andsi3 (gen_rtx_MEM (SImode,
					  plus_constant (destination, n / 4)),
			     gen_rtx_MEM (SImode,
					  plus_constant (destination, n / 4)),
			     temp));
      break;
    }

  return 1;
}

#if TARGET_STRING
/* Emit instructions to use MOVST to clear a memory block of LENGTH
   storage units from SOURCE to DESTINATION.  Return nonzero on
   success.  */
static int
expand_movst_clear (destination, length)
     rtx destination;
     rtx length;
{
  abort ();
}
#endif

/* Emit instructions to clear a memory block.  Return nonzero on
   success.  */
int
pdp10_expand_clrstrsi (operands)
     rtx *operands;
{
  /* operands[0] is the block destination address
     operands[1] is the block length
     operands[2] is the known alignment.  */

#if TARGET_STRING
  if (HAVE_MOVST && TARGET_STRING)
    return expand_movst_clear (operands[0], operands[1]);
#endif

  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) != UNITS_PER_WORD)
    return 0;

  operands[0] = XEXP (operands[0], 0);

  if (use_xblt (operands[0], NULL_RTX, operands[1]))
    return expand_xblt_clear (operands[0], operands[1]);
  else
    return expand_blt_clear (operands[0], operands[1]);
}

#if TARGET_STRING
/* Emit instructions to compare two memory blocks.  Return nonzero on
   success.  */
int
pdp10_expand_cmpstrsi (operands)
     rtx *operands ATTRIBUTE_UNUSED;
{
  /* operands[0] is the result
     operands[1] is the address of the first string
     operands[2] is the address of the second string
     operands[3] is the string length
     operands[4] is the known alignmen.  */

  if (!HAVE_CMPS || !TARGET_STRING)
    return 0;

  abort ();
}
#endif


/**********************************************************************

	Pointer Arithmetic

**********************************************************************/

static void	emit_cmp_and_jump_insn_1 PARAMS ((rtx x, rtx y,
					  enum machine_mode mode,
					  enum rtx_code comparison,
					  int unsignedp, rtx label));
static rtx	expand_adjust_byte_pointer PARAMS ((tree type, rtx target,
						    rtx op1, rtx op2));
static void	avoid_reg_equal PARAMS ((rtx op0, rtx op1));
static void	pdp10_emit_ibp1 PARAMS ((rtx op0, rtx op1));
static int	maybe_expand_ibp PARAMS ((int byte_size, rtx op0, rtx op1,
					  int increment));
static rtx	expand_add_or_sub_pointer PARAMS ((rtx target, rtx op0,
						   tree type0, rtx op1,
						   tree type1, int add));
static rtx	expand_subtract_pointers PARAMS ((enum machine_mode mode,
						  rtx target, rtx op0,
						  tree type0, rtx op1,
						  tree type1));

/* Return the value of the S field in byte pointers used to point to
   variables of type TYPE.  A return value of 36 means that a word
   pointer should be used.  */
int
pdp10_bytesize (type)
     tree type;
{
  int size;

  while (TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);

  if (TREE_CODE (type) == RECORD_TYPE)
    {
      tree field;
      int field_size;
      int best_size = 0;

      /* A record containing any word-sized field is addressed as a
         word object.  If all fields are PDP-10 byte objects, keep the
         largest field byte size as the record pointer byte size.

         This is needed not only for explicitly packed records such as
         struct { char9 x[3]; }, but also for ordinary small records
         that fit into a single word, e.g. struct { short a; char b; }.
         Their local addresses must carry the local byte-pointer P,S
         field; treating them as plain word pointers loses that state.  */
      for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
	{
	  if (TREE_CODE (field) != FIELD_DECL)
	    continue;

	  field_size = pdp10_bytesize (TREE_TYPE (field));
	  if (field_size >= 36)
	    return 36;
	  if (field_size > best_size)
	    best_size = field_size;
	}

      return best_size ? best_size : 36;
    }

  if (VOID_TYPE_P (type))
    size = 9;
  else
    size = tree_low_cst (TYPE_SIZE (type), 1);
  if (size > 36)
    size = 36;

  /* Do not collapse explicit 16-bit PDP-10 byte types into 18-bit
     halfword pointer types.  short16 arrays are stored with POINT 16
     fields, and pointer arithmetic must therefore use 16-bit byte
     pointers/IBP stepping.  short18 remains the normal halfword case.  */
  return size == 32 ? 36 : size;
}

/* Emit a dummy move instruction which will be optimized away later.
   The purpose of this instruction is to fool GCC into not generating
   a REG_EQUAL note for a previously emitted (sequence of)
   instruction(s).

   OP0 and OP1 are two operands that, if convenient, can be used in
   the dummy move.  */
static void
avoid_reg_equal (op0, op1)
     rtx op0;
     rtx op1;
{
  rtx temp;

  if (GET_CODE (op0) == REG)
    temp = op0;
  else if (GET_CODE (op1) == REG)
    temp = op1;
  else
    temp = gen_reg_rtx (SImode);

  emit_move_insn (temp, temp);
}

/* Emit instructions, without ADJBP, to adjust the byte pointer in OP0
   by OP1 units, and store the result in TARGET if that's convenient.
   Return the result.  */
static rtx
expand_adjust_byte_pointer (type, target, op0, op1)
     tree type;
     rtx target;
     rtx op0;
     rtx op1;
{
  int bytes_per_word;
  int byte_size;
  int shift;
  int i;
  rtx words, rem;
  rtx loop_label, nonpos_label, neg_loop_label, done_label;

  byte_size = pdp10_bytesize (type);
  bytes_per_word = BITS_PER_WORD / byte_size;

  /* This is the non-KL10up fallback for dynamic byte-pointer
     adjustment.  KL10up targets use ADJBP instead.

     For power-of-two byte counts per word, decompose the byte count N
     as:

       words = floor (N / bytes_per_word)
       rem   = N mod bytes_per_word

     using an arithmetic right shift and a low-bit mask.  This is also
     correct for negative N in two's complement; for example, with
     9-bit bytes, -1 becomes words = -1 and rem = 3, i.e. back one
     word and then IBP forward three byte positions.

     Non-power-of-two byte counts, such as 6-bit and 7-bit PDP-10 byte
     pointers, cannot use a cheap shift/mask split.  Do those with a
     correctness-first byte-step loop.  This avoids depending on IDIV
     while the integer division patterns are still being stabilized.  */
  switch (bytes_per_word)
    {
    case 2:
      shift = 1;
      break;
    case 4:
      shift = 2;
      break;
    default:
      shift = 0;
      break;
    }

  if (!target || !register_operand (target, Pmode)
      || reg_overlap_mentioned_p (target, op1))
    target = gen_reg_rtx (Pmode);

  if (!general_operand (op0, Pmode))
    op0 = force_reg (Pmode, op0);
  if (!register_operand (op1, Pmode))
    op1 = force_reg (Pmode, op1);

  emit_move_insn (target, op0);

  if (shift != 0)
    {
      words = gen_reg_rtx (Pmode);
      rem = gen_reg_rtx (Pmode);
      emit_move_insn (words, op1);
      emit_move_insn (rem, op1);

      emit_insn (gen_andsi3 (rem, rem, GEN_INT (bytes_per_word - 1)));
      emit_insn (gen_ashrsi3_pointer (words, words, GEN_INT (shift)));
      emit_insn (gen_addsi3 (target, target, words));

      loop_label = gen_label_rtx ();
      done_label = gen_label_rtx ();

      emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, EQ, 0,
				done_label);
      emit_label (loop_label);
      pdp10_emit_ibp1 (target, target);
      emit_insn (gen_addsi3 (rem, rem, constm1_rtx));
      emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, NE, 0,
				loop_label);
      emit_label (done_label);

      return target;
    }

  /* Slow but general fallback for 5-, 6-, and other non-power-of-two
     byte counts per word.  Positive N is just N applications of IBP.

     Negative N is implemented as repeated one-byte decrements.  A
     single one-byte decrement is:
       word address -= 1
       IBP byte_pointer, repeated bytes_per_word - 1 times
     which lands on the previous byte while preserving the byte-pointer
     P/S fields.  */
  rem = gen_reg_rtx (Pmode);
  emit_move_insn (rem, op1);

  loop_label = gen_label_rtx ();
  nonpos_label = gen_label_rtx ();
  neg_loop_label = gen_label_rtx ();
  done_label = gen_label_rtx ();

  emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, LE, 0,
			    nonpos_label);

  emit_label (loop_label);
  pdp10_emit_ibp1 (target, target);
  emit_insn (gen_addsi3 (rem, rem, constm1_rtx));
  emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, GT, 0,
			    loop_label);

  emit_label (nonpos_label);
  emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, EQ, 0,
			    done_label);

  emit_label (neg_loop_label);
  emit_insn (gen_addsi3 (target, target, constm1_rtx));
  for (i = 1; i < bytes_per_word; i++)
    pdp10_emit_ibp1 (target, target);
  emit_insn (gen_addsi3 (rem, rem, GEN_INT (1)));
  emit_cmp_and_jump_insn_1 (rem, CONST0_RTX (SImode), SImode, LT, 0,
			    neg_loop_label);

  emit_label (done_label);

  return target;
}

/* Emit a one-byte byte-pointer increment.  GCC's virtual-register
   instantiation can fail on a matching memory operand like

      (set (mem (plus (virtual-stack-vars) C))
           (unspec [(mem (plus (virtual-stack-vars) C)) 1] UNSPEC_ADJBP))

   because it validates the rewritten destination before the matching source
   has been rewritten.  A register temporary is equivalent to IBP memory for
   ordinary compiler-generated byte-pointer variables, and leaves reload with
   simple RTL.  */
static void
pdp10_emit_ibp1 (op0, op1)
     rtx op0;
     rtx op1;
{
  if (GET_CODE (op0) == MEM)
    {
      rtx tmp;

      if (!reg_or_mem_operand (op1, Pmode))
	op1 = force_reg (Pmode, op1);

      tmp = gen_reg_rtx (Pmode);
      emit_move_insn (tmp, op1);
      emit_insn (gen_IBP (tmp, tmp));
      emit_move_insn (op0, tmp);
      return;
    }

  emit_insn (gen_IBP (op0, op1));
}

/* Try to emit instructions to use IBP to add INCREMENT to OP1 and
   store the result in OP0.  Return nonzero on success.  */
static int
maybe_expand_ibp (byte_size, op0, op1, increment)
     int byte_size;
     rtx op0;
     rtx op1;
     int increment;
{
  if (increment == 0)
    return 1;

  if (TARGET_KL10up && increment != 1)
    return 0;

  {
    int bytes_per_word;
    int addi, ibp;
    int i;

    bytes_per_word = BITS_PER_WORD / byte_size;

    if (increment > 0)
      addi = increment / bytes_per_word;
    else
      addi = (increment - bytes_per_word + 1) / bytes_per_word;
    ibp = increment - bytes_per_word * addi;
 
   if (addi != 0)
      {
        /* OP1 may be a constant local byte pointer.  Adding a whole-word
           offset to such a pointer must adjust only the 18-bit Y address
           field and must preserve the P,S selector bits.  A plain
           plus_constant/gen_addsi3 on the full 36-bit constant can borrow
           through the low address field and corrupt S, producing examples
           like POINT 5,foo-1,35 from POINT 6,foo,35 plus -1. */
        if (CONSTANT_P (op1) && pdp10_const_word36_byte_pointer_p (op1))
          {
            rtx base;
            pdp10_word36 offset;
            pdp10_word36 high;
            pdp10_word36 addr;
            rtx adjusted;

            if (!pdp10_decompose_symbolic_word36 (op1, &base, &offset))
              abort ();

            high = offset & ~((pdp10_word36) ADDRESS_MASK);
            addr = (offset + (pdp10_word36) addi) & (pdp10_word36) ADDRESS_MASK;
            offset = (high | addr) & PDP10_WORD36_MASK;

            if (base != NULL_RTX)
              adjusted = gen_rtx_CONST (Pmode,
                                        gen_rtx_PLUS (Pmode, base,
                                                      pdp10_gen_word36_constant (offset)));
            else
              adjusted = pdp10_gen_word36_constant (offset);

            emit_move_insn (op0, adjusted);
          }
        else
          {
            rtx tmp;

            /* This is byte-pointer arithmetic, not plain integer
               arithmetic.  A local PDP-10 byte pointer stores its byte
               selector in the left half and the word address in the right
               half.  A normal 36-bit ADDI/SUBI may borrow/carry through the
               18-bit Y field and corrupt the P,S selector, e.g. turning
               POINT 6,foo+1,35 plus -2 into POINT 5,foo-1,35.

               Preserve OP0's left half, compute the adjusted right half in a
               scratch register, then copy only that right half back.  */
            if (!rtx_equal_p (op0, op1))
              emit_move_insn (op0, op1);

            tmp = gen_reg_rtx (Pmode);
            emit_move_insn (tmp, op0);
            emit_insn (gen_addsi3 (tmp, tmp, GEN_INT (addi)));
            emit_insn (gen_HRR (op0, tmp));
          }
      }
    else
      {
	if (!reg_or_mem_operand (op1, Pmode))
	  op1 = force_reg (Pmode, op1);
	pdp10_emit_ibp1 (op0, op1);
	ibp--;
      }

    for (i = 0; i < ibp; i++)
      pdp10_emit_ibp1 (op0, op0);
  }

  return 1;
}

/* Emit instructions to add OP1 to the byte pointer in OP0 that points
   to items of type TYPE.  Store the result in TARGET if that's
   convenient.  Return the result.  */
rtx
pdp10_expand_addbp3 (type, target, op0, op1)
     tree type;
     rtx target;
     rtx op0;
     rtx op1;
{
  int byte_size = pdp10_bytesize (type);
  int bytes_per_word = 36 / byte_size;
  int units_per_byte = (byte_size + BITS_PER_UNIT - 1) / BITS_PER_UNIT;
  HOST_WIDE_INT increment;

  /* Some constant pointer adjustments arrive from GCC already scaled in
     addressable C storage units rather than PDP-10 byte-pointer units.  For
     short16/short18, TYPE_SIZE_UNIT is 2 while one array element occupies
     one PDP-10 byte.  Fold those constant multiples back to a PDP-10 byte
     count before doing IBP/ADJBP decomposition.

     Do not apply the same division blindly to dynamic OP1.  Dynamic ARRAY_REF
     expansion for short18 fields such as p->h[i] reaches this routine with
     OP1 as the element/byte index i, not as i * TYPE_SIZE_UNIT.  Shifting the
     dynamic value here makes the later halfword decomposition shift it a
     second time, so i=0 and i=1 address the same halfword.  The regression is
     visible as h[0] being read back as h[1].  */
  if (units_per_byte > 1 && GET_CODE (op1) == CONST_INT)
    {
      HOST_WIDE_INT n = INTVAL (op1);

      /* GCC's PDP-10 subword address paths are not uniform here.

         short18 direct address-taking has already been seen to reach this
         routine as a PDP-10 byte count: &a[2] must advance two halfwords,
         not one.  Keep the existing conservative rule for 18-bit
         halfwords.

         short16, however, reaches this path scaled in C storage units.
         A short16 object is two 9-bit chars but one PDP-10 16-bit byte.
         Leaving &a[1] as increment 2 makes the later bytes_per_word==2
         decomposition treat it as one whole word instead of one IBP.  */
      if (byte_size != 18)
        {
          if (n % units_per_byte == 0)
            op1 = GEN_INT (n / units_per_byte);
        }
    }

  /* First, try to use ADDI or SUBI.  On KS10/KL10-class machines,
     prefer ADJBP for non-one byte-pointer increments, even when the
     increment is a whole number of PDP-10 words.  Adding to the word
     address part of a byte pointer is only a local optimization; ADJBP
     is the canonical and fully general byte-pointer operation there.  */
  if (GET_CODE (op1) == CONST_INT)
    {
      increment = INTVAL (op1);

      if (!(TARGET_KL10up && increment != 0 && increment != 1)
	  && increment % bytes_per_word == 0)
	{
	  int n = increment / bytes_per_word;
	  if (CONSTANT_P (op0))
	    {
	      /* OP0 may be a symbolic local byte pointer.  Adjusting a
	         whole-word byte-pointer increment must change only the Y
	         address field.  A plain plus_constant on the whole 36-bit
	         byte pointer can borrow into the P/S field for negative
	         offsets, corrupting e.g. POINT 6,foo,35 + -1 into
	         POINT 5,foo-1,35.  */
	      if (pdp10_const_word36_byte_pointer_p (op0))
	        {
	          rtx base;
	          pdp10_word36 offset;
	          pdp10_word36 high;
	          pdp10_word36 addr;

	          if (!pdp10_decompose_symbolic_word36 (op0, &base, &offset))
	            abort ();

	          high = offset & ~((pdp10_word36) ADDRESS_MASK);
	          addr = (offset + (pdp10_word36) n) & (pdp10_word36) ADDRESS_MASK;
	          offset = (high | addr) & PDP10_WORD36_MASK;

	          return base != NULL_RTX
	            ? pdp10_plus_constant_word36 (base, offset)
	            : pdp10_gen_word36_constant (offset);
	        }
	      return plus_constant (op0, n);
	    }
	  else
	    emit_insn (gen_addsi3 (target, op0, GEN_INT (n)));
	  return target;
	}

      /* Second, try to use IBP.  For KS10/KL10 this deliberately
	 accepts only the single-byte case; larger constant adjustments
	 fall through to ADJBP below.  */
      if (maybe_expand_ibp (byte_size, target, op0, increment))
	return target;
    }

  /* If we get here, either the increment is not a constant, or a
     sequence of IBP instructions is too costly.  */

  if (TARGET_KL10up)
    {
      if (target == op0)
	target = gen_reg_rtx (Pmode);
      else
	target = force_reg (Pmode, target);
      if (!general_operand (op1, Pmode))
	op1 = force_reg (Pmode, op1);
      emit_move_insn (target, op1);
      if (!general_operand (op0, Pmode))
	op0 = force_reg (Pmode, op0);
      emit_insn (gen_ADJBP (target, op0, target));
      return target;
    }
  else
    {
      /* Emit sequence to adjust a byte pointer without ADJBP.  */
      return expand_adjust_byte_pointer (type, target, op0, op1);
    }
}

/* Emit instructions to add/substract to/from a pointer.  */
static rtx
expand_add_or_sub_pointer (target, op0, type0, op1, type1, add)
     rtx target;
     rtx op0;
     tree type0;
     rtx op1;
     tree type1;
     int add;
{
  int neg = 0;
  tree type;

  if (!add && GET_CODE (op0) == CONST_INT)
    {
      rtx tmp = GEN_INT (-INTVAL (op0));
      op0 = op1;
      op1 = tmp;
      neg = 1;
      add = 1;
    }

  if (!target)
    target = gen_reg_rtx (Pmode);

  type = TREE_TYPE (POINTER_TYPE_P (type0) ? type0 : type1);

  if (WORD_TYPE_P (type))
    {
      /* Add to a word pointer by first dividing the increment by the
         number of storage units per word, which is always four.
         Then, add the increment to the pointer.  */

      rtx increment;

      if (UNITS_PER_WORD != 4)
	abort ();

      if (GET_CODE (op1) == CONST_INT)
	{
	  HOST_WIDE_INT i = INTVAL (op1);
	  HOST_WIDE_INT sign = TARGET_EXTENDED ?
	    (HWINT (1) << 29) : (HWINT (1) << 17);

	  if (CONSTANT_ADDRESS_P (op0))
	    {
	      emit_move_insn (target, plus_constant (op0, add ? i : -i));
	      if (neg)
		target = gen_rtx_NEG (Pmode, target);
	      return target;
	    }

	  i &= ADDRESS_MASK;
	  i = (i ^ sign) - sign;
	  if (!add)
	    {
	      i = -i;
	      add = 1;
	    }
	  increment = GEN_INT (i);
	}
      else
	increment = op1;

      /* Make sure that the first operand is a register, if possible.  */
      if (add && GET_CODE (op0) != REG && GET_CODE (op1) == REG)
	{
	  rtx tmp = op0;
	  op0 = increment;
	  increment = tmp;
	}

      if (!register_operand (op0, Pmode))
	op0 = force_reg (Pmode, op0);
      if (!reg_or_mem_operand (increment, Pmode))
	increment = force_reg (Pmode, increment);

      if (add)
	emit_insn (gen_addsi3 (target, op0, increment));
      else
	emit_insn (gen_subsi3 (target, op0, increment));
    }
  else
    {
      /* Add to a byte pointer by first dividing the increment by the
         number of bytes per word, which depends on the byte size.
         Then, use an IBP or ADJBP instruction.  */

      rtx increment = op1;

      if (!add)
	{
	  if (GET_CODE (increment) == CONST_INT)
	    increment = GEN_INT (-INTVAL (increment));
	  else
	    {
	      rtx temp = gen_reg_rtx (Pmode);
	      emit_insn (gen_negsi2 (temp, increment));
	      increment = temp;
	    }
	}

      target = pdp10_expand_addbp3 (type, target, op0, increment);
    }

  /* This is to fool GCC into not generating a REG_EQUAL note for this
     sequence.  */
  avoid_reg_equal (op0, op1);

  if (neg)
    return expand_simple_unop (Pmode, NEG, target, NULL_RTX, 0);

  return target;
}

/* Emit instructions to subtract two byte pointers.  */
static rtx
expand_subtract_pointers (mode, target, op0, type0, op1, type1)
     enum machine_mode mode;
     rtx target;
     rtx op0;
     tree type0;
     rtx op1;
     tree type1 ATTRIBUTE_UNUSED;
{
  tree type = TREE_TYPE (type0);

  if (!target)
    target = gen_reg_rtx (Pmode);

  if (WORD_TYPE_P (type))
    {
      /* Calculate the difference between two word pointers by
	 subtracting and multiplying by the number of storage units
	 per word.  */

      rtx temp = gen_reg_rtx (Pmode);
      emit_insn (gen_subsi3 (temp, op0, op1));
      emit_insn (gen_ashlsi3_pointer (target, temp, GEN_INT (2)));
    }
  else
    {
      if (HAVE_SUBBP)
	{
	  if (!register_operand (op0, Pmode))
	    op0 = force_reg (Pmode, op0);
	  if (!reg_or_mem_operand (op1, Pmode))
	    op1 = force_reg (Pmode, op1);
	  emit_insn (gen_SUBBP (target, op0, op1));
	}
      else
	{
	  /* Calculate the difference between two byte pointers.

	     This algorithm is copied from KCC with some small
	     modifications.  Basically, the steps are:

		; Subtract the pointers.
		SUB op0,op1

		; Multiply the difference by the number of bytes per
		; word and a magic shift factor.
		MULI op0,<bytes per word>*<2^shift>

		; Shift back the lower word of the product to
		; compensate for the shift factor.  op0+1 will be an
		; approximation of the final result.
		ASH op0+1,-shift

		; Add a magic value from a table to adjust for the
		; difference in byte position.  The table is indexed
		; by op0 which is the upper bits of the result of
		; the multiplication, and thus depends on the byte
		; positions in the pointers.
		ADD op0+1,table(op0)

	     The result is in op0+1.  */

	  int bytesize = pdp10_bytesize (type);
	  rtx temp = gen_reg_rtx (DImode), diff;
	  rtx a = gen_rtx_SUBREG (SImode, temp, 0);
	  char *str;
	  int shift, mul;
	  rtx sym, mem;

	  if (bytesize == 16 && !TARGET_EXTENDED)
	    {
	      /* Local PDP-10 16-bit byte pointers have two byte positions per
	         word, but their P/S selectors are not the same as 18-bit
	         halfword pointers.  Reusing the %BADLH KCC table corrupts the
	         high bits of the result.  For the local PDP-6/KA10 case, compute
	         the element difference directly:

	           2 * (Y0 - Y1) + pos0 - pos1

	         where pos is 0 for POINT 16,Y,15 and 1 for POINT 16,Y,31.
	         The 020000 left-half bit is set in the first selector and clear
	         in the second.  */
	      rtx y0 = gen_reg_rtx (Pmode);
	      rtx y1 = gen_reg_rtx (Pmode);
	      rtx bits = gen_reg_rtx (Pmode);
	      rtx lab;

	      if (GET_MODE (op0) != Pmode)
	        op0 = convert_to_mode (Pmode, op0, 1);
	      if (GET_MODE (op1) != Pmode)
	        op1 = convert_to_mode (Pmode, op1, 1);
	      if (!register_operand (op0, Pmode))
	        op0 = force_reg (Pmode, op0);
	      if (!register_operand (op1, Pmode))
	        op1 = force_reg (Pmode, op1);

	      emit_insn (gen_HRRZ (y0, op0));
	      emit_insn (gen_HRRZ (y1, op1));
	      emit_insn (gen_subsi3 (target, y0, y1));
	      emit_insn (gen_ashlsi3_pointer (target, target, GEN_INT (1)));

	      emit_move_insn (bits, op0);
	      emit_insn (gen_andsi3 (bits, bits, GEN_INT (0200000000000)));
	      lab = gen_label_rtx ();
	      emit_cmp_and_jump_insn_1 (bits, CONST0_RTX (SImode), SImode, NE, 0, lab);
	      emit_insn (gen_addsi3 (target, target, GEN_INT (1)));
	      emit_label (lab);

	      emit_move_insn (bits, op1);
	      emit_insn (gen_andsi3 (bits, bits, GEN_INT (0200000000000)));
	      lab = gen_label_rtx ();
	      emit_cmp_and_jump_insn_1 (bits, CONST0_RTX (SImode), SImode, NE, 0, lab);
	      emit_insn (gen_addsi3 (target, target, GEN_INT (-1)));
	      emit_label (lab);

	      /* Return PDP-10 storage units here.  The generic pointer-difference
	         conversion that follows divides by TYPE_SIZE_UNIT for short16,
	         producing the C element count.  */
	      emit_insn (gen_ashlsi3_pointer (target, target, GEN_INT (1)));

	      goto subtract_done;
	    }

	  if (bytesize == 16)
	    bytesize = 18;

	  /* ADDRESS: the code differs slightly in extended and
	     unextended mode.  */

	  if (TARGET_EXTENDED)
	    {
	      if (bytesize == 18 || bytesize == 7)
		shift = 5;
	      else
		shift = 4;
	    }
	  else
	    shift = 1;
	  mul = (BITS_PER_WORD / bytesize) << shift;

	  str = xstrdup ("%BAD..");
	  str[4] = TARGET_EXTENDED ? 'X' : 'L';
	  str[5] = bytesize == 18 ? 'H' : '0' + bytesize;
	  sym = gen_rtx_SYMBOL_REF (Pmode, str);
	  used_byte_subtraction_table[bytesize] = 1;

	  diff = gen_reg_rtx (SImode);

	  if (GET_MODE (op0) != Pmode)
	    op0 = convert_to_mode (Pmode, op0, 1);
	  if (GET_MODE (op1) != Pmode)
	    op1 = convert_to_mode (Pmode, op1, 1);

	  emit_insn (gen_subsi3 (a, op0, op1));
	  emit_insn (gen_MUL (temp, a, GEN_INT (mul)));
	  emit_move_insn (diff, gen_rtx_SUBREG (SImode, temp, 4));
	  emit_insn (gen_ashrsi3 (diff, diff, GEN_INT (shift)));

	  if (TARGET_EXTENDED && !TARGET_SMALLISH)
	    {
	      emit_insn (gen_addsi3 (a, a, sym));
	      mem = gen_rtx_MEM (SImode, a);
	    }
	  else
	    mem = gen_rtx_MEM (SImode, gen_rtx_PLUS (Pmode, a, sym));

	  emit_insn (gen_addsi3 (diff, diff, mem));

	  if (bytesize == 18)
	    emit_insn (gen_ashlsi3_pointer (diff, diff, GEN_INT (1)));

	  emit_move_insn (target, diff);
	subtract_done: ;
	}
    }

  /* This is to fool GCC into not generating a REG_EQUAL note for this
     sequence.  */
  avoid_reg_equal (op0, op1);

  if (GET_MODE (target) != mode)
    target = convert_to_mode (mode, target, 0);

  return target;
}


/**********************************************************************

	Unconditional Jumps

**********************************************************************/

/* Return nonzero if the call instruction can be output as JRST.  */
int
pdp10_output_call_as_jrst_p (insn)
     rtx insn;
{
  return SIBLING_CALL_P (insn) || find_reg_note (insn, REG_NORETURN, 0);
}

int
pdp10_output_call_as_jsp_p (sym)
     rtx sym;
{
  tree id = maybe_get_identifier (XSTR (sym, 0));
  tree fn, type, attr;

  return
    id != NULL_TREE
    && (fn = IDENTIFIER_GLOBAL_VALUE (id)) != NULL_TREE
    && TREE_CODE (TREE_TYPE (fn)) == FUNCTION_DECL
    && (type = TREE_TYPE (fn)) != NULL_TREE
    && (attr = TYPE_ATTRIBUTES (type)) != NULL_TREE
    && lookup_attribute ("fastcall", attr);
}


/**********************************************************************

	Conditional Jumps

**********************************************************************/

static int	jrst_or_popj_p PARAMS ((rtx insn));
static rtx	jump_target PARAMS ((rtx insn));
static int	large_jump_address_p PARAMS ((rtx address));

int
pdp10_generate_cbranchdi (code, op0, op1)
     enum rtx_code code;
     rtx op0, op1;
{
  return 0;
}

/* Will this instruction be output as a JRST or a POPJ?  */
static int
jrst_or_popj_p (insn)
     rtx insn;
{
  /* A call instruction output as JRST is ok.  */
  if (GET_CODE (insn) == CALL_INSN && pdp10_output_call_as_jrst_p (insn))
    return 1;

  /* An unconditional jump without side effects is ok.  */
  if (GET_CODE (insn) == JUMP_INSN && onlyjump_p (insn)
      /* Note that any_uncondjump_p doesn't work here, since it only
	 recognizes jumps to labels.  */
      && !any_condjump_p (insn))
    return 1;

  /* A return instruction is ok.  */
  if (returnjump_p (insn))
    return 1;

  return 0;
}

static rtx
jump_target (insn)
     rtx insn;
{
  rtx set = pc_set (insn);
  rtx call;

  if (set)
    return SET_SRC (set);

  if (GET_CODE (insn) != CALL_INSN)
    return NULL_RTX;

  call = PATTERN (insn);
  if (GET_CODE (call) == SET)
    call = SET_SRC (call);

  if (GET_CODE (call) != CALL)
    abort ();
  if (GET_CODE (XEXP (call, 0)) != MEM)
    abort ();

  return XEXP (XEXP (call, 0), 0);
}

/* Return nonzero if the address in the jump instruction requires an
   additional (assembler literal) word.  */
static int
large_jump_address_p (address)
     rtx address;
{
  if (!TARGET_EXTENDED)
    return 0;

  /* Bail out and return 0 for now.  */
  return 0;
}

/* Output a JRST instruction, unless the jump target is another
   unconditional jump instruction (i.e. JRST or POPJ).  In that case
   output the target instruction directly (unless optimize_size is on
   and the result would be larger).  Return an empty string for the
   convenience of the caller.  */
const char *
pdp10_output_jrst (label)
     rtx label;
{
  rtx target = next_active_insn (label);
  rtx address;

  if (optimize >= 2
      && jrst_or_popj_p (target)
      && ((address = jump_target (target)) == NULL_RTX
	  || GET_CODE (address) != LABEL_REF
	  || XEXP (address, 0) != label)
      && !(optimize_size && large_jump_address_p (address)))
    {
      final_scan_insn (target, asm_out_file, optimize, 0, 0);
      maybe_remove_label_and_following_code (label);
    }
  else
    output_asm_insn ("jrst %F0", &label);

  return "";
}

/* Generate code to invert the sign bit of X and return the result.
   Called to prepare operands for an unsigned comparison.  */
rtx
pdp10_flip_sign_bit (x)
     rtx x;
{
  if (GET_CODE (x) == CONST_INT)
    return gen_int_mode (INTVAL (x) ^ SIGN_BIT, SImode);
  else
    {
      rtx y = gen_reg_rtx (SImode);
      if (GET_MODE (x) == DImode)
	x = gen_rtx_SUBREG (SImode, x, 0);
      else if (!register_operand (x, SImode))
	x = force_reg (SImode, x);
      emit_insn (gen_xorsi3 (y, x, gen_int_mode (SIGN_BIT, SImode)));
      return y;
    }
}

/* Return a suitable T[RLD]N[EN] instruction.  OPERANDS are the
   operands and LENGTH is the RTL insn length.  */
const char *
pdp10_output_test_and_skip (insn, operands)
     rtx insn;
     rtx *operands;
{
  static const char *asms[] =
  { "trn%0 %1,%2",
    "tln%0 %1,%S2",
    "tdn%0 %1,[%2]",
    "tdn%0 %1,%2" };
  int which_alternative;

  if (GET_CODE (operands[2]) == CONST_INT)
    {
      HOST_WIDE_INT i = INTVAL (operands[2]);

      if (CONST_OK_FOR_LETTER_P (i, 'I'))
	which_alternative = 0;
      else if (CONST_OK_FOR_LETTER_P (i, 'L'))
	which_alternative = 1;
      else
	which_alternative = 2;
    }
  else
    which_alternative = 3;

  if (get_attr_length (insn) == 1)
    return asms[which_alternative];
  else
    {
      /* Reverse the skip condition and output a JRST.  */
      operands[0] = gen_rtx_fmt_ee (reverse_condition (GET_CODE (operands[0])),
				    VOIDmode, NULL_RTX, NULL_RTX);
      output_asm_insn (asms[which_alternative], operands);
      return pdp10_output_jrst (operands[3]);
    }
}


/**********************************************************************

	Function Prologue and Epilogue

**********************************************************************/

#define emit_frame_insn(PATTERN) RTX_FRAME_RELATED_P (emit_insn (PATTERN)) = 1

static void	expand_save_or_restore_return_address PARAMS ((int save,
							       int clob_size));
static void	expand_save_or_restore_regs PARAMS ((int save,
						     int *clobbered,
						     int clobbered_size,
						     int single));
static void	expand_set_up_frame_pointer PARAMS ((void));

/* Return the number of clobbered registers that the current function
   should save on stack, and mark those registers in the CLOBBERED
   array.  */
static int
clobbered_regs (clobbered)
     int *clobbered;
{
  int i, n;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    clobbered[i] = regs_ever_live[i] && !call_used_regs[i];

  clobbered[STACK_POINTER_REGNUM] = 0;

  n = 0;
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (clobbered[i])
	n += UNITS_PER_WORD;
    }

  return n;
}

/* Emit instructions to save or restore the return address on stack.
   This only happens when the register arguments must be stored in the
   stack.  */
static void
expand_save_or_restore_return_address (save, clobbered_size)
     int save;
     int clobbered_size;
{
  int offset1, offset2;

  if (!current_function_pretend_args_size)
    return;

  /* If we need to store pretend args, move the return address out of
     their way.  */

  offset1 = - (current_function_outgoing_args_size +
	       get_frame_size () +
	       clobbered_size +
	       current_function_pretend_args_size) / UNITS_PER_WORD;
  offset2 = - (current_function_outgoing_args_size +
	       get_frame_size () +
	       clobbered_size) / UNITS_PER_WORD;

  if (!save)
    {
      int tmp = offset1;
      offset1 = offset2;
      offset2 = tmp;
    }

  emit_move_insn (gen_rtx_REG (SImode, 0),
		  GEN_MEM_SP_PLUS (SImode, offset1));
  emit_move_insn (GEN_MEM_SP_PLUS (SImode, offset2),
		  gen_rtx_REG (SImode, 0));
}

/* Save or restore clobbered callee-saved registers.  SAVE is nonzero
   when saving, and zero when restoring.  CLOBBERED indicates
   clobbered registers.  SINGLE is nonzero when a single PUSH or POP
   instruction can be used.  */
static void
expand_save_or_restore_regs (save, clobbered, clobbered_size, single)
     int save;
     int *clobbered;
     int clobbered_size;
     int single;
{
  int fp_clobbered = 0;
  int go_back = 1;
  int max_move;
  int min_blt;
  int offset;
  int regs;
  int i, j, k;

  if (clobbered_size == 0)
    return;

  offset = -((current_function_outgoing_args_size +
	      get_frame_size () +
	      clobbered_size)
	     / UNITS_PER_WORD) + 1;

  max_move = MOVE_MAX / UNITS_PER_WORD;

  /* ADDRESS: Don't use BLT when generating multi-section code.
     Otherwise, use BLT if using (D)MOVE instructions would generate
     more code.  */
  if (TARGET_EXTENDED)
    min_blt = 1000;
  else
    min_blt = 3 * max_move + 1;

  /* Start with the frame pointer.  */
  for (i = HARD_FRAME_POINTER_REGNUM; i < 16;
       (i == HARD_FRAME_POINTER_REGNUM && go_back) ? (i = go_back = 0) : (i += regs))
    {
      regs = 1;
      if (!clobbered[i])
	continue;

      /* If a single PUSH or POP instruction can be used, do so and
	 return.  */
      if (single && !TARGET_EXTENDED)
	{
	  rtx stack;
	  if (save)
	    {
	      stack = gen_rtx_PRE_INC (Pmode, stack_pointer_rtx);
	      emit_frame_insn (gen_PUSH (gen_rtx_MEM (SImode, stack),
					 gen_rtx_REG (SImode, i)));
	    }
	  else
	    {
	      stack = gen_rtx_POST_DEC (Pmode, stack_pointer_rtx);
	      emit_frame_insn (gen_POP (gen_rtx_REG (SImode, i),
					gen_rtx_MEM (SImode, stack)));
	    }
	  return;
	}

      /* Find a consecutive block of clobbered registers.

	 ADDRESS: When extended addressing is in use, the stack can
	 span multiple sections, so BLT can't be used.  In this case,
	 the search is limited to just examine the next register.  */
      if (i == HARD_FRAME_POINTER_REGNUM)
	{
	  k = i + 1;
	  fp_clobbered = 1;
	  clobbered[i] = 0;
	}
      else if (TARGET_EXTENDED
	  && !TARGET_SMALLISH
	  && i <= FIRST_PSEUDO_REGISTER - 2)
	k = i + 2;
      else
	k = FIRST_PSEUDO_REGISTER;

      for (j = i + 1; j < k; j++)
	{
	  if (clobbered[j])
	    regs++;
	  else
	    break;
	}
      
      if (regs >= min_blt)
	{
	  /* Use a BLT instruction.  */

	  rtx from_start, to_start, to_end;

	  if (save)
	    {
	      from_start = GEN_INT (i);
	      to_start = plus_constant (stack_pointer_rtx, offset);
	      to_end
		= GEN_MEM_SP_PLUS (BLKmode, offset + regs - 1);
	    }
	  else
	    {
	      from_start = plus_constant (stack_pointer_rtx, offset);
	      to_start = GEN_INT (i);
	      to_end
		= gen_rtx_MEM (BLKmode, GEN_INT (i + regs - 1));
	    }

#define REG0 gen_rtx_REG (Pmode, 0)
	  emit_frame_insn (gen_pdp10_blt_setup (REG0, from_start, to_start));
	  emit_frame_insn (gen_BLT (REG0, to_end));
#undef REG0
	}
      else
	{
	  /* Use a single MOVE or DMOVE instruction.  */

	  enum machine_mode mode;
	  rtx insn;

	  if (regs > max_move)
	    regs = max_move;
	  mode = (regs == 1 ? SImode : DImode);

	  if (save)
	    insn = emit_move_insn (GEN_MEM_SP_PLUS (mode, offset),
				   gen_rtx_REG (mode, i));
	  else
	    insn = emit_move_insn (gen_rtx_REG (mode, i),
				   GEN_MEM_SP_PLUS (mode, offset));
	  RTX_FRAME_RELATED_P (insn) = 1;
	}

      offset += regs;
    }

  clobbered[HARD_FRAME_POINTER_REGNUM] = fp_clobbered;
}

/* Emit an instruction to set up the frame pointer.  */
static void
expand_set_up_frame_pointer (void)
{
  int offset;

  if (!frame_pointer_needed)
    return;

  INITIAL_ELIMINATION_OFFSET (HARD_FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM,
			      offset);
  emit_frame_insn (gen_MOVEI (hard_frame_pointer_rtx,
			      plus_constant (stack_pointer_rtx, offset)));
}

/* Emit an instruction to adjust the stack pointer by an amount
   specified by ADJUST.  */
rtx
pdp10_gen_stack_adjust (adjust)
     rtx adjust;
{
  rtx insn;

  /* ADDRESS: Use addition in extended mode, ADJSP otherwise.  */
  if (TARGET_EXTENDED)
    {
      if (GET_CODE (adjust) == CONST_INT)
	insn = gen_MOVEI (stack_pointer_rtx,
			  gen_rtx_PLUS (Pmode, stack_pointer_rtx, adjust));
      else
	insn = gen_addsi3 (stack_pointer_rtx,
			   stack_pointer_rtx,
			   adjust);
    }
  else
    {
      insn = gen_ADJSP (stack_pointer_rtx, adjust);
    }

  return insn;
}

/* Emit function prologue instructions.  */
void
pdp10_expand_prologue ()
{
  int clobbered[FIRST_PSEUDO_REGISTER];
  int clobbered_size;
  int stack_adjust;
  int single_push;

  clobbered_size = clobbered_regs (clobbered);

  /* If only one word is to be saved on stack, it can be stored with a
     single PUSH instruction.  */
  single_push =
    clobbered_size == UNITS_PER_WORD
    && !TARGET_EXTENDED
    && current_function_outgoing_args_size == 0
    && current_function_pretend_args_size == 0
    && get_frame_size () == 0;

  stack_adjust =
    current_function_outgoing_args_size
    + get_frame_size ()
    + clobbered_size
    + current_function_pretend_args_size;
  if (!single_push && stack_adjust > 0)
    emit_frame_insn (pdp10_gen_stack_adjust (GEN_INT (stack_adjust / 4)));

  expand_save_or_restore_return_address (SAVE, clobbered_size);
  expand_save_or_restore_regs (SAVE, clobbered, clobbered_size, single_push);
  expand_set_up_frame_pointer ();
}

/* Emit function epilogue instructions.  Emit the final popj
   instruction only if POPJ is nonzero.  */
void
pdp10_expand_epilogue (popj)
     int popj;
{
  int clobbered[FIRST_PSEUDO_REGISTER];
  int clobbered_size;
  int stack_adjust;
  int single_pop;

  clobbered_size = clobbered_regs (clobbered);

  /* If only one word was saved on stack, it can be restored with a
     single POP instruction.  */
  single_pop =
    clobbered_size == UNITS_PER_WORD
    && !TARGET_EXTENDED
    && current_function_outgoing_args_size == 0
    && current_function_pretend_args_size == 0
    && get_frame_size () == 0;

  expand_save_or_restore_regs (RESTORE, clobbered, clobbered_size, single_pop);
  expand_save_or_restore_return_address (RESTORE, clobbered_size);

  stack_adjust =
    current_function_outgoing_args_size
    + get_frame_size ()
    + clobbered_size
    + current_function_pretend_args_size;
  if (!single_pop && stack_adjust > 0)
    emit_frame_insn (pdp10_gen_stack_adjust (GEN_INT (-stack_adjust / 4)));

  if (popj)
    emit_jump_insn (gen_POPJ_17 ());
}

const char *
pdp10_output_return ()
{
  if (lookup_attribute ("fastcall",
			TYPE_ATTRIBUTES (TREE_TYPE (current_function_decl))))
    return "jrst (?)";
  else
    return "popj 17,";
}


/**********************************************************************

	Unsorted

**********************************************************************/

static int	pdp10_normalize_pointer_bytesize PARAMS ((int size));
static int	pdp10_global_ps_for_conversion PARAMS ((int ps, int to,
							 int byte, int *fromp));
static int	pdp10_local_ps_for_conversion PARAMS ((int ps, int from,
						int to, int byte,
						int *psp, int *fromp));
static int	pdp10_initializer_type_bitsize PARAMS ((tree type,
						  pdp10_word36 *bitsp));
static int	pdp10_initializer_record_field_bitpos PARAMS ((tree type,
							    tree wanted,
							    pdp10_word36 *bitposp));
static int	pdp10_initializer_ref_bitpos PARAMS ((tree exp, tree *basep,
					       pdp10_word36 *bitposp));
static int	pdp10_dynamic_ref_bitpos PARAMS ((tree exp,
					      pdp10_word36 *bitposp));
static int	pdp10_addr_expr_array_byte PARAMS ((tree exp, int bytesize,
					       int *bytep));
static int	pdp10_addr_expr_array_position PARAMS ((tree exp, int bytesize,
					           int *bytep,
					           HOST_WIDE_INT *wordp));
static int	pdp10_dynamic_ref_base_bitpos PARAMS ((tree exp, tree *basep,
						    pdp10_word36 *bitposp));
static rtx	pdp10_dynamic_addr_expr_byte_pointer PARAMS ((tree exp, int bytesize));
static rtx	pdp10_strip_byte_pointer_address PARAMS ((rtx x));
static rtx	pdp10_initializer_byte_pointer PARAMS ((tree exp));
static rtx	convert_local_pointer PARAMS ((rtx op0, int from,
					       int to, int stack));
static rtx	convert_global_pointer PARAMS ((rtx op0, int from,
						int to, int stack));
static int	noconv_charp PARAMS ((int bytesize, tree exp));
#if XKL_STUFF
static void	maybe_warn_about_pointer_conversion PARAMS ((tree, int, int));
#endif
static tree	strip_conversions PARAMS ((enum tree_code code, tree type,
					   tree exp));
static tree	pdp10_strip_all_conversions PARAMS ((tree exp));
static rtx	pdp10_expand_int_plus_ptr_minus_const PARAMS ((enum machine_mode mode,
							       rtx target,
							       tree exp));
static void	emit_cmp_and_jump_insn_1 PARAMS ((rtx x, rtx y,
						  enum machine_mode mode,
						  enum rtx_code comparison,
						  int unsignedp, rtx label));
extern void	prepare_cmp_insn PARAMS ((rtx *, rtx *, enum rtx_code *, rtx,
					  enum machine_mode *, int *,
					  enum can_compare_purpose));
static void	pdp10_emit_cmp_and_jump_insns PARAMS ((tree exp, rtx x, rtx y,
						       enum rtx_code
						       comparison,
						       rtx size,
						       enum machine_mode mode,
						       int unsignedp,
						       rtx label));

rtx pdp10_compare_op0, pdp10_compare_op1;

/* Print a number to the stdio stream STREAM.  X is the number to
   print, DIV is a divisor to divide by, BITS is the precision in
   number of bits.  ZERO is zero if zero should be printend, and
   nonzero if zero should be supressed.  */
static void
pdp10_print_number (stream, x, div, bits, zero)
     FILE *stream;
     HOST_WIDE_INT x;
     int div;
     int bits;
     int zero;
{
  HOST_WIDE_INT mask;

  if (x < 0)
    x -= div - 1;
  x /= div;

  if (bits < 0)
    {
      bits = -bits;
      if (x < 0)
	{
	  fputc ('-', stream);
	  x = -x;
	}
    }

  mask = HWINT (-1) << bits;
  if ((x & mask) != 0 && (x & mask) != mask)
    fprintf (stream, "pdp10.c:%d:TOOBIG:", __LINE__);

  x &= ~mask;

  if (zero == 0 || x > 0)
    {
      if (bits == 9)
	fprintf (stream, "%03llo", x);
      else
	fprintf (stream, HOST_WIDE_INT_PRINT_OCT, x);
    }
}

/* Return the expression EXP hiding inside NOP_EXPR and CONVERT_EXPR.  */
static tree
strip_conversions (code, type, exp)
     enum tree_code code;
     tree type;
     tree exp;
{
  if ((TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
      && ((type == ptrdiff_type_node && TREE_TYPE (exp) == ptrdiff_type_node)
	  || (code == MINUS_EXPR && POINTER_TYPE_P (type) &&
	      POINTER_TYPE_P (TREE_TYPE (exp)))))
    {
      while (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
	exp = TREE_OPERAND (exp, 0);
    }
  return exp;
}

static tree
pdp10_strip_all_conversions (exp)
     tree exp;
{
  while (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
    exp = TREE_OPERAND (exp, 0);
  return exp;
}


#define PDP10_MAX_FOLDED_PTR_TERMS 4

struct pdp10_folded_ptr_terms
{
  tree int_terms[PDP10_MAX_FOLDED_PTR_TERMS * 2];
  tree ptr_terms[PDP10_MAX_FOLDED_PTR_TERMS];
  tree base_terms[PDP10_MAX_FOLDED_PTR_TERMS];
  int n_int;
  int n_ptr;
  int n_base;
  int failed;
};

static void pdp10_collect_folded_ptr_terms PARAMS ((tree,
                                                    struct pdp10_folded_ptr_terms *,
                                                    int));
static rtx pdp10_expand_folded_ptr_sum PARAMS ((enum machine_mode, rtx, tree));

static void
pdp10_collect_folded_ptr_terms (exp, out, neg)
     tree exp;
     struct pdp10_folded_ptr_terms *out;
     int neg;
{
  tree stripped;

  if (out->failed)
    return;

  stripped = pdp10_strip_all_conversions (exp);

  if (!neg && TREE_CODE (stripped) == PLUS_EXPR)
    {
      pdp10_collect_folded_ptr_terms (TREE_OPERAND (stripped, 0), out, 0);
      pdp10_collect_folded_ptr_terms (TREE_OPERAND (stripped, 1), out, 0);
      return;
    }

  if (!neg && TREE_CODE (stripped) == MINUS_EXPR)
    {
      pdp10_collect_folded_ptr_terms (TREE_OPERAND (stripped, 0), out, 0);
      pdp10_collect_folded_ptr_terms (TREE_OPERAND (stripped, 1), out, 1);
      return;
    }

  if (!neg && TREE_CODE (stripped) == NEGATE_EXPR)
    {
      pdp10_collect_folded_ptr_terms (TREE_OPERAND (stripped, 0), out, 1);
      return;
    }

  if (!neg && POINTER_TYPE_P (TREE_TYPE (stripped)))
    {
      if (out->n_ptr >= PDP10_MAX_FOLDED_PTR_TERMS)
        out->failed = 1;
      else
        out->ptr_terms[out->n_ptr++] = stripped;
      return;
    }

  if (neg && POINTER_TYPE_P (TREE_TYPE (stripped)))
    {
      if (out->n_base >= PDP10_MAX_FOLDED_PTR_TERMS)
        out->failed = 1;
      else
        out->base_terms[out->n_base++] = stripped;
      return;
    }

  if (neg)
    out->failed = 1;
  else if (out->n_int >= PDP10_MAX_FOLDED_PTR_TERMS * 2)
    out->failed = 1;
  else
    out->int_terms[out->n_int++] = exp;
}



static rtx
pdp10_expand_folded_ptr_sum (mode, target, exp)
     enum machine_mode mode;
     rtx target;
     tree exp;
{
  struct pdp10_folded_ptr_terms terms;
  int i;
  rtx acc;

  if (POINTER_TYPE_P (TREE_TYPE (exp)))
    return NULL_RTX;

  memset ((char *) &terms, 0, sizeof terms);
  pdp10_collect_folded_ptr_terms (exp, &terms, 0);

  if (terms.failed || terms.n_ptr == 0 || terms.n_ptr != terms.n_base)
    return NULL_RTX;

  acc = gen_reg_rtx (SImode);
  emit_move_insn (acc, const0_rtx);

  for (i = 0; i < terms.n_int; i++)
    {
      rtx x = expand_expr (terms.int_terms[i], NULL_RTX, SImode,
                           EXPAND_NORMAL);
      if (GET_MODE (x) != SImode && GET_MODE (x) != VOIDmode)
        x = convert_to_mode (SImode, x, 0);
      if (!register_operand (x, SImode))
        x = force_reg (SImode, x);
      emit_insn (gen_addsi3 (acc, acc, x));
    }

  for (i = 0; i < terms.n_ptr; i++)
    {
      tree pexp = terms.ptr_terms[i];
      tree bexp = terms.base_terms[i];
      rtx p = expand_expr (pexp, NULL_RTX, Pmode, EXPAND_NORMAL);
      rtx b = expand_expr (bexp, NULL_RTX, Pmode, EXPAND_CONST_ADDRESS);
      rtx d;
      if (GET_MODE (p) != Pmode && GET_MODE (p) != VOIDmode)
        p = convert_to_mode (Pmode, p, 1);
      if (optimize == 1
          && terms.n_int == 1
          && TREE_CODE (pdp10_strip_all_conversions (terms.int_terms[0]))
             == VAR_DECL)
        {
          rtx rawp = p;
          if (!register_operand (rawp, SImode))
            rawp = force_reg (SImode, rawp);
          emit_insn (gen_subsi3 (acc, acc, rawp));
        }
      d = expand_subtract_pointers (SImode, gen_reg_rtx (SImode),
                                    p, TREE_TYPE (pexp), b,
                                    TREE_TYPE (pexp));
      if (GET_MODE (d) != SImode && GET_MODE (d) != VOIDmode)
        d = convert_to_mode (SImode, d, 0);
      if (!register_operand (d, SImode))
        d = force_reg (SImode, d);
      emit_insn (gen_addsi3 (acc, acc, d));
    }

  return acc;
}

/* Expand folded integer forms like

     s + (PTR - CONST_BYTE_POINTER)

   when the middle end has exposed them as

     (s + (ptrdiff_t) PTR) + -CONST_BYTE_POINTER

   A plain integer expansion would add the encoded byte-pointer word to S.
   A pointer expansion is worse: it adjusts S as if S itself were a byte
   pointer.  Rebuild the intended byte-pointer subtraction from the tree and
   then add the resulting byte count to the integer term.  */
static rtx
pdp10_expand_int_plus_ptr_minus_const (mode, target, exp)
     enum machine_mode mode;
     rtx target;
     tree exp;
{
  tree lhs;
  tree rhs;
  tree a;
  tree b;
  tree int_exp;
  tree ptr_exp;
  tree base_exp;
  rtx int_rtx;
  rtx ptr_rtx;
  rtx base_rtx;
  rtx diff;

  if (TREE_CODE (exp) != PLUS_EXPR || POINTER_TYPE_P (TREE_TYPE (exp)))
    return NULL_RTX;

  lhs = pdp10_strip_all_conversions (TREE_OPERAND (exp, 0));
  rhs = pdp10_strip_all_conversions (TREE_OPERAND (exp, 1));
  if (TREE_CODE (lhs) != PLUS_EXPR || TREE_CODE (rhs) != NEGATE_EXPR)
    return NULL_RTX;


  a = pdp10_strip_all_conversions (TREE_OPERAND (lhs, 0));
  b = pdp10_strip_all_conversions (TREE_OPERAND (lhs, 1));

  if (POINTER_TYPE_P (TREE_TYPE (a)) && !POINTER_TYPE_P (TREE_TYPE (b)))
    {
      ptr_exp = a;
      int_exp = TREE_OPERAND (lhs, 1);
    }
  else if (!POINTER_TYPE_P (TREE_TYPE (a)) && POINTER_TYPE_P (TREE_TYPE (b)))
    {
      int_exp = TREE_OPERAND (lhs, 0);
      ptr_exp = b;
    }
  else
    return NULL_RTX;


  base_exp = TREE_OPERAND (rhs, 0);
  base_rtx = expand_expr (base_exp, NULL_RTX, Pmode, EXPAND_CONST_ADDRESS);
  if (!CONSTANT_P (base_rtx) || !pdp10_const_byte_pointer_p (base_rtx))
    return NULL_RTX;

  if (!target)
    target = gen_reg_rtx (mode);

  int_rtx = expand_expr (int_exp, NULL_RTX, mode, EXPAND_NORMAL);
  ptr_rtx = expand_expr (ptr_exp, NULL_RTX, Pmode, EXPAND_NORMAL);
  diff = expand_subtract_pointers (mode, gen_reg_rtx (Pmode), ptr_rtx,
				       TREE_TYPE (ptr_exp), base_rtx,
				       TREE_TYPE (ptr_exp));

  if (!register_operand (int_rtx, mode))
    int_rtx = force_reg (mode, int_rtx);
  if (!register_operand (diff, mode))
    diff = force_reg (mode, diff);
  emit_insn (gen_addsi3 (target, int_rtx, diff));
  return target;
}

/* Expand RTL instructions from a binary parse tree expression.  */
rtx
pdp10_expand_binop (mode, binoptab, op0, op1, target, unsignedp, methods, exp)
     enum machine_mode mode;
     optab binoptab;
     rtx op0, op1;
     rtx target;
     int unsignedp;
     enum optab_methods methods;
     tree exp;
{
  enum tree_code code = TREE_CODE (exp);
  tree exp0, exp1, type0, type1;
  int ptr0, ptr1, add;

  /* GCC parses a pointer subtraction like this:
        T *x, *y;
        int n = x - y;
     as:
	int n = (ptrdiff_t)x - (ptrdiff_t)y;
     Therefore, we need to strip away the ptrdiff_t conversions from the
     operands in order to see their real types.  */

  exp0 = strip_conversions (code, TREE_TYPE (exp), TREE_OPERAND (exp, 0));
  type0 = TREE_TYPE (exp0);
  ptr0 = POINTER_TYPE_P (type0);

  if (TREE_CODE_CLASS (code) == '<' || TREE_CODE_CLASS (code) == '2')
    {
      exp1 = strip_conversions (code, TREE_TYPE (exp), TREE_OPERAND (exp, 1));
      type1 = TREE_TYPE (exp1);
      ptr1 = POINTER_TYPE_P (type1) && GET_CODE (op1) != CONST_INT;

      /* A pointer difference hidden under conversions may be used as an
	 integer value, for example in

	   s += (int)(p - global_byte_array)

	 Do not classify the outer integer PLUS_EXPR as pointer arithmetic just
	 because its right operand contains a pointer subtraction.  That makes
	 expand_add_or_sub_pointer treat the integer accumulator as a byte
	 pointer.  The hidden-pointer test is only useful when the outer
	 expression itself produces a pointer value.  */
      if (!ptr1 && POINTER_TYPE_P (TREE_TYPE (exp))
	  && TREE_CODE (exp1) == MINUS_EXPR)
	{
	  if (POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp1, 0))))
	    {
	      ptr1 = 1;
	      type1 = TREE_TYPE (TREE_OPERAND (exp1, 0));
	    }
	  else if (POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp1, 1))))
	    {
	      ptr1 = 1;
	      type1 = TREE_TYPE (TREE_OPERAND (exp1, 1));
	    }
	}
    }

  switch (code)
    {
    case PLUS_EXPR:
    case MINUS_EXPR:
      if (code == PLUS_EXPR)
	{
	  rtx x = pdp10_expand_folded_ptr_sum (mode, target, exp);
	  if (x != NULL_RTX)
	    return x;
	  x = pdp10_expand_int_plus_ptr_minus_const (mode, target, exp);
	  if (x != NULL_RTX)
	    return x;
	}
      add = (code == PLUS_EXPR);
      if (GET_CODE (op0) == NEG)
	{
	  op0 = XEXP (op0, 0);
	  add = !add;
	}
      if (GET_CODE (op1) == NEG)
	{
	  op1 = XEXP (op1, 0);
	  add = !add;
	}
      if (!add && ptr0 && ptr1)
        {
          /* If the requested result mode is narrower than a pointer,
             expand_expr may already have truncated pointer operands to
             their address halves before this target hook sees them.  PDP-10
             byte pointers need the full P/S selector for subtraction, so
             re-expand both tree operands in Pmode and let the subtract path
             truncate only the final byte count.  */
          if (mode != Pmode)
            {
              op0 = expand_expr (exp0, NULL_RTX, Pmode, EXPAND_NORMAL);
              op1 = expand_expr (exp1, NULL_RTX, Pmode, EXPAND_NORMAL);
            }
          return expand_subtract_pointers (mode, target, op0, type0, op1, type1);
        }

      /* The middle end can also leave pointer subtraction against a
	 constant byte pointer as a plain MINUS_EXPR whose result type is
	 integer ptrdiff_t.  In that case PTR1 is false because the RTL
	 operand is a constant, not a register-valued pointer.  Do not send
	 this to expand_add_or_sub_pointer: that routine adjusts a byte
	 pointer and returns a byte pointer, which corrupts integer forms such
	 as

	   s += p - global_byte_array;

	 Use the byte-pointer subtraction path so the result is a byte count.  */
      if (!add && ptr0 && !POINTER_TYPE_P (TREE_TYPE (exp))
	  && CONSTANT_P (op1) && pdp10_const_byte_pointer_p (op1))
	return expand_subtract_pointers (mode, target, op0, type0,
					 op1, type0);

      /* The middle end can fold pointer subtraction against a constant
	 byte pointer into a PLUS_EXPR whose second RTL operand is the
	 negated pointer constant, while the tree result is still an
	 integer ptrdiff_t.  Do not treat that as pointer plus integer:
	 expand_add_or_sub_pointer would use TARGET as the byte pointer
	 being adjusted, which corrupts expressions such as

	   s += p - global_byte_array;

	 when TARGET already holds the running integer sum.  Recover the
	 positive byte-pointer constant and use the byte-pointer subtraction
	 path instead.  */
      if (add && ptr0 && !POINTER_TYPE_P (TREE_TYPE (exp))
	  && CONSTANT_P (op1))
	{
	  rtx rhs = negate_rtx (Pmode, op1);

	  if (pdp10_const_byte_pointer_p (rhs))
	    return expand_subtract_pointers (mode, target, op0, type0,
					     rhs, type0);
	}

      /* Do not perform pointer adjustment for an outer integer expression
	 whose only pointer operand came from stripping a ptrdiff_t conversion.
	 Such forms are part of folded pointer-difference arithmetic.  Let the
	 ordinary integer expander handle the already-expanded operands unless a
	 more specific pointer-difference pattern above matched.  */
      if (add && !POINTER_TYPE_P (TREE_TYPE (exp))
	  && ((ptr0 && !ptr1) || (!ptr0 && ptr1)))
	break;

      if (ptr0 || ptr1)
	{
	  if (POINTER_TYPE_P (TREE_TYPE (exp)))
	    {
	      type0 = TREE_TYPE (exp);
	      type1 = NULL_TREE;
	    }
	  return expand_add_or_sub_pointer (target, op0, type0, op1, type1,
					    add);
	}
      break;

    case PREINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      if (ptr0)
	return expand_add_or_sub_pointer (target, op0, type0, op1, type1, 1);
      break;

    default:
      break;
    }

  return expand_binop (mode, binoptab, op0, op1, target, unsignedp, methods);
}


/* Normalize the source-language byte sizes used by the front end to the
   pointer byte sizes that the PDP-10 backend actually encodes.  */
static int
pdp10_normalize_pointer_bytesize (size)
     int size;
{
  /* Do not normalize 16-bit scalar byte pointers to 18-bit halfword
     pointers.  short16 arrays are laid out with POINT 16 byte fields;
     using an 18-bit byte pointer for &array[i] makes IBP/LDB fetch the
     wrong bit slice.  Keep 32-bit scalar pointers normalized to word
     pointers for now.  */
  if (size == 32)
    return 36;
  return size;
}

/* Convert the one-word global byte-pointer P/S selector PS to byte size TO.
   BYTE is used when converting a word address to a byte pointer.  Store the
   decoded source byte size in *FROMP if FROMP is non-NULL.  */
static int
pdp10_global_ps_for_conversion (ps, to, byte, fromp)
     int ps;
     int to;
     int byte;
     int *fromp;
{
  int from;

  to = pdp10_normalize_pointer_bytesize (to);
  from = bytesize_for_ps (ps);
  if (fromp)
    *fromp = from;

  if (from == to)
    return ps;

  switch (from)
    {
    case 6:
      {
	int b = ps - 046;
	if (b < 0)
	  b = 0;
	if (b > 5)
	  b = 5;

	switch (to)
	  {
	  case 6:
	    break;
	  case 7:
	    ps = 062 + (b > 4 ? 4 : b);
	    break;
	  case 8:
	    ps = 055 + (b > 3 ? 3 : b);
	    break;
	  case 9:
	    ps = 070 + (b > 3 ? 3 : b);
	    break;
	  case 18:
	    ps = 075 + b / 3;
	    break;
	  case 36:
	    ps = 0;
	    break;
	  default:
	    abort ();
	  }
      }
      break;

    case 7:
      switch (to)
	{
	case 6:
	  ps = 046 + (ps - 062);
	  break;
	case 7:
	  break;
	case 8:
	  if (ps == 066)
	    ps = 060;
	  else
	    ps = 055 + (ps - 062);
	  break;
	case 9:
	  if (ps == 066)
	    ps = 073;
	  else
	    ps = 070 + (ps - 062);
	  break;
	case 18:
	  if (ps < 064)
	    ps = 075;
	  else
	    ps = 076;
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}
      break;

    case 8:
      switch (to)
	{
	case 6:
	  ps = 046 + (ps - 055);
	  break;
	case 7:
	  ps = 062 + (ps - 055);
	  break;
	case 8:
	  break;
	case 9:
	  ps = 070 + (ps - 055);
	  break;
	case 18:
	  ps = 075 + (ps - 055) / 2;
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}
      break;

    case 9:
      switch (to)
	{
	case 6:
	  ps = 046 + (ps - 070);
	  break;
	case 7:
	  ps = 062 + (ps - 070);
	  break;
	case 8:
	  ps = 055 + (ps - 070);
	  break;
	case 9:
	  break;
	case 18:
	  ps = 075 + (ps - 070) / 2;
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}
      break;

    case 18:
      switch (to)
	{
	case 6:
	  ps = 046 + (ps - 075) * 2;
	  break;
	case 7:
	  ps = 062 + (ps - 075) * 2;
	  break;
	case 8:
	  ps = 055 + (ps - 075) * 2;
	  break;
	case 9:
	  ps = 070 + (ps - 075) * 2;
	  break;
	case 18:
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}
      break;

    case 36:
      switch (to)
	{
	case 6:
	  ps = 046 + byte;
	  break;
	case 7:
	  ps = 062 + byte;
	  break;
	case 8:
	  ps = 055 + byte;
	  break;
	case 9:
	  ps = 070 + byte;
	  break;
	case 18:
	  ps = 075 + byte;
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}
      break;

    default:
      abort ();
    }

  return ps;
}

/* Convert a local byte-pointer P/S selector.  Return nonzero if the
   conversion can be folded as a constant.  The converted selector is
   stored in *PSP, and the decoded source byte size in *FROMP when FROMP
   is non-NULL.  */
static int
pdp10_local_ps_for_conversion (ps, from, to, byte, psp, fromp)
     int ps;
     int from;
     int to;
     int byte;
     int *psp;
     int *fromp;
{
  int p, s;

  from = pdp10_normalize_pointer_bytesize (from);
  to = pdp10_normalize_pointer_bytesize (to);

  if (from == 36)
    {
      switch (to)
	{
	case 6:
	case 7:
	case 8:
	case 9:
	case 16:
	case 18:
	  if (byte == PDP10_SCALAR_RIGHT_BYTE)
	    {
	      /* Standalone scalar slots are right-justified in their
	         PDP-10 word.  This is not the same as the last canonical
	         array byte for 16-bit quantities: a short16 scalar uses
	         POINT 16,word,35, while byte 1 of a short16 array ends at
	         bit 31.  */
	      ps = to;
	    }
	  else
	    ps = ((BITS_PER_WORD - to * byte - to) << 6) + to;
	  break;
	case 36:
	  ps = 0;
	  break;
	default:
	  abort ();
	}

      if (psp)
	*psp = ps;
      if (fromp)
	*fromp = from;
      return 1;
    }

  if (ps == 0 || decode_ps (ps, &p, &s) != 0)
    return 0;

  from = s;
  if (fromp)
    *fromp = from;

  if (from == to)
    {
      if (psp)
	*psp = ps;
      return 1;
    }

  switch (to)
    {
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 18:
      /* Preserve the exact source bit position.  Local PDP-10 P,S
	 selectors are not limited to canonical byte-array positions;
	 rounding through p / to changes valid pointers such as a 7-bit
	 byte at bit position 24 or an 8-bit byte at bit position 12.

	 If the new byte size would cross the word boundary, this cannot be
	 folded as a simple local P,S selector.  Let the runtime conversion
	 path handle it.  */
      if (p + to > BITS_PER_WORD)
	return 0;
      ps = ((BITS_PER_WORD - p - to) << 6) + to;
      break;

    case 36:
      ps = 0;
      break;

    default:
      abort ();
    }

  if (psp)
    *psp = ps;
  return 1;
}

#define emit_TLZ(X, Y, Z) \
  emit_insn (gen_andsi3 (X, Y, gen_int_mode (~(HWINT (Z) << 18), SImode)))
#define emit_TLO(X, Y, Z) \
  emit_insn (gen_iorsi3 (X, Y, gen_int_mode (HWINT (Z) << 18, SImode)))
#define emit_TLC(X, Y, Z) \
  emit_insn (gen_xorsi3 (X, Y, gen_int_mode (HWINT (Z) << 18, SImode)))
#define emit_ADD(X, Y, Z) \
  emit_insn (gen_addsi3 (X, Y, gen_int_mode (HWINT (Z) << 18, SImode)))
#define emit_SUB(X, Y, Z) \
  emit_insn (gen_subsi3 (X, Y, gen_int_mode (HWINT (Z) << 18, SImode)))
#define emit_TLNE(X, Y, L) \
  emit_jump_insn (gen_test_and_skip (GEN_EQ, X, gen_int_mode (HWINT (Y)<<18, SImode), L))
#define emit_TLNN(X, Y, L) \
  emit_jump_insn (gen_test_and_skip (GEN_NE, X, gen_int_mode (HWINT (Y)<<18, SImode), L))
#define emit_TLZN(X, Y, L) \
  emit_jump_insn (gen_TLZN (X, gen_int_mode (HWINT (Y) << 18, SImode), L))

/* Return the size, in PDP-10 object bits, of TYPE when it is used as a
   component of a byte-packed object.  For scalar PDP-10 byte types this is
   their logical byte size.  For records and arrays, use the laid-out type
   size; array and record fields occupy their whole object, not just one
   element-byte selected by pdp10_bytesize.  */
static int
pdp10_initializer_type_bitsize (type, bitsp)
     tree type;
     pdp10_word36 *bitsp;
{
  int bytesize;

  if (type == error_mark_node)
    return 0;

  if ((TREE_CODE (type) == RECORD_TYPE || TREE_CODE (type) == ARRAY_TYPE)
      && TYPE_SIZE (type) != 0
      && host_integerp (TYPE_SIZE (type), 1))
    {
      *bitsp = (pdp10_word36) tree_low_cst (TYPE_SIZE (type), 1);
      return 1;
    }

  bytesize = pdp10_normalize_pointer_bytesize (pdp10_bytesize (type));
  if (bytesize <= 0)
    return 0;

  *bitsp = (pdp10_word36) bytesize;
  return 1;
}

/* Compute the PDP-10 byte-packed bit offset of WANTED inside record TYPE.

   Do not use int_bit_position here.  GCC's generic record layout measures
   offsets in its abstract 9-bit storage units and can assign repeated or
   otherwise unsuitable bit positions for PDP-10 sub-word scalar fields.  The
   ordinary load/store expanders ultimately treat homogeneous char6/uchar6
   records like packed PDP-10 bytes, e.g.

       c0..c5 => POINT 6,record,5/11/17/23/29/35
       u0..u5 => POINT 6,record+1,5/11/17/23/29/35

   Static address constants such as &b6.c1 must use the same packed PDP-10
   byte numbering, otherwise they point at the wrong byte even if the
   initializer is accepted.  */
static int
pdp10_initializer_record_field_bitpos (type, wanted, bitposp)
     tree type;
     tree wanted;
     pdp10_word36 *bitposp;
{
  tree field;
  pdp10_word36 bitpos;

  if (TREE_CODE (type) != RECORD_TYPE)
    return 0;

  bitpos = 0;
  for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
    {
      tree ftype;
      pdp10_word36 fbits;
      int bytesize;

      if (TREE_CODE (field) != FIELD_DECL)
        continue;

      if (field == wanted)
        {
          *bitposp = bitpos;
          return 1;
        }

      ftype = TREE_TYPE (field);
      bytesize = pdp10_normalize_pointer_bytesize (pdp10_bytesize (ftype));
      if (!pdp10_initializer_type_bitsize (ftype, &fbits))
        return 0;

      /* struct-subword-packed-array-field-v19:
         Word-sized fields and nested RECORD_TYPE fields are addressed on
         whole PDP-10 words.  Do not force every ARRAY_TYPE field to a word
         boundary: arrays of PDP-10 subword scalar elements are part of the
         same packed byte run.  Otherwise &record.halfword_array[0] loses a
         starting right-half position and word->18-bit pointer conversion
         emits POINT 18,word,17 instead of POINT 18,word,35.  */
      if (bytesize >= BITS_PER_WORD
          || TREE_CODE (ftype) == RECORD_TYPE)
        {
          pdp10_word36 rem = bitpos % BITS_PER_WORD;

          if (rem != 0)
            bitpos += BITS_PER_WORD - rem;
          bitpos += ((fbits + BITS_PER_WORD - 1) / BITS_PER_WORD)
            * BITS_PER_WORD;
        }
      else
        {
          pdp10_word36 rem = bitpos % BITS_PER_WORD;

          if (rem + (pdp10_word36) bytesize > BITS_PER_WORD)
            bitpos += BITS_PER_WORD - rem;
          bitpos += fbits;
        }
    }

  return 0;
}

static int
pdp10_initializer_ref_bitpos (exp, basep, bitposp)
     tree exp;
     tree *basep;
     pdp10_word36 *bitposp;
{
  switch (TREE_CODE (exp))
    {
    case VAR_DECL:
      *basep = exp;
      *bitposp = 0;
      return 1;

    case ARRAY_REF:
      {
        tree array = TREE_OPERAND (exp, 0);
        tree index = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 elem_bits;
        HOST_WIDE_INT idx;

        if (!host_integerp (index, 0))
          return 0;
        if (!pdp10_initializer_ref_bitpos (array, basep, &base_bits))
          return 0;
        idx = tree_low_cst (index, 0);
        if (idx < 0)
          return 0;
        if (!pdp10_initializer_type_bitsize (TREE_TYPE (exp), &elem_bits))
          return 0;
        *bitposp = base_bits + (pdp10_word36) idx * elem_bits;
        return 1;
      }

    case COMPONENT_REF:
      {
        tree object = TREE_OPERAND (exp, 0);
        tree field = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 field_bits;

        if (!pdp10_initializer_ref_bitpos (object, basep, &base_bits))
          return 0;
        if (!pdp10_initializer_record_field_bitpos (TREE_TYPE (object),
                                                    field, &field_bits))
          return 0;
        *bitposp = base_bits + field_bits;
        return 1;
      }

    default:
      return 0;
    }
}


/* Compute the bit position of EXP relative to the beginning of its
   containing PDP-10 object.  Unlike pdp10_initializer_ref_bitpos, this
   deliberately stops at any dynamic base object, such as *p, because the
   caller only needs the byte position inside the addressed word.

   This is used for dynamic ADDR_EXPR of ARRAY_REF.  GCC may expand
   &p->h[0] as the word address of the storage word containing h[0], losing
   the fact that h starts in the right half of that word.  We recover the
   byte number from the tree shape and feed it to the normal word-pointer to
   byte-pointer conversion.  */
static int
pdp10_dynamic_ref_bitpos (exp, bitposp)
     tree exp;
     pdp10_word36 *bitposp;
{
  switch (TREE_CODE (exp))
    {
    case NOP_EXPR:
    case CONVERT_EXPR:
      return pdp10_dynamic_ref_bitpos (TREE_OPERAND (exp, 0), bitposp);

    case VAR_DECL:
    case PARM_DECL:
    case INDIRECT_REF:
      *bitposp = 0;
      return 1;

    case ARRAY_REF:
      {
        tree array = TREE_OPERAND (exp, 0);
        tree index = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 elem_bits;
        HOST_WIDE_INT idx;

        if (!host_integerp (index, 0))
          return 0;
        if (!pdp10_dynamic_ref_bitpos (array, &base_bits))
          return 0;
        idx = tree_low_cst (index, 0);
        if (idx < 0)
          return 0;
        if (!pdp10_initializer_type_bitsize (TREE_TYPE (exp), &elem_bits))
          return 0;
        *bitposp = base_bits + (pdp10_word36) idx * elem_bits;
        return 1;
      }

    case COMPONENT_REF:
      {
        tree object = TREE_OPERAND (exp, 0);
        tree field = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 field_bits;

        if (!pdp10_dynamic_ref_bitpos (object, &base_bits))
          return 0;
        if (!pdp10_initializer_record_field_bitpos (TREE_TYPE (object),
                                                    field, &field_bits))
          return 0;
        *bitposp = base_bits + field_bits;
        return 1;
      }

    default:
      return 0;
    }
}

/* Return the byte number within a word for dynamic ADDR_EXPR of a
   sub-word ARRAY_REF or COMPONENT_REF.

   This is the dynamic counterpart of the static initializer byte-pointer
   path.  It keeps address-taking of sub-word array/field objects from being
   flattened into a generic word address whose conversion always starts at
   byte zero.  The conversion expander may see either ADDR_EXPR directly or a
   NOP_EXPR/CONVERT_EXPR around it, so strip harmless conversions here.  */
static int
pdp10_addr_expr_array_byte (exp, bytesize, bytep)
     tree exp;
     int bytesize;
     int *bytep;
{
  HOST_WIDE_INT word_offset;

  return pdp10_addr_expr_array_position (exp, bytesize, bytep,
                                         &word_offset);
}

static int
pdp10_addr_expr_array_position (exp, bytesize, bytep, wordp)
     tree exp;
     int bytesize;
     int *bytep;
     HOST_WIDE_INT *wordp;
{
  tree object;
  pdp10_word36 bitpos;
  pdp10_word36 byte_pos;
  HOST_WIDE_INT word_offset;

  if (exp == NULL_TREE)
    return 0;

  while (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
    exp = TREE_OPERAND (exp, 0);

  if (TREE_CODE (exp) != ADDR_EXPR)
    return 0;
  object = TREE_OPERAND (exp, 0);
  if (TREE_CODE (object) != ARRAY_REF
      && TREE_CODE (object) != COMPONENT_REF)
    return 0;

  bytesize = pdp10_normalize_pointer_bytesize (bytesize);
  if (!VALID_OWGBP_BYTE_SIZE_P (bytesize) || bytesize >= BITS_PER_WORD)
    return 0;

  /* struct-subword-dynamic-arrayref-generic-fieldpos-v20:
     For dynamic address-taking of an array field, the normal load/store
     expanders already use GCC's laid-out field position.  The custom packed
     initializer walker is not always the right truth source here: if it
     returns the beginning of the containing word, word->18-bit conversion
     emits POINT 18,word,17 and one IBP stores into the previous halfword.

     Keep this fallback narrow: ARRAY_REF whose array operand is a
     COMPONENT_REF, with a constant index.  Use the enclosing object bit
     position from the existing dynamic walker, but use int_bit_position for
     the array field itself so address-taking agrees with generated loads and
     stores of the same field.  */
  if (TREE_CODE (object) == ARRAY_REF)
    {
      tree array = TREE_OPERAND (object, 0);
      tree index = TREE_OPERAND (object, 1);

      if (TREE_CODE (array) == COMPONENT_REF
          && host_integerp (index, 0))
        {
          tree record_object = TREE_OPERAND (array, 0);
          tree field = TREE_OPERAND (array, 1);
          pdp10_word36 base_bits;
          pdp10_word36 elem_bits;
          HOST_WIDE_INT idx = tree_low_cst (index, 0);

          if (idx >= 0
              && pdp10_dynamic_ref_bitpos (record_object, &base_bits)
              && pdp10_initializer_type_bitsize (TREE_TYPE (object),
                                                 &elem_bits))
            {
              bitpos = base_bits
                + (pdp10_word36) int_bit_position (field)
                + (pdp10_word36) idx * elem_bits;

              byte_pos = bitpos % BITS_PER_WORD;
              if (byte_pos + (pdp10_word36) bytesize <= BITS_PER_WORD
                  && byte_pos % (pdp10_word36) bytesize == 0)
                {
                  word_offset = (HOST_WIDE_INT) (bitpos / BITS_PER_WORD);
                  *bytep = (int) (byte_pos / (pdp10_word36) bytesize);
                  if (wordp)
                    *wordp = word_offset;
                  return 1;
                }
            }
        }
    }

  if (!pdp10_dynamic_ref_bitpos (object, &bitpos))
    return 0;

  byte_pos = bitpos % BITS_PER_WORD;
  if (byte_pos + (pdp10_word36) bytesize > BITS_PER_WORD)
    return 0;

  /* convert_local_pointer() converts from a word pointer using a byte
     number, so only canonical byte-array positions can use this shortcut.
     Noncanonical local byte starts need the ZERO_EXTRACT based path.  */
  if (byte_pos % (pdp10_word36) bytesize != 0)
    return 0;

  word_offset = (HOST_WIDE_INT) (bitpos / BITS_PER_WORD);
  *bytep = (int) (byte_pos / (pdp10_word36) bytesize);
  if (wordp)
    *wordp = word_offset;
  return 1;
}

static rtx
pdp10_strip_byte_pointer_address (x)
     rtx x;
{
  if (GET_CODE (x) == CONST)
    {
      rtx y = XEXP (x, 0);
      if (GET_CODE (y) == PLUS)
        return XEXP (y, 0);
    }
  return x;
}

static rtx
pdp10_initializer_byte_pointer (exp)
     tree exp;
{
  tree object;
  tree base_tree;
  tree ptr_type;
  pdp10_word36 bitpos;
  pdp10_word36 word_offset;
  pdp10_word36 byte_pos;
  pdp10_word36 encoded;
  int bytesize;
  int ps;
  rtx base_rtx;
  rtx base_addr;
  rtx offset_rtx;

  /* Static initializers and function returns often reach the target as
     a NOP_EXPR/CONVERT_EXPR around ADDR_EXPR.  The destination pointer
     type still tells us which PDP-10 byte pointer to build, but the
     actual byte position lives in the inner ADDR_EXPR.  */
  ptr_type = TREE_TYPE (exp);
  while (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
    exp = TREE_OPERAND (exp, 0);

  if (TREE_CODE (exp) != ADDR_EXPR)
    return NULL_RTX;
  object = TREE_OPERAND (exp, 0);
  if (TREE_CODE (object) != ARRAY_REF
      && TREE_CODE (object) != COMPONENT_REF)
    return NULL_RTX;

  /* For ARRAY_REF, the destination pointer type is the most reliable
     description of the byte pointer to build.  For COMPONENT_REF, however,
     use the selected field type itself.  Address-taking of packed PDP-10
     record fields can otherwise be routed through generic field-position
     arithmetic before the final pointer type has enough information to
     recover the correct sub-word field.  */
  if (TREE_CODE (object) == COMPONENT_REF)
    bytesize = pdp10_normalize_pointer_bytesize
      (pdp10_bytesize (TREE_TYPE (object)));
  else
    {
      if (!POINTER_TYPE_P (ptr_type))
        return NULL_RTX;
      bytesize = pdp10_normalize_pointer_bytesize
        (pdp10_bytesize (TREE_TYPE (ptr_type)));
    }
  if (!VALID_OWGBP_BYTE_SIZE_P (bytesize))
    return NULL_RTX;

  /* struct-subword-packed-component-addr-bytepos-v24:
     For direct address-taking of a packed QImode component, use GCC's laid-out
     field position.  The older custom static initializer walker packs by
     subword value width, which is correct for some homogeneous PDP-10 byte
     records, but it puts heterogeneous packed fields such as
     c6,c7,c8,c9 at bit 21.  The actual load/store paths place c9 in the
     fourth 9-bit storage unit, bit 27, so &packed_rec.c9 must build a byte
     pointer equivalent to [POINT 9,packed_rec,35], not ...29.  */
  if (TREE_CODE (object) == COMPONENT_REF && bytesize == 9)
    {
      tree record_object = TREE_OPERAND (object, 0);
      tree field = TREE_OPERAND (object, 1);
      tree record_type = TREE_TYPE (record_object);
      pdp10_word36 base_bits;
      tree computed_base = NULL_TREE;

      if (field != NULL_TREE
          && TREE_CODE (field) == FIELD_DECL
          && (DECL_PACKED (field)
              || (record_type != NULL_TREE
                  && TREE_CODE (record_type) == RECORD_TYPE
                  && TYPE_PACKED (record_type)))
          && pdp10_initializer_ref_bitpos (record_object,
                                           &computed_base, &base_bits))
        {
          base_tree = computed_base;
          bitpos = base_bits + (pdp10_word36) int_bit_position (field);
        }
      else if (!pdp10_initializer_ref_bitpos (object, &base_tree, &bitpos))
        return NULL_RTX;
    }
  else if (!pdp10_initializer_ref_bitpos (object, &base_tree, &bitpos))
    return NULL_RTX;

  byte_pos = bitpos % BITS_PER_WORD;

  /* Local PDP-10 byte pointers carry an explicit P,S selector.
     They can represent noncanonical byte starts, such as a 7-bit
     byte at bit position 24 or an 8-bit byte at bit position 12.
     Only reject fields that actually run past the end of the word.

     Extended one-word global byte pointers, on the other hand, encode
     a byte number rather than an arbitrary local P value, so keep the
     canonical-alignment check there.  */
  if (byte_pos + (pdp10_word36) bytesize > BITS_PER_WORD)
    return NULL_RTX;
  if (TARGET_EXTENDED && byte_pos % bytesize != 0)
    return NULL_RTX;
  word_offset = bitpos / BITS_PER_WORD;
  base_rtx = expand_expr (base_tree, NULL_RTX, VOIDmode,
                          EXPAND_CONST_ADDRESS);
  if (GET_CODE (base_rtx) == MEM)
    base_addr = XEXP (base_rtx, 0);
  else if (CONSTANT_ADDRESS_P (base_rtx))
    base_addr = base_rtx;
  else
    return NULL_RTX;
  base_addr = pdp10_strip_byte_pointer_address (base_addr);

  /* Do not build literal byte pointers to automatic/frame objects here.
     For stack locals, expand_expr(..., EXPAND_CONST_ADDRESS) can leave a
     virtual frame address such as <fp> inside the literal.  Such a literal is
     not a real run-time address after frame/stack elimination, and using it
     for &struct.subbyte_array[0] makes later IDPB store through a stale
     frame placeholder instead of the actual stack slot.  Let the normal
     dynamic ADDR_EXPR path emit MOVEI plus PDP-10 local P,S conversion for
     frame-based objects.  Static/global aggregate fields still use this
     initializer path.  */
  if (pdp10_frame_address_p (base_addr))
    return NULL_RTX;

  if (TARGET_EXTENDED)
    ps = ps_base_for_bytesize (bytesize) + byte_pos / bytesize;
  else
    ps = ((BITS_PER_WORD - byte_pos - bytesize) << 6) + bytesize;
  encoded = (((pdp10_word36) ps) << (TARGET_EXTENDED ? 30 : 24));
  encoded = (encoded + word_offset) & PDP10_WORD36_MASK;
  offset_rtx = pdp10_gen_word36_constant (encoded);
  return gen_rtx_CONST (Pmode, gen_rtx_PLUS (Pmode, base_addr, offset_rtx));
}


/* Build a byte pointer address from a ZERO_EXTRACT produced by the generic
   COMPONENT_REF/ARRAY_REF expander.

   GCC's component expansion for PDP-10 sub-word fields represents a field as

       (subreg (zero_extract (mem ADDR) BITSIZE BITPOS) ...)

   when the object is in memory.  The address-taking path must not simply drop
   the ZERO_EXTRACT and keep ADDR, because BITPOS is the only place where the
   selected PDP-10 byte position is still recorded.  Doing so makes &b6.c1 look
   like &b6.c0, and similar duplicates show up in static byte-pointer
   initializers.

   ADDR may already contain a byte-pointer P,S field from DATA_PADDING.  Strip
   that P,S field but keep the word address, then install the P,S field selected
   by BITPOS and BITSIZE.  */
static rtx
pdp10_address_from_zero_extract (zx, exp)
     rtx zx;
     tree exp;
{
  rtx mem;
  rtx addr;
  rtx base;
  pdp10_word36 offset;
  pdp10_word36 word_addr;
  pdp10_word36 encoded;
  int bitsize;
  int bitpos;
  int bytesize;
  int ps;

  if (GET_CODE (zx) != ZERO_EXTRACT)
    return NULL_RTX;

  mem = XEXP (zx, 0);
  if (GET_CODE (mem) != MEM)
    return NULL_RTX;

  if (GET_CODE (XEXP (zx, 1)) != CONST_INT
      || GET_CODE (XEXP (zx, 2)) != CONST_INT)
    return NULL_RTX;

  bitsize = INTVAL (XEXP (zx, 1));
  bitpos = INTVAL (XEXP (zx, 2));

  if (bitsize <= 0 || bitsize >= BITS_PER_WORD)
    return NULL_RTX;

  bytesize = pdp10_normalize_pointer_bytesize
    (pdp10_bytesize (TREE_TYPE (TREE_TYPE (exp))));
  if (bytesize != bitsize || !VALID_OWGBP_BYTE_SIZE_P (bytesize))
    return NULL_RTX;

  if (bitpos < 0 || bitpos >= BITS_PER_WORD
      || bitpos + bytesize > BITS_PER_WORD)
    return NULL_RTX;
  if (TARGET_EXTENDED && bitpos % bytesize != 0)
    return NULL_RTX;

  addr = XEXP (mem, 0);

  /* The ZERO_EXTRACT shortcut is safe for static/global addresses, but not for
     automatic objects whose apparent address still contains a virtual frame
     placeholder.  If we return a CONST byte pointer containing <fp>, final
     output cannot repair it into the real stack address.  Fall back to the
     dynamic ADDR_EXPR path, which emits a real run-time address and converts
     that to a local byte pointer.  */
  if (pdp10_frame_address_p (addr))
    return NULL_RTX;

  if (!pdp10_decompose_symbolic_word36 (addr, &base, &offset))
    return NULL_RTX;

  word_addr = offset & ADDRESS_MASK;

  if (TARGET_EXTENDED)
    ps = ps_base_for_bytesize (bytesize) + bitpos / bytesize;
  else
    ps = ((BITS_PER_WORD - bitpos - bytesize) << 6) + bytesize;

  encoded = (word_addr + (((pdp10_word36) ps)
                          << (TARGET_EXTENDED ? 30 : 24)))
    & PDP10_WORD36_MASK;

  return base != NULL_RTX
    ? pdp10_plus_constant_word36 (base, encoded)
    : pdp10_gen_word36_constant (encoded);
}

/* Compute both the dynamic base object and the PDP-10 bit position of EXP.
   This is the address-producing sibling of pdp10_dynamic_ref_bitpos().  The
   old ADDR_EXPR path expanded the selected sub-word field itself, which may
   materialize a temporary.  For address-taking we instead need the address of
   the containing object plus the selected packed byte position.

   Stop at VAR_DECL/PARM_DECL/INDIRECT_REF: those are the objects whose word
   address can be expanded directly.  */
static int
pdp10_dynamic_ref_base_bitpos (exp, basep, bitposp)
     tree exp;
     tree *basep;
     pdp10_word36 *bitposp;
{
  switch (TREE_CODE (exp))
    {
    case NOP_EXPR:
    case CONVERT_EXPR:
      return pdp10_dynamic_ref_base_bitpos (TREE_OPERAND (exp, 0),
                                            basep, bitposp);

    case VAR_DECL:
    case PARM_DECL:
    case INDIRECT_REF:
      *basep = exp;
      *bitposp = 0;
      return 1;

    case ARRAY_REF:
      {
        tree array = TREE_OPERAND (exp, 0);
        tree index = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 elem_bits;
        HOST_WIDE_INT idx;

        /* Match the dynamic ARRAY_REF special case in
           pdp10_addr_expr_array_byte(): for an array field inside a record,
           use GCC's laid-out field bit position for the array field itself.
           The custom packed initializer walk is useful for some homogeneous
           records, but it can put heterogeneous fields such as c6/u7/c8 at a
           different bit position than the generated DPB/LDB field accesses. */
        if (TREE_CODE (array) == COMPONENT_REF
            && host_integerp (index, 0))
          {
            tree record_object = TREE_OPERAND (array, 0);
            tree field = TREE_OPERAND (array, 1);
            tree record_base = NULL_TREE;
            pdp10_word36 field_bits;

            idx = tree_low_cst (index, 0);
            if (idx < 0)
              return 0;
            if (!pdp10_dynamic_ref_base_bitpos (record_object,
                                                &record_base, &base_bits))
              return 0;
            if (!pdp10_initializer_type_bitsize (TREE_TYPE (exp), &elem_bits))
              return 0;
            if (!pdp10_initializer_record_field_bitpos (TREE_TYPE (record_object),
                                                        field, &field_bits))
              return 0;

            *basep = record_base;
            *bitposp = base_bits + field_bits
              + (pdp10_word36) idx * elem_bits;
            return 1;
          }

        if (!host_integerp (index, 0))
          return 0;
        if (!pdp10_dynamic_ref_base_bitpos (array, basep, &base_bits))
          return 0;
        idx = tree_low_cst (index, 0);
        if (idx < 0)
          return 0;
        if (!pdp10_initializer_type_bitsize (TREE_TYPE (exp), &elem_bits))
          return 0;
        *bitposp = base_bits + (pdp10_word36) idx * elem_bits;
        return 1;
      }

    case COMPONENT_REF:
      {
        tree object = TREE_OPERAND (exp, 0);
        tree field = TREE_OPERAND (exp, 1);
        pdp10_word36 base_bits;
        pdp10_word36 field_bits;

        if (!pdp10_dynamic_ref_base_bitpos (object, basep, &base_bits))
          return 0;
        if (!pdp10_initializer_record_field_bitpos (TREE_TYPE (object),
                                                    field, &field_bits))
          return 0;
        *bitposp = base_bits + field_bits;
        return 1;
      }

    default:
      return 0;
    }
}

/* Build a run-time local byte pointer for ADDR_EXPR of a sub-word field or
   array element.  This is deliberately dynamic: it uses the word address of
   the containing object, preserves the computed word offset, and installs the
   exact local P,S selector.  It avoids the generic ADDR_EXPR path that can
   take the address of a temporary copy of a sub-word field.

   This helper is for non-extended PDP-6/KA10-style local pointers.  Leave
   extended/global byte-pointer construction to the older constant paths.  */
static rtx
pdp10_dynamic_addr_expr_byte_pointer (exp, bytesize)
     tree exp;
     int bytesize;
{
  tree object;
  tree base_tree = NULL_TREE;
  tree dyn_index = NULL_TREE;
  tree dyn_elem_type = NULL_TREE;
  HOST_WIDE_INT dyn_index_scale = 1;
  rtx base_rtx;
  rtx base_addr;
  rtx addr;
  rtx reg;
  pdp10_word36 bitpos;
  pdp10_word36 byte_pos;
  pdp10_word36 word_offset;
  int ps;

  if (TARGET_EXTENDED)
    return NULL_RTX;

  if (exp == NULL_TREE)
    return NULL_RTX;

  while (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
    exp = TREE_OPERAND (exp, 0);

  if (TREE_CODE (exp) != ADDR_EXPR)
    return NULL_RTX;

  object = TREE_OPERAND (exp, 0);
  if (TREE_CODE (object) != ARRAY_REF
      && TREE_CODE (object) != COMPONENT_REF)
    return NULL_RTX;

  bytesize = pdp10_normalize_pointer_bytesize (bytesize);
  if (!VALID_OWGBP_BYTE_SIZE_P (bytesize) || bytesize >= BITS_PER_WORD)
    return NULL_RTX;

  if (TREE_CODE (object) == COMPONENT_REF
      && TREE_CODE (TREE_OPERAND (object, 0)) == ARRAY_REF
      && !host_integerp (TREE_OPERAND (TREE_OPERAND (object, 0), 1), 0))
    {
      tree array_ref = TREE_OPERAND (object, 0);
      tree array = TREE_OPERAND (array_ref, 0);
      tree index = TREE_OPERAND (array_ref, 1);
      tree field = TREE_OPERAND (object, 1);
      pdp10_word36 base_bits;
      pdp10_word36 record_bits;

      if (field == NULL_TREE || TREE_CODE (field) != FIELD_DECL)
        return NULL_RTX;
      if (!pdp10_dynamic_ref_base_bitpos (array, &base_tree, &base_bits))
        return NULL_RTX;
      if (!pdp10_initializer_type_bitsize (TREE_TYPE (array_ref),
                                           &record_bits))
        return NULL_RTX;
      if ((record_bits % (pdp10_word36) bytesize) != 0)
        return NULL_RTX;

      /* Dynamic a[i].field must first build a byte pointer to a[0].field and
         only then add i scaled by the record stride in field-sized bytes.
         The generic path can add the stride to a record pointer and then DPB
         through that pointer.  For subword fields this uses the record
         pointer's byte size (often 18) instead of the selected field byte
         size (usually 9), corrupting adjacent fields.  */
      bitpos = base_bits + (pdp10_word36) int_bit_position (field);
      dyn_index = index;
      dyn_elem_type = TREE_TYPE (object);
      dyn_index_scale = (HOST_WIDE_INT) (record_bits / (pdp10_word36) bytesize);
    }
  else if (TREE_CODE (object) == ARRAY_REF
      && TREE_CODE (TREE_OPERAND (object, 0)) == COMPONENT_REF
      && !host_integerp (TREE_OPERAND (object, 1), 0))
    {
      tree array = TREE_OPERAND (object, 0);
      tree index = TREE_OPERAND (object, 1);
      tree record_object = TREE_OPERAND (array, 0);
      tree field = TREE_OPERAND (array, 1);
      pdp10_word36 base_bits;

      if (field == NULL_TREE || TREE_CODE (field) != FIELD_DECL)
        return NULL_RTX;
      if (!pdp10_dynamic_ref_base_bitpos (record_object, &base_tree,
                                          &base_bits))
        return NULL_RTX;

      /* Dynamic p->field[i] must build a byte pointer to field[0] first,
         then adjust that byte pointer by i elements.  The generic address
         path may instead adjust the incoming struct pointer and only add the
         field word offset later.  On PDP-6/KA10 this is wrong for a struct
         pointer represented as a halfword byte pointer: IBP then walks
         18-bit slots while the element index is a 9-bit char count.  */
      bitpos = base_bits + (pdp10_word36) int_bit_position (field);
      dyn_index = index;
      dyn_elem_type = TREE_TYPE (object);
      dyn_index_scale = 1;
    }
  else if (!pdp10_dynamic_ref_base_bitpos (object, &base_tree, &bitpos))
    return NULL_RTX;

  byte_pos = bitpos % BITS_PER_WORD;
  if (byte_pos + (pdp10_word36) bytesize > BITS_PER_WORD)
    return NULL_RTX;

  word_offset = bitpos / BITS_PER_WORD;
  ps = ((BITS_PER_WORD - byte_pos - bytesize) << 6) + bytesize;

  base_rtx = expand_expr (base_tree, NULL_RTX, VOIDmode, EXPAND_CONST_ADDRESS);
  if (GET_CODE (base_rtx) == MEM)
    base_addr = XEXP (base_rtx, 0);
  else if (CONSTANT_ADDRESS_P (base_rtx) || GET_CODE (base_rtx) == REG)
    base_addr = base_rtx;
  else
    return NULL_RTX;

  if (GET_CODE (base_addr) == REG
      && TREE_CODE (base_tree) == INDIRECT_REF
      && byte_pos % (pdp10_word36) bytesize == 0
      && pdp10_normalize_pointer_bytesize (pdp10_bytesize (TREE_TYPE (base_tree)))
         < BITS_PER_WORD)
    {
      rtx inc = GEN_INT ((HOST_WIDE_INT) (bitpos / (pdp10_word36) bytesize));
      reg = gen_reg_rtx (Pmode);
      return pdp10_expand_addbp3 (TREE_TYPE (object), reg, base_addr, inc);
    }

  if (word_offset != 0 && GET_CODE (base_addr) != REG)
    addr = plus_constant (base_addr, (HOST_WIDE_INT) word_offset);
  else
    addr = base_addr;

  /* Static/global addresses can still be returned as constants here.  Frame
     addresses and register-held pointers must be materialized at run time. */
  if (CONSTANT_ADDRESS_P (addr) && !pdp10_frame_address_p (addr))
    return pdp10_plus_constant_word36 (addr,
                                      ((pdp10_word36) ps) << 24);

  reg = gen_reg_rtx (Pmode);
  if (GET_CODE (base_addr) == REG
      && base_addr != virtual_stack_vars_rtx
      && !REGNO_PTR_FRAME_P (REGNO (base_addr)))
    {
      emit_move_insn (reg, base_addr);
      /* BASE_ADDR may itself be a PDP-10 byte pointer, for example a
         struct pointer used to form p->char_field[i].  Clear the old P,S
         bits before installing this object's P,S selector; ORing the new
         selector into an 18-bit record pointer leaves a corrupt byte
         pointer for dynamic char array accesses.  */
      emit_insn (gen_HRRZ (reg, reg));
      if (word_offset != 0)
        emit_insn (gen_addsi3 (reg, reg, GEN_INT ((HOST_WIDE_INT) word_offset)));
    }
  else if (GET_CODE (base_addr) == MEM)
    {
      /* BASE_ADDR is the run-time pointer value loaded from an object such as
         a pointer parameter's stack slot.  Do not wrap MEM+offset in
         UNSPEC_ADDRESS/MOVEI; that is an invalid address expression after
         virtual register instantiation.  Load the pointer value first, clear
         any old P,S byte-pointer selector, then add the word offset before
         installing the selected field/element P,S.  */
      emit_move_insn (reg, base_addr);
      emit_insn (gen_HRRZ (reg, reg));
      if (word_offset != 0)
        emit_insn (gen_addsi3 (reg, reg, GEN_INT ((HOST_WIDE_INT) word_offset)));
    }
  else
    {
      if (word_offset != 0)
        addr = plus_constant (base_addr, (HOST_WIDE_INT) word_offset);
      emit_insn (gen_MOVEI (reg, addr));
    }

  /* emit_TLO takes the left-half immediate, not the compact 12-bit
     P,S selector.  A literal POINT word stores P,S in the high 12 bits
     of the left half, so the TLO immediate must be PS<<6.  Passing PS
     directly generates e.g. TLO ...,1210 instead of TLO ...,121000,
     which leaves IDPB/ILDB pointing at nonsense and misses packed struct
     fields. */
  emit_TLO (reg, reg, ps << 6);

  if (dyn_index != NULL_TREE)
    {
      rtx index_rtx;

      index_rtx = expand_expr (dyn_index, NULL_RTX, Pmode, EXPAND_NORMAL);
      if (!register_operand (index_rtx, Pmode))
        index_rtx = force_reg (Pmode, index_rtx);
      if (dyn_index_scale != 1)
        {
          rtx scaled = gen_reg_rtx (Pmode);
          emit_insn (gen_mulsi3 (scaled, index_rtx,
                                 GEN_INT (dyn_index_scale)));
          index_rtx = scaled;
        }
      return pdp10_expand_addbp3 (dyn_elem_type, reg, reg, index_rtx);
    }

  return reg;
}

/* Convert a global pointer in OP0 with byte size FROM to a global
   pointer with byte size TO.  Byte size 36 means word pointer.
   Return the result.  */
static rtx
convert_global_pointer (op0, from, to, byte)
     rtx op0;
     int from;
     int to;
     int byte;
{
  rtx label;

  switch (to)
    {
    case 6:
      switch (from)
	{
	case 7:
	  emit_SUB (op0, op0, 0140000);
	  break;
	case 8:
	  emit_SUB (op0, op0, 0070000);
	  break;
	case 9:
	  emit_SUB (op0, op0, 0220000);
	  break;
	case 36:
	  emit_TLO (op0, op0, 0460000 + byte * 0010000);
	  break;
	default:
	  abort ();
	}
      break;

    case 7:
      switch (from)
	{
	case 8:
	  emit_ADD (op0, op0, 0050000);
	  break;
	case 9:
	  emit_SUB (op0, op0, 0060000);
	  break;
	case 36:
	  emit_TLO (op0, op0, 0620000 + byte * 0010000);
	  break;
	default:
	  abort ();
	}
      break;

    case 8:
      switch (from)
	{
	case 9:
	  emit_SUB (op0, op0, 0130000);
	  break;
	case 18:
	  emit_TLO (op0, op0, 0010000);
	  emit_TLZ (op0, op0, 0200000);
	  break;
	case 36:
	  emit_TLO (op0, op0, 0550000 + byte * 0010000);
	  break;
	default:
	  abort ();
	}
      break;

    case 9:
      switch (from)
	{
	case 7:
	  emit_ADD (op0, op0, 0060000);
	  break;
	case 8:
	  emit_ADD (op0, op0, 0130000);
	  break;
	case 18:
	  emit_TLZ (op0, op0, 0050000);
	  break;
	case 36:
	  emit_TLO (op0, op0, 0700000 + byte * 0010000);
	  break;
	default:
	  abort ();
	}
      break;

    case 18:
      switch (from)
	{
	case 8:
	  emit_ADD (op0, op0, 0170000);
	  emit_insn (gen_TLNE_TLZA_TLO (op0, GEN_INT (0020000),
					GEN_INT (0010000), GEN_INT (0010000)));
	  break;
	case 9:
	  emit_TLO  (op0, op0, 0050000);
	  emit_TLNE (op0, 0020000, label = gen_label_rtx ());
	  emit_TLZ  (op0, op0, 0010000);
	  emit_label (label);
	  break;
	case 36:
	  emit_TLO (op0, op0, 0750000 + byte * 0010000);
	  break;
	default:
	  abort ();
	}
      break;

    case 36:
      emit_TLZ (op0, op0, 0770000);
      break;

    default:
      abort ();
    }

  return op0;
}

/* Convert a local pointer in OP0 with byte size FROM to a local
   pointer with byte size TO.  Byte size 36 means word pointer.
   Return the result.  */
static rtx
convert_local_pointer (op0, from, to, byte)
     rtx op0;
     int from;
     int to;
     int byte;
{
  rtx reg = gen_reg_rtx (Pmode);
  rtx label;
  rtx temp;

  switch (to)
    {
    case 6:
      switch (from)
	{
	case 7:
	  emit_TLZ  (reg, op0, 0010100);
	  emit_ADD  (reg, reg, 0020000);
	  emit_TLNN (reg, 0160000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0020000);
	  emit_label (label);
	  emit_TLNN (reg, 0200000, label = gen_label_rtx ());
	  emit_TLC  (reg, reg, 0060000);
	  emit_label (label);
	  break;
	case 8:
	  emit_TLC  (reg, op0, 0001600);
	  emit_TLZ  (reg, reg, 0060000);
	  emit_TLNE (reg, 0200000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0020000);
	  emit_label (label);
	  emit_TLNE (reg, 0100000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0040000);
	  emit_label (label);
	  break;
	case 9:
	  emit_TLC  (reg, op0, 0041700);
	  emit_TLZN (reg, 0010000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0040000);
	  emit_label (label);
	  break;
	case 18:
	  /* 18-bit local byte pointer -> 6-bit local byte pointer.
	     Preserve the source bit position by changing only P,S:
	     delta = ((18 - 6) << 6) + (6 - 18).  */
	  emit_ADD (reg, op0, 0136400);
	  break;
	case 36:
	  emit_TLO (reg, op0, 0360600 - byte * 0060000);
	  break;
	default:
	  abort ();
	}
      break;

    case 7:
      switch (from)
	{
	case 6:
	  /* 6-bit local byte pointer -> 7-bit local byte pointer.
	     Preserve the exact source bit position by changing P,S by
	     ((6 - 7) << 6) + (7 - 6).  */
	  emit_SUB (reg, op0, 007700);
	  break;
	case 8:
	  emit_TLC (reg, op0, 0001700);
	  emit_TLZ (reg, op0, 0070000);
	  temp = gen_reg_rtx (Pmode);
	  emit_move_insn (temp, reg);
	  emit_insn (gen_rotlsi3 (temp, temp, GEN_INT (3)));
	  emit_insn (gen_ANDCx (temp, temp, GEN_INT (7)));
	  emit_insn (gen_addsi3 (temp, temp, GEN_INT (1)));
	  emit_insn (gen_rotrsi3 (temp, temp, GEN_INT (6)));
	  emit_insn (gen_iorsi3 (reg, reg, temp));
	  break;
	case 9:
	  emit_TLC (reg, op0, 0071600);
	  emit_ADD (reg, reg, 0010000);
	  break;
	case 18:
	  /* 18-bit local byte pointer -> 7-bit local byte pointer. */
	  emit_ADD (reg, op0, 0126500);
	  break;
	case 36:
	  emit_TLO (reg, op0, 0350700 - byte * 0070000);
	  break;
	default:
	  abort ();
	}
      break;

    case 8:
      switch (from)
	{
	case 6:
	  /* 6-bit local byte pointer -> 8-bit local byte pointer. */
	  emit_SUB (reg, op0, 017600);
	  break;
	case 7:
	  /* 7-bit local byte pointer -> 8-bit local byte pointer. */
	  emit_SUB (reg, op0, 007700);
	  break;
	case 9:
	  emit_TLO (reg, op0, 0040000);
	  emit_TLZ (reg, reg, 0030100);
	  break;
	case 18:
	  emit_TLO (reg, op0, 0141000);
	  emit_TLZ (reg, reg, 0022200);
	  break;
	case 36:
	  emit_TLO (reg, op0, 0341000 - byte * 0100000);
	  break;
	default:
	  abort ();
	}
      break;

    case 9:
      switch (from)
	{
	case 6:
	  /* 6-bit local byte pointer -> 9-bit local byte pointer.
	     For positions that remain within the same word, this changes
	     P,S by ((-3 << 6) + 3).  emit_SUB takes the left-half
	     selector value, so subtract octal 027500.  This is the
	     missing aggregate/block-store conversion path hit by
	     store_before_char6_7.  */
	  emit_SUB (reg, op0, 027500);
	  break;
	case 7:
	  /* 7-bit local byte pointer -> 9-bit local byte pointer.
	     This is the same local P,S-size conversion class as the
	     6-bit case above, but with delta ((-2 << 6) + 2).  */
	  emit_SUB (reg, op0, 017600);
	  break;
	case 8:
	  emit_TLC  (reg, op0, 0040100);
	  emit_TLNE (reg, 0100000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0010000);
	  emit_label (label);
	  emit_TLNE (reg, 0200000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0020000);
	  emit_label (label);
	  break;
	case 18:
	  emit_TLC (reg, op0, 0113300);
	  break;
	case 36:
	  emit_TLO (reg, op0, 0331100 - byte * 0110000);
	  break;
	default:
	  abort ();
	}
      break;

    case 18:
      switch (from)
	{
	case 6:
	  /* 6-bit local byte pointer -> 18-bit local byte pointer.
	     This fills the matrix hole hit by convert_char6_short18.  */
	  emit_SUB (reg, op0, 0136400);
	  break;
	case 7:
	  /* 7-bit local byte pointer -> 18-bit local byte pointer. */
	  emit_SUB (reg, op0, 0126500);
	  break;
	case 8:
	  emit_TLZ  (reg, op0, 0141000);
	  emit_TLO  (reg, reg, 0002200);
	  emit_TLNE (reg, 0200000, label = gen_label_rtx ());
	  emit_TLO  (reg, reg, 0020000);
	  emit_label (label);
	  break;
	case 9:
	  emit_TLC (reg, op0, 0003300);
	  emit_TLZ (reg, reg, 0110000);
	  break;
	case 36:
	  emit_TLO (reg, op0, 0222200 - byte * 0220000);
	  break;
	default:
	  abort ();
	}
      break;

    case 16:
      switch (from)
	{
	case 18:
	  /* 18-bit local byte pointer -> 16-bit local byte pointer.
	     Preserve the current bit position; change only P,S.  */
	  emit_ADD (reg, op0, 017600);
	  break;
	case 36:
	  /* Word pointer -> 16-bit local byte pointer.  BYTE is normally the
	     canonical 16-bit byte number within the PDP-10 word.  A standalone
	     short16 scalar is different: it lives right-justified in the full
	     scalar word, so &scalar must become POINT 16,word,35 rather than
	     the canonical array byte ending at bit 31.  */
	  if (byte == PDP10_SCALAR_RIGHT_BYTE)
	    emit_TLO (reg, op0, 002000);
	  else
	    emit_TLO (reg, op0, 0242000 - byte * 0200000);
	  break;
	default:
	  abort ();
	}
      break;

    case 36:
      emit_TLZ (reg, op0, 0777777);
      break;

    default:
      abort ();
    }

  return reg;
}

static int
noconv_charp (bytesize, exp)
     int bytesize;
     tree exp;
{
  tree type, id;
  const char *name;

  if (bytesize < 6 || bytesize > 9)
    return 0;

  type = TREE_TYPE (TREE_TYPE (exp));
  if (TREE_CODE (type) == ARRAY_TYPE)
    type = TREE_TYPE (type);

  return TYPE_MODE (type) == QImode;

  id = TYPE_NAME (type);
  if (id == NULL_TREE)
    return 0;
  if (TREE_CODE (id) == TYPE_DECL)
    id = DECL_NAME (id);

  name = IDENTIFIER_POINTER (id);
  if (strcmp (name, "char") == 0
      || strcmp (name, "signed char") == 0
      || strcmp (name, "unsigned char") == 0)
    return 1;

  return 0;
}

#if XKL_STUFF
static void
maybe_warn_about_pointer_conversion (exp, from, to)
     tree exp;
     int from;
     int to;
{
  static int last_lineno = -1;

  if (exp == NULL ||
      VOID_TYPE_P (TREE_TYPE (TREE_TYPE (TREE_OPERAND (exp, 0)))))
    return;

  if (from <= 18 && to > from && to >= 18 && lineno > last_lineno)
    {
      if (to == 36)
	warning ("converting %d-bit byte pointer to word address", from);
      else
	warning ("converting %d-bit byte pointer to %d-bit byte pointer",
		 from, to);
    }

  last_lineno = lineno;
}
#else
#define maybe_warn_about_pointer_conversion(EXP, FROM, TO)
#endif

/* Emit instructions to perform the pointer conversion in EXP.  Store
   the result in TARGET if convenient, and return the result.  If OP0
   is not NULL_RTX, it's RTL for the unconverted pointer.  */
rtx
pdp10_convert_pointer (exp, op0, target, mod, stack, not_null)
     tree exp;
     rtx op0;
     rtx target ATTRIBUTE_UNUSED;
     int mod;
     int stack;
     int not_null;
{
  enum expand_modifier modifier = mod;
  int from, to, byte = 0, emit_jump;
  HOST_WIDE_INT subword_word = 0;
  int have_subword_word = 0;
  rtx label = gen_label_rtx ();

  if (stack)
    not_null = 1;

  /* Do not return immediately for EXPAND_INITIALIZER.  Constant
     pointer conversions, such as a string object's word address to a
     local byte pointer, must still be folded here.  Non-constant
     initializer conversions are rejected later before emitting code.  */

#ifdef ASM_NETBSD
  if (TREE_CODE (TREE_TYPE (exp)) == INTEGER_TYPE
      && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0))))
    {
      rtx temp1, temp2;
      int size, shift, base;

      if (!op0)
	op0 = expand_expr (TREE_OPERAND (exp, 0), NULL_RTX, Pmode, 0);

      size = pdp10_bytesize (TREE_TYPE (TREE_TYPE (TREE_OPERAND (exp, 0))));

      if (size == 36)
	return expand_shift (LSHIFT_EXPR, Pmode, op0, build_int_2 (2, 0),
			     NULL_RTX, 1);

      shift = 30 - (size == 18);
      base = ps_base_for_bytesize (size);

      temp1 = expand_shift (RSHIFT_EXPR, Pmode, op0, build_int_2 (shift, 0),
			    NULL_RTX, 1);
      temp1 = expand_binop (Pmode, sub_optab, temp1, GEN_INT (base), NULL_RTX,
			    1, OPTAB_LIB_WIDEN);
      temp1 = expand_binop (Pmode, and_optab, temp1, GEN_INT (0777777),
			    NULL_RTX, 1, OPTAB_LIB_WIDEN);
      temp2 = expand_shift (LSHIFT_EXPR, Pmode, op0, build_int_2 (2, 0),
			    NULL_RTX, 1);
      temp2 = expand_binop (Pmode, add_optab, temp2, temp1, NULL_RTX, 1,
			    OPTAB_LIB_WIDEN);
      temp2 = expand_binop (Pmode, and_optab, temp2, GEN_INT (07777777777),
			    NULL_RTX, 1, OPTAB_LIB_WIDEN);

      return temp2;
    }
  else if (POINTER_TYPE_P (TREE_TYPE (exp))
	   && TREE_CODE (TREE_TYPE (TREE_OPERAND (exp, 0))) == INTEGER_TYPE)
    {
      rtx temp1, temp2;
      int size, shift, base;

      if (!op0)
	op0 = expand_expr (TREE_OPERAND (exp, 0), NULL_RTX, Pmode, 0);

      size = pdp10_bytesize (TREE_TYPE (TREE_TYPE (exp)));

      if (size == 36)
	return expand_shift (RSHIFT_EXPR, Pmode, op0, build_int_2 (2, 0),
			     NULL_RTX, 1);

      shift = 30 - (size == 18);
      base = ps_base_for_bytesize (size);

      temp1 = expand_binop (Pmode, and_optab, op0, GEN_INT (3), NULL_RTX, 1,
			    OPTAB_LIB_WIDEN);
      temp1 = expand_binop (Pmode, add_optab, temp1, GEN_INT (base), NULL_RTX,
			    1, OPTAB_LIB_WIDEN);
      temp1 = expand_shift (LSHIFT_EXPR, Pmode, temp1, build_int_2 (shift, 0),
			    NULL_RTX, 1);
      temp2 = expand_shift (RSHIFT_EXPR, Pmode, op0, build_int_2 (2, 0),
			    NULL_RTX, 1);
      temp2 = expand_binop (Pmode, ior_optab, temp2, temp1, NULL_RTX, 1,
			    OPTAB_LIB_WIDEN);

      return temp2;
    }
#endif

  if (!(POINTER_TYPE_P (TREE_TYPE (exp))
	 && (stack || POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0))))))
    return NULL_RTX;

  {
    rtx init_addr = pdp10_initializer_byte_pointer (exp);
    if (init_addr != NULL_RTX)
      return init_addr;
  }

  /* A word pointer cast through a byte pointer and immediately back to
     a word pointer is a no-op.  Recognize this before expanding the
     inner conversion, otherwise (int *)(char *)x emits a useless
     word -> byte -> word conversion sequence, and worse, can skip the
     initializing move on the null path.

     Keep this deliberately narrow.  The opposite byte -> word -> byte
     sequence is not necessarily an identity on a PDP-10, because the
     intermediate word pointer discards the original byte position.  */
  if (!stack
      && (TREE_CODE (exp) == NOP_EXPR || TREE_CODE (exp) == CONVERT_EXPR)
      && (TREE_CODE (TREE_OPERAND (exp, 0)) == NOP_EXPR
	  || TREE_CODE (TREE_OPERAND (exp, 0)) == CONVERT_EXPR)
      && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0)))
      && POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (TREE_OPERAND (exp, 0), 0))))
    {
      tree inner = TREE_OPERAND (exp, 0);
      tree base = TREE_OPERAND (inner, 0);
      int to_size = pdp10_bytesize (TREE_TYPE (TREE_TYPE (exp)));
      int inner_size = pdp10_bytesize (TREE_TYPE (TREE_TYPE (inner)));
      int base_size = pdp10_bytesize (TREE_TYPE (TREE_TYPE (base)));

      if (base_size == 36 && inner_size != 36 && to_size == 36)
	return expand_expr (base, target, Pmode, modifier);
    }

  if (target_flags & OPT_VOIDP)
    {
      /* The XKL people want conversions to or from void * to do
         nothing.  */
      if (TREE_CODE (exp) == NOP_EXPR
	  && (VOID_TYPE_P (TREE_TYPE (TREE_TYPE (exp)))
	      || VOID_TYPE_P (TREE_TYPE (TREE_TYPE (TREE_OPERAND (exp, 0))))))
	return NULL_RTX;
    }

  to = pdp10_bytesize (TREE_TYPE (TREE_TYPE (exp)));
  if (stack)
    from = 36;
  else
    {
      from = pdp10_bytesize (TREE_TYPE (TREE_TYPE (TREE_OPERAND (exp, 0))));
      if (to == from)
	return NULL_RTX;
    }

  /* If converting an "((un)signed) charN *" to or from an
     "((un)signed) char *", do nothing.  */
  if ((from <= 9 && noconv_charp (to, exp))
      || (to <= 9 && noconv_charp (from, TREE_OPERAND (exp, 0))))
    return NULL_RTX;

  /* Dynamic sub-word addresses may start in the middle of a PDP-10 word
     even when the conversion expression is wrapped in NOP/CONVERT nodes.
     Try to recover the original byte position before falling back to the
     scalar right-byte rule.  */
  {
    int subword_byte;

    HOST_WIDE_INT word_offset;

    if (pdp10_addr_expr_array_position (exp, to, &subword_byte,
                                        &word_offset))
      {
        byte = subword_byte;
        subword_word = word_offset;
        have_subword_word = 1;
      }
    else if (TREE_CODE (exp) == ADDR_EXPR
	     && !AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0))))
      {
	/* Scalar char variables of all sub-9-bit sizes occupy one
	   9-bit C storage unit when addressed as standalone scalar
	   objects.  Build a pointer to the rightmost 9-bit byte, not
	   an 18-bit right-half pointer.  The 18-bit form can read stale
	   neighbouring bits from a reused stack word.  */
	if (to <= 9)
	  {
	    to = 9;
	    byte = BITS_PER_WORD / to - 1;
	  }
	else if (to == 16)
	  byte = PDP10_SCALAR_RIGHT_BYTE;
	else
	  byte = BITS_PER_WORD / to - 1;
      }
  }

  if (!op0)
    op0 = expand_expr (TREE_OPERAND (exp, 0), NULL_RTX, Pmode, 0);

  if (have_subword_word && subword_word != 0 && from == 36)
    op0 = plus_constant (op0, subword_word);

  if (from == to)
    return op0;

  if (CONSTANT_ADDRESS_P (op0))
    {
      pdp10_word36 offset = 0;
      pdp10_word36 addr;
      pdp10_word36 new_offset;
      int ps;
      rtx base;

      if (!pdp10_decompose_symbolic_word36 (op0, &base, &offset))
	{
	  if (modifier == EXPAND_INITIALIZER)
	    return NULL_RTX;
	}
      else if (!TARGET_EXTENDED)
	{
	  ps = (offset >> 24) & 07777;
	  addr = offset & ADDRESS_MASK;

	  if (pdp10_local_ps_for_conversion (ps, from, to, byte,
					       &ps, &from))
	    {
	      maybe_warn_about_pointer_conversion (exp, from, to);
	      new_offset = ((((pdp10_word36) ps) << 24) + addr)
		& PDP10_WORD36_MASK;
	      return base != NULL_RTX
		? pdp10_plus_constant_word36 (base, new_offset)
		: pdp10_gen_word36_constant (new_offset);
	    }

	  if (modifier == EXPAND_INITIALIZER)
	    return NULL_RTX;
	}
      else
	{
	  ps = (offset >> 30) & 077;
	  addr = offset & ADDRESS_MASK;

	  ps = pdp10_global_ps_for_conversion (ps, to, byte, &from);
	  maybe_warn_about_pointer_conversion (exp, from, to);

	  new_offset = ((((pdp10_word36) ps) << 30) + addr)
	    & PDP10_WORD36_MASK;
	  return base != NULL_RTX
	    ? pdp10_plus_constant_word36 (base, new_offset)
	    : pdp10_gen_word36_constant (new_offset);
	}
    }

  if (modifier == EXPAND_INITIALIZER)
    return NULL_RTX;

  {
    rtx old = op0;
    op0 = gen_reg_rtx (Pmode);
    emit_move_insn (op0, old);
  }

  emit_jump = !(not_null
		|| to == 36
		|| (to == 9 && from == 18 && TARGET_EXTENDED));
  if (emit_jump)
    emit_jump_insn (gen_cbranchsi (label, op0, GEN_INT (0), GEN_EQ));

  maybe_warn_about_pointer_conversion (exp, from, to);

  if (TARGET_EXTENDED)
    op0 = convert_global_pointer (op0, from, to, byte);
  else
    {
      if (stack)
	emit_insn (gen_andsi3 (op0, op0, GEN_INT (0777777)));
      op0 = convert_local_pointer (op0, from, to, byte);
    }

  if (emit_jump)
    emit_label (label);

  return op0;
}

/* Emit instructions to convert the pointer in EXP or OP0 (at least
   one must be non-NULL) from a pointer to bytes of FROM bits to a
   pointer to bytes of TO bits.  Store the result in TARGET if
   convenient, and return the result.  */
rtx
pdp10_convert_ptr (exp, op0, target, from, to, not_null)
     tree exp;
     rtx op0;
     rtx target ATTRIBUTE_UNUSED;
     int from;
     int to;
     int not_null;
{
  int byte = 0, emit_jump;
  HOST_WIDE_INT subword_word = 0;
  int have_subword_word = 0;
  rtx label = gen_label_rtx ();

  if (exp)
    {
      rtx init_addr = pdp10_initializer_byte_pointer (exp);
      if (init_addr != NULL_RTX)
        return init_addr;
    }

  /* Dynamic sub-word addresses may start in the middle of a PDP-10 word
     even when the conversion expression is wrapped in NOP/CONVERT nodes.
     Try to recover the original byte position before falling back to the
     scalar right-byte rule.  */
  {
    int subword_byte;

    HOST_WIDE_INT word_offset;

    if (pdp10_addr_expr_array_position (exp, to, &subword_byte,
                                        &word_offset))
      {
        byte = subword_byte;
        subword_word = word_offset;
        have_subword_word = 1;
      }
    else if (exp && TREE_CODE (exp) == ADDR_EXPR
	     && !AGGREGATE_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0))))
      {
	if (to <= 9)
	  {
	    to = 9;
	    byte = BITS_PER_WORD / to - 1;
	  }
	else if (to == 16)
	  byte = PDP10_SCALAR_RIGHT_BYTE;
	else
	  byte = BITS_PER_WORD / to - 1;
      }
  }

  if (!op0)
    op0 = expand_expr (TREE_OPERAND (exp, 0), NULL_RTX, Pmode, 0);

  if (have_subword_word && subword_word != 0 && from == 36)
    op0 = plus_constant (op0, subword_word);

  if (from == to)
    return op0;

  if (from == 32)
    from = 36;

  if (to == 32)
    to = 36;

  if (CONSTANT_ADDRESS_P (op0))
    {
      pdp10_word36 offset = 0;
      pdp10_word36 addr;
      pdp10_word36 new_offset;
      int ps;
      rtx base;

      if (pdp10_decompose_symbolic_word36 (op0, &base, &offset))
	{
	  if (!TARGET_EXTENDED)
	    {
	      ps = (offset >> 24) & 07777;
	      addr = offset & ADDRESS_MASK;

	      if (pdp10_local_ps_for_conversion (ps, from, to, byte,
					       &ps, &from))
		{
		  maybe_warn_about_pointer_conversion (exp, from, to);
		  new_offset = ((((pdp10_word36) ps) << 24) + addr)
		    & PDP10_WORD36_MASK;
		  return base != NULL_RTX
		    ? pdp10_plus_constant_word36 (base, new_offset)
		    : pdp10_gen_word36_constant (new_offset);
		}
	    }
	  else
	    {
	      ps = (offset >> 30) & 077;
	      addr = offset & ADDRESS_MASK;

	      ps = pdp10_global_ps_for_conversion (ps, to, byte, &from);
	      maybe_warn_about_pointer_conversion (exp, from, to);

	      new_offset = ((((pdp10_word36) ps) << 30) + addr)
		& PDP10_WORD36_MASK;
	      return base != NULL_RTX
		? pdp10_plus_constant_word36 (base, new_offset)
		: pdp10_gen_word36_constant (new_offset);
	    }
	}
    }

  {
    rtx old = op0;
    op0 = gen_reg_rtx (Pmode);
    emit_move_insn (op0, old);
  }

  emit_jump = !(not_null
		|| to == 36
		|| (to == 9 && from == 18 && TARGET_EXTENDED));
  if (emit_jump)
    emit_jump_insn (gen_cbranchsi (label, op0, GEN_INT (0), GEN_EQ));

  maybe_warn_about_pointer_conversion (exp, from, to);

  if (TARGET_EXTENDED)
    op0 = convert_global_pointer (op0, from, to, byte);
  else
    op0 = convert_local_pointer (op0, from, to, byte);

  if (emit_jump)
    emit_label (label);

  return op0;
}

/* Expand the tree ADDR_EXP tree expression in EXP into RTL instructions.
   Store the result in TARGET if convenient, and return the result.  */
rtx
pdp10_expand_ADDR_EXP (exp, type, target, ignore, mod)
     tree exp;
     tree type;
     rtx target ATTRIBUTE_UNUSED;
     int ignore;
     int mod;
{
  enum expand_modifier modifier = mod;
  rtx op0, temp;

  /* If nonzero, TEMP will be set to the address of something that might
     be a MEM corresponding to a stack slot.  */
  temp = 0;

  {
    rtx init_addr = pdp10_initializer_byte_pointer (exp);
    if (init_addr != NULL_RTX)
      return init_addr;
  }

  if (POINTER_TYPE_P (TREE_TYPE (exp))
      && (TREE_CODE (TREE_OPERAND (exp, 0)) == ARRAY_REF
          || TREE_CODE (TREE_OPERAND (exp, 0)) == COMPONENT_REF))
    {
      rtx dyn_addr = pdp10_dynamic_addr_expr_byte_pointer
        (exp, pdp10_bytesize (TREE_TYPE (TREE_TYPE (exp))));
      if (dyn_addr != NULL_RTX)
        return dyn_addr;
    }

  /* Are we taking the address of a nested function?  */
  if (TREE_CODE (TREE_OPERAND (exp, 0)) == FUNCTION_DECL
      && decl_function_context (TREE_OPERAND (exp, 0)) != 0
      && !DECL_NO_STATIC_CHAIN (TREE_OPERAND (exp, 0))
      && !TREE_STATIC (exp))
    op0 = trampoline_address (TREE_OPERAND (exp, 0));
  /* If we are taking the address of something erroneous, just
     return a zero.  */
  else if (TREE_CODE (TREE_OPERAND (exp, 0)) == ERROR_MARK)
    return const0_rtx;
  else
    {
      /* We make sure to pass const0_rtx down if we came in with
	 ignore set, to avoid doing the cleanups twice for something.  */
      op0 = expand_expr (TREE_OPERAND (exp, 0),
			 ignore ? const0_rtx : NULL_RTX, VOIDmode,
			 (modifier == EXPAND_INITIALIZER
			  ? modifier : EXPAND_CONST_ADDRESS));

      /* If we are going to ignore the result, OP0 will have been set
	 to const0_rtx, so just return it.  Don't get confused and
	 think we are taking the address of the constant.  */
      if (ignore)
	return op0;

      /* Pass 1 for MODIFY, so that protect_from_queue doesn't get
	 clever and returns a REG when given a MEM.  */
      op0 = protect_from_queue (op0, 1);

      /* We would like the object in memory.  If it is a constant, we can
	 have it be statically allocated into memory.  For a non-constant,
	 we need to allocate some memory and store the value into it.  */

      if (CONSTANT_P (op0))
	op0 = force_const_mem (TYPE_MODE (TREE_TYPE (TREE_OPERAND (exp, 0))),
			       op0);
      else if (GET_CODE (op0) == MEM)
	{
	  mark_temp_addr_taken (op0);
	  temp = XEXP (op0, 0);
	}

      else if (GET_CODE (op0) == REG
	       || (GET_CODE (op0) == SUBREG
		   && GET_CODE (XEXP (op0, 0)) != ZERO_EXTRACT
		   && GET_CODE (XEXP (XEXP (op0, 0), 0)) != MEM)
	       || GET_CODE (op0) == CONCAT || GET_CODE (op0) == ADDRESSOF
	       || GET_CODE (op0) == PARALLEL)
	{
	  /* If this object is in a register, it must be not
	     be BLKmode.  */
	  tree inner_type = TREE_TYPE (TREE_OPERAND (exp, 0));
	  tree nt = build_qualified_type (inner_type,
					  (TYPE_QUALS (inner_type)
					   | TYPE_QUAL_CONST));
	  rtx memloc = assign_temp (nt, 1, 1, 1);

	  mark_temp_addr_taken (memloc);
	  if (GET_CODE (op0) == PARALLEL)
	    /* Handle calls that pass values in multiple non-contiguous
	       locations.  The Irix 6 ABI has examples of this.  */
	    emit_group_store (memloc, op0,
			      int_size_in_bytes (inner_type));
	  else
	    emit_move_insn (memloc, op0);
	  op0 = memloc;
	}

      if (GET_CODE (op0) == SUBREG && GET_CODE (XEXP (op0, 0)) == ZERO_EXTRACT)
	{
	  rtx addr = pdp10_address_from_zero_extract (XEXP (op0, 0), exp);

	  if (addr != NULL_RTX)
	    return addr;

	  op0 = XEXP (XEXP (op0, 0), 0);
	}

      if (GET_CODE (op0) != MEM)
	abort ();

      op0 = XEXP (op0, 0);

      if (modifier == EXPAND_INITIALIZER || CONSTANT_ADDRESS_P (op0))
	{
	  rtx temp2;

	  temp2 = pdp10_convert_pointer (exp, op0, NULL_RTX,
					   EXPAND_INITIALIZER, 1, 1);
	  if (temp2)
	    op0 = temp2;
	  return op0;
	}
    }

  switch (GET_CODE (op0))
    {
    case REG:
      if (REGNO_PTR_FRAME_P (REGNO (op0)))
	{
	  rtx x = gen_reg_rtx (Pmode);
	  emit_insn (gen_MOVEI (x, op0));
	  op0 = x;
	  break;
	}
      /* Fall through.  */
    case SUBREG:
    case MEM:
    case ADDRESSOF:
      break;
    case PLUS:
      {
	rtx temp = gen_reg_rtx (Pmode);
	if (!address_operand (op0, Pmode))
	  op0 = force_reg (Pmode, op0);
	emit_insn (gen_MOVEI (temp, op0));
	op0 = temp;
      }
      break;
    case ZERO_EXTRACT:
      if (GET_CODE (XEXP (op0, 0)) != MEM)
	abort ();
      op0 = XEXP (XEXP (op0, 0), 0);
      break;
    default:
      if (CONSTANT_P (op0))
	break;
      abort ();
    }

  if (GET_CODE (op0) != LABEL_REF)
    {
      rtx temp = pdp10_convert_pointer (exp, op0, NULL_RTX, EXPAND_NORMAL,
					1, 1);
      if (temp)
	op0 = temp;
    }

  if (flag_force_addr && GET_CODE (op0) != REG)
    op0 = force_reg (Pmode, op0);

  if (GET_CODE (op0) == REG
      && !REG_USERVAR_P (op0))
    mark_reg_pointer (op0, TYPE_ALIGN (TREE_TYPE (type)));

  /* If we might have had a temp slot, add an equivalent address
     for it.  */
  if (temp != 0)
    update_temp_slot_address (temp, op0);

  return op0;
}

/* Adjust the 12-bit P and S field (or PS code) in PS by *ADJUST
   bytes.  Return the new P and S field (or -1 on failure), and the
   store the number of words to adjust a word pointer in *ADJUST.  */
int
adjust_ps (ps, adjust)
     int ps;
     int *adjust;
{
  int s, n;

  if (ps >= 04600)
    {
      int base, oldps;

      if (ps & 077)
	return -1;

      ps = ps >> 6;
      switch (ps)
	{
	case 046: case 047: case 050: case 051:	case 052: case 053:
	  base = 046;
	  s = 6;
	  break;
	case 062: case 063: case 064: case 065: case 066:
	  base = 062;
	  s = 7;
	  break;
	case 055: case 056: case 057: case 060:
	  base = 055;
	  s = 8;
	  break;
	case 070: case 071: case 072: case 073:
	  base = 070;
	  s = 9;
	  break;
	case 075: case 076:
	  base = 075;
	  s = 18;
	  break;
	default:
	  return -1;
	}

      n = BITS_PER_WORD / s;
      oldps = ps - base;
      ps = (*adjust + oldps) % n;
      if (ps < 0)
	{
	  ps += n;
	  *adjust -= n;
	}
      else if (ps < oldps)
	{
	  *adjust += n;
	}
      ps += base;
      ps <<= 6;
      *adjust /= n;
    }
  else
    {
      int p = ps >> 6;
      int byte_pos;
      int total;

      s = ps & 077;

      if (!VALID_OWGBP_BYTE_SIZE_P (s))
	return -1;

      byte_pos = BITS_PER_WORD - p - s;
      if (byte_pos < 0 || byte_pos + s > BITS_PER_WORD)
	return -1;

      /* Local P,S byte pointers use the actual bit position of the
	 selected byte, not an extended byte-number code.  Seven- and
	 eight-bit byte arrays therefore have perfectly valid positions
	 whose P field is not divisible by S: POINT 7,foo,35 is the
	 first 7-bit byte in a word, and POINT 8,foo,34 is the first
	 8-bit byte.  The old p % s check made constant ADJBP folding
	 fail for static 7/8-bit byte-pointer initializers.

	 Only fold canonical byte-array slots here.  Direct address-of
	 noncanonical fields is still handled by the initializer byte
	 pointer builder; pointer arithmetic through ADJBP should not
	 invent a guessed rollover rule for arbitrary bit positions.  */
      if (byte_pos % s != 0)
	return -1;

      n = BITS_PER_WORD / s;
      total = byte_pos / s + *adjust;
      if (total >= 0)
	{
	  *adjust = total / n;
	  total %= n;
	}
      else
	{
	  *adjust = - ((-total + n - 1) / n);
	  total -= *adjust * n;
	}

      p = BITS_PER_WORD - total * s - s;
      ps = (p << 6) + s;
    }

  return ps;
}

/* Called by the MD_SIMPLIFY_UNSPEC macro.  */
rtx
pdp10_simplify_unspec (x, insn)
     rtx x;
     rtx insn;
{
  switch (XINT (x, 1))
    {
    case UNSPEC_ADJBP:
      {
	HOST_WIDE_INT offset;
	rtx pointer = XVECEXP (x, 0, 0);
	rtx address = NULL_RTX;
	rtx adj = XVECEXP (x, 0, 1);
	int ps, adjust;

	/* If the pointer adjustment is a register, find the last
	   value that register was set to.  */
	if (GET_CODE (adj) == REG)
	  {
	    rtx i = insn;
	    if (insn == NULL_RTX)
	      break;
	    adj = pdp10_find_last_value (adj, &i);
	  }
	if (adj == NULL_RTX || GET_CODE (adj) != CONST_INT)
	  break;

	adjust = INTVAL (adj);
	if (adjust == 0)
	  return pointer;

	/* If the pointer to be adjusted is a register, find the last
	   value that register was set to.  */
	if (GET_CODE (pointer) == REG)
	  {
	    if (insn == NULL_RTX)
	      break;
	    pointer = pdp10_find_last_value (pointer, &insn);
	    if (pointer == NULL_RTX)
	      break;
	  }

	{
	  rtx base;
	  pdp10_word36 word36_offset;

	  if (!pdp10_decompose_symbolic_word36 (pointer, &base, &word36_offset)
	      || base == NULL_RTX)
	    break;

	  ps = (word36_offset >> 24) & (TARGET_EXTENDED ? 07700 : 07777);
	  offset = word36_offset & ADDRESS_MASK;
	  pointer = base;
	}

	/* If the pointer inside the plus expression is a register,
	   find the last value that register was set to.  */
	if (GET_CODE (pointer) == REG)
	  {
	    address = pdp10_find_last_value (pointer, &insn);
	    if (address == NULL_RTX
		|| GET_CODE (address) != UNSPEC
		|| XINT (address, 1) != UNSPEC_ADDRESS)
	      break;
	  }

	/* Adjust the PS field.  */
	ps = adjust_ps (ps, &adjust);
	if (ps == -1)
	  break;

	/* If there was a word offset, add it to the pointer.  Keep the
	   PDP-10 P,S selector in full 36-bit form; HOST_WIDE_INT may be
	   only 32 bits on the build host.  */
	offset += adjust;
	{
	  pdp10_word36 new_offset;

	  new_offset = ((((pdp10_word36) ps) << 24)
			+ ((pdp10_word36) offset & ADDRESS_MASK))
	    & PDP10_WORD36_MASK;
	  return gen_rtx_CONST (Pmode,
				gen_rtx_PLUS (Pmode, pointer,
					      pdp10_gen_word36_constant (new_offset)));
	}
      }
      break;
    }

  return NULL_RTX;
}

/* Add to *OFFSET the offset in storage units resulting from the array
   reference made by EXP.  INDEX is the array index, counting from
   zero.  UNIT_SIZE is the size of an array element, in storage units.
   If an offset in bits is desired, *BIT_OFFSET may be added to
   instead, but note that the sum must be a constant expression in
   this case.  */
void pdp10_md_array_offset (tree exp, tree index, tree unit_size,
			    tree *offset, tree *bit_offset)
{
  if (!TREE_CONSTANT (index) || !TREE_CONSTANT (unit_size))
    {
      tree elem_type = TREE_TYPE (exp);

      /* For arrays of aggregates, keep the offset in GCC addressable
	 units.  Component references below the ARRAY_REF still need the
	 byte offset so subword fields inside the aggregate land in the
	 proper PDP-10 word and byte position.  Scalar arrays, on the
	 other hand, address whole PDP-10 words/halfwords directly.  */
      if (0
	  && (TREE_CODE (elem_type) == RECORD_TYPE
	      || TREE_CODE (elem_type) == UNION_TYPE)
	  && tree_low_cst (unit_size, 1) >= UNITS_PER_WORD
	  && (tree_low_cst (unit_size, 1) % UNITS_PER_WORD) == 0)
	unit_size = size_binop (TRUNC_DIV_EXPR,
				unit_size,
				convert (sizetype,
					 build_int_2 (2, 0)));
      else if (TREE_CODE (elem_type) != RECORD_TYPE
	       && TREE_CODE (elem_type) != UNION_TYPE
	       && tree_low_cst (unit_size, 1) >= UNITS_PER_WORD)
	unit_size = size_binop (TRUNC_DIV_EXPR,
				unit_size,
				convert (sizetype,
					 build_int_2 (UNITS_PER_WORD, 0)));
      else if (TREE_CODE (elem_type) != RECORD_TYPE
	       && TREE_CODE (elem_type) != UNION_TYPE
	       && tree_low_cst (unit_size, 1) == 2)
	unit_size = size_binop (TRUNC_DIV_EXPR,
				unit_size,
				convert (sizetype,
					 build_int_2 (2, 0)));
#if 0
      *offset = size_binop (TRUNC_DIV_EXPR, *offset,
			    convert (sizetype,
				     build_int_2 (UNITS_PER_WORD, 0)));
#endif
      *offset = size_binop (PLUS_EXPR, *offset,
			    size_binop (MULT_EXPR,
					convert (sizetype, index),
					unit_size));
    }
  else
    {
      tree unit_bitsize = TYPE_SIZE (TREE_TYPE (exp));
      tree bits_per_word = convert (bitsizetype,
				    build_int_2 (BITS_PER_WORD, 0));
      tree leftover_bits = bitsize_zero_node;

      if (!TREE_CONSTANT (unit_bitsize))
	abort ();


      /* Native 9-bit QImode arrays occupy one PDP-10 storage unit per
	 element, but constant subscripts inside aggregate objects still select
	 bytes inside PDP-10 words.  Keep the byte index in BIT_OFFSET so global
	 component stores such as s.a[2] become POINT ...,26 instead of advancing
	 the word address by two.  */
      if (compare_tree_int (unit_bitsize, BITS_PER_UNIT) == 0)
	{
	  *bit_offset = size_binop (PLUS_EXPR, *bit_offset,
				     size_binop (MULT_EXPR,
						 convert (bitsizetype, index),
						 unit_bitsize));
	  return;
	}

      *bit_offset = size_binop (PLUS_EXPR, *bit_offset,
				size_binop (MULT_EXPR,
					    convert (bitsizetype,
						     index),
					    unit_bitsize));

      if (compare_tree_int (unit_bitsize, BITS_PER_WORD) == -1)
	leftover_bits = size_binop (FLOOR_MOD_EXPR, bits_per_word,
				    unit_bitsize);
      if (compare_tree_int (leftover_bits, 0) != 0)
	{
	  tree units_per_word = size_binop (FLOOR_DIV_EXPR,
					    bits_per_word,
					    unit_bitsize);
	  tree words = size_binop (FLOOR_DIV_EXPR,
				   convert (bitsizetype, index),
				   units_per_word);
	  *bit_offset = size_binop (PLUS_EXPR, *bit_offset,
				    size_binop (MULT_EXPR, words,
						leftover_bits));
	}
    }
}

static void
emit_cmp_and_jump_insn_1 (x, y, mode, comparison, unsignedp, label)
     rtx x, y;
     enum machine_mode mode;
     enum rtx_code comparison;
     int unsignedp;
     rtx label;
{
  rtx test = gen_rtx_fmt_ee (comparison, mode, x, y);
  enum mode_class class = GET_MODE_CLASS (mode);
  enum machine_mode wider_mode = mode;

  /* Try combined insns first.  */
  do
    {
      enum insn_code icode;
      PUT_MODE (test, wider_mode);

      if (label)
	{	  
	  icode = cbranch_optab->handlers[(int)wider_mode].insn_code;
	  
	  if (icode != CODE_FOR_nothing
	      && (*insn_data[icode].operand[0].predicate) (test, wider_mode))
	    {
	      x = prepare_operand (icode, x, 1, mode, wider_mode, unsignedp);
	      y = prepare_operand (icode, y, 2, mode, wider_mode, unsignedp);
	      emit_jump_insn (GEN_FCN (icode) (test, x, y, label));
	      return;
	    }
	}

      /* Handle some compares against zero.  */
      icode = (int) tst_optab->handlers[(int) wider_mode].insn_code;
      if (y == CONST0_RTX (mode) && icode != CODE_FOR_nothing)
	{
	  x = prepare_operand (icode, x, 0, mode, wider_mode, unsignedp);
	  emit_insn (GEN_FCN (icode) (x));
	  if (label)
	    emit_jump_insn ((*bcc_gen_fctn[(int) comparison]) (label));
	  return;
	}

      /* Handle compares for which there is a directly suitable insn.  */

      icode = (int) cmp_optab->handlers[(int) wider_mode].insn_code;
      if (icode != CODE_FOR_nothing)
	{
	  x = prepare_operand (icode, x, 0, mode, wider_mode, unsignedp);
	  y = prepare_operand (icode, y, 1, mode, wider_mode, unsignedp);
	  emit_insn (GEN_FCN (icode) (x, y));
	  if (label)
	    emit_jump_insn ((*bcc_gen_fctn[(int) comparison]) (label));
	  return;
	}

      if (class != MODE_INT && class != MODE_FLOAT
	  && class != MODE_COMPLEX_FLOAT)
	break;

      wider_mode = GET_MODE_WIDER_MODE (wider_mode);
    } while (wider_mode != VOIDmode);

  abort ();
}

static void
pdp10_emit_cmp_and_jump_insns (exp, x, y, comparison, size, mode, unsignedp,
			       label)
     tree exp;
     rtx x, y;
     enum rtx_code comparison;
     rtx size;
     enum machine_mode mode;
     int unsignedp;
     rtx label;
{
  rtx op0 = x, op1 = y;

  /* Swap operands and condition to ensure canonical RTL.  */
  if (swap_commutative_operands_p (x, y))
    {
      /* If we're not emitting a branch, this means some caller
         is out of sync.  */
      if (!label)
	abort ();

      op0 = y, op1 = x;
      comparison = swap_condition (comparison);
    }

#ifdef HAVE_cc0
  /* If OP0 is still a constant, then both X and Y must be constants.  Force
     X into a register to avoid aborting in emit_cmp_insn due to non-canonical
     RTL.  */
  if (CONSTANT_P (op0))
    op0 = force_reg (mode, op0);
#endif

  emit_queue ();
  prepare_cmp_insn (&op0, &op1, &comparison, size, &mode, &unsignedp,
		    ccp_jump);

  if (WORD_TYPE_P (TREE_TYPE (TREE_TYPE (TREE_OPERAND (exp, 0))))
      || comparison == EQ || comparison == NE)
    emit_cmp_and_jump_insn_1 (op0, op1, mode, comparison, unsignedp, label);
  else if (HAVE_CMPBP)
    {
      rtx comparison_rtx = gen_rtx_fmt_ee (comparison, VOIDmode, 0, 0);
      if (!register_operand (op0, Pmode))
	op0 = force_reg (Pmode, op0);
      if (!reg_or_mem_operand (op1, Pmode))
	op1 = force_reg (Pmode, op1);
      emit_jump_insn (gen_CMPBP (comparison_rtx, op0, op1, label));
    }
  else
    {
      rtx temp0 = gen_reg_rtx (Pmode);
      rtx temp1 = gen_reg_rtx (Pmode);
      rtx temp0a, temp1a;

      if (TARGET_EXTENDED)
	{
	  temp0a = force_reg (Pmode, op0);
	  temp1a = force_reg (Pmode, op1);
	}
      else
	{
	  temp0a = gen_reg_rtx (Pmode);
	  temp1a = gen_reg_rtx (Pmode);
	  emit_insn (gen_xorsi3 (temp0a, op0,
				 gen_int_mode (HWINT (077) << 30, SImode)));
	  emit_insn (gen_xorsi3 (temp1a, op1,
				 gen_int_mode (HWINT (077) << 30, SImode)));
	}

      emit_insn (gen_rotlsi3 (temp0, temp0a, GEN_INT (6)));
      emit_insn (gen_rotlsi3 (temp1, temp1a, GEN_INT (6)));
      emit_cmp_and_jump_insn_1 (temp0, temp1, mode, comparison, unsignedp,
				label);
    }
}

void
pdp10_compare_and_jump (exp, op0, op1, code, unsignedp, mode, size,
			if_false_label, if_true_label)
     tree exp;
     rtx op0, op1;
     enum rtx_code code;
     int unsignedp;
     enum machine_mode mode;
     rtx size;
     rtx if_false_label, if_true_label;
{
  rtx tem;
  int dummy_true_label = 0;

  if (!POINTER_TYPE_P (TREE_TYPE (TREE_OPERAND (exp, 0))))
    {
      do_compare_rtx_and_jump (op0, op1, code, unsignedp, mode, size,
			       if_false_label, if_true_label);
      return;
    }

  /* No need to make pointer comparisons unsigned.  */
  code = signed_condition (code);

  /* Reverse the comparison if that is safe and we want to jump if it is
     false.  */
  if (!if_true_label && !FLOAT_MODE_P (mode))
    {
      if_true_label = if_false_label;
      if_false_label = 0;
      code = reverse_condition (code);
    }

  /* If one operand is constant, make it the second one.  Only do this
     if the other operand is not constant as well.  */

  if (swap_commutative_operands_p (op0, op1))
    {
      tem = op0;
      op0 = op1;
      op1 = tem;
      code = swap_condition (code);
    }

  if (flag_force_mem)
    {
      op0 = force_not_mem (op0);
      op1 = force_not_mem (op1);
    }

  do_pending_stack_adjust ();

  if (GET_CODE (op0) == CONST_INT && GET_CODE (op1) == CONST_INT
      && (tem = simplify_relational_operation (code, mode, op0, op1)) != 0)
    {
      if (tem == const_true_rtx)
	{
	  if (if_true_label)
	    emit_jump (if_true_label);
	}
      else
	{
	  if (if_false_label)
	    emit_jump (if_false_label);
	}
      return;
    }

#if 0
  /* There's no need to do this now that combine.c can eliminate lots of
     sign extensions.  This can be less efficient in certain cases on other
     machines.  */

  /* If this is a signed equality comparison, we can do it as an
     unsigned comparison since zero-extension is cheaper than sign
     extension and comparisons with zero are done as unsigned.  This is
     the case even on machines that can do fast sign extension, since
     zero-extension is easier to combine with other operations than
     sign-extension is.  If we are comparing against a constant, we must
     convert it to what it would look like unsigned.  */
  if ((code == EQ || code == NE) && !unsignedp
      && GET_MODE_BITSIZE (GET_MODE (op0)) <= HOST_BITS_PER_WIDE_INT)
    {
      if (GET_CODE (op1) == CONST_INT
	  && (INTVAL (op1) & GET_MODE_MASK (GET_MODE (op0))) != INTVAL (op1))
	op1 = GEN_INT (INTVAL (op1) & GET_MODE_MASK (GET_MODE (op0)));
      unsignedp = 1;
    }
#endif

  if (!if_true_label)
    {
      dummy_true_label = 1;
      if_true_label = gen_label_rtx ();
    }

  pdp10_emit_cmp_and_jump_insns (exp, op0, op1, code, size, mode, unsignedp,
                                 if_true_label);

  if (if_false_label)
    emit_jump (if_false_label);
  if (dummy_true_label)
    emit_label (if_true_label);
  }

  /* Called by the ASM_OUTPUT_ALIGN macro.  */
  void
  pdp10_asm_output_align (file, power)
     FILE *file;
     int power;
  {
#ifdef __PDP10_GAS_H__
  pdp10_output_byte (file, NULL_RTX, 0);
#endif
  if (power == 2 && in_text_section ())
    return;
  fprintf (file, "\t.align\t%d\n", power);
  }


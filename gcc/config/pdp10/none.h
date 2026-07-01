/* Freestanding DAIMOS/PDP-10 driver specifications.  */
#ifndef __PDP10_NONE_H__
#define __PDP10_NONE_H__

/* DAS defaults to final DXR output.  Compiler assembly must instead produce
   relocatable DOBJ members for DARC/DLINK.  */
#undef ASM_SPEC
#define ASM_SPEC "-c"

/* The bare target uses the DOBJ/DARC linker from pdp10-tools.  Do not route
   links through collect2/host ld: those tools do not understand PDP-10 DOBJ
   objects.  dlink also has no -L/-l search syntax, so have the GCC driver
   locate libgcc.a itself and pass the full archive pathname.  */
#undef LINKER_NAME
#define LINKER_NAME "dlink"

#define LINK_LIBGCC_SPECIAL 1

#undef STARTFILE_SPEC
#define STARTFILE_SPEC ""

#undef ENDFILE_SPEC
#define ENDFILE_SPEC ""

#undef LIB_SPEC
#define LIB_SPEC ""

#undef LINK_GCC_C_SEQUENCE_SPEC
#define LINK_GCC_C_SEQUENCE_SPEC "%G"

#endif /* __PDP10_NONE_H__ */

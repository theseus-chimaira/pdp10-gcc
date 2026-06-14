/* Freestanding DAIMOS/PDP-10 driver specifications.  */
#ifndef __PDP10_NONE_H__
#define __PDP10_NONE_H__

/* DAS defaults to final DXR output.  Compiler assembly must instead produce
   relocatable DOBJ members for DARC/DLINK.  */
#undef ASM_SPEC
#define ASM_SPEC "-c"

#endif /* __PDP10_NONE_H__ */

/*
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Lars Brinkhoff (lars@nocrew.org), funded by XKL, LLC.

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

/* A C string representing the suffix for object files in TOPS-20.  */
#define HOST_OBJECT_SUFFIX ".REL"

#undef CPP_PREDEFINES
#if XKL_STUFF
#define CPP_PREDEFINES  "-D__PDP10__ -D__PDP10 -D__TOPS20__ -D__TOPS20 -DGNUCC -DGCC -Amachine=pdp10"
#else
#define CPP_PREDEFINES  "-D__PDP10__ -D__PDP10 -D__TOPS20__ -D__TOPS20 -Amachine=pdp10"
#endif

/* A C string representing the suffix for executable files in TOPS-20.  */
#define HOST_EXECUTABLE_SUFFIX ".EXE"

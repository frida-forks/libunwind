/*
 * Copyright (C) 2026 libunwind contributors
 *
 * This file is part of libunwind.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "config.h"

#include "unwind_i.h"
#include "offsets.h"

int
unw_is_signal_frame (unw_cursor_t *cursor)
{
  /* Not supported yet */
  return 0;
}

HIDDEN int
x86_handle_signal_frame (unw_cursor_t *cursor)
{
  return -UNW_EUNSPEC;
}

HIDDEN dwarf_loc_t
x86_get_scratch_loc (struct cursor *c, unw_regnum_t reg)
{
  /* Signal frames are not recognized, so there is never a scratch area.  */
  return DWARF_REG_LOC (&c->dwarf, reg);
}

#ifndef UNW_REMOTE_ONLY
HIDDEN void *
x86_r_uc_addr (ucontext_t *uc, int reg)
{
  void *addr;

  switch (reg)
    {
    case UNW_X86_EAX: addr = &uc->uc_mcontext.cpu.eax; break;
    case UNW_X86_EBX: addr = &uc->uc_mcontext.cpu.ebx; break;
    case UNW_X86_ECX: addr = &uc->uc_mcontext.cpu.ecx; break;
    case UNW_X86_EDX: addr = &uc->uc_mcontext.cpu.edx; break;
    case UNW_X86_ESI: addr = &uc->uc_mcontext.cpu.esi; break;
    case UNW_X86_EDI: addr = &uc->uc_mcontext.cpu.edi; break;
    case UNW_X86_EBP: addr = &uc->uc_mcontext.cpu.ebp; break;
    case UNW_X86_EIP: addr = &uc->uc_mcontext.cpu.eip; break;
    case UNW_X86_ESP: addr = &uc->uc_mcontext.cpu.esp; break;
    case UNW_X86_CS:  addr = &uc->uc_mcontext.cpu.cs; break;
    case UNW_X86_EFLAGS: addr = &uc->uc_mcontext.cpu.efl; break;
    case UNW_X86_SS:  addr = &uc->uc_mcontext.cpu.ss; break;

    default:
      addr = NULL;
    }
  return addr;
}

HIDDEN int
x86_local_resume (unw_addr_space_t as UNUSED, unw_cursor_t *cursor, void *arg UNUSED)
{
  struct cursor *c = (struct cursor *) cursor;
  ucontext_t *uc = c->uc;

  Debug (8, "resuming at ip=%x via setcontext()\n", c->dwarf.ip);
  _Ux86_setcontext (uc);
  return -UNW_EINVAL;
}
#endif

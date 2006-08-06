// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// Linking Gnash statically or dynamically with other modules is making a
// combined work based on Gnash. Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.
//
// As a special exception, the copyright holders of Gnash give you
// permission to combine Gnash with free software programs or libraries
// that are released under the GNU LGPL and with code included in any
// release of Talkback distributed by the Mozilla Foundation. You may
// copy and distribute such a system following the terms of the GNU GPL
// for all but the LGPL-covered parts and Talkback, and following the
// LGPL for the LGPL-covered parts.
//
// Note that people who make modified versions of Gnash are not obligated
// to grant this special exception for their modified versions; it is their
// choice whether to do so. The GNU General Public License gives permission
// to release a modified version without this exception; this exception
// also makes it possible to release a modified version which carries
// forward this exception.
// 
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_DMALLOC
#include "utility.h"
#include "dlmalloc.h"

// Overrides of new/delete that use Doug Lea's malloc.  Very helpful
// on certain lame platforms.

void*	operator new(size_t size)
{
	return dlmalloc(size);
}

void	operator delete(void* ptr)
{
	if (ptr) dlfree(ptr);
}

void*	operator new[](size_t size)
{
	return dlmalloc(size);
}

void	operator delete[](void* ptr)
{
	if (ptr) dlfree(ptr);
}
// end of HAVE_DMALLOC
#endif


void dump_memory_stats(const char */* from */ , int /* line */, const char */* label */) 
// Dump the internal statistics from malloc() so we can track memory leaks
{

  
// This does not work with DMALLOC, since the internal data structures
// differ.
#ifdef HAVE_DMALLOC
#ifdef HAVE_MALLINFO

	struct mallinfo mi;
	static int allocated = 0;
	static int freeb = 0;
  
	mi = mallinfo();
	if (label != 0) {
		printf("Malloc Statistics from %s() (line #%d): %s\n", from, line, label);
	} else { 
		printf("Malloc Statistics from %s() (line #%d):\n", from, line);
	}
  
	//printf("\tnon-mapped space from system:  %d\n", mi.arena);
	printf("\ttotal allocated space:         %d\n", mi.uordblks);
	printf("\ttotal free space:              %d\n", mi.fordblks);
	//printf("\tspace in mmapped regions:      %d\n", mi.hblkhd);
	//printf("\ttop-most, releasable space:    %d\n", mi.keepcost); // Prints 78824
	if (freeb != mi.fordblks) {
		printf("\t%d bytes difference in free space.\n", freeb - mi.fordblks);
		freeb = mi.fordblks;
	}

	//if (allocated != mi.uordblks) {
	//  printf("\t%d bytes difference in allocated space.\n", mi.uordblks - allocated);
	//  allocated = mi.uordblks;
	//}  

#endif // HAVE_DMALLOC
#endif // HAVE_MALLINFO
}

// Local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

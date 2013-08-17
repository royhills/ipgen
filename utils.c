/*
 * ipgen is Copyright (C) 2012 Roy Hills, NTA Monitor Ltd.
 *
 * This file is part of ipgen.
 * 
 * ipgen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ipgen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ipgen.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You are encouraged to send comments, improvements or suggestions to
 * me at ipgen@nta-monitor.com.
 *
 * Author: Roy Hills
 * Date: 5 April 2004
 *
 * This file contains various utility functions used by ipgen.
 */

#include "ipgen.h"

/*
 *	dupstr -- duplicate a string
 *
 *	Inputs:
 *
 *	str	The string to duplcate
 *
 *	Returns:
 *
 *	A pointer to the duplicate string.
 *
 *	This is a replacement for the common but non-standard "strdup"
 *	function.
 *
 *	The returned pointer points to Malloc'ed memory, which must be
 *	free'ed by the caller.
 */
char *
dupstr(const char *str) {
   char *cp;
   size_t len;

   len = strlen(str) + 1;	/* Allow space for terminating NULL */
   cp = Malloc(len);
   strlcpy(cp, str, len);
   return cp;
}

/*
 * "$Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/mxml-2.4/mxml-private.h#1 $"
 *
 * Private definitions for Mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2007 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Include necessary headers...
 */

#include "config.h"
#include "mxml.h"


/*
 * Global, per-thread data...
 */

typedef struct _mxml_global_s
{
  void	(*error_cb)(const char *);
  int	num_entity_cbs;
  int	(*entity_cbs[100])(const char *name);
  int	wrap;
  mxml_custom_load_cb_t	custom_load_cb;
  mxml_custom_save_cb_t	custom_save_cb;
} _mxml_global_t;


/*
 * Functions...
 */

extern _mxml_global_t	*_mxml_global(void);
extern int		_mxml_entity_cb(const char *name);


/*
 * End of "$Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/mxml-2.4/mxml-private.h#1 $".
 */

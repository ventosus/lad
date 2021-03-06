/*
  Copyright (C) 2003-2008 Fons Adriaensen <fons@kokkinizita.net>
   
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <math.h>

static inline float exp2ap (float x)
{
    const int i = (int)(floorf (x));
    x -= i;
    return ldexpf (1 + x * (0.6930f + x * (0.2416f + x * (0.0517f + x * 0.0137f))), i);
}



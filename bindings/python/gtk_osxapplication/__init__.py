# -*- Mode: Python; py-indent-offset: 4 -*-

# Copyright (C) 2007 Imendio AB
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; version 2.1
# of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA

import sys

if 'gtk_osxapplication._gtk_osxapplication' in sys.modules:
    _gtk_osxapplication = sys.modules['gtk_osxapplication._gtk_osxapplication']
else:
    from gtk_osxapplication import _gtk_osxapplication

from gtk_osxapplication._gtk_osxapplication import *


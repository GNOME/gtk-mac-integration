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

if 'igemacintegration._igemacintegration' in sys.modules:
    _igemacintegration = sys.modules['igemacintegration._igemacintegration']
else:
    from igemacintegration import _igemacintegration

from igemacintegration._igemacintegration import *

# Wrapper class for the menu
class MacMenu:
    def set_menu_bar(self, menubar):
        ige_mac_menu_set_menu_bar(menubar)

    def set_quit_menu_item(self, item):
        ige_mac_menu_set_quit_menu_item(item)

    def add_app_menu_group(self):
        return MacMenuAppGroup(ige_mac_menu_add_app_menu_group())

class MacMenuAppGroup:
    def __init__(self, group):
        self._group = group

    def add_app_menu_item(self, item, label=None):
        ige_mac_menu_add_app_menu_item(self._group, item, label)

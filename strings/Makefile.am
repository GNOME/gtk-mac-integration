# Makefile for installing MacOSX translation files. The MacOSX
# localization facilty uses human-readable text files; no compilation
# is necessary.
#
# Copyright 2010 John Ralls


dist_string_DATA =  GtkOSXApplication.strings.ca    \
		GtkOSXApplication.strings.cs    \
		GtkOSXApplication.strings.da    \
		GtkOSXApplication.strings.de    \
		GtkOSXApplication.strings.en    \
		GtkOSXApplication.strings.es    \
		GtkOSXApplication.strings.eu    \
		GtkOSXApplication.strings.fa    \
		GtkOSXApplication.strings.fi    \
		GtkOSXApplication.strings.fr    \
		GtkOSXApplication.strings.he    \
		GtkOSXApplication.strings.hu    \
		GtkOSXApplication.strings.it    \
		GtkOSXApplication.strings.ja    \
		GtkOSXApplication.strings.ko    \
		GtkOSXApplication.strings.lt    \
		GtkOSXApplication.strings.lv_LV \
		GtkOSXApplication.strings.nb    \
		GtkOSXApplication.strings.nl    \
		GtkOSXApplication.strings.pl    \
		GtkOSXApplication.strings.pt    \
		GtkOSXApplication.strings.ro    \
		GtkOSXApplication.strings.ru    \
		GtkOSXApplication.strings.sk    \
		GtkOSXApplication.strings.sv    \
		GtkOSXApplication.strings.tr    \
		GtkOSXApplication.strings.uk    \
		GtkOSXApplication.strings.vi    \
		GtkOSXApplication.strings.zh_CN \
		GtkOSXApplication.strings.zh_TW


stringdir = ${datadir}/strings

install-dist_stringDATA: ${dist_string_DATA}
	${mkdir_p} ${am_installdirs}; \
	for file in ${dist_string_DATA}; do \
	  dir_pref=`echo $$file | cut -f 3 -d '.' -`; \
	  dest_dir=${am_installdirs}/$$dir_pref.lproj; \
	  dest_name=`echo $$file | cut -f 1 -f 2 -d '.' -`; \
	  ${mkdir_p} $$dest_dir; \
	  ${install_sh} $(srcdir)/$$file $$dest_dir/$$dest_name; \
	done

uninstall-dist_stringDATA:
	rm -rf ${am_installdirs}




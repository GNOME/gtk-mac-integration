# Makefile for installing MacOSX translation files. The MacOSX
# localization facilty uses human-readable text files; no compilation
# is necessary.
#
# Copyright 2010 John Ralls


dist_string_DATA =  GtkosxApplication.strings.ca    \
		GtkosxApplication.strings.cs    \
		GtkosxApplication.strings.da    \
		GtkosxApplication.strings.de    \
		GtkosxApplication.strings.en    \
		GtkosxApplication.strings.es    \
		GtkosxApplication.strings.eu    \
		GtkosxApplication.strings.fa    \
		GtkosxApplication.strings.fi    \
		GtkosxApplication.strings.fr    \
		GtkosxApplication.strings.he    \
		GtkosxApplication.strings.hu    \
		GtkosxApplication.strings.it    \
		GtkosxApplication.strings.ja    \
		GtkosxApplication.strings.ko    \
		GtkosxApplication.strings.lt    \
		GtkosxApplication.strings.lv_LV \
		GtkosxApplication.strings.nb    \
		GtkosxApplication.strings.nl    \
		GtkosxApplication.strings.pl    \
		GtkosxApplication.strings.pt    \
		GtkosxApplication.strings.ro    \
		GtkosxApplication.strings.ru    \
		GtkosxApplication.strings.sk    \
		GtkosxApplication.strings.sv    \
		GtkosxApplication.strings.tr    \
		GtkosxApplication.strings.uk    \
		GtkosxApplication.strings.vi    \
		GtkosxApplication.strings.zh_CN \
		GtkosxApplication.strings.zh_TW


stringdir = ${datadir}/strings

install-dist_stringDATA: ${dist_string_DATA}
	${mkdir_p} ${am__installdirs}; \
	for file in ${dist_string_DATA}; do \
	  dir_pref=`echo $$file | cut -f 3 -d '.' -`; \
	  dest_dir=${am__installdirs}/$$dir_pref.lproj; \
	  dest_name=`echo $$file | cut -f 1 -f 2 -d '.' -`; \
	  ${mkdir_p} $$dest_dir; \
	  ${install_sh} $(srcdir)/$$file $$dest_dir/$$dest_name; \
	done

uninstall-dist_stringDATA:
	rm -rf ${am__installdirs}




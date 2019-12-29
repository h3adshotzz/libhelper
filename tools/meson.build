#
#	Tools built with libhelper. All of these can be compiled with no
#	other depndencies
#


#
#	macho_split		-	Splits Mach-O files that contain multiple architectures (Universal Binaries)
#	macho_sect		-	Extracts a specifies section of a given Mach-O (Given file should be single-arch, not universal)
#
macho_split 	= executable ('macho_split', sources: ['macho_split.c'], link_with: libhelper_static, include_directories : incdir)
macho_sect		= executable ('macho_sect', sources: ['macho_sect.c'], link_with: libhelper_static, include_directories : incdir)

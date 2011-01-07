defineTest(addSubDirs)
{
	for(subdirs, 1) {
		entries = $$files($$subdirs)
		for(entry, entries) {
			name = $$replace(entry, [/\\\\], _)
			SUBDIRS += $$name
			eval ($${name}.subdir = $$entry)
			for(dep, 2):eval ($${name}.depends += $$replace(dep, [/\\\\], _))
			export ($${name}.subdir)
			export ($${name}.depends)
		}
	}
	export (SUBDIRS)
}

import platform, os.path
 
srcdir="."
APPNAME = "bgen"
VERSION = "1.2-dev"

def options( opt ):
	opt.load( 'compiler_cxx' )
	opt.load( 'compiler_c' )
	opt.add_option( '--mode', action = 'store', default = 'release', dest = 'mode' )
	
def configure( cfg ):
	cfg.load( 'compiler_c')
	cfg.load( 'compiler_cxx')
	cfg.env.CXXFLAGS = [ '-Wall', '-pedantic', '-Wno-unused-local-typedefs', '-Wno-c++11-long-long', '-Wno-deprecated-declarations', '-Wno-long-long', '-fPIC' ]
	cfg.env.CFLAGS = [ '-Wall', '-pedantic', '-Wno-unused-local-typedefs', '-Wno-c++11-long-long', 'Wno-deprecated-declarations', '-Wno-long-long', '-fPIC' ]
	if cfg.options.mode == 'release':
		cfg.env.CXXFLAGS += [ '-O3' ]
		cfg.env.CFLAGS = [ '-O3' ]
	elif cfg.options.mode == 'debug':
		cfg.env.CXXFLAGS += [ '-g' ]
		cfg.env.CFLAGS = [ '-g' ]
	else:
		raise Exception( "Unknown value for --mode, please specify --mode=debug or --mode=release" )

	cfg.check_cxx( lib='z', uselib_store='zlib', msg = 'zlib' )
	if platform.system() != "Darwin":
		cfg.check_cxx( lib='rt', uselib_store='rt', msg = 'rt' )
		cfg.check_cxx( lib='pthread', uselib_store='pthread', msg = 'pthread' )
		cfg.check_cxx( lib='dl', uselib_store='dl', msg = 'dl' )

def build( bld ):
	print "Creating %s build..." % bld.options.mode
	bld(
		rule = """printf '#ifndef BGEN_REVISION_HPP\n#define BGEN_REVISION_HPP\nnamespace globals {\n\tchar const* bgen_version = \"%%s\" ;\n\tchar const* const bgen_revision = \"%%s\" ;\n}\n#endif\n' `echo """ + VERSION + "` `hg parents --template={node}` > ${TGT}""",
		always = True,
		target = "bgen_revision_autogenerated.hpp",
		name = "bgen_revision_autogenerated",
		uselib = "",
		#on_results = True
		on_results = False
	)

	bgen_sources =bld.path.ant_glob( 'src/*.cpp' )
	bld.stlib(
		source = bgen_sources,
		target = 'bgen',
		includes = 'genfile/include',
		use = 'zlib zstd sqlite3 db',
		export_includes = 'genfile/include'
	)
	bld.recurse( [ '3rd_party', 'appcontext', 'genfile', 'db', 'apps', 'example', 'test', 'R' ] )
	# Copy files into rbgen package directory
	for source in bgen_sources:
		bld( rule = 'cp ${SRC} ${TGT}', source = source, target = 'R/rbgen/src/bgen/' + os.path.basename( source.abspath() ), always = True )

class ReleaseBuilder:
	def __init__( self, APPNAME, VERSION ):
		self.APPNAME = APPNAME
		self.VERSION = VERSION
		self.apps = [ 'bgenix', 'cat-bgen', 'edit-bgen' ]

	def makedirs( self, path ):
		try:
			os.makedirs( path )
		except os.error, e:
			pass

	def create_pkgname_stub( self, packageName, includePlatform = True ):
		import platform
		stub = '%s_v%s' % ( packageName, self.VERSION )
		if includePlatform:
			if platform.system() == 'Darwin':
				stub = '%s-osx' % stub
			elif platform.system() == 'Linux':
				distro = platform.linux_distribution()
				stub = '%s-%s%s-%s' % ( stub, distro[0], distro[1], platform.machine() )
		return stub
	
	def build_bgen( self ):
		import os, tempfile, shutil, subprocess
		tempdir = tempfile.mkdtemp()
		release_stub = self.create_pkgname_stub( 'bgen' )
		release_dir = os.path.join( tempdir, release_stub )
		os.mkdir( release_dir )
		shutil.copyfile( "LICENSE_1_0.txt", os.path.join( release_dir, "LICENSE_1_0.txt" ) )
		for app in self.apps:
			source = os.path.join( 'build', 'apps', app )
			target = os.path.join( release_dir, app )
			shutil.copyfile( source, target )
			shutil.copymode( source, target )
		shutil.copytree( 'example', os.path.join( release_dir, 'example' ), ignore = shutil.ignore_patterns( '*.cpp', 'wscript' ))
		tarball = self.create_tarball( 'bgen', release_stub, tempdir )
		return os.path.join( tempdir, tarball )
	
	def create_tarball( self, name, source, workingdir ):
		import subprocess
		tarball = "%s.tgz" % source
		tarballPath = os.path.join( workingdir, tarball )
		process = subprocess.Popen( [ 'tar', '-czf', tarball, source ], cwd = workingdir )
		process.wait()
		print 'Created %s release tarball in "%s"' % ( name, tarball )
		print "Contents are:"
		print subprocess.Popen( [ 'tar', '-tzf',tarballPath ], stdout = subprocess.PIPE ).communicate()[0]
		return tarball
		
	def build_rbgen( self ):
		import os, tempfile, shutil, subprocess, platform
		tempdir = tempfile.mkdtemp()
		release_stub = self.create_pkgname_stub( 'rbgen', includePlatform = False )
		rbgen_dir = os.path.join( tempdir, release_stub )
		shutil.copytree( 'R/package/', rbgen_dir )
		os.makedirs( os.path.join( rbgen_dir, "src", "include" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "include", "boost" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "include", "zstd-1.1.0" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "db" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "bgen" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "boost" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "sqlite3" ))
		os.makedirs( os.path.join( rbgen_dir, "src", "zstd-1.1.0" ))

		# Copy source files in
		from glob import glob
		for filename in glob( 'src/*.cpp' ):
			shutil.copy( filename, os.path.join( rbgen_dir, "src", "bgen", os.path.basename( filename ) ) )

		for filename in glob( 'db/src/*.cpp' ):
			shutil.copy( filename, os.path.join( rbgen_dir, "src", "db", os.path.basename( filename ) ) )

		for filename in glob( '3rd_party/sqlite3/sqlite3/sqlite3.c' ):
			shutil.copy( filename, os.path.join( rbgen_dir, "src", "sqlite3", os.path.basename( filename ) ) )

		for filename in glob( '3rd_party/zstd-1.1.0/lib/common/*.c' ) + glob( '3rd_party/zstd-1.1.0/lib/compress/*.c' ) + glob( '3rd_party/zstd-1.1.0/lib/decompress/*.c' ):
			shutil.copy( filename, os.path.join( rbgen_dir, "src", "zstd-1.1.0", os.path.basename( filename ) ) )

		boostGlobs = [
			'libs/system/src/*.cpp',
			'libs/thread/src/*.cpp',
			'libs/thread/src/*.inl',
			'libs/thread/src/pthread/once_atomic.cpp',
			'libs/thread/src/pthread/thread.cpp',
			'libs/thread/src/pthread/timeconv.inl',
			'libs/filesystem/src/*.cpp',
			'libs/date_time/src/posix_time/*.cpp',
			'libs/timer/src/*.cpp',
			'libs/chrono/src/*.cpp',
		]

		for pattern in boostGlobs:
			for filename in glob( '3rd_party/boost_1_55_0/%s' % pattern ):
				shutil.copy( filename, os.path.join( rbgen_dir, "src", "boost", os.path.basename( filename ) ) )

		include_paths = [
			"3rd_party/boost_1_55_0/boost/",
			"3rd_party/zstd-1.1.0/",
			"3rd_party/sqlite3/",
			"genfile/include/genfile",
			"db/include/db"
		]
		
		boostHeaderLibs = [
			'system',
			'thread',
			'filesystem',
			'date_time',
			'timer',
			'chrono',
			'preprocessor',
			'function',
			'optional',
			'mpl'
		]
		
		for include_path in include_paths:
			for root, path, filenames in os.walk( include_path ):
				self.makedirs( os.path.join( rbgen_dir, "src", "include", root ))
				for name in filenames:
					fullname = os.path.join( root, name )
					if os.path.isfile( fullname ) and (
						name.endswith( '.h')
						or name.endswith( ".hpp" )
						or name.endswith( ".ipp" )
						or name.endswith( ".inl" )
					):
						dest = os.path.join( rbgen_dir, "src", "include", root, name )
						shutil.copy( fullname, os.path.join( rbgen_dir, "src", "include", fullname ))
						#print "Copied \"%s\"." % fullname

		tarball = self.create_tarball( 'rbgen', release_stub, tempdir )
		return os.path.join( tempdir, tarball )
		

def release( bld ):
	print "Building bgen release tarball..."
	release = ReleaseBuilder( APPNAME, VERSION )
	result = release.build_bgen()
	print "Created %s release tarball in \"%s\"" % ( 'bgen', result )

def build_rbgen( bld ):
	print "Building rbgen source package tarball..."
	release = ReleaseBuilder( APPNAME, VERSION )
	result = release.build_rbgen()
	print "Created %s release tarball in \"%s\"" % ( 'rbgen', result )

import platform
 
srcdir="."
APPNAME = "bgen"
VERSION = "1.2"

def options( opt ):
	opt.load( 'compiler_cxx' )
	opt.load( 'compiler_c' )
	opt.add_option( '--mode', action = 'store', default = 'debug', dest = 'mode' )
	
def configure( cfg ):
	cfg.load( 'compiler_c')
	cfg.load( 'compiler_cxx')
	cfg.env.CXXFLAGS = [ '-Wall', '-pedantic', '-Wno-unused-local-typedefs', '-Wno-c++11-long-long' ]
	cfg.env.CFLAGS = [ '-Wall', '-pedantic', '-Wno-unused-local-typedefs', '-Wno-c++11-long-long' ]
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
		rule = """printf '#ifndef BGEN_REVISION_HPP\n#define BGEN_REVISION_HPP\nnamespace globals {\n\tchar const* const bgen_revision = \"%%s\" ;\n}\n#endif\n' `hg parents --template={node}` > ${TGT}""",
		always = True,
		target = "bgen_revision_autogenerated.hpp",
		name = "bgen_revision_autogenerated",
		uselib = "",
		#on_results = True
		on_results = False
	)

	bld.stlib(
		source = bld.path.ant_glob( 'src/*.cpp' ),
		target = 'bgen',
		includes = 'genfile/include',
		cxxflags = [ '-std=c++0x' ],
		export_includes = 'genfile/include'
	)
	bld.recurse( [ '3rd_party', 'appcontext', 'db', 'apps' ] )

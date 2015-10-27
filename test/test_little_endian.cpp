
//          Copyright Gavin Band 2008 - 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#include "stdint.h"
#include "catch.hpp"
#include "test_utils.hpp"
#include "genfile/bgen/bgen.hpp"

namespace {
	char buffer[100] ;
	template< typename Integer >
	void test_rw( Integer value ) {
		Integer result ;
		char const* end = genfile::bgen::write_little_endian_integer( buffer, buffer + 100, value ) ;
		genfile::bgen::read_little_endian_integer( buffer, end, &result ) ;
		REQUIRE( result == value ) ;
		// Test little endian property
		REQUIRE( static_cast< unsigned char >( buffer[0] ) == static_cast< unsigned char>(value & 0xFF) ) ;
	}
}

TEST_CASE( "Test that integers can be written and recovered from a buffer." ) {
	char buffer[100] ;

	test_rw<char>( 0 ) ;
	test_rw<char>( 1 ) ;
	test_rw<char>( 127 ) ;
	test_rw<char>( -128 ) ;
	test_rw<char>( std::numeric_limits<uint16_t>::max() ) ;
	test_rw<char>( std::numeric_limits<uint16_t>::min() ) ;

	test_rw<unsigned char>( 0 ) ;
	test_rw<unsigned char>( 1 ) ;
	test_rw<unsigned char>( 127 ) ;
	test_rw<unsigned char>( 255 ) ;
	test_rw<unsigned char>( std::numeric_limits<uint16_t>::max() ) ;
	test_rw<unsigned char>( std::numeric_limits<uint16_t>::min() ) ;

	test_rw<uint16_t>( 0 ) ;
	test_rw<uint16_t>( 1 ) ;
	test_rw<uint16_t>( 255 ) ;
	test_rw<uint16_t>( std::numeric_limits<uint16_t>::max() ) ;
	test_rw<uint16_t>( std::numeric_limits<uint16_t>::min() ) ;

	test_rw<int16_t>( 0 ) ;
	test_rw<int16_t>( 1 ) ;
	test_rw<int16_t>( -1 ) ;
	test_rw<int16_t>( 255 ) ;
	test_rw<int16_t>( std::numeric_limits<int16_t>::max() ) ;
	test_rw<int16_t>( std::numeric_limits<int16_t>::min() ) ;

	test_rw<uint32_t>( 0 ) ;
	test_rw<uint32_t>( 1 ) ;
	test_rw<uint32_t>( 255 ) ;
	test_rw<uint32_t>( std::numeric_limits<uint32_t>::max() ) ;
	test_rw<uint32_t>( std::numeric_limits<uint32_t>::min() ) ;

	test_rw<int32_t>( 0 ) ;
	test_rw<int32_t>( 1 ) ;
	test_rw<int32_t>( -1 ) ;
	test_rw<int32_t>( 255 ) ;
	test_rw<int32_t>( std::numeric_limits<int32_t>::max() ) ;
	test_rw<int32_t>( std::numeric_limits<int32_t>::min() ) ;

	test_rw<uint64_t>( 0 ) ;
	test_rw<uint64_t>( 1 ) ;
	test_rw<uint64_t>( 255 ) ;
	test_rw<uint64_t>( std::numeric_limits<uint64_t>::max() ) ;
	test_rw<uint64_t>( std::numeric_limits<uint64_t>::min() ) ;

	test_rw<int64_t>( 0 ) ;
	test_rw<int64_t>( 1 ) ;
	test_rw<int64_t>( -1 ) ;
	test_rw<int64_t>( 255 ) ;
	test_rw<int64_t>( std::numeric_limits<int64_t>::max() ) ;
	test_rw<int64_t>( std::numeric_limits<int64_t>::min() ) ;
}
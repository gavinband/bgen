
//          Copyright Gavin Band 2008 - 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BGEN_INDEX_QUERY_HPP
#define BGEN_INDEX_QUERY_HPP

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include "db/sqlite3.hpp"

namespace genfile {
	namespace bgen {
		// Base class representing a query against a BGEN file index
		struct IndexQuery {
		public:
			// We use std::auto_ptr to avoid using C++11 features here.
			typedef std::auto_ptr< IndexQuery > UniquePtr ;
			typedef uint8_t byte_t ;

		public:
			struct FileMetadata ;
			typedef boost::tuple< std::string, uint32_t, uint32_t > GenomicRange ;
			typedef std::pair< int64_t, int64_t> FileRange ;
			typedef boost::function< void ( std::size_t n, boost::optional< std::size_t > total ) > ProgressCallback ;

		public:
			virtual ~IndexQuery() {} ;
			virtual FileMetadata const& file_metadata() const = 0 ;

			// Initialise must be called before calling number_of_variants() or locate_variant().
			virtual void initialise( ProgressCallback callback = ProgressCallback() ) = 0 ;
			// Report the number of variants in this query.
			virtual std::size_t number_of_variants() const = 0 ;
			// Report the number of variants in this query.
			virtual FileRange locate_variant( std::size_t index ) const = 0 ;

			struct FileMetadata {
				FileMetadata():
					size(0)
				{}

				FileMetadata( FileMetadata const& other ):
					filename( other.filename ),
					size( other.size ),
					last_write_time( other.last_write_time ),
					first_bytes( other.first_bytes )
				{}

				FileMetadata& operator=( FileMetadata const& other ) {
					filename = other.filename ;
					size = other.size ;
					last_write_time = other.last_write_time ;
					first_bytes = other.first_bytes ;
					return *this ;
				}

				std::string filename ;
				int64_t size ;
				std::time_t last_write_time ;
				std::vector< byte_t > first_bytes ;
			} ;
		} ;
		
		// Class for index queries implemented using a sqlite file, a la bgenix.
		struct SqliteIndexQuery: public IndexQuery {
		public:
			// We use auto_ptr to avoid using C++11 features here.
			typedef std::auto_ptr< SqliteIndexQuery > UniquePtr ;

			// Construct given an index file and an index table name
			SqliteIndexQuery( std::string const& filename, std::string const& table_name = "Variant" ) ;

		public:
			// Methods for building queries
			// Each method returns this object, allowing methods to be chained
			SqliteIndexQuery& include_range( GenomicRange const& range ) ;
			SqliteIndexQuery& exclude_range( GenomicRange const& range ) ;
			SqliteIndexQuery& include_rsids( std::vector< std::string > const& ids ) ;
			SqliteIndexQuery& exclude_rsids( std::vector< std::string > const& ids ) ;

		public:
			// IndexQuery methods
			void initialise( ProgressCallback callback = ProgressCallback() ) ;
			FileMetadata const& file_metadata() const ;
			std::size_t number_of_variants() const ;
			FileRange locate_variant( std::size_t index ) const ;

		private:
			db::Connection::UniquePtr open_connection( std::string const& filename ) const ;
			FileMetadata load_metadata( db::Connection& connection ) const ;
			db::Connection::StatementPtr build_query() const ;
	
		private:
			db::Connection::UniquePtr m_connection ;
			FileMetadata const m_metadata ;
			std::string const m_index_table_name ;
			struct QueryParts {
				std::string join ;
				std::string inclusion ;
				std::string exclusion ;
			} ;
			QueryParts m_query_parts ;
			bool m_initialised ;
			std::vector< std::pair< int64_t, int64_t> > m_positions ;
		} ;
		
	}
}

#endif
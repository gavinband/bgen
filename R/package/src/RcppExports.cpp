// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <unordered_set>
#include <Rcpp.h>

using namespace Rcpp;

// bgen.load
Rcpp::List load(
	std::string const& filename,
	std::string const& index_filename,
	Rcpp::DataFrame const& ranges,
	std::size_t max_entries_per_sample
) ;

Rcpp::List load(
	std::string const& filename,
	std::string const& index_filename,
	Rcpp::DataFrame const& ranges,
	std::size_t max_entries_per_sample,
	Rcpp::StringVector const& requestedSamples
) ;

RcppExport SEXP rbgen_load(
	SEXP filenameSEXP,
	SEXP index_filenameSEXP,
	SEXP rangesSEXP,
	SEXP max_entries_per_sampleSEXP
) {
BEGIN_RCPP
	Rcpp::RObject rcpp_result_gen;
	Rcpp::RNGScope rcpp_rngScope_gen;
	Rcpp::traits::input_parameter< std::string const& >::type filename(filenameSEXP);
	Rcpp::traits::input_parameter< std::string const& >::type index_filename(index_filenameSEXP);
	Rcpp::traits::input_parameter< Rcpp::DataFrame const& >::type ranges(rangesSEXP);
	Rcpp::traits::input_parameter< std::size_t >::type max_entries_per_sample(max_entries_per_sampleSEXP);
	rcpp_result_gen = Rcpp::wrap(load(filename, index_filename, ranges, max_entries_per_sample ));
	return rcpp_result_gen;
END_RCPP
}

RcppExport SEXP rbgen_load_samples(
	SEXP filenameSEXP,
	SEXP index_filenameSEXP,
	SEXP rangesSEXP,
	SEXP max_entries_per_sampleSEXP,
	SEXP samplesSEXP
) {
BEGIN_RCPP
	Rcpp::RObject rcpp_result_gen;
	Rcpp::RNGScope rcpp_rngScope_gen;
	Rcpp::traits::input_parameter< std::string const& >::type filename(filenameSEXP);
	Rcpp::traits::input_parameter< std::string const& >::type index_filename(index_filenameSEXP);
	Rcpp::traits::input_parameter< Rcpp::DataFrame const& >::type ranges(rangesSEXP);
	Rcpp::traits::input_parameter< std::size_t >::type max_entries_per_sample(max_entries_per_sampleSEXP);
	Rcpp::traits::input_parameter< Rcpp::StringVector >::type samples(samplesSEXP);
	rcpp_result_gen = Rcpp::wrap(load(filename, index_filename, ranges, max_entries_per_sample, samples));
	return rcpp_result_gen;
END_RCPP
}

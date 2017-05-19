// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// bgen.load
Rcpp::List load(std::string const& filename, Rcpp::DataFrame const& ranges);
RcppExport SEXP rbgen_load(SEXP filenameSEXP, SEXP rangesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string const& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< Rcpp::DataFrame const& >::type ranges(rangesSEXP);
    rcpp_result_gen = Rcpp::wrap(load(filename, ranges));
    return rcpp_result_gen;
END_RCPP
}

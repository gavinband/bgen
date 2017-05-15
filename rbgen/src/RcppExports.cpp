// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// giveMeMyData
Rcpp::List giveMeMyData(std::string const& filename, std::string const& chromosome, uint32_t start, uint32_t end);
RcppExport SEXP rbgen_giveMeMyData(SEXP filenameSEXP, SEXP chromosomeSEXP, SEXP startSEXP, SEXP endSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string const& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< std::string const& >::type chromosome(chromosomeSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type start(startSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type end(endSEXP);
    rcpp_result_gen = Rcpp::wrap(giveMeMyData(filename, chromosome, start, end));
    return rcpp_result_gen;
END_RCPP
}

// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// solver_rcpp
Rcpp::List solver_rcpp(const Rcpp::NumericMatrix& reward, const Rcpp::NumericMatrix& cost, const Rcpp::IntegerMatrix& order, const Rcpp::NumericVector& sample_weights, const std::vector<size_t>& clusters, uint samples_per_cluster, double budget, size_t num_bootstrap, uint num_threads, uint seed);
RcppExport SEXP _maq_solver_rcpp(SEXP rewardSEXP, SEXP costSEXP, SEXP orderSEXP, SEXP sample_weightsSEXP, SEXP clustersSEXP, SEXP samples_per_clusterSEXP, SEXP budgetSEXP, SEXP num_bootstrapSEXP, SEXP num_threadsSEXP, SEXP seedSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const Rcpp::NumericMatrix& >::type reward(rewardSEXP);
    Rcpp::traits::input_parameter< const Rcpp::NumericMatrix& >::type cost(costSEXP);
    Rcpp::traits::input_parameter< const Rcpp::IntegerMatrix& >::type order(orderSEXP);
    Rcpp::traits::input_parameter< const Rcpp::NumericVector& >::type sample_weights(sample_weightsSEXP);
    Rcpp::traits::input_parameter< const std::vector<size_t>& >::type clusters(clustersSEXP);
    Rcpp::traits::input_parameter< uint >::type samples_per_cluster(samples_per_clusterSEXP);
    Rcpp::traits::input_parameter< double >::type budget(budgetSEXP);
    Rcpp::traits::input_parameter< size_t >::type num_bootstrap(num_bootstrapSEXP);
    Rcpp::traits::input_parameter< uint >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< uint >::type seed(seedSEXP);
    rcpp_result_gen = Rcpp::wrap(solver_rcpp(reward, cost, order, sample_weights, clusters, samples_per_cluster, budget, num_bootstrap, num_threads, seed));
    return rcpp_result_gen;
END_RCPP
}
// convex_hull_rcpp
Rcpp::List convex_hull_rcpp(const Rcpp::NumericMatrix& reward, const Rcpp::NumericMatrix& cost, const Rcpp::IntegerMatrix& order);
RcppExport SEXP _maq_convex_hull_rcpp(SEXP rewardSEXP, SEXP costSEXP, SEXP orderSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const Rcpp::NumericMatrix& >::type reward(rewardSEXP);
    Rcpp::traits::input_parameter< const Rcpp::NumericMatrix& >::type cost(costSEXP);
    Rcpp::traits::input_parameter< const Rcpp::IntegerMatrix& >::type order(orderSEXP);
    rcpp_result_gen = Rcpp::wrap(convex_hull_rcpp(reward, cost, order));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_maq_solver_rcpp", (DL_FUNC) &_maq_solver_rcpp, 10},
    {"_maq_convex_hull_rcpp", (DL_FUNC) &_maq_convex_hull_rcpp, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_maq(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

/**
 * @file kernel_pca_impl.hpp
 * @author Ajinkya Kale
 * @author Marcus Edel
 *
 * Implementation of Kernel PCA class to perform Kernel Principal Components
 * Analysis on the specified data set.
 */
#ifndef __MLPACK_METHODS_KERNEL_PCA_KERNEL_PCA_IMPL_HPP
#define __MLPACK_METHODS_KERNEL_PCA_KERNEL_PCA_IMPL_HPP

// In case it hasn't already been included.
#include "kernel_pca.hpp"

namespace mlpack {
namespace kpca {

template <typename KernelType, typename KernelRule>
KernelPCA<KernelType, KernelRule>::KernelPCA(const KernelType kernel,
                                 const bool centerTransformedData) :
      kernel(kernel),
      centerTransformedData(centerTransformedData)
{ }

//! Apply Kernel Principal Component Analysis to the provided data set.
template <typename KernelType, typename KernelRule>
void KernelPCA<KernelType, KernelRule>::Apply(const arma::mat& data,
                                  arma::mat& transformedData,
                                  arma::vec& eigval,
                                  arma::mat& eigvec,
                                  const size_t newDimension)
{
  KernelRule::ApplyKernelMatrix(data, transformedData, eigval,
                                eigvec, newDimension, kernel);

  // Center the transformed data, if the user asked for it.
  if (centerTransformedData)
  {
    arma::colvec transformedDataMean = arma::mean(transformedData, 1);
    transformedData = transformedData - (transformedDataMean *
        arma::ones<arma::rowvec>(transformedData.n_cols));
  }
}

//! Apply Kernel Principal Component Analysis to the provided data set.
template <typename KernelType, typename KernelRule>
void KernelPCA<KernelType, KernelRule>::Apply(const arma::mat& data,
                                  arma::mat& transformedData,
                                  arma::vec& eigval,
                                  arma::mat& eigvec)
{
  Apply(data, transformedData, eigval, eigvec, data.n_cols);
}

//! Apply Kernel Principal Component Analysis to the provided data set.
template <typename KernelType, typename KernelRule>
void KernelPCA<KernelType, KernelRule>::Apply(const arma::mat& data,
                                  arma::mat& transformedData,
                                  arma::vec& eigVal)
{
  arma::mat coeffs;
  Apply(data, transformedData, eigVal, coeffs, data.n_cols);
}

//! Use KPCA for dimensionality reduction.
template <typename KernelType, typename KernelRule>
void KernelPCA<KernelType, KernelRule>::Apply(arma::mat& data,
                                    const size_t newDimension)
{
  arma::mat coeffs;
  arma::vec eigVal;

  Apply(data, data, eigVal, coeffs, newDimension);

  if (newDimension < coeffs.n_rows && newDimension > 0)
    data.shed_rows(newDimension, data.n_rows - 1);
}

} // namespace mlpack
} // namespace kpca

#endif

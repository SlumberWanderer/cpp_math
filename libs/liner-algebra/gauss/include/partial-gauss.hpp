#pragma once

#include "linear-core.hpp"

namespace linear_algebra::gauss {

    class PartialGaussResolver : public linear_core::ILinearSystemResolver {
    public:
        PartialGaussResolver() = default;
        ~PartialGaussResolver() override = default;

        [[nodiscard]] Vector resolve(const RectangleMatrix& m, const Vector& v) const override;
        [[nodiscard]] Vector deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const override;
    };

    class PartialGaussDeterminantResolver : public linear_core::IDeterminantResolver {
    public:
        PartialGaussDeterminantResolver() = default;
        ~PartialGaussDeterminantResolver() override = default;

        [[nodiscard]] double determinant(const RectangleMatrix& m) const override;
    };

    class PartialGaussInverseMatrixResolver : public linear_core::IInverseMatrixResolver {
    public:
        PartialGaussInverseMatrixResolver() = default;
        ~PartialGaussInverseMatrixResolver() override = default;

        [[nodiscard]] RectangleMatrix inverse(const RectangleMatrix& m) const override;
    };

}
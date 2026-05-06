/*	NAME:
 *		matrices.hpp
 *
 *	DESCRIPTION:
 *		Matrices tools
 *
 *	CONTRIBUTORS:
 *		 Racc 22-MAR-26
*/





#ifndef MATRICES_H
#define MATRICES_H





// ===========================================================================
//		Includes
// ---------------------------------------------------------------------------
#include <array>
#include <cassert>
#include <type_traits>





template <typename T, std::size_t Row, std::size_t Col>
struct Mat
{
	std::array<T, Row * Col> v{};

	constexpr
	T& At(std::size_t row, std::size_t col) noexcept
	{
		assert(row < Row && col < Col);
		return v[row*Col + col];
	}
	constexpr
	const T& At(std::size_t row, std::size_t col) const noexcept
	{
		assert( row < Row && col < Col);
		return v[row*Col + col];
	}

	static constexpr std::size_t kRows = Row;
	static constexpr std::size_t kCols = Col;
};





template <typename T, std::size_t R, std::size_t C, std::size_t K>
constexpr Mat<T, R, C> operator*(const Mat<T, R, K>& a, const Mat<T, K, C>& b)
{
	Mat<T, R, C> out{};

	for (std::size_t r = 0; r < R; ++r)
	{
		for (std::size_t c = 0; c < C; ++c)
		{
			for (std::size_t k = 0; k < K; ++k)
			{
				out.At(r, c) += a.At(r, k) * b.At(k, c);
			}
		}
	}

	return out;
}





template <typename T, std::size_t R, std::size_t C, typename S,
          typename = std::enable_if_t<std::is_arithmetic_v<S>>>
constexpr auto operator*(const Mat<T, R, C>& a, const S& scalar)
{
	using OutT = std::common_type_t<T, S>;

	Mat<OutT, R, C> out{};

	for (std::size_t r = 0; r < R; ++r)
	{
		for (std::size_t c = 0; c < C; ++c)
		{
			out.At(r, c) = a.At(r,c) * scalar;
		}
	}

	return out;
}





template <typename T, std::size_t R, std::size_t C, typename S,
          typename = std::enable_if_t<std::is_arithmetic_v<S>>>
constexpr auto operator*(const S& scalar, const Mat<T, R, C>& a)
{
	return a * scalar;
}





template <typename T, std::size_t R, std::size_t C>
constexpr Mat<T, R, C>& operator+=(Mat<T, R, C>& lhs, const Mat<T, R, C>& rhs) noexcept
{
	for (std::size_t r = 0; r < R; ++r)
	{
		for (std::size_t c = 0; c < C; ++c)
		{
			lhs.At(r, c) += rhs.At(r, c);
		}
	}

	return lhs;
}





template <typename T, std::size_t R, std::size_t C>
constexpr Mat<T, R, C> operator+(Mat<T, R, C> lhs, const Mat<T, R, C>& rhs) noexcept
{
	lhs += rhs;
	return lhs;
}





template <typename T, std::size_t R, std::size_t C>
constexpr Mat<T, R, C>& operator-=(Mat<T, R, C>& lhs, const Mat<T, R, C>& rhs) noexcept
{
	for (std::size_t r = 0; r < R; ++r)
	{
		for (std::size_t c = 0; c < C; ++c)
		{
			lhs.At(r, c) -= rhs.At(r, c);
		}
	}

	return lhs;
}





template <typename T, std::size_t R, std::size_t C>
constexpr Mat<T, R, C> operator-(Mat<T, R, C> lhs, const Mat<T, R, C>& rhs) noexcept
{
	lhs -= rhs;
	return lhs;
}





#endif // MATRICES_H






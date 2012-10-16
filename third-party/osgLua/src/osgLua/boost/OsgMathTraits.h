/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a
#define INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a

// Internal Includes
#include "MathTypes.h"
#include "Tags.h"
#include "TagMetafunctions.h"

// Library/third-party includes
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

// Standard includes
// - none

namespace osgTraits {

	template<typename T>
	struct is_scalar : boost::is_arithmetic<T>::type {};

	template<typename T>
	struct is_math_or_scalar : boost::mpl::or_<is_math_type<T>, is_scalar<T> >::type {};

	template<typename T>
	struct GetCategory : detail::ComputeCategoryTag<T> {};

	template<typename CategoryTag>
	struct GetDimension_impl {
		template<typename T>
		struct apply;
	};

	template<typename T>
	struct GetDimension : GetDimension_impl<typename detail::ComputeCategoryTag<T>::type >::template apply<T> {};

	template<>
	struct GetDimension_impl<tags::Vec> {
		template<typename T>
		struct apply {
			typedef boost::mpl::int_<T::num_components> type;
		};
	};

	template<>
	struct GetDimension_impl<tags::Matrix> {
		template<typename T>
		struct apply {
			typedef boost::mpl::int_<4> type;
		};
	};

	template<>
	struct GetDimension_impl<tags::Scalar> {
		template<typename T>
		struct apply {
			typedef boost::mpl::int_<1> type;
		};
	};

	template<typename MathTag>
	struct GetScalar_impl {
		template<typename T>
		struct apply;
	};

	template<typename T>
	struct GetScalar : GetScalar_impl<typename detail::ComputeIsMathTag<T>::type >::template apply<T> {};

	template<>
	struct GetScalar_impl<tags::MathType> {
		template<typename T>
		struct apply {
			typedef typename T::value_type type;
		};
	};
	template<>
	struct GetScalar_impl<tags::Scalar> {
		template<typename T>
		struct apply {
			typedef T type;
		};
	};

	template<typename CategoryTag, typename ScalarTag, typename DimensionTag = void>
	struct MathTypeDetail {
		typedef CategoryTag category_tag;
		typedef ScalarTag scalar_tag;
		typedef DimensionTag dimension_tag;
	};

	template<typename T>
	struct GetMathTypeDetail {
		typedef MathTypeDetail<typename GetCategory<T>::type, typename GetScalar<T>::type, typename GetDimension<T>::type> type;
	};

} // end of namespace osgTraits
#endif // INCLUDED_OsgMathTraits_h_GUID_6c3ba294_4496_423b_9e5c_598e2ca2040a
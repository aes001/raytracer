/*	NAME:
 *		RaccDebug.hpp
 *
 *	DESCRIPTION:
 *		Useful debug utils
 *
 *	CONTRIBUTORS:
 *		RACC 07-OCT-25
*/





#ifndef RACC_DEBUG_HPP
#define RACC_DEBUG_HPP





#include <iostream>
#include <sstream>





#define RACC_CONSUME_SEMICOLON do {} while(0)





#ifndef NDEBUG
	#if defined(__clang__) || defined(__GNUC__)
		#define RACC_CRASH() __builtin_trap()

	#elif defined(_MSC_VER)
		#define RACC_CRASH() __debugbreak()

	#else
		#include <cstdlib>
		#define RACC_CRASH() std::abort()

	#endif // RACC_CRASH
#else // NDEBUG
	#define RACC_CRASH() RACC_CONSUME_SEMICOLON
#endif // NDEBUG





#ifndef NDEBUG
	#define RACC_REQUIRE(_boolExpression, _messageStream) \
	do { \
		if(!(_boolExpression)) \
		{ \
			std::ostringstream assertStream; \
			assertStream << _messageStream; \
			std::cerr << "Assertion failed!\n" \
					  << "File: " << __FILE__ << "\n" \
					  << "Line: " << __LINE__ << "\n" \
					  << "Expression: " << #_boolExpression << "\n" \
					  << assertStream.str() << "\n" \
					  << std::endl; \
			RACC_CRASH(); \
		} \
	} while (0)
#else // NDEBUG
	#define RACC_REQUIRE(_boolExpression, _messageStream) RACC_CONSUME_SEMICOLON
#endif // NDEBUG





#endif // RACC_DEBUG_HPP






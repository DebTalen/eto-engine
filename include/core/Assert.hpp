#ifndef ETO_ASSERT_HPP
#define ETO_ASSERT_HPP

#ifdef ASSERTIONS_ENABLED

void eto_AssertErrorHandler(const char *message, const char *fileName, unsigned line);

/*!
 *  \brief  Macro for assertion
 */
#define ETO_ASSERT(expr) \
	if (expr) { }    \
	else {           \
		eto_AssertErrorHandler(#expr, __FILE__, __LINE__);\
	}                

#else 

#define ETO_ASSERT(expr) // evaluates to nothing

#endif // ASSERTIONS_ENABLED

#endif // ETO_ASSERT_HPP

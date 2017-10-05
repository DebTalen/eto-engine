#ifndef ETO_ASSERT_HPP
#define ETO_ASSERT_HPP

void eto_AssertErrorHandler(const char *message, const char *fileName, unsigned line);

#ifdef ETO_ASSERTIONS_ENABLED

#define ETO_ASSERT(expr) \
	if (expr) { }    \
	else {           \
		eto_AssertErrorHandler(#expr, __FILE__, __LINE__);\
	}                

#else 

#define ETO_ASSERT(expr) // evaluates to nothing

#endif // ETO_ASSERTIONS_ENABLED

#endif // ETO_ASSERT_HPP

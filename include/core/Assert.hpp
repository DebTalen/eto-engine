#ifndef ETO_ASSERT_HPP
#define ETO_ASSERT_HPP

void eto_Assert_error_handler(const char *message, const char *file_name, unsigned line);

//#ifdef ETO_ASSERTIONS_ENABLED

#define ETO_ASSERT(expr) \
	if (expr) { }    \
	else {           \
		eto_Assert_error_handler(#expr, __FILE__, __LINE__);\
	}                

// #else 

// #define ETO_ASSERT(expr) // evaluates to nothing

// #endif // ETO_ASSERTIONS_ENABLED

#endif // ETO_ASSERT_HPP

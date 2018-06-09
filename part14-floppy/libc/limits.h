#ifndef LIMITS_H
#define LIMITS_H

/*
 * Sizes of integral types
 */
#define	CHAR_BIT	8		/* max # of bits in a "char" */
#define	SCHAR_MIN	(-128)		/* min value of a "signed char" */
#define	SCHAR_MAX	127		/* max value of a "signed char" */
#define	UCHAR_MAX	255		/* max value of an "unsigned char" */

#define	MB_LEN_MAX	5

#define	CHAR_MIN	SCHAR_MIN	/* min value of a "char" */
#define	CHAR_MAX	SCHAR_MAX	/* max value of a "char" */

#define	SHRT_MIN	(-32768)	/* min value of a "short int" */
#define	SHRT_MAX	32767		/* max value of a "short int" */
#define	USHRT_MAX	65535		/* max value of "unsigned short int" */
#define	INT_MIN		(-2147483647-1)	/* min value of an "int" */
#define	INT_MAX		2147483647	/* max value of an "int" */
#define	UINT_MAX	4294967295U	/* max value of an "unsigned int" */
#define	LONG_MIN	(-2147483647L-1L)	/* min value of a "long int" */
#define	LONG_MAX	2147483647L	/* max value of a "long int" */
#define	ULONG_MAX	4294967295UL 	/* max value of "unsigned long int" */

#endif
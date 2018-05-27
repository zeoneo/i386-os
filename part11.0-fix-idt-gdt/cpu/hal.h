#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#define far
#define near

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! initialize hardware abstraction layer
extern	int		hal_initialize ();

//! shutdown hardware abstraction layer
extern	int		hal_shutdown ();

//! generates interrupt
extern	void	geninterrupt (int n);

#endif

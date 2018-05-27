#ifndef _CPU_H_INCLUDED
# define _CPU_H_INCLUDED
//****************************************************************************
//**
//**    cpu.h
//**
//**	This is the processor interface. Everything outside of this module
//**	must use this interface when working on processor data.
//**
//**	A processor is a module that manages the very basic data structures
//**	and data within the system. The processor interface provides the interface
//**	for managing processors, processor cores, accessing processor structures,
//**	and more
//**
//****************************************************************************

#include <stdint.h>
#include "regs.h"

//! initialize the processors
extern int i86_cpu_initialize ();

//! shutdown the processors
extern void i86_cpu_shutdown ();

#endif

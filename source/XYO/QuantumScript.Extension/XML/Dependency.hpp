// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_XML_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

#ifndef XYO_FILEXML_HPP
#	include <XYO/FileXML.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_INTERNAL
#	ifdef QUANTUM_SCRIPT__XML_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_XML_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_XML_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT XYO_LIBRARY_IMPORT
#endif

#endif

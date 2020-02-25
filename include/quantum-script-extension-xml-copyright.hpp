//
// Quantum Script Extension XML
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_COPYRIGHT_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_COPYRIGHT_HPP

#define QUANTUM_SCRIPT_EXTENSION_XML_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_XML_PUBLISHER            "Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_XML_COMPANY              QUANTUM_SCRIPT_EXTENSION_XML_PUBLISHER
#define QUANTUM_SCRIPT_EXTENSION_XML_CONTACT              "g_stefan@yahoo.com"
#define QUANTUM_SCRIPT_EXTENSION_XML_FULL_COPYRIGHT       QUANTUM_SCRIPT_EXTENSION_XML_COPYRIGHT " <" QUANTUM_SCRIPT_EXTENSION_XML_CONTACT ">"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_XML__EXPORT_HPP
#include "quantum-script-extension-xml--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {
				namespace Copyright {
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *copyright();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *publisher();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *company();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *contact();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *fullCopyright();
				};
			};
		};
	};
};

#endif
#endif

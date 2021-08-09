//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_XML_VERSION_ABCD                 2,0,0,10
#define QUANTUM_SCRIPT_EXTENSION_XML_VERSION_STR                 "2.0.0"
#define QUANTUM_SCRIPT_EXTENSION_XML_VERSION_STR_BUILD           "10"
#define QUANTUM_SCRIPT_EXTENSION_XML_VERSION_STR_DATETIME        "2021-08-05 20:38:18"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_XML__EXPORT_HPP
#include "quantum-script-extension-xml--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_XML_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif


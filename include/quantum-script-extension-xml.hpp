//
// Quantum Script Extension XML
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_XML__EXPORT_HPP
#include "quantum-script-extension-xml--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_COPYRIGHT_HPP
#include "quantum-script-extension-xml-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_LICENSE_HPP
#include "quantum-script-extension-xml-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VERSION_HPP
#include "quantum-script-extension-xml-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace Quantum::Script;

				class XMLContext:
					public Object {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(XMLContext);
					public:

						Symbol symbolFunctionXMLAttribute;
						TPointerX<Prototype> prototypeXMLAttribute;

						Symbol symbolFunctionXMLAttributes;
						TPointerX<Prototype> prototypeXMLAttributes;

						Symbol symbolFunctionXMLNode;
						TPointerX<Prototype> prototypeXMLNode;

						Symbol symbolFunctionXMLDocument;
						TPointerX<Prototype> prototypeXMLDocument;

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT XMLContext();
				};

				QUANTUM_SCRIPT_EXTENSION_XML_EXPORT XMLContext *getContext();

				QUANTUM_SCRIPT_EXTENSION_XML_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_XML_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif


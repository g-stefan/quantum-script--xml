//
// Quantum Script Extension XML
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLDOCUMENT_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLDOCUMENT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				class VariableXMLDocument;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::XML::VariableXMLDocument>:
			public TMemoryPoolActive<Quantum::Script::Extension::XML::VariableXMLDocument> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableXMLDocument :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLDocument);
					protected:
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLDocument;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *typeXMLDocumentKey;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const void *typeXMLDocument;
					public:

						TPointerX<XMLDocument> value;

						inline VariableXMLDocument() {
							value.pointerLink(this);
							variableType = registerType(typeXMLDocument, typeXMLDocumentKey);
						};

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable &operatorReference(Symbol symbolId);

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String toString();

						//
						inline static bool isVariableXMLDocument(const Variable *value) {
							if(typeXMLDocument == nullptr) {
								typeXMLDocument = registerType(typeXMLDocument, typeXMLDocumentKey);
							};
							return (value->variableType == typeXMLDocument);
						};

				};

			};
		};
	};
};


#endif

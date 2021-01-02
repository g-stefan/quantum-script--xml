//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTE_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				class VariableXMLAttribute;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::XML::VariableXMLAttribute>:
			public TMemoryPoolActive<Quantum::Script::Extension::XML::VariableXMLAttribute> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableXMLAttribute :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLAttribute);
					protected:
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLAttribute;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *typeXMLAttributeKey;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const void *typeXMLAttribute;
					public:

						TPointer<XMLAttribute> value;

						inline VariableXMLAttribute() {
							variableType = registerType(typeXMLAttribute, typeXMLAttributeKey);
						};

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable &operatorReference(Symbol symbolId);

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String toString();

						//
						inline static bool isVariableXMLAttribute(const Variable *value) {
							if(typeXMLAttribute == nullptr) {
								typeXMLAttribute = registerType(typeXMLAttribute, typeXMLAttributeKey);
							};
							return (value->variableType == typeXMLAttribute);
						};

				};

			};
		};
	};
};


#endif

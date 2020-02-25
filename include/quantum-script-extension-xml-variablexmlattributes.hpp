//
// Quantum Script Extension XML
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTES_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTES_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				class VariableXMLAttributes;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::XML::VariableXMLAttributes>:
			public TMemoryPoolActive<Quantum::Script::Extension::XML::VariableXMLAttributes> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableXMLAttributes :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLAttributes);
					protected:
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLAttributes;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *typeXMLAttributesKey;
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const void *typeXMLAttributes;
					public:

						TPointer<XMLNode::Attributes> value;

						inline VariableXMLAttributes() {
							variableType = registerType(typeXMLAttributes, typeXMLAttributesKey);
						};

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable &operatorReference(Symbol symbolId);

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String toString();

						//
						inline static bool isVariableXMLAttributes(const Variable *value) {
							if(typeXMLAttributes == nullptr) {
								typeXMLAttributes = registerType(typeXMLAttributes, typeXMLAttributesKey);
							};
							return (value->variableType == typeXMLAttributes);
						};

				};

			};
		};
	};
};


#endif

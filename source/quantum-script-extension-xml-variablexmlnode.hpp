//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLNODE_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLNODE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				class VariableXMLNode;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::XML::VariableXMLNode>:
			public TMemoryPoolActive<Quantum::Script::Extension::XML::VariableXMLNode> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableXMLNode :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLNode);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_XML_EXPORT, VariableXMLNode);
					protected:
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLNode;
					public:

						TPointerX<XMLNode> value;

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT VariableXMLNode();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_XML_EXPORT String toString();

				};

			};
		};
	};
};


#endif

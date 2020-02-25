//
// Quantum Script Extension XML
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-xml-variablexmlattribute.hpp"

#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;

				const char *VariableXMLAttribute::typeXMLAttributeKey = "{7706E296-8BEC-44EE-92CF-C7FB2BE5FC84}";
				const void *VariableXMLAttribute::typeXMLAttribute;
				const char *VariableXMLAttribute::strTypeXMLAttribute = "XMLAttribute";

				String VariableXMLAttribute::getType() {
					return strTypeXMLAttribute;
				};

				Variable *VariableXMLAttribute::newVariable() {
					return (Variable *) TMemory<VariableXMLAttribute>::newMemory();
				};

				Variable &VariableXMLAttribute::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::XML::getContext())->prototypeXMLAttribute->prototype);
				};

				Variable *VariableXMLAttribute::instancePrototype() {
					return (Extension::XML::getContext())->prototypeXMLAttribute->prototype;
				};

				bool VariableXMLAttribute::toBoolean() {
					return true;
				};

				String VariableXMLAttribute::toString() {
					return strTypeXMLAttribute;
				};

			};
		};
	};
};



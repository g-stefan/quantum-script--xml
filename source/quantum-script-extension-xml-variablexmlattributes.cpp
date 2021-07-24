//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-xml-variablexmlattributes.hpp"

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

				XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLAttributes, "{640ABD57-96BE-426F-8CC7-6B92A437AD1C}");
				const char *VariableXMLAttributes::strTypeXMLAttributes = "XMLAttributes";

				VariableXMLAttributes::VariableXMLAttributes() {
					XYO_DYNAMIC_TYPE_PUSH(VariableXMLAttributes);
				};

				String VariableXMLAttributes::getVariableType() {
					return strTypeXMLAttributes;
				};

				Variable *VariableXMLAttributes::newVariable() {
					return (Variable *) TMemory<VariableXMLAttributes>::newMemory();
				};

				Variable *VariableXMLAttributes::instancePrototype() {
					return (Extension::XML::getContext())->prototypeXMLAttributes->prototype;
				};

				bool VariableXMLAttributes::toBoolean() {
					return true;
				};

				String VariableXMLAttributes::toString() {
					return strTypeXMLAttributes;
				};

			};
		};
	};
};



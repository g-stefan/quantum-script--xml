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
#include "quantum-script-extension-xml-variablexmlnode.hpp"

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

				const char *VariableXMLNode::typeXMLNodeKey = "{F271D918-1AAC-4D19-845E-985F97B069C5}";
				const void *VariableXMLNode::typeXMLNode;
				const char *VariableXMLNode::strTypeXMLNode = "XMLNode";

				String VariableXMLNode::getType() {
					return strTypeXMLNode;
				};

				Variable *VariableXMLNode::newVariable() {
					return (Variable *) TMemory<VariableXMLNode>::newMemory();
				};

				Variable &VariableXMLNode::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::XML::getContext())->prototypeXMLNode->prototype);
				};

				Variable *VariableXMLNode::instancePrototype() {
					return (Extension::XML::getContext())->prototypeXMLNode->prototype;
				};

				bool VariableXMLNode::toBoolean() {
					return true;
				};

				String VariableXMLNode::toString() {
					return strTypeXMLNode;
				};

			};
		};
	};
};



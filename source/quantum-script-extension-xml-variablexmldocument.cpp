//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-xml-variablexmldocument.hpp"

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

				XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLDocument, "{58C5681E-BBD7-43EE-A4AE-0B1B98838214}");
				const char *VariableXMLDocument::strTypeXMLDocument = "XMLDocument";

				VariableXMLDocument::VariableXMLDocument() {
					XYO_DYNAMIC_TYPE_PUSH(VariableXMLDocument);
					value.pointerLink(this);
				};

				String VariableXMLDocument::getVariableType() {
					return strTypeXMLDocument;
				};

				Variable *VariableXMLDocument::newVariable() {
					return (Variable *)TMemory<VariableXMLDocument>::newMemory();
				};

				Variable *VariableXMLDocument::instancePrototype() {
					return (Extension::XML::getContext())->prototypeXMLDocument->prototype;
				};

				bool VariableXMLDocument::toBoolean() {
					return true;
				};

				String VariableXMLDocument::toString() {
					return strTypeXMLDocument;
				};

			};
		};
	};
};

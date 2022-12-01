//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <XYO/QuantumScript.Extension/XML/VariableXMLAttribute.hpp>
#include <XYO/QuantumScript.Extension/XML/Context.hpp>

namespace XYO::QuantumScript::Extension::XML {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLAttribute, "{7706E296-8BEC-44EE-92CF-C7FB2BE5FC84}");
	const char *VariableXMLAttribute::strTypeXMLAttribute = "XMLAttribute";

	VariableXMLAttribute::VariableXMLAttribute() {
		XYO_DYNAMIC_TYPE_PUSH(VariableXMLAttribute);
	};

	String VariableXMLAttribute::getVariableType() {
		return strTypeXMLAttribute;
	};

	Variable *VariableXMLAttribute::newVariable() {
		return (Variable *)TMemory<VariableXMLAttribute>::newMemory();
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

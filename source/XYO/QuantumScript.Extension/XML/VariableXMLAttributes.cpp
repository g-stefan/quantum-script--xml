//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <XYO/QuantumScript.Extension/XML/VariableXMLAttributes.hpp>
#include <XYO/QuantumScript.Extension/XML/Context.hpp>

namespace XYO::QuantumScript::Extension::XML {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLAttributes, "{640ABD57-96BE-426F-8CC7-6B92A437AD1C}");
	const char *VariableXMLAttributes::strTypeXMLAttributes = "XMLAttributes";

	VariableXMLAttributes::VariableXMLAttributes() {
		XYO_DYNAMIC_TYPE_PUSH(VariableXMLAttributes);
	};

	String VariableXMLAttributes::getVariableType() {
		return strTypeXMLAttributes;
	};

	Variable *VariableXMLAttributes::newVariable() {
		return (Variable *)TMemory<VariableXMLAttributes>::newMemory();
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

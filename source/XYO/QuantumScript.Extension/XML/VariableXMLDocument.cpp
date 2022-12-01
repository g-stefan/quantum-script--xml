//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <XYO/QuantumScript.Extension/XML/VariableXMLDocument.hpp>
#include <XYO/QuantumScript.Extension/XML/Context.hpp>

namespace XYO::QuantumScript::Extension::XML {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLDocument, "{58C5681E-BBD7-43EE-A4AE-0B1B98838214}");
	const char *VariableXMLDocument::strTypeXMLDocument = "XMLDocument";

	VariableXMLDocument::VariableXMLDocument() {
		XYO_DYNAMIC_TYPE_PUSH(VariableXMLDocument);
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

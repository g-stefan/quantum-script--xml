//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/XML/VariableXMLNode.hpp>
#include <XYO/QuantumScript.Extension/XML/Context.hpp>

namespace XYO::QuantumScript::Extension::XML {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableXMLNode, "{F271D918-1AAC-4D19-845E-985F97B069C5}");
	const char *VariableXMLNode::strTypeXMLNode = "XMLNode";

	VariableXMLNode::VariableXMLNode() {
		XYO_DYNAMIC_TYPE_PUSH(VariableXMLNode);
	};

	String VariableXMLNode::getVariableType() {
		return strTypeXMLNode;
	};

	Variable *VariableXMLNode::newVariable() {
		return (Variable *)TMemory<VariableXMLNode>::newMemory();
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

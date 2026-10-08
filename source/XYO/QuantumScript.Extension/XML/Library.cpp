// Quantum Script Extension XML
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <math.h>

#include <XYO/QuantumScript.Extension/XML/Library.hpp>
#include <XYO/QuantumScript.Extension/XML/Copyright.hpp>
#include <XYO/QuantumScript.Extension/XML/License.hpp>
#include <XYO/QuantumScript.Extension/XML/Version.hpp>

#include <XYO/QuantumScript.Extension/XML/VariableXMLAttribute.hpp>
#include <XYO/QuantumScript.Extension/XML/VariableXMLAttributes.hpp>
#include <XYO/QuantumScript.Extension/XML/VariableXMLNode.hpp>
#include <XYO/QuantumScript.Extension/XML/VariableXMLDocument.hpp>
#include <XYO/QuantumScript.Extension/XML/Context.hpp>

namespace XYO::QuantumScript::Extension::XML {

	XMLContext::XMLContext() {
		symbolFunctionXMLAttribute = 0;
		prototypeXMLAttribute.pointerLink(this);
		symbolFunctionXMLAttributes = 0;
		prototypeXMLAttributes.pointerLink(this);
		symbolFunctionXMLNode = 0;
		prototypeXMLNode.pointerLink(this);
		symbolFunctionXMLDocument = 0;
		prototypeXMLDocument.pointerLink(this);
	};

	XMLContext *getContext() {
		return TSingleton<XMLContext>::getValue();
	};

	// index argument, false for undefined, NaN, infinite or negative values
	static bool getIndexArgument(Variable *value, size_t &index) {
		Number number = value->toNumber();
		if (isnan(number) || isinf(number) || (number < 0)) {
			return false;
		};
		index = static_cast<size_t>(number);
		return true;
	};

	// parser mode argument, Normal for missing or unknown values
	static FileXML::Mode getModeArgument(Variable *value) {
		Number number = value->toNumber();
		if (isnan(number) || (number < 0) || (number > static_cast<Number>(FileXML::Mode::IndentationTab))) {
			return FileXML::Mode::Normal;
		};
		return static_cast<FileXML::Mode>(static_cast<int>(number));
	};

	// true if branch is the child list of node or of one of its descendants
	static bool nodeOwnsBranch(FileXML::Node *node, FileXML::Node::Branch *branch) {
		if (!node) {
			return false;
		};
		FileXML::Node::Branch *nodeBranch = node->branch;
		if (!nodeBranch) {
			return false;
		};
		if (nodeBranch == branch) {
			return true;
		};
		for (FileXML::Node::Branch::Node *scan = nodeBranch->head; scan != nullptr; scan = scan->next) {
			if (nodeOwnsBranch(scan->value, branch)) {
				return true;
			};
		};
		return false;
	};

	// true if node is in branch or below it
	static bool branchContainsNode(FileXML::Node::Branch *branch, FileXML::Node *node) {
		if (!branch) {
			return false;
		};
		for (FileXML::Node::Branch::Node *scan = branch->head; scan != nullptr; scan = scan->next) {
			if (!scan->value) {
				continue;
			};
			if (scan->value == node) {
				return true;
			};
			if (branchContainsNode(scan->value->branch, node)) {
				return true;
			};
		};
		return false;
	};

	static TPointer<Variable> functionXMLAttribute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		return VariableXMLAttribute::newVariable();
	};

	static TPointer<Variable> functionXMLAttributes(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		return VariableXMLAttributes::newVariable();
	};

	static TPointer<Variable> functionXMLNode(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		return VariableXMLNode::newVariable();
	};

	static TPointer<Variable> functionXMLDocument(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		return VariableXMLDocument::newVariable();
	};

	static void deleteContext() {
		XMLContext *xmlContext = getContext();

		xmlContext->prototypeXMLAttribute.deleteMemory();
		xmlContext->symbolFunctionXMLAttribute = 0;

		xmlContext->prototypeXMLAttributes.deleteMemory();
		xmlContext->symbolFunctionXMLAttributes = 0;

		xmlContext->prototypeXMLNode.deleteMemory();
		xmlContext->symbolFunctionXMLNode = 0;

		xmlContext->prototypeXMLDocument.deleteMemory();
		xmlContext->symbolFunctionXMLDocument = 0;
	};

	static void newContext(Executive *executive, void *extensionId) {
		VariableFunction *defaultPrototypeFunction;

		XMLContext *xmlContext = getContext();
		executive->setExtensionDeleteContext(extensionId, deleteContext);

		//

		xmlContext->symbolFunctionXMLAttribute = Context::getSymbol("XMLAttribute");
		xmlContext->prototypeXMLAttribute.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLAttribute, nullptr, nullptr);
		(Context::getGlobalObject())->setPropertyBySymbol(xmlContext->symbolFunctionXMLAttribute, defaultPrototypeFunction);
		xmlContext->prototypeXMLAttribute = defaultPrototypeFunction->prototype;

		//

		xmlContext->symbolFunctionXMLAttributes = Context::getSymbol("XMLAttributes");
		xmlContext->prototypeXMLAttributes.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLAttributes, nullptr, nullptr);
		(Context::getGlobalObject())->setPropertyBySymbol(xmlContext->symbolFunctionXMLAttributes, defaultPrototypeFunction);
		xmlContext->prototypeXMLAttributes = defaultPrototypeFunction->prototype;

		//

		xmlContext->symbolFunctionXMLNode = Context::getSymbol("XMLNode");
		xmlContext->prototypeXMLNode.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLNode, nullptr, nullptr);
		(Context::getGlobalObject())->setPropertyBySymbol(xmlContext->symbolFunctionXMLNode, defaultPrototypeFunction);
		xmlContext->prototypeXMLNode = defaultPrototypeFunction->prototype;

		//

		xmlContext->symbolFunctionXMLDocument = Context::getSymbol("XMLDocument");
		xmlContext->prototypeXMLDocument.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLDocument, nullptr, nullptr);
		(Context::getGlobalObject())->setPropertyBySymbol(xmlContext->symbolFunctionXMLDocument, defaultPrototypeFunction);
		xmlContext->prototypeXMLDocument = defaultPrototypeFunction->prototype;
	};

	static TPointer<Variable> xml_load(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-load\n");
#endif
		String fileName = (arguments->index(0))->toString();
		FileXML::Mode mode = getModeArgument(arguments->index(1));
		TPointer<VariableXMLDocument> xml;

		xml.newMemory();
		if (XYO::FileXML::load(fileName, xml->value, mode)) {
			return xml.value();
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xml_loadFromString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-load-from-string\n");
#endif
		// undefined / null are not text, do not parse "undefined"
		if (TIsTypeExact<VariableUndefined>(arguments->index(0)) || TIsType<VariableNull>(arguments->index(0))) {
			return Context::getValueUndefined();
		};

		String text = (arguments->index(0))->toString();
		FileXML::Mode mode = getModeArgument(arguments->index(1));
		TPointer<VariableXMLDocument> xml;

		xml.newMemory();
		if (XYO::FileXML::loadFromString(text, xml->value, mode)) {
			return xml.value();
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_save(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-save\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		String fileName = (arguments->index(0))->toString();
		FileXML::Mode mode = getModeArgument(arguments->index(1));

		return VariableBoolean::newVariable(XYO::FileXML::save(fileName, ((VariableXMLDocument *)this_)->value, mode));
	};

	static TPointer<Variable> xmlDocument_saveToString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-save-to-string\n");
#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		FileXML::Mode mode = getModeArgument(arguments->index(0));
		String output;
		XYO::FileXML::saveToString(output, ((VariableXMLDocument *)this_)->value, mode);

		return VariableString::newVariable(output);
	};

	static TPointer<Variable> xmlAttribute_getName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attribute-get-name\n");

#endif
		if (!TIsType<VariableXMLAttribute>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttribute *)this_)->value)) {
			((VariableXMLAttribute *)this_)->value.newMemory();
		};

		return VariableString::newVariable(((VariableXMLAttribute *)this_)->value->name);
	};

	static TPointer<Variable> xmlAttribute_setName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attribute-set-name\n");

#endif
		if (!TIsType<VariableXMLAttribute>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttribute *)this_)->value)) {
			((VariableXMLAttribute *)this_)->value.newMemory();
		};

		((VariableXMLAttribute *)this_)->value->name = (arguments->index(0))->toString();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlAttribute_getValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attribute-get-value\n");

#endif
		if (!TIsType<VariableXMLAttribute>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttribute *)this_)->value)) {
			((VariableXMLAttribute *)this_)->value.newMemory();
		};

		return VariableString::newVariable(((VariableXMLAttribute *)this_)->value->value);
	};

	static TPointer<Variable> xmlAttribute_setValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attribute-set-value\n");

#endif
		if (!TIsType<VariableXMLAttribute>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttribute *)this_)->value)) {
			((VariableXMLAttribute *)this_)->value.newMemory();
		};

		((VariableXMLAttribute *)this_)->value->value = (arguments->index(0))->toString();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlAttributes_getIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-get-index\n");

#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttributes *)this_)->value)) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		// past the end: undefined, the list does not grow
		size_t index;
		if (!getIndexArgument(arguments->index(0), index)) {
			return Context::getValueUndefined();
		};
		if (index >= ((VariableXMLAttributes *)this_)->value->length()) {
			return Context::getValueUndefined();
		};

		TPointer<FileXML::Attribute> &attribute_ = ((VariableXMLAttributes *)this_)->value->index(index);
		if (!attribute_) {
			return Context::getValueUndefined();
		};
		VariableXMLAttribute *retV = static_cast<VariableXMLAttribute *>(VariableXMLAttribute::newVariable());
		retV->value = attribute_;
		return retV;
	};

	static TPointer<Variable> xmlAttributes_setIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-set-index\n");

#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!(((VariableXMLAttributes *)this_)->value)) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		if (!TIsType<VariableXMLAttribute>(arguments->index(1))) {
			throw(Error("invalid parameter"));
		};

		// index == length() appends, past that nothing happens (no gaps)
		size_t index;
		if (!getIndexArgument(arguments->index(0), index)) {
			return Context::getValueUndefined();
		};
		if (index > ((VariableXMLAttributes *)this_)->value->length()) {
			return Context::getValueUndefined();
		};

		if (!((VariableXMLAttribute *)((arguments->index(1)).value()))->value) {
			((VariableXMLAttribute *)((arguments->index(1)).value()))->value.newMemory();
		};
		(((VariableXMLAttributes *)this_)->value->index(index)) = ((VariableXMLAttribute *)((arguments->index(1)).value()))->value;

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlAttributes_length(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-length\n");

#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLAttributes *)this_)->value) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		return VariableNumber::newVariable(((VariableXMLAttributes *)this_)->value->length());
	};

	// index of the first attribute named name, or -1
	static Number attributesIndexOf(FileXML::Node::Attributes *attributes, const String &name) {
		size_t length = attributes->length();
		for (size_t index = 0; index < length; ++index) {
			TPointer<FileXML::Attribute> &attribute_ = attributes->index(index);
			if (attribute_) {
				if (attribute_->name == name) {
					return static_cast<Number>(index);
				};
			};
		};
		return -1;
	};

	static TPointer<Variable> xmlAttributes_removeIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-remove-index\n");
#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLAttributes *)this_)->value) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		size_t index;
		if (getIndexArgument(arguments->index(0), index)) {
			((VariableXMLAttributes *)this_)->value->remove(index);
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlAttributes_indexOf(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-index-of\n");
#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLAttributes *)this_)->value) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		return VariableNumber::newVariable(attributesIndexOf(((VariableXMLAttributes *)this_)->value, (arguments->index(0))->toString()));
	};

	static TPointer<Variable> xmlAttributes_find(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-find\n");
#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLAttributes *)this_)->value) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		Number index = attributesIndexOf(((VariableXMLAttributes *)this_)->value, (arguments->index(0))->toString());
		if (index < 0) {
			return Context::getValueUndefined();
		};

		VariableXMLAttribute *retV = static_cast<VariableXMLAttribute *>(VariableXMLAttribute::newVariable());
		retV->value = ((VariableXMLAttributes *)this_)->value->index(static_cast<size_t>(index));
		return retV;
	};

	static TPointer<Variable> xmlAttributes_remove(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-attributes-remove\n");
#endif
		if (!TIsType<VariableXMLAttributes>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLAttributes *)this_)->value) {
			((VariableXMLAttributes *)this_)->value.newMemory();
		};

		// every attribute named name, from the end so indexes stay valid
		FileXML::Node::Attributes *attributes = ((VariableXMLAttributes *)this_)->value;
		String name = (arguments->index(0))->toString();
		size_t count = 0;
		for (size_t index = attributes->length(); index > 0; --index) {
			TPointer<FileXML::Attribute> &attribute_ = attributes->index(index - 1);
			if (attribute_) {
				if (attribute_->name == name) {
					attributes->remove(index - 1);
					++count;
				};
			};
		};

		return VariableNumber::newVariable(static_cast<Number>(count));
	};

	static TPointer<Variable> xmlNode_getType(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-get-type\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		return VariableNumber::newVariable(static_cast<Number>(((VariableXMLNode *)this_)->value->type));
	};

	static TPointer<Variable> xmlNode_setType(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-set-type\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		Number type = (arguments->index(0))->toNumber();
		if (isnan(type) || (type < 0) || (type > static_cast<Number>(FileXML::NodeType::DocumentTypeDefinition))) {
			throw(Error("invalid parameter"));
		};

		((VariableXMLNode *)this_)->value->type = static_cast<FileXML::NodeType>(static_cast<int>(type));

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlNode_getName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-get-name\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		return VariableString::newVariable(((VariableXMLNode *)this_)->value->name);
	};

	static TPointer<Variable> xmlNode_setName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-set-name\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		((VariableXMLNode *)this_)->value->name = (arguments->index(0))->toString();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlNode_getAttributes(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-get-attributes\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		if (!((VariableXMLNode *)this_)->value->attributes) {
			((VariableXMLNode *)this_)->value->attributes.newMemory();
		};

		VariableXMLAttributes *attributes = static_cast<VariableXMLAttributes *>(VariableXMLAttributes::newVariable());
		attributes->value = ((VariableXMLNode *)this_)->value->attributes;
		return attributes;
	};

	static TPointer<Variable> xmlNode_setAttributes(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-set-attributes\n");

#endif
		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		if (!TIsType<VariableXMLAttributes>(arguments->index(0))) {
			throw(Error("invalid parameter"));
		};

		if (!(static_cast<VariableXMLAttributes *>((arguments->index(0)).value()))->value) {
			(static_cast<VariableXMLAttributes *>((arguments->index(0)).value()))->value.newMemory();
		};
		((VariableXMLNode *)this_)->value->attributes = (static_cast<VariableXMLAttributes *>((arguments->index(0)).value()))->value;

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlNode_getBranch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-get-branch\n");

#endif

		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		VariableXMLDocument *document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
		document->value.newMemory();
		if (!((VariableXMLNode *)this_)->value->branch) {
			((VariableXMLNode *)this_)->value->branch.newMemory();
		};
		(*(document->value)) = ((VariableXMLNode *)this_)->value->branch.value();

		return document;
	};

	static TPointer<Variable> xmlNode_setBranch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-node-set-branch\n");

#endif

		if (!TIsType<VariableXMLNode>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLNode *)this_)->value) {
			((VariableXMLNode *)this_)->value.newMemory();
		};

		if (!TIsType<VariableXMLDocument>(arguments->index(0))) {
			throw(Error("invalid parameter"));
		};

		VariableXMLDocument *document = static_cast<VariableXMLDocument *>((arguments->index(0)).value());
		if (!document->value) {
			document->value.newMemory();
		};
		// an empty document gets its list now, so the node and the document share it
		if (!document->value->root) {
			document->value->root.newMemory();
		};
		if (branchContainsNode(document->value->root, ((VariableXMLNode *)this_)->value)) {
			throw(Error("node cannot contain itself"));
		};
		((VariableXMLNode *)this_)->value->branch = document->value->root;
		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_length(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-length\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		return VariableNumber::newVariable(((VariableXMLDocument *)this_)->value->length());
	};

	static TPointer<Variable> xmlDocument_getIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-get-index\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		size_t index;
		if (!getIndexArgument(arguments->index(0), index)) {
			return Context::getValueUndefined();
		};

		TPointer<FileXML::Node> node = ((VariableXMLDocument *)this_)->value->getIndex(index);
		if (node) {
			VariableXMLNode *retV = static_cast<VariableXMLNode *>(VariableXMLNode::newVariable());
			retV->value = node;
			return retV;
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_setIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-set-index\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		if (!TIsType<VariableXMLNode>(arguments->index(1))) {
			throw(Error("invalid parameter"));
		};

		VariableXMLNode *node = (VariableXMLNode *)((arguments->index(1)).value());
		if (!node->value) {
			node->value.newMemory();
		};

		size_t index;
		if (!getIndexArgument(arguments->index(0), index)) {
			return Context::getValueUndefined();
		};

		if (nodeOwnsBranch(node->value, ((VariableXMLDocument *)this_)->value->root)) {
			throw(Error("node cannot contain itself"));
		};

		((VariableXMLDocument *)this_)->value->setIndex(index, node->value);
		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_add(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-add\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		if (TIsType<VariableXMLNode>(arguments->index(0))) {
			VariableXMLNode *node = (VariableXMLNode *)((arguments->index(0)).value());
			if (!node->value) {
				node->value.newMemory();
			};

			if (nodeOwnsBranch(node->value, ((VariableXMLDocument *)this_)->value->root)) {
				throw(Error("node cannot contain itself"));
			};

			((VariableXMLDocument *)this_)->value->add(node->value);

			return arguments->index(0);
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_get(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-get\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
		document->value.newMemory();

		(*document->value) = ((VariableXMLDocument *)this_)->value->get((arguments->index(0))->toString());
		if (document->value) {
			return document.value();
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_find(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-find\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
		document->value.newMemory();

		(*document->value) = ((VariableXMLDocument *)this_)->value->find((arguments->index(0))->toString());
		if (document->value) {
			return document.value();
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_findWithAttributeValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-find-with-attribute-value\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
		document->value.newMemory();

		(*document->value) = ((VariableXMLDocument *)this_)->value->findWithAttributeValue((arguments->index(0))->toString(), (arguments->index(1))->toString(), (arguments->index(2))->toString());
		if (document->value) {
			return document.value();
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_addDocument(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-add-document\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!TIsType<VariableXMLDocument>(arguments->index(0))) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		VariableXMLDocument *document = (VariableXMLDocument *)((arguments->index(0)).value());
		if (!document->value) {
			document->value.newMemory();
		};

		FileXML::Node::Branch *root = ((VariableXMLDocument *)this_)->value->root;
		if (root && document->value->root) {
			for (FileXML::Node::Branch::Node *scan = document->value->root->head; scan != nullptr; scan = scan->next) {
				if (nodeOwnsBranch(scan->value, root)) {
					throw(Error("node cannot contain itself"));
				};
			};
		};

		((VariableXMLDocument *)this_)->value->addDocument(*(document->value));

		return Context::getValueUndefined();
	};

	static TPointer<Variable> xmlDocument_removeIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- xml-document-remove-index\n");

#endif
		if (!TIsType<VariableXMLDocument>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (!((VariableXMLDocument *)this_)->value) {
			((VariableXMLDocument *)this_)->value.newMemory();
		};

		size_t index;
		if (getIndexArgument(arguments->index(0), index)) {
			((VariableXMLDocument *)this_)->value->removeIndex(index);
		};

		return Context::getValueUndefined();
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("XML", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "XML\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "XML");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::XML::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		newContext(executive, extensionId);

		char buffer[128];

		//
		executive->compileStringX("var XMLNodeType={};");
		sprintf(buffer, "XMLNodeType.None=%d;", static_cast<int>(FileXML::NodeType::None));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.Element=%d;", static_cast<int>(FileXML::NodeType::Element));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.Content=%d;", static_cast<int>(FileXML::NodeType::Content));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.CDATA=%d;", static_cast<int>(FileXML::NodeType::CDATA));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.Comment=%d;", static_cast<int>(FileXML::NodeType::Comment));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.Declaration=%d;", static_cast<int>(FileXML::NodeType::Declaration));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.Bom=%d;", static_cast<int>(FileXML::NodeType::Bom));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLNodeType.DocumentTypeDefinition=%d;", static_cast<int>(FileXML::NodeType::DocumentTypeDefinition));
		executive->compileStringX(buffer);

		//
		executive->compileStringX("var XMLParserMode={};");
		sprintf(buffer, "XMLParserMode.Normal=%d;", static_cast<int>(FileXML::Mode::Normal));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLParserMode.Minified=%d;", static_cast<int>(FileXML::Mode::Minified));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLParserMode.Indentation4Spaces=%d;", static_cast<int>(FileXML::Mode::Indentation4Spaces));
		executive->compileStringX(buffer);
		sprintf(buffer, "XMLParserMode.IndentationTab=%d;", static_cast<int>(FileXML::Mode::IndentationTab));
		executive->compileStringX(buffer);

		//
		executive->compileStringX("var XML={};");
		executive->setFunction2("XML.load(fileName,parserMode)", xml_load);
		executive->setFunction2("XML.loadFromString(text,parserMode)", xml_loadFromString);
		//
		executive->setFunction2("XMLAttribute.prototype.getName()", xmlAttribute_getName);
		executive->setFunction2("XMLAttribute.prototype.setName(value)", xmlAttribute_setName);
		executive->setFunction2("XMLAttribute.prototype.getValue()", xmlAttribute_getValue);
		executive->setFunction2("XMLAttribute.prototype.setValue(value)", xmlAttribute_setValue);
		//
		executive->setFunction2("XMLAttributes.prototype.getIndex(index)", xmlAttributes_getIndex);
		executive->setFunction2("XMLAttributes.prototype.setIndex(index,value)", xmlAttributes_setIndex);
		executive->setFunction2("XMLAttributes.prototype.length()", xmlAttributes_length);
		executive->setFunction2("XMLAttributes.prototype.removeIndex(index)", xmlAttributes_removeIndex);
		executive->setFunction2("XMLAttributes.prototype.indexOf(name)", xmlAttributes_indexOf);
		executive->setFunction2("XMLAttributes.prototype.find(name)", xmlAttributes_find);
		executive->setFunction2("XMLAttributes.prototype.remove(name)", xmlAttributes_remove);
		//
		executive->setFunction2("XMLNode.prototype.getType()", xmlNode_getType);
		executive->setFunction2("XMLNode.prototype.setType(value)", xmlNode_setType);
		executive->setFunction2("XMLNode.prototype.getName()", xmlNode_getName);
		executive->setFunction2("XMLNode.prototype.setName(value)", xmlNode_setName);
		executive->setFunction2("XMLNode.prototype.getAttributes()", xmlNode_getAttributes);
		executive->setFunction2("XMLNode.prototype.setAttributes(value)", xmlNode_setAttributes);
		executive->setFunction2("XMLNode.prototype.getBranch()", xmlNode_getBranch);
		executive->setFunction2("XMLNode.prototype.setBranch(value)", xmlNode_setBranch);
		//
		executive->setFunction2("XMLDocument.prototype.save(fileName,parserMode)", xmlDocument_save);
		executive->setFunction2("XMLDocument.prototype.saveToString(parserMode)", xmlDocument_saveToString);
		executive->setFunction2("XMLDocument.prototype.length()", xmlDocument_length);
		executive->setFunction2("XMLDocument.prototype.getIndex(idx)", xmlDocument_getIndex);
		executive->setFunction2("XMLDocument.prototype.setIndex(idx,value)", xmlDocument_setIndex);
		executive->setFunction2("XMLDocument.prototype.add(node)", xmlDocument_add);
		executive->setFunction2("XMLDocument.prototype.get(name)", xmlDocument_get);
		executive->setFunction2("XMLDocument.prototype.find(name)", xmlDocument_find);
		executive->setFunction2("XMLDocument.prototype.findWithAttributeValue(name,attribute,value)", xmlDocument_findWithAttributeValue);
		executive->setFunction2("XMLDocument.prototype.addDocument(document)", xmlDocument_addDocument);
		executive->setFunction2("XMLDocument.prototype.removeIndex(index)", xmlDocument_removeIndex);
	};

};

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY
#	ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::XML::initExecutive(executive, extensionId);
};
#	endif
#endif

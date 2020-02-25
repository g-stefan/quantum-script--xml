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

#include "quantum-script-extension-xml-license.hpp"
#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-xml-variablexmlattribute.hpp"
#include "quantum-script-extension-xml-variablexmlattributes.hpp"
#include "quantum-script-extension-xml-variablexmlnode.hpp"
#include "quantum-script-extension-xml-variablexmldocument.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_XML_NO_VERSION
#include "quantum-script-extension-xml-version.hpp"
#endif

#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace XML {

				using namespace XYO;
				using namespace Quantum::Script;


				XMLContext::XMLContext() {
					symbolFunctionXMLDocument = 0;
					prototypeXMLDocument.pointerLink(this);
				};

				XMLContext *getContext() {
					return TSingleton<XMLContext>::getValue();
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
					((Context::getGlobalObject())->operatorReferenceOwnProperty(xmlContext->symbolFunctionXMLAttribute))=defaultPrototypeFunction;
					xmlContext->prototypeXMLAttribute = defaultPrototypeFunction->prototype;

					//

					xmlContext->symbolFunctionXMLAttributes = Context::getSymbol("XMLAttributes");
					xmlContext->prototypeXMLAttributes.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLAttributes, nullptr, nullptr);
					((Context::getGlobalObject())->operatorReferenceOwnProperty(xmlContext->symbolFunctionXMLAttributes))=defaultPrototypeFunction;
					xmlContext->prototypeXMLAttributes = defaultPrototypeFunction->prototype;

					//

					xmlContext->symbolFunctionXMLNode = Context::getSymbol("XMLNode");
					xmlContext->prototypeXMLNode.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLNode, nullptr, nullptr);
					((Context::getGlobalObject())->operatorReferenceOwnProperty(xmlContext->symbolFunctionXMLNode))=defaultPrototypeFunction;
					xmlContext->prototypeXMLNode = defaultPrototypeFunction->prototype;

					//

					xmlContext->symbolFunctionXMLDocument = Context::getSymbol("XMLDocument");
					xmlContext->prototypeXMLDocument.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionXMLDocument, nullptr, nullptr);
					((Context::getGlobalObject())->operatorReferenceOwnProperty(xmlContext->symbolFunctionXMLDocument))=defaultPrototypeFunction;
					xmlContext->prototypeXMLDocument = defaultPrototypeFunction->prototype;

				};

				static TPointer<Variable> xml_load(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-load\n");
#endif
					String fileName = (arguments->index(0))->toString();
					size_t mode = (arguments->index(1))->toIndex();
					TPointer<VariableXMLDocument> xml;

					xml.newMemory();
					xml->value = XYO::XMLReader::load(fileName, mode);
					if(xml->value) {
						return xml.value();
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlDocument_save(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-save\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					String fileName = (arguments->index(0))->toString();
					size_t mode = (arguments->index(1))->toIndex();

					return VariableBoolean::newVariable(XYO::XMLWriter::save(*(((VariableXMLDocument *)this_)->value), fileName, mode));
				};

				static TPointer<Variable> xmlAttribute_getName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attribute-get-name\n");

#endif
					if(!VariableXMLAttribute::isVariableXMLAttribute(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttribute *)this_)->value)) {
						((VariableXMLAttribute *)this_)->value.newMemory();
					};

					return VariableString::newVariable(((VariableXMLAttribute *)this_)->value->name);
				};


				static TPointer<Variable> xmlAttribute_setName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attribute-set-name\n");

#endif
					if(!VariableXMLAttribute::isVariableXMLAttribute(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttribute *)this_)->value)) {
						((VariableXMLAttribute *)this_)->value.newMemory();
					};

					((VariableXMLAttribute *)this_)->value->name = (arguments->index(0))->toString();

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlAttribute_getValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attribute-get-value\n");

#endif
					if(!VariableXMLAttribute::isVariableXMLAttribute(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttribute *)this_)->value)) {
						((VariableXMLAttribute *)this_)->value.newMemory();
					};

					return VariableString::newVariable(((VariableXMLAttribute *)this_)->value->value);
				};

				static TPointer<Variable> xmlAttribute_setValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attribute-set-value\n");

#endif
					if(!VariableXMLAttribute::isVariableXMLAttribute(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttribute *)this_)->value)) {
						((VariableXMLAttribute *)this_)->value.newMemory();
					};

					((VariableXMLAttribute *)this_)->value->value = (arguments->index(0))->toString();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlAttributes_getIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attributes-get-index\n");

#endif
					if(!VariableXMLAttributes::isVariableXMLAttributes(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttributes *)this_)->value)) {
						((VariableXMLAttributes *)this_)->value.newMemory();
					};


					size_t index = (arguments->index(0))->toIndex();


					VariableXMLAttribute *retV = static_cast<VariableXMLAttribute *>(VariableXMLAttribute::newVariable());
					TPointer<XMLDocument::Attribute> &attribute_ = ((VariableXMLAttributes *)this_)->value->index(index);
					if(!attribute_) {
						attribute_.newMemory();
					};
					retV->value = attribute_;
					return retV;
				};

				static TPointer<Variable> xmlAttributes_setIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attributes-setIndex\n");

#endif
					if(!VariableXMLAttributes::isVariableXMLAttributes(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!(((VariableXMLAttributes *)this_)->value)) {
						((VariableXMLAttributes *)this_)->value.newMemory();
					};

					size_t index = (arguments->index(0))->toIndex();

					if(!VariableXMLAttribute::isVariableXMLAttribute(arguments->index(1))) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLAttribute *)((arguments->index(1)).value()))->value) {
						((VariableXMLAttribute *)((arguments->index(1)).value()))->value.newMemory();
					};
					(((VariableXMLAttributes *)this_)->value->index(index)) = ((VariableXMLAttribute *)((arguments->index(1)).value()))->value;

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlAttributes_length(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-attributes-length\n");

#endif
					if(!VariableXMLAttributes::isVariableXMLAttributes(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLAttributes *)this_)->value) {
						((VariableXMLAttributes *)this_)->value.newMemory();
					};

					return VariableNumber::newVariable(((VariableXMLAttributes *)this_)->value->length());
				};

				static TPointer<Variable> xmlNode_getType(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-get-type\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					return VariableNumber::newVariable(((VariableXMLNode *)this_)->value->type);
				};


				static TPointer<Variable> xmlNode_setType(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-set-type\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					((VariableXMLNode *)this_)->value->type = (arguments->index(0))->toIndex();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlNode_getName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-get-name\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					return VariableString::newVariable(((VariableXMLNode *)this_)->value->name);
				};

				static TPointer<Variable> xmlNode_setName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-name\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					((VariableXMLNode *)this_)->value->name = (arguments->index(0))->toString();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlNode_getAttributes(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-get-attributes\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					VariableXMLAttributes *attributes = static_cast<VariableXMLAttributes *>(VariableXMLAttributes::newVariable());
					attributes->value = ((VariableXMLNode *)this_)->value->attributes;
					return attributes;
				};

				static TPointer<Variable> xmlNode_setAttributes(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-set-attributes\n");

#endif
					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					if(!VariableXMLAttributes::isVariableXMLAttributes(arguments->index(1))) {
						throw(Error("invalid parameter"));
					};

					if(!(static_cast<VariableXMLAttributes *>((arguments->index(1)).value()))->value) {
						(static_cast<VariableXMLAttributes *>((arguments->index(1)).value()))->value.newMemory();
					};
					((VariableXMLNode *)this_)->value->attributes = (static_cast<VariableXMLAttributes *>((arguments->index(1)).value()))->value;

					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlNode_getBranch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-get-branch\n");

#endif

					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					VariableXMLDocument *document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
					document->value.newMemory();
					if(!((VariableXMLNode *)this_)->value->branch) {
						((VariableXMLNode *)this_)->value->branch.newMemory();
					};
					(*(document->value)) = ((VariableXMLNode *)this_)->value->branch.value();

					return document;
				};


				static TPointer<Variable> xmlNode_setBranch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-node-set-branch\n");

#endif

					if(!VariableXMLNode::isVariableXMLNode(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)this_)->value) {
						((VariableXMLNode *)this_)->value.newMemory();
					};

					if(!VariableXMLDocument::isVariableXMLDocument(arguments->index(1))) {
						throw(Error("invalid parameter"));
					};

					if(!(static_cast<VariableXMLDocument *>((arguments->index(1)).value()))->value) {
						(static_cast<VariableXMLDocument *>((arguments->index(1)).value()))->value.newMemory();
					};
					((VariableXMLNode *)this_)->value->branch = (static_cast<VariableXMLDocument *>((arguments->index(1)).value()))->value->root;
					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlDocument_length(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-length\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					return VariableNumber::newVariable(((VariableXMLDocument *)this_)->value->length());
				};


				static TPointer<Variable> xmlDocument_getIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-get-index\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					TPointer<XMLDocument::Node> node = ((VariableXMLDocument *)this_)->value->getIndex((arguments->index(0))->toIndex());
					if(node) {
						VariableXMLNode *retV = static_cast<VariableXMLNode *>(VariableXMLNode::newVariable());
						retV->value = node;
						return retV;
					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_setIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-set-index\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					if(!VariableXMLNode::isVariableXMLNode(arguments->index(1))) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLNode *)((arguments->index(1)).value()))->value) {
						((VariableXMLNode *)((arguments->index(1)).value()))->value.newMemory();
					};

					((VariableXMLDocument *)this_)->value->setIndex((arguments->index(0))->toIndex(), ((VariableXMLNode *)((arguments->index(1)).value()))->value);
					return Context::getValueUndefined();
				};

				static TPointer<Variable> xmlDocument_add(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-add\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					if(VariableXMLNode::isVariableXMLNode(arguments->index(0))) {
						if(!((VariableXMLNode *)((arguments->index(0)).value()))->value) {
							((VariableXMLNode *)((arguments->index(0)).value()))->value.newMemory();
						};

						((VariableXMLDocument *)this_)->value->add(((VariableXMLNode *)((arguments->index(0)).value()))->value);

						return arguments->index(0);
					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_get(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-get\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};


					TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
					document->value.newMemory();

					(*document->value) = ((VariableXMLDocument *)this_)->value->get((arguments->index(0))->toString());
					if(document->value) {
						return document.value();
					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_find(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-find\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};


					TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
					document->value.newMemory();

					(*document->value) = ((VariableXMLDocument *)this_)->value->find((arguments->index(0))->toString());
					if(document->value) {
						return document.value();
					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_findWithAttributeValue(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-find-with-attribute-value\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};


					TPointer<VariableXMLDocument> document = static_cast<VariableXMLDocument *>(VariableXMLDocument::newVariable());
					document->value.newMemory();

					(*document->value) = ((VariableXMLDocument *)this_)->value->findWithAttributeValue((arguments->index(0))->toString(), (arguments->index(1))->toString(), (arguments->index(2))->toString());
					if(document->value) {
						return document.value();
					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_addDocument(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-add-document\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!VariableXMLDocument::isVariableXMLDocument(arguments->index(0))) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					if(!((VariableXMLDocument *)((arguments->index(0)).value()))->value) {
						((VariableXMLDocument *)((arguments->index(0)).value()))->value.newMemory();
					};

					((VariableXMLDocument *)this_)->value->addDocument(*(((VariableXMLDocument *)((arguments->index(0)).value()))->value));

					return Context::getValueUndefined();
				};


				static TPointer<Variable> xmlDocument_removeIndex(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- xml-document-add-list\n");

#endif
					if(!VariableXMLDocument::isVariableXMLDocument(this_)) {
						throw(Error("invalid parameter"));
					};

					if(!VariableXMLDocument::isVariableXMLDocument(arguments->index(0))) {
						throw(Error("invalid parameter"));
					};

					if(!((VariableXMLDocument *)this_)->value) {
						((VariableXMLDocument *)this_)->value.newMemory();
					};

					((VariableXMLDocument *)this_)->value->removeIndex((arguments->index(0))->toIndex());

					return Context::getValueUndefined();
				};


				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("XML", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "XML\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "XML");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_XML_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::XML::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					char buffer[128];

					//
					executive->compileStringX("var XMLNodeType={};");
					sprintf(buffer, "XMLNodeType.None=%d;", XMLNodeType::None);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.Element=%d;", XMLNodeType::Element);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.Content=%d;", XMLNodeType::Content);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.CDATA=%d;", XMLNodeType::CDATA);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.Comment=%d;", XMLNodeType::Comment);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.Declaration=%d;", XMLNodeType::Declaration);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.Bom=%d;", XMLNodeType::Bom);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLNodeType.DocumentTypeDefinition=%d;", XMLNodeType::DocumentTypeDefinition);
					executive->compileStringX(buffer);

					//
					executive->compileStringX("var XMLParserMode={};");
					sprintf(buffer, "XMLParserMode.Normal=%d;", XMLParserMode::Normal);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLParserMode.Trim=%d;", XMLParserMode::Trim);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLParserMode.Indentation4Spaces=%d;", XMLParserMode::Indentation4Spaces);
					executive->compileStringX(buffer);
					sprintf(buffer, "XMLParserMode.IndentationTab=%d;", XMLParserMode::IndentationTab);
					executive->compileStringX(buffer);

					//
					executive->compileStringX("var XML={};");
					executive->setFunction2("XML.load(fileName,parserMode)", xml_load);
					//
					executive->setFunction2("XMLAttribute.prototype.getName()", xmlAttribute_getName);
					executive->setFunction2("XMLAttribute.prototype.setName(value)", xmlAttribute_setName);
					executive->setFunction2("XMLAttribute.prototype.getValue()", xmlAttribute_getValue);
					executive->setFunction2("XMLAttribute.prototype.setValue(value)", xmlAttribute_setValue);
					//
					executive->setFunction2("XMLAttributes.prototype.getIndex()", xmlAttributes_getIndex);
					executive->setFunction2("XMLAttributes.prototype.setIndex(value)", xmlAttributes_setIndex);
					executive->setFunction2("XMLAttributes.prototype.length()", xmlAttributes_length);
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
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_XML_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::XML::initExecutive(executive, extensionId);
};
#endif


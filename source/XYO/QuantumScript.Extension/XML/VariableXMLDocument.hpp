//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLDOCUMENT_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLDOCUMENT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/XML/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::XML {

	class VariableXMLDocument;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::XML::VariableXMLDocument> : public TMemoryPoolActive<XYO::QuantumScript::Extension::XML::VariableXMLDocument> {};
};

namespace XYO::QuantumScript::Extension::XML {

	class VariableXMLDocument : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLDocument);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT, VariableXMLDocument);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLDocument;

		public:
			TPointer<FileXML::Document> value;

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT VariableXMLDocument();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String toString();
	};

};

#endif

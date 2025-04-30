//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTES_HPP
#define QUANTUM_SCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTES_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/XML/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::XML {
	class VariableXMLAttributes;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::XML::VariableXMLAttributes> : public TMemoryPoolActive<XYO::QuantumScript::Extension::XML::VariableXMLAttributes> {};
};

namespace XYO::QuantumScript::Extension::XML {

	class VariableXMLAttributes : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLAttributes);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT, VariableXMLAttributes);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLAttributes;

		public:
			TPointer<FileXML::Node::Attributes> value;

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT VariableXMLAttributes();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String toString();
	};

};

#endif

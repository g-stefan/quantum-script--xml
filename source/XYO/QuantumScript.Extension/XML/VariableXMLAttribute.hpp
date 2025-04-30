//
// Quantum Script Extension XML
//
// Copyright (c) 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_XML_VARIABLEXMLATTRIBUTE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/XML/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::XML {
	class VariableXMLAttribute;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::XML::VariableXMLAttribute> : public TMemoryPoolActive<XYO::QuantumScript::Extension::XML::VariableXMLAttribute> {};
};

namespace XYO::QuantumScript::Extension::XML {

	class VariableXMLAttribute : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableXMLAttribute);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT, VariableXMLAttribute);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static const char *strTypeXMLAttribute;

		public:
			TPointer<FileXML::Attribute> value;

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT VariableXMLAttribute();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT String toString();
	};
};

#endif

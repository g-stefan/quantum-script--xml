// Quantum Script Extension XML
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_XML_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_XML_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

namespace XYO::QuantumScript::Extension::XML {

	class XMLContext : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(XMLContext);

		public:
			Symbol symbolFunctionXMLAttribute;
			TPointerX<Prototype> prototypeXMLAttribute;

			Symbol symbolFunctionXMLAttributes;
			TPointerX<Prototype> prototypeXMLAttributes;

			Symbol symbolFunctionXMLNode;
			TPointerX<Prototype> prototypeXMLNode;

			Symbol symbolFunctionXMLDocument;
			TPointerX<Prototype> prototypeXMLDocument;

			XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT XMLContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT XMLContext *getContext();
};

#endif

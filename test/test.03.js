// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Reading test.03.xml, parser modes, round trips, malformed input

Script.requireExtension("Console");
Script.requireExtension("XML");

function check(name, value, expected) {
	if (value != expected) {
		throw new Error(name + ": expected [" + expected + "], got [" + value + "]");
	};
	Console.writeLn("ok: " + name);
};

function newNode(type, name) {
	var node = new XMLNode();
	node.setType(type);
	node.setName(name);
	return node;
};

// node tree as text: (type:name attributes[children])
function dump(list) {
	var retV = "";
	for (var k = 0; k < list.length(); ++k) {
		var node = list.getIndex(k);
		retV += "(" + node.getType() + ":" + node.getName();
		if (node.getType() == XMLNodeType.Element) {
			var attributes = node.getAttributes();
			for (var i = 0; i < attributes.length(); ++i) {
				retV += " " + attributes.getIndex(i).getName() + "=" + attributes.getIndex(i).getValue();
			};
			retV += "[" + dump(node.getBranch()) + "]";
		};
		retV += ")";
	};
	return retV;
};

function types(list) {
	var retV = "";
	for (var k = 0; k < list.length(); ++k) {
		retV += list.getIndex(k).getType();
	};
	return retV;
};

function attributeList(node) {
	var retV = "";
	var attributes = node.getAttributes();
	for (var k = 0; k < attributes.length(); ++k) {
		retV += attributes.getIndex(k).getName() + "=" + attributes.getIndex(k).getValue() + ";";
	};
	return retV;
};

function text(node) {
	return node.getBranch().getIndex(0).getName();
};

// a file holding text exactly as given (the writer does not escape text)
function writeRaw(fileName, value) {
	var document = new XMLDocument();
	document.add(newNode(XMLNodeType.Content, value));
	return document.save(fileName);
};

var fileName = "../../test/test.03.xml";

// --- Minified: the data

var xml = XML.load(fileName, XMLParserMode.Minified);
check("load Minified", typeof(xml), "XMLDocument");
check("top level types", types(xml), "" + XMLNodeType.Declaration + XMLNodeType.DocumentTypeDefinition + XMLNodeType.Comment + XMLNodeType.Element);
check("declaration", xml.getIndex(0).getName(), "xml version=\"1.0\" encoding=\"UTF-8\"");
check("DOCTYPE", xml.getIndex(1).getName(), "catalog");
check("comment", xml.getIndex(2).getName(), " catalog of books ");

var root = xml.get("c:catalog").getIndex(0);
check("prefixed root name", root.getName(), "c:catalog");
check("no namespace processing", xml.get("catalog").length(), 0);
check("root attributes", attributeList(root), "xmlns:c=urn:catalog;version=2;");
check("root children", types(root.getBranch()), "111");

var books = xml.find("book");
check("find book", books.length(), 2);
check("get book on root children", root.getBranch().get("book").length(), 2);
check("get book on top level", xml.get("book").length(), 0);

var book1 = books.getIndex(0);
check("book 1 attributes, single quotes", attributeList(book1), "id=b1;lang=en;");
check("book 1 children", types(book1.getBranch()), "111");
check("entity kept raw", text(book1.getBranch().get("title").getIndex(0)), "XML &amp; You");
var note = book1.getBranch().get("note").getIndex(0).getBranch().getIndex(0);
check("CDATA type", note.getType(), XMLNodeType.CDATA);
check("CDATA text", note.getName(), "a < b && c > d");

var book2 = xml.findWithAttributeValue("book", "id", "b2").getIndex(0);
check("findWithAttributeValue", book2.getAttributes().getIndex(0).getValue(), "b2");
check("price USD", text(xml.findWithAttributeValue("price", "currency", "USD").getIndex(0)), "7");
check("price EUR", text(xml.findWithAttributeValue("price", "currency", "EUR").getIndex(0)), "10.50");
check("findWithAttributeValue no match", xml.findWithAttributeValue("price", "currency", "RON").length(), 0);
check("<empty></empty> has no children", book2.getBranch().get("empty").getIndex(0).getBranch().length(), 0);
check("<closed/> has no children", book2.getBranch().get("closed").getIndex(0).getBranch().length(), 0);

var paragraph = root.getBranch().get("p").getIndex(0);
check("mixed content", dump(paragraph.getBranch()), "(2:Hello )(1:b[(2:big)])(2: world)");
check("find inside mixed content", xml.find("b").length(), 1);

// --- Normal: the layout

var layout = XML.load(fileName);
check("load Normal", typeof(layout), "XMLDocument");
check("Normal top level types", types(layout), "" + XMLNodeType.Declaration + XMLNodeType.Content + XMLNodeType.DocumentTypeDefinition + XMLNodeType.Content + XMLNodeType.Comment + XMLNodeType.Content + XMLNodeType.Element + XMLNodeType.Content);
check("Normal line break", layout.getIndex(1).getName(), "\r\n");
var layoutBook = layout.find("book").getIndex(0);
check("Normal keeps indentation", layoutBook.getBranch().length(), 7);
check("Normal indentation text", layoutBook.getBranch().getIndex(0).getName(), "\r\n\t\t");
check("Normal and Minified queries agree", layout.find("price").length(), xml.find("price").length());

// --- round trips

var data = dump(xml);
var modes = [XMLParserMode.Normal, XMLParserMode.Minified, XMLParserMode.Indentation4Spaces, XMLParserMode.IndentationTab];
for (var k = 0; k < modes.length; ++k) {
	var outputName = "test.03.mode." + modes[k] + ".xml";
	check("save mode " + modes[k], xml.save(outputName, modes[k]), true);
	check("round trip mode " + modes[k], dump(XML.load(outputName, XMLParserMode.Minified)), data);
};

check("save Normal layout", layout.save("test.03.layout.xml"), true);
check("Normal round trip keeps the layout", dump(XML.load("test.03.layout.xml")), dump(layout));

// change a value, keep the layout
layout.findWithAttributeValue("price", "currency", "USD").getIndex(0).getBranch().getIndex(0).setName("8");
layout.save("test.03.changed.xml");
var changed = XML.load("test.03.changed.xml");
check("changed value", text(changed.findWithAttributeValue("price", "currency", "USD").getIndex(0)), "8");
check("layout kept after a change", types(changed), types(layout));

// escaped text stays escaped, raw CDATA stays raw
var escaped = new XMLDocument();
var element = escaped.add(newNode(XMLNodeType.Element, "e"));
element.getBranch().add(newNode(XMLNodeType.Content, "a &lt; b &amp; c"));
element.getBranch().add(newNode(XMLNodeType.CDATA, "x < y"));
escaped.save("test.03.escaped.xml");
check("escaped text round trip", dump(XML.load("test.03.escaped.xml")), "(1:e[(2:a &lt; b &amp; c)(3:x < y)])");

// byte order mark
var bom = new XMLDocument();
bom.add(newNode(XMLNodeType.Bom, "\xEF\xBB\xBF"));
bom.add(newNode(XMLNodeType.Element, "r"));
bom.save("test.03.bom.xml");
var bomLoaded = XML.load("test.03.bom.xml");
check("BOM node", bomLoaded.getIndex(0).getType(), XMLNodeType.Bom);
check("BOM bytes", bomLoaded.getIndex(0).getName(), "\xEF\xBB\xBF");
check("BOM then element", bomLoaded.getIndex(1).getName(), "r");

// nesting limit of the reader: 1024 elements
function nested(fileName, depth) {
	var document = new XMLDocument();
	var node = document.add(newNode(XMLNodeType.Element, "n"));
	for (var k = 1; k < depth; ++k) {
		node = node.getBranch().add(newNode(XMLNodeType.Element, "n"));
	};
	document.save(fileName);
	return fileName;
};
check("1024 nested elements load", XML.load(nested("test.03.depth.1024.xml", 1024)).find("n").length(), 1024);
check("1025 nested elements do not load", typeof(XML.load(nested("test.03.depth.1025.xml", 1025))), "undefined");

// --- input that loads

var good = ["<a/>", "<a></a>", "<a x='1' y=\"2\"/>", "text only", "<!-- c --><a/><b/>", "<a>&unknown;</a>", "<a><![CDATA[<b>]]></a>"];
for (var k = 0; k < good.length; ++k) {
	writeRaw("test.03.good.xml", good[k]);
	check("loads: " + good[k], typeof(XML.load("test.03.good.xml")), "XMLDocument");
};
writeRaw("test.03.good.xml", "  \r\n  ");
check("whitespace only, Normal", XML.load("test.03.good.xml").length(), 1);
check("whitespace only, Minified", XML.load("test.03.good.xml", XMLParserMode.Minified).length(), 0);

// --- malformed input

var bad = ["<a>", "<a>x", "<a></b>", "</a>", "<a></a></b>", "a < b", "<a", "<a x/>", "<a x=1/>", "<a x=\"1/>", "<a =\"1\"/>", "<!x>", "<!-- x", "<![CDATA[x]]", "<?xml", "<!DOCTYPE x [ "];
for (var k = 0; k < bad.length; ++k) {
	writeRaw("test.03.bad.xml", bad[k]);
	check("does not load: " + bad[k], typeof(XML.load("test.03.bad.xml")), "undefined");
};

// unescaped text written by a script makes a file that does not load
var unescaped = new XMLDocument();
unescaped.add(newNode(XMLNodeType.Element, "e")).getBranch().add(newNode(XMLNodeType.Content, "1 < 2"));
unescaped.save("test.03.unescaped.xml");
check("unescaped text does not load back", typeof(XML.load("test.03.unescaped.xml")), "undefined");

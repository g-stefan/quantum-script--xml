// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// XML.loadFromString, XMLDocument.saveToString, XMLAttributes removeIndex / indexOf / find / remove

Script.requireExtension("Console");
Script.requireExtension("XML");

function check(name, value, expected) {
	if (value != expected) {
		throw new Error(name + ": expected [" + expected + "], got [" + value + "]");
	};
	Console.writeLn("ok: " + name);
};

function checkThrows(name, fn, expected) {
	var message = "no error";
	try {
		fn();
	} catch (e) {
		message = e.message;
	};
	check(name, message, expected);
};

function newNode(type, name) {
	var node = new XMLNode();
	node.setType(type);
	node.setName(name);
	return node;
};

function newAttribute(name, value) {
	var attribute = new XMLAttribute();
	attribute.setName(name);
	attribute.setValue(value);
	return attribute;
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

function attributeNames(attributes) {
	var retV = "";
	for (var k = 0; k < attributes.length(); ++k) {
		if (k > 0) {
			retV += ",";
		};
		retV += attributes.getIndex(k).getName();
	};
	return retV;
};

// --- XML.loadFromString

var xml = XML.loadFromString("<a x='1'><b>t &amp; u</b><!-- c --></a>");
check("loadFromString", typeof(xml), "XMLDocument");
check("loadFromString content", dump(xml), "(1:a x=1[(1:b[(2:t &amp; u)])(4: c )])");

var text = "<r>\n  <a/>\n  <b> x </b>\n</r>";
check("loadFromString Normal", dump(XML.loadFromString(text)), "(1:r[(2:\n  )(1:a[])(2:\n  )(1:b[(2: x )])(2:\n)])");
check("loadFromString Minified", dump(XML.loadFromString(text, XMLParserMode.Minified)), "(1:r[(1:a[])(1:b[(2:x)])])");
check("loadFromString unknown mode is Normal", dump(XML.loadFromString(text, 9)), dump(XML.loadFromString(text)));

check("loadFromString malformed", typeof(XML.loadFromString("<a></b>")), "undefined");
check("loadFromString unescaped <", typeof(XML.loadFromString("<a>1 < 2</a>")), "undefined");
check("loadFromString empty string", typeof(XML.loadFromString("")), "undefined");
check("loadFromString()", typeof(XML.loadFromString()), "undefined");
check("loadFromString(undefined)", typeof(XML.loadFromString(undefined)), "undefined");
check("loadFromString(null)", typeof(XML.loadFromString(null)), "undefined");
check("loadFromString(5) is text", dump(XML.loadFromString(5)), "(2:5)");
check("loadFromString UTF-8", dump(XML.loadFromString("<a v=\"ă\">ş</a>")), "(1:a v=ă[(2:ş)])");
check("loadFromString BOM", XML.loadFromString("\xEF\xBB\xBF<a/>").getIndex(0).getType(), XMLNodeType.Bom);

// --- XMLDocument.saveToString

var document = new XMLDocument();
document.add(newNode(XMLNodeType.Declaration, "xml version=\"1.0\""));
var root = document.add(newNode(XMLNodeType.Element, "r"));
root.getAttributes().setIndex(0, newAttribute("v", "1"));
root.getBranch().add(newNode(XMLNodeType.Element, "a"));
root.getBranch().add(newNode(XMLNodeType.Element, "b")).getBranch().add(newNode(XMLNodeType.Content, "x"));

check("saveToString()", document.saveToString(), "<?xml version=\"1.0\"?><r v=\"1\"><a /><b>x</b></r>");
check("saveToString Normal", document.saveToString(XMLParserMode.Normal), document.saveToString());
check("saveToString Minified", document.saveToString(XMLParserMode.Minified), "<?xml version=\"1.0\"?><r v=\"1\"><a /><b>x</b></r>");
check("saveToString IndentationTab", document.saveToString(XMLParserMode.IndentationTab), "<?xml version=\"1.0\"?>\r\n<r v=\"1\">\r\n\t<a />\r\n\t<b>x</b>\r\n</r>\r\n");
check("saveToString Indentation4Spaces", document.saveToString(XMLParserMode.Indentation4Spaces), "<?xml version=\"1.0\"?>\r\n<r v=\"1\">\r\n    <a />\r\n    <b>x</b>\r\n</r>\r\n");
check("saveToString unknown mode is Normal", document.saveToString(9), document.saveToString());
check("saveToString empty document", (new XMLDocument()).saveToString(), "");
check("saveToString a query result", document.find("b").saveToString(), "<b>x</b>");
check("saveToString a branch view", root.getBranch().saveToString(), "<a /><b>x</b>");
check("saveToString type", typeof(document.saveToString()), "String");
checkThrows("saveToString on XMLNode", function() {
	XMLDocument.prototype.saveToString.call(new XMLNode());
}, "invalid parameter");

// same text as save to a file
document.save("test.04.xml", XMLParserMode.IndentationTab);
check("saveToString matches save", dump(XML.loadFromString(document.saveToString(XMLParserMode.IndentationTab))), dump(XML.load("test.04.xml")));

// round trips through every mode
var sample = XML.load("../../test/test.03.xml", XMLParserMode.Minified);
var data = dump(sample);
for (var mode = XMLParserMode.Normal; mode <= XMLParserMode.IndentationTab; ++mode) {
	check("string round trip mode " + mode, dump(XML.loadFromString(sample.saveToString(mode), XMLParserMode.Minified)), data);
};
var layout = XML.load("../../test/test.03.xml");
check("string round trip keeps the layout", dump(XML.loadFromString(layout.saveToString())), dump(layout));

// --- XMLAttributes find / indexOf / removeIndex / remove

var node = newNode(XMLNodeType.Element, "e");
var attributes = node.getAttributes();
// more than 8 attributes: the list is stored in blocks of 8
for (var k = 0; k < 12; ++k) {
	attributes.setIndex(attributes.length(), newAttribute("a" + k, "" + k));
};
attributes.setIndex(3, newAttribute("dup", "x"));
attributes.setIndex(8, newAttribute("dup", "y"));
attributes.setIndex(11, newAttribute("dup", "z"));
check("attributes", attributeNames(attributes), "a0,a1,a2,dup,a4,a5,a6,a7,dup,a9,a10,dup");

check("indexOf", attributes.indexOf("a5"), 5);
check("indexOf after the first block", attributes.indexOf("a10"), 10);
check("indexOf first of duplicates", attributes.indexOf("dup"), 3);
check("indexOf missing", attributes.indexOf("zz"), -1);
check("indexOf()", attributes.indexOf(), -1);
check("indexOf is case sensitive", attributes.indexOf("A5"), -1);

check("find", attributes.find("a9").getValue(), "9");
check("find first of duplicates", attributes.find("dup").getValue(), "x");
check("find missing", typeof(attributes.find("zz")), "undefined");
attributes.find("a9").setValue("nine");
check("find returns the attribute of the node", node.getAttributes().getIndex(9).getValue(), "nine");

attributes.removeIndex(1);
check("removeIndex", attributeNames(attributes), "a0,a2,dup,a4,a5,a6,a7,dup,a9,a10,dup");
attributes.removeIndex(9);
check("removeIndex after the first block", attributeNames(attributes), "a0,a2,dup,a4,a5,a6,a7,dup,a9,dup");
attributes.removeIndex(-1);
attributes.removeIndex(10);
attributes.removeIndex();
attributes.removeIndex(0 / 0);
check("removeIndex invalid or past the end", attributes.length(), 10);

check("remove duplicates", attributes.remove("dup"), 3);
check("after remove", attributeNames(attributes), "a0,a2,a4,a5,a6,a7,a9");
check("remove missing", attributes.remove("zz"), 0);
check("remove one", attributes.remove("a0"), 1);
check("indexOf after remove", attributes.indexOf("a2"), 0);
check("node sees the removals", attributeNames(node.getAttributes()), "a2,a4,a5,a6,a7,a9");
while (attributes.length() > 0) {
	attributes.removeIndex(attributes.length() - 1);
};
check("remove all", attributes.length(), 0);
attributes.removeIndex(0);
check("removeIndex on an empty list", attributes.length(), 0);
check("remove on an empty list", attributes.remove("a2"), 0);
check("find on an empty list", typeof(attributes.find("a2")), "undefined");
attributes.setIndex(0, newAttribute("again", "1"));
check("append after removing all", attributeNames(attributes), "again");

// on a loaded document, written back
var loaded = XML.loadFromString("<a x=\"1\" y=\"&amp;\" z=\"3\" y=\"4\"/>");
var loadedAttributes = loaded.getIndex(0).getAttributes();
check("find raw value", loadedAttributes.find("y").getValue(), "&amp;");
check("remove from loaded", loadedAttributes.remove("y"), 2);
loadedAttributes.removeIndex(loadedAttributes.indexOf("x"));
check("saved after removing", loaded.saveToString(), "<a z=\"3\" />");

checkThrows("removeIndex on XMLNode", function() {
	XMLAttributes.prototype.removeIndex.call(new XMLNode(), 0);
}, "invalid parameter");
checkThrows("find on XMLAttribute", function() {
	XMLAttributes.prototype.find.call(new XMLAttribute(), "x");
}, "invalid parameter");
checkThrows("indexOf on XMLDocument", function() {
	XMLAttributes.prototype.indexOf.call(new XMLDocument(), "x");
}, "invalid parameter");
checkThrows("remove on a string", function() {
	XMLAttributes.prototype.remove.call("x", "x");
}, "invalid parameter");

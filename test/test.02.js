// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Arguments, errors and tree integrity

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

function names(list) {
	var retV = "";
	for (var k = 0; k < list.length(); ++k) {
		retV += list.getIndex(k).getName();
	};
	return retV;
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

// --- XMLDocument index arguments

var list = new XMLDocument();
list.add(newNode(XMLNodeType.Element, "a"));
list.add(newNode(XMLNodeType.Element, "b"));
list.add(newNode(XMLNodeType.Element, "c"));

check("getIndex(-1)", typeof(list.getIndex(-1)), "undefined");
check("getIndex()", typeof(list.getIndex()), "undefined");
check("getIndex(NaN)", typeof(list.getIndex(0 / 0)), "undefined");
check("getIndex(Infinity)", typeof(list.getIndex(1 / 0)), "undefined");
check("getIndex(3)", typeof(list.getIndex(3)), "undefined");
check("getIndex(1.9)", list.getIndex(1.9).getName(), "b");
check("getIndex(\"2\")", list.getIndex("2").getName(), "c");

list.removeIndex(-1);
list.removeIndex();
list.removeIndex(3);
check("removeIndex invalid or past the end", names(list), "abc");

list.setIndex(-1, newNode(XMLNodeType.Element, "z"));
list.setIndex(3, newNode(XMLNodeType.Element, "z"));
check("setIndex invalid or past the end", names(list), "abc");
list.setIndex(1, newNode(XMLNodeType.Element, "z"));
check("setIndex", names(list), "azc");
list.removeIndex(1);
check("removeIndex", names(list), "ac");

var empty = new XMLDocument();
check("empty getIndex(0)", typeof(empty.getIndex(0)), "undefined");
empty.removeIndex(0);
empty.setIndex(0, newNode(XMLNodeType.Element, "z"));
check("empty removeIndex / setIndex", empty.length(), 0);

// --- XMLAttributes

var attributes = new XMLAttributes();
check("attributes getIndex(0) on empty", typeof(attributes.getIndex(0)), "undefined");
check("attributes do not grow on getIndex", attributes.length(), 0);
attributes.setIndex(0, newAttribute("a", "1"));
attributes.setIndex(5, newAttribute("x", "x"));
attributes.setIndex(-1, newAttribute("x", "x"));
attributes.setIndex(0 / 0, newAttribute("x", "x"));
check("attributes setIndex past length()", attributes.length(), 1);
attributes.setIndex(attributes.length(), newAttribute("b", "2"));
check("attributes setIndex(length()) appends", attributes.length(), 2);
attributes.setIndex(0, newAttribute("c", "3"));
check("attributes setIndex replaces", attributes.getIndex(0).getName() + attributes.getIndex(1).getName(), "cb");
check("attributes getIndex(2)", typeof(attributes.getIndex(2)), "undefined");
check("attributes getIndex(-1)", typeof(attributes.getIndex(-1)), "undefined");
check("attributes getIndex(1e12)", typeof(attributes.getIndex(1000000000000)), "undefined");
check("attributes getIndex()", typeof(attributes.getIndex()), "undefined");
check("attributes length unchanged", attributes.length(), 2);
checkThrows("attributes setIndex(0, string)", function() {
	attributes.setIndex(0, "x");
}, "invalid parameter");

var shared = newAttribute("s", "1");
var first = newNode(XMLNodeType.Element, "first");
var second = newNode(XMLNodeType.Element, "second");
first.getAttributes().setIndex(0, shared);
second.getAttributes().setIndex(0, shared);
shared.setValue("2");
check("attribute shared by two nodes", first.getAttributes().getIndex(0).getValue() + second.getAttributes().getIndex(0).getValue(), "22");

var sharedAttributes = new XMLAttributes();
first.setAttributes(sharedAttributes);
sharedAttributes.setIndex(0, newAttribute("t", "3"));
check("setAttributes shares the list", first.getAttributes().getIndex(0).getName(), "t");

// --- XMLNode type

var node = new XMLNode();
for (var type = XMLNodeType.None; type <= XMLNodeType.DocumentTypeDefinition; ++type) {
	node.setType(type);
	check("setType(" + type + ")", node.getType(), type);
};
node.setType(XMLNodeType.Element);
checkThrows("setType(8)", function() {
	node.setType(8);
}, "invalid parameter");
checkThrows("setType(-1)", function() {
	node.setType(-1);
}, "invalid parameter");
checkThrows("setType(\"abc\")", function() {
	node.setType("abc");
}, "invalid parameter");
checkThrows("setType()", function() {
	node.setType();
}, "invalid parameter");
check("type unchanged after invalid setType", node.getType(), XMLNodeType.Element);
node.setType("3");
check("setType(\"3\")", node.getType(), XMLNodeType.CDATA);

// --- parser modes

var mixed = new XMLDocument();
var paragraph = mixed.add(newNode(XMLNodeType.Element, "p"));
paragraph.getBranch().add(newNode(XMLNodeType.Content, "  Hello "));
paragraph.getBranch().add(newNode(XMLNodeType.Element, "b")).getBranch().add(newNode(XMLNodeType.Content, "big"));
paragraph.getBranch().add(newNode(XMLNodeType.Content, " world  "));

check("save Normal", mixed.save("test.02.normal.xml", XMLParserMode.Normal), true);
var normal = dump(XML.load("test.02.normal.xml"));
check("save without mode is Normal", mixed.save("test.02.mode.xml") && (dump(XML.load("test.02.mode.xml")) == normal), true);
check("save mode 9 is Normal", mixed.save("test.02.mode.xml", 9) && (dump(XML.load("test.02.mode.xml")) == normal), true);
check("save mode -1 is Normal", mixed.save("test.02.mode.xml", -1) && (dump(XML.load("test.02.mode.xml")) == normal), true);
check("save mode \"abc\" is Normal", mixed.save("test.02.mode.xml", "abc") && (dump(XML.load("test.02.mode.xml")) == normal), true);
check("load mode 9 is Normal", dump(XML.load("test.02.normal.xml", 9)), normal);
check("load mode \"1\" is Minified", dump(XML.load("test.02.normal.xml", "1")), dump(XML.load("test.02.normal.xml", XMLParserMode.Minified)));

// --- cycles

var tree = new XMLDocument();
var parent = tree.add(newNode(XMLNodeType.Element, "parent"));
var child = parent.getBranch().add(newNode(XMLNodeType.Element, "child"));
var grandchild = child.getBranch().add(newNode(XMLNodeType.Element, "grandchild"));
var before = dump(tree);

checkThrows("add a node to its own children", function() {
	parent.getBranch().add(parent);
}, "node cannot contain itself");
checkThrows("add an ancestor", function() {
	grandchild.getBranch().add(parent);
}, "node cannot contain itself");
checkThrows("setIndex an ancestor", function() {
	child.getBranch().setIndex(0, parent);
}, "node cannot contain itself");
var holder = new XMLDocument();
holder.add(parent);
checkThrows("addDocument with an ancestor", function() {
	grandchild.getBranch().addDocument(holder);
}, "node cannot contain itself");
checkThrows("setBranch to a list holding the node", function() {
	child.setBranch(parent.getBranch());
}, "node cannot contain itself");
checkThrows("setBranch to a list holding an ancestor", function() {
	grandchild.setBranch(holder);
}, "node cannot contain itself");
check("tree unchanged after refused changes", dump(tree), before);

// not cycles: the same node twice in one list, a document added to itself, moving a node
var twice = new XMLDocument();
var leaf = newNode(XMLNodeType.Element, "leaf");
twice.add(leaf);
twice.add(leaf);
twice.addDocument(twice);
check("same node in a list several times", twice.length(), 4);
child.getBranch().removeIndex(0);
parent.getBranch().add(grandchild);
check("move a node", dump(tree), "(1:parent[(1:child[])(1:grandchild[])])");
check("tree saves after the changes", tree.save("test.02.tree.xml", XMLParserMode.IndentationTab), true);
check("tree loads back", dump(XML.load("test.02.tree.xml", XMLParserMode.Minified)), "(1:parent[(1:child[])(1:grandchild[])])");

// --- setBranch shares the list

var owner = newNode(XMLNodeType.Element, "owner");
var children = new XMLDocument();
owner.setBranch(children);
children.add(newNode(XMLNodeType.Element, "k"));
check("setBranch(new XMLDocument()) shares the list", owner.getBranch().length(), 1);
owner.getBranch().add(newNode(XMLNodeType.Element, "l"));
check("changes through getBranch() reach the document", names(children), "kl");

// --- wrong objects

checkThrows("XMLNode method on XMLDocument", function() {
	XMLNode.prototype.getName.call(new XMLDocument());
}, "invalid parameter");
checkThrows("XMLDocument method on XMLNode", function() {
	XMLDocument.prototype.length.call(new XMLNode());
}, "invalid parameter");
checkThrows("XMLAttributes method on XMLAttribute", function() {
	XMLAttributes.prototype.length.call(new XMLAttribute());
}, "invalid parameter");
checkThrows("XMLAttribute method on a string", function() {
	XMLAttribute.prototype.getName.call("x");
}, "invalid parameter");
checkThrows("setAttributes(XMLDocument)", function() {
	owner.setAttributes(new XMLDocument());
}, "invalid parameter");
checkThrows("setBranch(XMLNode)", function() {
	owner.setBranch(new XMLNode());
}, "invalid parameter");
checkThrows("setBranch()", function() {
	owner.setBranch();
}, "invalid parameter");
checkThrows("setIndex(0, 5)", function() {
	children.setIndex(0, 5);
}, "invalid parameter");
checkThrows("addDocument(5)", function() {
	children.addDocument(5);
}, "invalid parameter");
check("add(5)", typeof(children.add(5)), "undefined");
check("add()", typeof(children.add()), "undefined");
check("list unchanged", names(children), "kl");

// --- files

check("load a missing file", typeof(XML.load("test.02.missing.xml")), "undefined");
check("load a directory", typeof(XML.load(".")), "undefined");
check("save into a missing directory", children.save("test.02.missing/x.xml"), false);
check("save an empty document", (new XMLDocument()).save("test.02.empty.xml"), true);
check("load an empty file", typeof(XML.load("test.02.empty.xml")), "undefined");

// --- objects

check("typeof", typeof(new XMLDocument()) + " " + typeof(new XMLNode()) + " " + typeof(new XMLAttributes()) + " " + typeof(new XMLAttribute()), "XMLDocument XMLNode XMLAttributes XMLAttribute");
check("toString", "" + new XMLNode(), "XMLNode");
check("truthy", (new XMLDocument()) ? "yes" : "no", "yes");

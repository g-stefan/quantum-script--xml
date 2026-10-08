// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

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

// new objects are empty, also when the engine reuses a released wrapper
for (var k = 0; k < 8; ++k) {
	var scratch = new XMLNode();
	check("new XMLNode " + k, scratch.getName() + "/" + scratch.getType(), "/" + XMLNodeType.None);
	scratch.setType(XMLNodeType.Element);
	scratch.setName("scratch" + k);
	var scratchAttribute = new XMLAttribute();
	check("new XMLAttribute " + k, scratchAttribute.getName(), "");
	scratchAttribute.setName("a" + k);
	var scratchAttributes = new XMLAttributes();
	check("new XMLAttributes " + k, scratchAttributes.length(), 0);
	scratchAttributes.setIndex(0, scratchAttribute);
	var scratchDocument = new XMLDocument();
	check("new XMLDocument " + k, scratchDocument.length(), 0);
	scratchDocument.add(scratch);
};

// build a document
var xml = new XMLDocument();
xml.add(newNode(XMLNodeType.Declaration, "xml version=\"1.0\" encoding=\"UTF-8\""));
var config = newNode(XMLNodeType.Element, "config");
check("add returns the node", typeof(xml.add(config)), "XMLNode");
check("add ignores other values", typeof(xml.add(5)), "undefined");

var attribute = new XMLAttribute();
attribute.setName("version");
attribute.setValue("1");
var attributes = new XMLAttributes();
attributes.setIndex(attributes.length(), attribute);
config.setAttributes(attributes);
check("setAttributes", config.getAttributes().length(), 1);

var children = config.getBranch();
var server = newNode(XMLNodeType.Element, "server");
children.add(server);
var serverAttributes = server.getAttributes();
var host = new XMLAttribute();
host.setName("host");
host.setValue("localhost");
serverAttributes.setIndex(serverAttributes.length(), host);
check("getAttributes of a new node is attached",server.getAttributes().getIndex(0).getValue(), "localhost");
children.add(newNode(XMLNodeType.Element, "note")).getBranch().add(newNode(XMLNodeType.Content, "a &lt; b"));
children.add(newNode(XMLNodeType.Comment, " end "));
check("branch length", config.getBranch().length(), 3);

// setBranch
var list = new XMLDocument();
list.add(newNode(XMLNodeType.Element, "item"));
list.add(newNode(XMLNodeType.Element, "item"));
var items = newNode(XMLNodeType.Element, "items");
items.setBranch(list);
check("setBranch", items.getBranch().length(), 2);
children.add(items);

// removeIndex, setIndex
var temporary = new XMLDocument();
temporary.add(newNode(XMLNodeType.Element, "a"));
temporary.add(newNode(XMLNodeType.Element, "b"));
temporary.add(newNode(XMLNodeType.Element, "c"));
temporary.removeIndex(1);
check("removeIndex", temporary.length() + " " + temporary.getIndex(1).getName(), "2 c");
temporary.setIndex(0, newNode(XMLNodeType.Element, "z"));
check("setIndex", temporary.getIndex(0).getName(), "z");
check("getIndex past the end", typeof(temporary.getIndex(9)), "undefined");

// addDocument
var merged = new XMLDocument();
merged.addDocument(temporary);
merged.addDocument(merged);
check("addDocument", merged.length(), 4);

// save and load back
check("save", xml.save("test.01.xml", XMLParserMode.IndentationTab), true);
var loaded = XML.load("test.01.xml", XMLParserMode.Minified);
check("load", typeof(loaded), "XMLDocument");
check("load missing file", typeof(XML.load("test.01.missing.xml")), "undefined");
check("top level", loaded.length(), 2);
var root = loaded.get("config");
check("get", root.length(), 1);
check("get missing", loaded.get("missing").length(), 0);
check("get does not descend", loaded.get("config").get("server").length(), 0);
check("find", loaded.find("item").length(), 2);
check("attribute", root.getIndex(0).getAttributes().getIndex(0).getName() + "=" + root.getIndex(0).getAttributes().getIndex(0).getValue(), "version=1");
var found = loaded.findWithAttributeValue("server", "host", "localhost");
check("findWithAttributeValue", found.length(), 1);
check("text is raw", loaded.find("note").getIndex(0).getBranch().getIndex(0).getName(), "a &lt; b");
check("comment", loaded.find("config").getIndex(0).getBranch().getIndex(2).getType(), XMLNodeType.Comment);

// query results share the nodes with the tree
found.getIndex(0).setName("host");
check("rename through a query result", loaded.find("host").length(), 1);

---
name: quantum-script--xml
description: >-
  How to use the Quantum Script XML extension (quantum-script--xml), loaded
  with Script.requireExtension("XML"), a script layer over the file-xml C++
  library: XML.load(fileName, mode) / XML.loadFromString(text, mode)
  (XMLDocument, or undefined when missing, empty or not well formed, never
  throws), XMLDocument.saveToString(mode), XMLParserMode Normal / Minified /
  Indentation4Spaces / IndentationTab (read Minified to drop whitespace text
  nodes, write IndentationTab), XMLNodeType None / Element / Content / CDATA
  / Comment / Declaration / Bom / DocumentTypeDefinition; XMLDocument is a
  LIST of nodes (loaded file, getBranch() view of an element's children, or
  a query result): save, length, getIndex (undefined past the end),
  setIndex, add (returns the node), removeIndex, addDocument, get(name)
  (this list only, does not descend), find(name) (whole subtree),
  findWithAttributeValue(name, attribute, value) - queries return an empty
  XMLDocument, never undefined; XMLNode getType / setType, getName / setName
  (tag name, or the text of text nodes), getAttributes / setAttributes,
  getBranch / setBranch; XMLAttributes length / getIndex (undefined past
  the end) / setIndex(length(), a) to append / removeIndex / indexOf(name)
  (-1 if none) / find(name) (undefined if none) / remove(name) (all
  matches, returns count); XMLAttribute getName /
  setName / getValue / setValue; handles share the tree; text and attribute
  values are raw (entities not decoded on read, not encoded on write);
  negative / missing indexes count as past the end; cycles are refused
  ("node cannot contain itself"); setType accepts only 0 .. 7;
  helpers for text, attributes, escaping; which hosts have it (dynamic
  quantum-script, magnet; NOT fabricare, NOT the static quantum-script);
  the C++ side (registerInternalExtension, initExecutive,
  quantumScriptExtension, VariableXMLDocument / Node / Attributes /
  Attribute wrapping TPointer<FileXML::...>). Use when writing or reviewing
  Quantum Script or magnet code that reads, changes or writes XML files,
  C++ code that includes <XYO/QuantumScript.Extension/XML.hpp>, a
  fabricare.json depending on "quantum-script--xml", or when working inside
  the quantum-script--xml repository.
---

# quantum-script--xml

XML extension of Quantum Script (see the `quantum-script` skill for the
language and its differences from JavaScript; its rules apply — no
hoisting, `typeof(x)` needs parentheses, `&&` / `||` return booleans,
statements and blocks end with `;`, no `String.prototype.repeat`).
Purpose: **read an XML file into a tree, query and change it, build new
documents, write them back**. Parsing and writing are done by `file-xml`
(`XYO::FileXML`); the script objects are handles to its nodes.

Full documentation: `docs/` in the quantum-script--xml repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--xml\docs` on this machine):
README (purpose), getting-started (build, first read / change / build,
hosts, C++ registration), **document-model** (lists, views, sharing,
whitespace, entities), **script-api** (every method, limitations,
recipes), cpp-api, reference (tables, edge cases). When in doubt read
`source/XYO/QuantumScript.Extension/XML/Library.cpp`, or run it:
`quantum-script test.js`.

## Script API

```javascript
Script.requireExtension("XML");

var xml = XML.load("menu.xml", XMLParserMode.Minified);   // XMLDocument; undefined on error
var doc = XML.loadFromString("<a x='1'/>");               // same, from a string
xml.length(); xml.getIndex(k);                            // nodes of this list (any type)
xml.get("menu");                                          // Elements of THIS list named "menu"
xml.find("food");                                         // Elements named "food" anywhere below
xml.findWithAttributeValue("food", "id", "2");            // ... with id="2" (raw compare)
var node = xml.get("menu").getIndex(0);                   // XMLNode, undefined if none
node.getType(); node.getName();                           // XMLNodeType.*, tag name / text
node.getBranch();                                         // XMLDocument view of the children
node.getAttributes();                                     // XMLAttributes (getIndex(k): XMLAttribute)
node.getAttributes().find("id");                          // XMLAttribute or undefined; indexOf(name): -1 if none
node.getAttributes().remove("id");                        // remove all named "id", returns count; removeIndex(k)
xml.save("out.xml", XMLParserMode.IndentationTab);        // true / false
xml.saveToString(XMLParserMode.IndentationTab);           // the same text as a String
```

Building: `new XMLDocument()`, `new XMLNode()` (+ `setType`, `setName`),
`new XMLAttribute()` (+ `setName`, `setValue`), `new XMLAttributes()`;
`list.add(node)` returns `node`; `node.setAttributes(attributes)`,
`node.setBranch(document)` share (not copy) their argument.

## Hard rules

1. **`XML.load` / `XML.loadFromString` return `undefined` on error**
   (missing file, empty text, not well formed), no message, no throw.
   Always check `Script.isUndefined(xml)`. `loadFromString(undefined /
   null)` is `undefined`; other values go through `toString`
   (`loadFromString(5)` = one text node). `saveToString(mode)` returns
   exactly what `save` writes (`""` for an empty list); deep copy:
   `XML.loadFromString(doc.saveToString())`.
2. **A document is a list of nodes.** The loaded document holds the top
   level nodes: declaration, comments, BOM, DOCTYPE and the root element.
   The root element is `xml.get("root").getIndex(0)`, not `xml`.
3. **`get` does not descend.** `xml.get("a").get("b")` is empty. Go down
   with `getBranch()`: `xml.get("a").getIndex(0).getBranch().get("b")`, or
   use `find`.
4. **Queries return an empty `XMLDocument`, never `undefined`**: test
   `.length() == 0`, or `getIndex(0)` (which is `undefined` past the end).
5. **Whitespace**: with the default `Normal` mode the indentation between
   elements is kept as `Content` nodes, counted by `length()` /
   `getIndex()`. Read with `XMLParserMode.Minified` for data; `get` /
   `find` skip non-elements anyway. Mode is used for both load and save;
   omitted / invalid → `Normal`.
6. **Text is a child node.** `<name>Waffles</name>`: the element's
   `getName()` is `name`; the text is `getBranch().getIndex(0).getName()`
   (type `Content`, or `CDATA`). Changing text = `setName` on the text node.
7. **Raw text both ways.** Entities are not decoded on read (`&amp;` stays
   `&amp;`) and not encoded on write (`setName("1 < 2")` saves a file that
   no longer loads). Escape `& < > "` before putting text or attribute
   values in the tree; unescape what you read. `findWithAttributeValue`
   compares raw values.
8. **Handles share.** Nodes / attributes from `find`, `get`, `getIndex`,
   `getAttributes`, `getBranch` are the tree's objects: changes are
   written by `save`. `add` / `setIndex` / `removeIndex` on a
   `getBranch()` view change the element; on a query result they change
   only the result list. `addDocument`, `setBranch`, `setAttributes`
   share nodes / lists, they do not copy (`setBranch(new XMLDocument())`
   then adding to that document adds children). A change that would put
   a node inside its own subtree (`add`, `setIndex`, `addDocument`,
   `setBranch`) throws `node cannot contain itself` and changes nothing;
   the same node twice in one list is allowed (written twice).
9. **Indexes**: from `0`; strings converted, fractions truncated;
   negative, missing or NaN count as past the end (`getIndex` →
   `undefined`, `setIndex` / `removeIndex` do nothing).
   `XMLAttributes.getIndex(k)` past the end is `undefined`;
   `attributes.setIndex(attributes.length(), attribute)` appends,
   `k > length()` does nothing. By name (exact, case sensitive):
   `find(name)` first match or `undefined`, `indexOf(name)` or `-1`,
   `remove(name)` removes **all** matches (names may repeat) and returns
   the count; `removeIndex(k)` shifts the rest down
   (`removeIndex(indexOf("missing"))` is a safe no-op).
10. `getAttributes()` / `getBranch()` on a node create and attach the
    list on first call. A `new XMLNode()` has type `None` (skipped by
    `save`): always `setType`.
11. No parent pointers: remove a node by index from its parent's
    `getBranch()`; loop backwards when removing several.
12. **Hosts**: dynamic `quantum-script` (loads `quantum-script--xml.dll`)
    and **magnet** (internal). **Not** fabricare build scripts and **not**
    the static `quantum-script` (`Unable to open "XML"`).
13. One engine per thread: each thread requires `XML` itself; documents
    belong to the thread that created them.
14. Calling a method on the wrong type, passing a wrong object to
    `setIndex` / `setAttributes` / `setBranch` / `addDocument`, or
    `setType` with anything but `0` .. `7`, throws `invalid parameter`.
    An unknown parser mode is `Normal`.

## Recipes

```javascript
Script.requireExtension("XML");

function xmlNode(type, name) {
	var node = new XMLNode();
	node.setType(type);
	node.setName(name);
	return node;
};

function childElement(node, name) {                       // undefined if none
	return node.getBranch().get(name).getIndex(0);
};

function getText(node) {                                  // raw
	var retV = "";
	var children = node.getBranch();
	for (var k = 0; k < children.length(); ++k) {
		var child = children.getIndex(k);
		if ((child.getType() == XMLNodeType.Content) || (child.getType() == XMLNodeType.CDATA)) {
			retV += child.getName();
		};
	};
	return retV;
};

function setText(node, text) {                            // text must be escaped
	var children = new XMLDocument();
	children.add(xmlNode(XMLNodeType.Content, text));
	node.setBranch(children);
};

function getAttribute(node, name, defaultValue) {
	var attribute = node.getAttributes().find(name);
	return Script.isUndefined(attribute) ? defaultValue : attribute.getValue();
};

function setAttribute(node, name, value) {                // value must be escaped
	var attributes = node.getAttributes();
	var attribute = attributes.find(name);
	if (Script.isUndefined(attribute)) {
		attribute = new XMLAttribute();
		attribute.setName(name);
		attributes.setIndex(attributes.length(), attribute);
	};
	attribute.setValue(value);
};

function xmlEscape(text) {                                // replace() replaces all
	return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\"", "&quot;");
};

function xmlUnescape(text) {
	return text.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"").replace("&apos;", "'").replace("&amp;", "&");
};

// read
var xml = XML.load("app.config", XMLParserMode.Minified);
if (Script.isUndefined(xml)) {
	throw new Error("app.config missing or invalid");
};
var port = xml.findWithAttributeValue("add", "key", "port").getIndex(0);
var value = Script.isUndefined(port) ? "80" : xmlUnescape(getAttribute(port, "value", "80"));

// change and write
setAttribute(port, "value", "9090");
xml.save("app.config", XMLParserMode.IndentationTab);

// build
var doc = new XMLDocument();
doc.add(xmlNode(XMLNodeType.Declaration, "xml version=\"1.0\" encoding=\"UTF-8\""));
var root = doc.add(xmlNode(XMLNodeType.Element, "config"));
var server = root.getBranch().add(xmlNode(XMLNodeType.Element, "server"));
setAttribute(server, "host", xmlEscape("localhost"));
setText(root.getBranch().add(xmlNode(XMLNodeType.Element, "note")), xmlEscape("a < b"));
doc.save("config.xml", XMLParserMode.IndentationTab);    // CRLF, tab indent, <server host="localhost" />

// text in, text out
var reply = XML.loadFromString("<r><v>1</v></r>", XMLParserMode.Minified);
var body = reply.saveToString(XMLParserMode.Minified);   // <r><v>1</v></r>
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/XML.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                // host init callback
	Extension::Console::registerInternalExtension(executive);
	Extension::XML::registerInternalExtension(executive);  // scripts still requireExtension("XML")
};
```

- fabricare.json dependency: `"quantum-script--xml"` (`dll-or-lib`: DLL
  on dynamic platforms, static lib on `*.static` platforms); it pulls in
  `quantum-script`, `quantum-script--console` and `file-xml`.
- DLL entry point `extern "C" quantumScriptExtension(Executive *, void *)`
  only with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and without
  `XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY`. Static hosts must register
  the extension as internal.
- To pass a C++ `TPointer<FileXML::Document>` to a script: create
  `VariableXMLDocument::newVariable()` and set its `value` (include
  `XML/VariableXMLDocument.hpp`; the extension must already be required
  in that thread).

## Working in this repository

- Everything is in `XML/Library.cpp`: `newContext` (constructors and
  prototypes), one static function per method, `initExecutive` (constants
  from the `FileXML` enums, `setFunction2` registrations). The argument
  list in a `setFunction2` string is documentation only; read arguments
  with `arguments->index(k)` from `0`.
- Arguments go through the helpers at the top of `Library.cpp`:
  `getIndexArgument` (not `toIndex()`, which maps negative / NaN to `0`),
  `getModeArgument` (unknown → `Normal`), and the cycle checks
  `nodeOwnsBranch` (before adding a node to a list) / `branchContainsNode`
  (before `setBranch`). Use them in new methods.
- `VariableXML*` classes hold one `TPointer<FileXML::...> value`, are
  pooled (`TMemoryPoolActive`) and reset `value` in `activeDestructor()`
  — keep that for any new field, otherwise `new XMLNode()` can return a
  recycled wrapper still pointing at an old node. Methods create `value`
  lazily (`if (!value) value.newMemory();`).
- Build: `fabricare make`, then `fabricare test`, `fabricare install`
  (see the `fabricare` skill). `quantum-script`, `quantum-script--console`
  and `file-xml` must be installed first. Tests: `test/test.0N.cpp`
  registers Console and XML as internal and runs `test/test.0N.js`;
  `check(name, value, expected)` / `checkThrows(name, fn, message)` throw
  on mismatch; files go to `output/test/`. `test.01` build / save / load /
  query and wrapper recycling, `test.02` arguments, errors, cycles,
  sharing, modes, `test.03` reading `test/test.03.xml`, round trips in all
  modes, BOM, nesting limit, good and malformed input (generated by
  saving raw text nodes), `test.04` `loadFromString` / `saveToString`
  (exact output per mode) and attribute `find` / `indexOf` /
  `removeIndex` / `remove` across storage blocks (more than 8
  attributes). Running a test exe by hand loads the installed
  SDK DLL unless `output/bin` is first on `PATH`.
- Behaviour changes: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, the tests and this skill together.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).

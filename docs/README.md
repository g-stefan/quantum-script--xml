# Quantum Script Extension XML — Documentation

`quantum-script--xml` is the **XML extension of Quantum Script**. Loaded
with `Script.requireExtension("XML")`, it lets scripts read an XML file
into a tree of nodes, query and change that tree, build new documents and
write them back to disk:

```javascript
Script.requireExtension("XML");

var xml = XML.load("menu.xml", XMLParserMode.Minified);   // XMLDocument, undefined on error
var foods = xml.find("food");                              // every <food> element
var name = foods.getIndex(0).getBranch().get("name");      // <name> children of the first <food>
xml.save("menu.out.xml", XMLParserMode.IndentationTab);    // true / false
```

XML is the markup format of configuration files, project files (MSBuild,
Maven, ...), Windows task and manifest files, feeds and many data
exchange formats. This extension is a thin script layer over the
`file-xml` C++ library, which does the parsing and the writing; the
script objects are handles to `file-xml` nodes.

- **Load.** `XML.load(fileName, mode)` parses a file and returns an
  `XMLDocument`, or `undefined` when the file is missing or not well
  formed; `XML.loadFromString(text, mode)` does the same for a string. `XMLParserMode.Minified` drops the indentation text between
  elements, which is what you want to read data.
- **Navigate and query.** A document is a **list of nodes**. `get(name)`
  returns the elements of that list with a given name, `find(name)` also
  searches all their descendants, `findWithAttributeValue(name, attribute,
  value)` filters by an attribute. `node.getBranch()` is the list of an
  element's children, so `getBranch().get("x")` descends one level.
- **Change and build.** Nodes, attributes and lists are live handles:
  `setName`, `setValue`, `add`, `setIndex`, `removeIndex` change the tree
  in place. `new XMLNode()`, `new XMLAttribute()`, `new XMLDocument()`
  create new pieces.
- **Save.** `document.save(fileName, mode)` (or `saveToString(mode)`) writes the list, with no
  layout change (`Normal`), on one line (`Minified`) or one node per line
  indented with a tab or four spaces.
- **Raw text.** Text and attribute values are kept exactly as written in
  the file: entities (`&amp;`, `&lt;`, ...) are **not decoded on read
  and not encoded on write**. See
  [Script API, text and entities](script-api.md#text-and-entities).

```
scripts: quantum-script .js, magnet, embedding hosts, ...
quantum-script--xml       <-- this extension: XML, XMLDocument, XMLNode, XMLAttributes, XMLAttribute
file-xml                  (XYO::FileXML: Document, Node, Attribute, load, save)
quantum-script            (Executive, Variable, Context)
quantum-script--console   (link dependency)
xyo-system, xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What the extension gives |
|------|--------------------------|
| Read settings from an XML configuration or project file | `XML.load` + `find` / `get` + `getAttributes` |
| Patch a value in an existing XML file | change the node in place, `save` |
| Generate an XML file (task definitions, manifests, reports) | `new XMLNode()` / `add` / `save` with indentation |
| Search a document for elements by name or attribute value | `find`, `findWithAttributeValue` |
| No external dependency in scripts | `file-xml` is part of the XYO C++ stack |

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("XML");` | not preloaded in fabricare; internal in magnet |
| Read a file | `var xml = XML.load(file, XMLParserMode.Minified);` | `undefined` on error: check `Script.isUndefined(xml)` |
| The root element | `xml.get("root").getIndex(0)` | the top level list also holds the declaration, comments, ... |
| Children of an element | `node.getBranch()` | an `XMLDocument` view of the element's own list |
| Child elements by name | `node.getBranch().get("name")` | `get` does not descend; chain through `getBranch()` |
| All elements by name | `xml.find("name")` | whole subtree, document order |
| Attribute value | `node.getAttributes().find("id")` | `XMLAttribute` or `undefined`; also `indexOf`, `remove(name)`, `removeIndex` |
| Parse text / produce text | `XML.loadFromString(text, mode)`, `xml.saveToString(mode)` | no temporary file |
| Text of an element | its `Content` / `CDATA` children's `getName()` | raw, entities not decoded |
| New document | `new XMLDocument()`, `new XMLNode()`, `add` | escape text yourself |
| Write a file | `xml.save(file, XMLParserMode.IndentationTab)` | `true` / `false` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension, read / change / write a first file, hosts, register it in a C++ host |
| [Document model](document-model.md) | Documents as lists, views and query results, node types, what is shared and what is copied, whitespace, entities |
| [Script API](script-api.md) | Every constant, object and method, with behavior on edge cases, limitations and recipes |
| [C++ API](cpp-api.md) | Registration, DLL entry point, build defines, how the script objects wrap `file-xml` |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`. The parser and writer are documented in the `file-xml`
repository, `docs/`.

## Source map

```
source/XYO/QuantumScript.Extension/XML.hpp             umbrella header, include this from C++
source/XYO/QuantumScript.Extension/XML.Amalgam.cpp     the whole extension in one translation unit
source/XYO/QuantumScript.Extension/XML/
    Dependency.hpp                                     <XYO/QuantumScript.hpp>, <XYO/FileXML.hpp>, export macro
    Library[.hpp/.cpp]                                 initExecutive, registerInternalExtension, every script
                                                       method, the constants, DLL entry point
    Context.hpp                                        XMLContext: the four prototypes (per thread singleton)
    VariableXMLDocument[.hpp/.cpp]                     XMLDocument: TPointer<FileXML::Document>
    VariableXMLNode[.hpp/.cpp]                         XMLNode: TPointer<FileXML::Node>
    VariableXMLAttributes[.hpp/.cpp]                   XMLAttributes: TPointer<FileXML::Node::Attributes>
    VariableXMLAttribute[.hpp/.cpp]                    XMLAttribute: TPointer<FileXML::Attribute>
    Copyright / License / Version                      library metadata
    Library.rc, *.rh                                   Windows version resource
test/test.0N.cpp                                       C++ hosts registering Console and XML as internal
test/test.01.js                                        build, change, save, load and query
test/test.02.js                                        arguments, errors, cycles, sharing, modes
test/test.03.js, test.03.xml                           reading a sample file, modes, round trips, malformed input
test/test.04.js                                        loadFromString, saveToString, attribute find / indexOf / remove
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--xml/`](../.claude/skills/quantum-script--xml/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `XML`
(Quantum Script tools, magnet scripts).

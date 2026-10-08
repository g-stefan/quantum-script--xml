# API reference

## Script

Available after `Script.requireExtension("XML")`. Every method throws
`invalid parameter` when called on an object of another type. A
negative, missing or non-numeric index counts as past the end.

### Constants

| Symbol | Value |
|--------|-------|
| `XMLNodeType.None` | `0` |
| `XMLNodeType.Element` | `1` |
| `XMLNodeType.Content` | `2` |
| `XMLNodeType.CDATA` | `3` |
| `XMLNodeType.Comment` | `4` |
| `XMLNodeType.Declaration` | `5` |
| `XMLNodeType.Bom` | `6` |
| `XMLNodeType.DocumentTypeDefinition` | `7` |
| `XMLParserMode.Normal` | `0`: read all text / write as is |
| `XMLParserMode.Minified` | `1`: read trimmed, whitespace text dropped / write on one line |
| `XMLParserMode.Indentation4Spaces` | `2`: read as `Normal` / write indented, 4 spaces, CRLF |
| `XMLParserMode.IndentationTab` | `3`: read as `Normal` / write indented, tab, CRLF |

### XML

| Symbol | Returns | Notes |
|--------|---------|-------|
| `XML.load(fileName[, mode])` | `XMLDocument` or `undefined` | `undefined` when the file is missing or not well formed; never throws; `mode` default / invalid → `Normal` |
| `XML.loadFromString(text[, mode])` | `XMLDocument` or `undefined` | as `load`, from a string; `undefined` for empty text, `undefined` / `null`; other values via `toString`; read up to the first `\0` |

### XMLDocument

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new XMLDocument()` | `XMLDocument` | empty list |
| `save(fileName[, mode])` | Boolean | `false` when the file cannot be written; empty list → empty file |
| `saveToString([mode])` | String | the text `save` writes; empty list → `""` |
| `length()` | Number | nodes of any type |
| `getIndex(index)` | `XMLNode` or `undefined` | `undefined` past the end |
| `setIndex(index, node)` | `undefined` | replace; no-op past the end; throws if `node` is not an `XMLNode`, or on a cycle |
| `add(node)` | `node` or `undefined` | append; creates `node`'s child list; non-`XMLNode` ignored → `undefined`; throws on a cycle |
| `removeIndex(index)` | `undefined` | no-op past the end |
| `addDocument(document)` | `undefined` | append `document`'s nodes (shared); throws if not an `XMLDocument`, or on a cycle |
| `get(name)` | `XMLDocument` | elements of this list named `name`; empty if none |
| `find(name)` | `XMLDocument` | elements named `name` in this list and below, document order; empty if none |
| `findWithAttributeValue(name, attribute, value)` | `XMLDocument` | as `find`, with attribute `attribute` exactly `value` (raw); empty if none |

### XMLNode

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new XMLNode()` | `XMLNode` | type `None`, name `""` |
| `getType()` / `setType(type)` | Number / `undefined` | `XMLNodeType.*`; `setType` throws for anything but `0` .. `7` |
| `getName()` / `setName(name)` | String / `undefined` | tag name, or the raw text of non-element nodes |
| `getAttributes()` | `XMLAttributes` | created and attached on first call; shared |
| `setAttributes(attributes)` | `undefined` | node uses `attributes` (shared); throws if not an `XMLAttributes` |
| `getBranch()` | `XMLDocument` | view of the children, created and attached on first call |
| `setBranch(document)` | `undefined` | children = `document`'s list (shared, also when empty); throws if not an `XMLDocument`, or on a cycle |

### XMLAttributes

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new XMLAttributes()` | `XMLAttributes` | empty |
| `length()` | Number | number of attributes |
| `getIndex(index)` | `XMLAttribute` or `undefined` | `undefined` past the end |
| `setIndex(index, attribute)` | `undefined` | store (shared); `index == length()` appends, `index > length()` does nothing; throws if not an `XMLAttribute` |
| `removeIndex(index)` | `undefined` | remove, following attributes shift down; no-op past the end |
| `indexOf(name)` | Number | first attribute named `name`, `-1` if none; exact, case sensitive |
| `find(name)` | `XMLAttribute` or `undefined` | first attribute named `name` (shared) |
| `remove(name)` | Number | remove every attribute named `name`, returns the count |

### XMLAttribute

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new XMLAttribute()` | `XMLAttribute` | name and value `""` |
| `getName()` / `setName(name)` | String / `undefined` | |
| `getValue()` / `setValue(value)` | String / `undefined` | raw, entities not decoded / encoded |

### Edge cases

| Expression | Result |
|------------|--------|
| `XML.load("missing.xml")` | `undefined` |
| `XML.load("bad.xml")` (not well formed, for example `<e>1 < 2</e>`) | `undefined` |
| `XML.load(f)` on an indented file | text nodes between elements are counted by `length()` |
| `XML.load(f, XMLParserMode.Minified)` | no whitespace only text nodes |
| `XML.load(f, "1")`, `XML.load(f, "abc")`, `XML.load(f, 9)` | `Minified`, `Normal`, `Normal` |
| `xml.getIndex(-1)`, `xml.getIndex()` | `undefined` |
| `XML.loadFromString("")`, `XML.loadFromString()`, `XML.loadFromString(null)` | `undefined` |
| `XML.loadFromString(5)` | document with one text node `5` |
| `(new XMLDocument()).saveToString()` | `""` |
| `attributes.removeIndex(attributes.indexOf("missing"))` | nothing removed (`-1`) |
| `attributes.remove("x")` with three `x` attributes | `3` |
| `xml.removeIndex(-1)` | nothing removed |
| `attributes.getIndex(attributes.length())` | `undefined`, the list does not grow |
| `node.setType(8)`, `node.setType("abc")` | throws `invalid parameter` |
| `xml.get("a").get("b")` | empty (`get` does not descend) |
| `xml.get("missing")` | empty `XMLDocument`, not `undefined` |
| `xml.getIndex(99)` | `undefined` |
| `typeof(xml)`, `"" + xml` | `"XMLDocument"`, `"XMLDocument"` |
| `xml ? 1 : 0` | `1` (always truthy) |
| `doc.add(5)` | `undefined`, nothing added |
| `node.getBranch().add(node)` | throws `node cannot contain itself` |
| `text.getName()` for `<a>x &amp; y</a>` | `x &amp; y` |
| `setName("1 < 2")` on a text node, then `save` | writes `1 < 2`; the file no longer loads |
| attribute value with `"` | written between `'` |
| `<a></a>` loaded and saved | `<a />` |
| `XMLNode.prototype.getName.call(xml)` | throws `invalid parameter` |

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "XML"` | the extension library was not found and no internal one is registered (for example in the static `quantum-script`) |
| `invalid parameter` | a method called on the wrong type, or given an argument of the wrong type, or `setType` with an unknown type |
| `node cannot contain itself` | `add`, `setIndex`, `addDocument` or `setBranch` would make a node its own descendant |

## Hosts

| Host | `XML` |
|------|-------|
| `quantum-script`, dynamic SDK | yes (DLL) |
| `quantum-script`, static SDK | no |
| magnet | yes, internal |
| fabricare build scripts | no |

## C++

Namespace `XYO::QuantumScript::Extension::XML`, umbrella header
`<XYO/QuantumScript.Extension/XML.hpp>`.

### Library (`XML/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"XML"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### Variables (`XML/VariableXML*.hpp`)

| Class | Member | Notes |
|-------|--------|-------|
| `VariableXMLDocument` | `TPointer<FileXML::Document> value` | `newVariable()`, `activeDestructor()` drops `value` |
| `VariableXMLNode` | `TPointer<FileXML::Node> value` | same |
| `VariableXMLAttributes` | `TPointer<FileXML::Node::Attributes> value` | same |
| `VariableXMLAttribute` | `TPointer<FileXML::Attribute> value` | same |

### Context (`XML/Context.hpp`)

| Symbol | Notes |
|--------|-------|
| `class XMLContext` | `symbolFunctionXML*`, `prototypeXML*` for the four types |
| `XMLContext *getContext()` | per thread singleton |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::XML::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--xml` | fabricare project, `dll-or-lib`; depends on `quantum-script`, `quantum-script--console`, `file-xml` |
| `test.01` .. `test.04` | fabricare test projects, run `test/test.0N.js` |
| `XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_XML_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__XML_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY` | static build: empty export macro, no DLL entry point |
| `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` | trace line per method call |

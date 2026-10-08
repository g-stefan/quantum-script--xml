# C++ API

For hosts that embed Quantum Script and for maintainers of the extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: native functions, `Variable` and
`TPointer` work the same way here. The tree itself is `file-xml`
(`XYO::FileXML`, documented in the `file-xml` repository).

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/XML.hpp>   // Library.hpp, Dependency.hpp (QuantumScript, FileXML)

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::XML`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_XML_EXPORT`:

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_XML_INTERNAL` (or `QUANTUM_SCRIPT__XML_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY` | export macro empty, no `quantumScriptExtension` entry point (sources compiled into another library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY` is not.

## Registering the extension

```cpp
void Extension::XML::registerInternalExtension(Executive *executive);
void Extension::XML::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"XML"` as an internal extension;
  call it from the host's init callback (see
  [Getting started](getting-started.md#6-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `XML` in a thread. It sets the extension name, info
  (`"XML"` plus the short license text), version and public flag, creates
  the four constructor functions and their prototypes (`newContext`),
  compiles the `XMLNodeType` and `XMLParserMode` constants from the
  `FileXML::NodeType` / `FileXML::Mode` enum values, then registers every
  method with `executive->setFunction2("XMLNode.prototype.getName()", ...)`.
  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--xml.dll`.

## How it works

### Variables

Each script type is a `Variable` subclass holding one managed pointer to a
`file-xml` object:

| Class | Script type | Member |
|-------|-------------|--------|
| `VariableXMLDocument` | `XMLDocument` | `TPointer<FileXML::Document> value` |
| `VariableXMLNode` | `XMLNode` | `TPointer<FileXML::Node> value` |
| `VariableXMLAttributes` | `XMLAttributes` | `TPointer<FileXML::Node::Attributes> value` |
| `VariableXMLAttribute` | `XMLAttribute` | `TPointer<FileXML::Attribute> value` |

All four:

- are allocated from `TMemoryPoolActive` (`TMemory<...>` is specialized in
  each header): released variables are recycled, so `activeDestructor()`
  drops `value`; a recycled wrapper must not keep, or show, the object of
  its previous use;
- create `value` lazily: every method starts with
  `if (!value) value.newMemory();`, so `new XMLNode()` costs nothing until
  used;
- report their type name from `getVariableType()` and `toString()`, and
  `toBoolean()` is `true`;
- find their prototype through `instancePrototype()` →
  `getContext()->prototypeXML*`.

`XMLContext` (`Context.hpp`) holds the four constructor symbols and
prototypes; it is a `TSingleton` (one per thread) and its `TPointerX`
members are linked to it in the constructor. `deleteContext` releases them
when the executive ends.

### Methods

Every method in `Library.cpp` follows the same shape:

```cpp
static TPointer<Variable> xmlNode_getName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	if (!TIsType<VariableXMLNode>(this_)) {
		throw(Error("invalid parameter"));
	};
	if (!((VariableXMLNode *)this_)->value) {
		((VariableXMLNode *)this_)->value.newMemory();
	};
	return VariableString::newVariable(((VariableXMLNode *)this_)->value->name);
};
```

How the script methods map to `file-xml`:

| Script | C++ |
|--------|-----|
| `XML.load(f, m)` | `FileXML::load(f, document, (Mode)m)`, `undefined` on `false` |
| `XML.loadFromString(t, m)` | `FileXML::loadFromString(t, document, (Mode)m)`, `undefined` on `false` or for `undefined` / `null` |
| `doc.saveToString(m)` | `FileXML::saveToString(output, document, (Mode)m)` |
| `doc.save(f, m)` | `FileXML::save(f, document, (Mode)m)` |
| `doc.length()`, `getIndex`, `setIndex`, `removeIndex` | `Document::length`, `getIndex`, `setIndex`, `removeIndex` |
| `doc.add(node)` | `Document::add(node)`; returns the script argument, not the C++ result |
| `doc.addDocument(other)` | `Document::addDocument(*other)` |
| `doc.get` / `find` / `findWithAttributeValue` | `Document::get` / `find` / `findWithAttributeValue` into a new `VariableXMLDocument` (always returned, empty when nothing matched) |
| `node.getBranch()` | creates `node->branch` if missing; new `VariableXMLDocument` whose `Document` shares `node->branch` |
| `node.setBranch(doc)` | `node->branch = doc->root` |
| `node.getAttributes()` | creates `node->attributes` if missing; new `VariableXMLAttributes` sharing it |
| `node.setAttributes(a)` | `node->attributes = a->value` |
| `attributes.getIndex(k)` | `Attributes::index(k)` for `k < length()`, else `undefined` (the array never grows on read) |
| `attributes.setIndex(k, a)` | `Attributes::index(k) = a->value` for `k <= length()` |
| `attributes.removeIndex(k)` | `Attributes::remove(k)` |
| `attributes.indexOf(n)` / `find(n)` | `attributesIndexOf`: first index whose `name == n`, `-1` if none |
| `attributes.remove(n)` | `Attributes::remove` for every match, from the end |
| `node.getType()` / `setType(t)` | `(Number)node->type` / `(FileXML::NodeType)t`, `invalid parameter` outside `0` .. `7` |

Argument helpers at the top of `Library.cpp`:

| Helper | Role |
|--------|------|
| `getIndexArgument(value, index)` | `false` for NaN, infinite or negative numbers (`toIndex()` would turn them into `0`); callers treat that as past the end |
| `getModeArgument(value)` | `FileXML::Mode`, `Normal` for anything outside `0` .. `3` (`file-xml` does not validate it) |
| `nodeOwnsBranch(node, branch)` | `branch` is the child list of `node` or of a descendant: adding `node` to `branch` would make a cycle (`add`, `setIndex`, `addDocument`) |
| `branchContainsNode(branch, node)` | `node` is in `branch` or below: `node->branch = branch` would make a cycle (`setBranch`) |

The cycle checks walk the subtree being added (or the list being
attached) once; they throw `node cannot contain itself` before anything
changes. `setBranch` creates the document's list when it has none, so
the node and the document share it.

With `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` defined every method prints a trace
line (`- xml-load`, `- xml-node-get-name`, ...).

## Using the tree from C++

There is no C++ API beyond the script functions. C++ code that already has
a `TPointer<FileXML::Document>` can hand it to a script by wrapping it:

```cpp
TPointer<Variable> wrap(TPointer<FileXML::Document> &document) {
	Extension::XML::VariableXMLDocument *retV =
	    static_cast<Extension::XML::VariableXMLDocument *>(Extension::XML::VariableXMLDocument::newVariable());
	retV->value = document;
	return retV;
};
```

(after a script ran `Script.requireExtension("XML")` in this thread, so
the prototypes exist). The same works the other way: `TIsType<VariableXMLNode>(variable)`,
then use `((VariableXMLNode *)variable)->value` as a `FileXML::Node`.
Include the `VariableXML*.hpp` headers for this; `XML.hpp` only declares
the registration functions.

## Notes for maintainers

- New methods: write a `static TPointer<Variable> name(VariableFunction *,
  Variable *this_, VariableArray *arguments)` in `Library.cpp` following
  the pattern above, register it in `initExecutive` with
  `executive->setFunction2("XMLType.prototype.name(args)", name)`. The
  argument list in that string is documentation only: read the arguments
  with `arguments->index(k)`, counting from `0`. Then update `README.md`,
  `docs/script-api.md`, `docs/reference.md`, the skill in
  `.claude/skills/quantum-script--xml/` and the tests.
- New fields in a `VariableXML*` class must be reset in its
  `activeDestructor()` (the classes are pooled).
- Tests (each `test/test.0N.cpp` registers Console and XML as internal
  and runs `test/test.0N.js`; a failed `check` throws, so `fabricare test`
  fails; files are written to `output/test/`):

  | Test | Covers |
  |------|--------|
  | `test.01` | `new` objects are empty when the engine recycles wrappers; building a document, saving, loading back, queries |
  | `test.02` | index arguments, `XMLAttributes` bounds, sharing, `setType` validation, unknown modes, cycle refusal, wrong objects, file errors |
  | `test.03` | reading `test/test.03.xml` (declaration, DOCTYPE, comment, prefixes, entities, CDATA, mixed content) in both modes, round trips through all four save modes, BOM, the 1024 nesting limit, input that loads and malformed input that does not |
  | `test.04` | `loadFromString` (modes, invalid input, UTF-8, BOM), `saveToString` (exact output of every mode, views, query results, same as `save`, round trips), `XMLAttributes` `find` / `indexOf` / `removeIndex` / `remove` with more than one storage block (8 attributes) and duplicates |

  Run a test by hand from `output/test` with `output/bin` first on
  `PATH` (otherwise the installed SDK DLL is loaded).
- `quantum-script--xml` must be rebuilt when `file-xml` changes its
  `Document` / `Node` layout (it is compiled against the headers).

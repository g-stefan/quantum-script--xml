# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...), `quantum-script--console` and
`file-xml` must be installed to the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 .. test.04 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`fabricare.json` declares two projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--xml` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `test.01` .. `test.04` | executables, category `test` | run `test/test.0N.js` with the extension registered as internal |

After `fabricare install`, `quantum-script--xml.dll` (Windows) /
`libquantum-script--xml.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe` and `file-xml.dll`, which is where
`Script.requireExtension("XML")` finds it.

## 2. Read a file

`menu.xml`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<menu lang="en">
	<food id="1"><name>Waffles</name><price>5.95</price></food>
	<food id="2"><name>Toast</name><price>2.50</price></food>
</menu>
```

`read-menu.js`:

```javascript
Script.requireExtension("Console");
Script.requireExtension("XML");

var xml = XML.load("menu.xml", XMLParserMode.Minified);
if (Script.isUndefined(xml)) {
	throw new Error("menu.xml missing or not well formed");
};

var foods = xml.find("food");
for (var k = 0; k < foods.length(); ++k) {
	var food = foods.getIndex(k);
	var name = food.getBranch().get("name").getIndex(0);     // <name> element
	var text = name.getBranch().getIndex(0);                 // its text node
	Console.writeLn(text.getName());
};
// Waffles
// Toast
```

Run it with:

```bash
quantum-script read-menu.js
```

What happens:

- `XML.load` returns an `XMLDocument`: the **list** of top level nodes
  (here the declaration and the `menu` element). With
  `XMLParserMode.Minified` the indentation between elements is dropped; with
  the default `Normal` mode it is kept as text nodes and counted by
  `length()` / `getIndex()`.
- `find("food")` returns a new list with every `food` element of the
  document.
- An element's children are a list too: `getBranch()`. `get("name")` looks
  only at that list, `getIndex(0)` takes the first match.
- The text of an element is a child node of type `XMLNodeType.Content`; its
  text is its **name**.

The [Script API recipes](script-api.md#recipes) have small helpers
(`childElement`, `getText`, `getAttribute`, ...) that make this shorter.

`Script.requireExtension("XML")` looks for an external
`quantum-script--xml` library first (the file as named, then every include
path folder: next to the interpreter, next to the script), then for an
internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "XML"`.

## 3. Change a file

```javascript
Script.requireExtension("XML");

var xml = XML.load("menu.xml", XMLParserMode.Minified);
var toast = xml.findWithAttributeValue("food", "id", "2").getIndex(0);
var price = toast.getBranch().get("price").getIndex(0);
price.getBranch().getIndex(0).setName("2.75");             // change the text

var attributes = toast.getAttributes();
var attribute = new XMLAttribute();
attribute.setName("vegan");
attribute.setValue("yes");
attributes.setIndex(attributes.length(), attribute);       // append an attribute
attributes.find("id").setValue("3");                       // change one by name

xml.save("menu.xml", XMLParserMode.IndentationTab);
```

Nodes returned by `find`, `get` and `getIndex` are the nodes of the tree:
changing them changes what `save` writes.

## 4. Build a new file

```javascript
Script.requireExtension("XML");

function newNode(type, name) {
	var node = new XMLNode();
	node.setType(type);
	node.setName(name);
	return node;
};

var xml = new XMLDocument();
xml.add(newNode(XMLNodeType.Declaration, "xml version=\"1.0\" encoding=\"UTF-8\""));
var config = xml.add(newNode(XMLNodeType.Element, "config"));   // add returns the node

var server = config.getBranch().add(newNode(XMLNodeType.Element, "server"));
var host = new XMLAttribute();
host.setName("host");
host.setValue("localhost");
server.getAttributes().setIndex(0, host);

config.getBranch().add(newNode(XMLNodeType.Element, "note"))
	.getBranch().add(newNode(XMLNodeType.Content, "a &lt; b"));  // escape text yourself

if (!xml.save("config.xml", XMLParserMode.IndentationTab)) {
	throw new Error("cannot write config.xml");
};
```

`config.xml` (CRLF line ends):

```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>
	<server host="localhost" />
	<note>a &lt; b</note>
</config>
```

## 5. Which hosts have it

| Host | `XML` available | How |
|------|-----------------|-----|
| `quantum-script` (dynamic SDK, `win64-msvc-2026/bin`) | yes | loads `quantum-script--xml.dll` |
| `quantum-script` (static SDK, `win64-msvc-2026.static/bin`) | **no** | a static executable cannot load extension DLLs: `Unable to open "XML"` |
| `magnet` scripts | yes | registered as internal by `quantum-script--magnet` |
| `fabricare` build scripts | **no** | fabricare does not register or ship `XML` |
| your own C++ host | if you register it | see section 6 |

## 6. Register it in a C++ host

A host that embeds Quantum Script makes `XML` available as an internal
extension by registering it in the init callback (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/XML.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::XML::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeFile("script.js")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("XML")`.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--xml"
	]
}
```

`quantum-script--xml` depends on `quantum-script`,
`quantum-script--console` and `file-xml`; fabricare resolves them
transitively.

## 7. Static builds

The project is `dll-or-lib`: on a static platform (for example
`win64-msvc-2026.static`, which sets `XYO_PLATFORM_COMPILE_STATIC`) the same
project builds a static library. The export macros become empty and the
`quantumScriptExtension` DLL entry point is left out (it is compiled only
with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`). Defining
`XYO_QUANTUMSCRIPT_EXTENSION_XML_LIBRARY` has the same effect when the
sources are compiled into another library.

A static host must register the extension with `registerInternalExtension`
(section 6): external DLLs cannot be loaded into a host that does not use
the engine DLL.

## 8. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("XML")`. The prototypes are
kept per thread. `file-xml` nodes are allocated from per thread memory
pools: a document belongs to the thread that loaded or built it; do not
pass `XMLDocument` / `XMLNode` objects between threads, pass the file name
or the saved text instead.

# Script API

Everything the extension defines after `Script.requireExtension("XML")`.
Read [Document model](document-model.md) first: a document is a list of
nodes, and every object is a handle into the tree.

Methods check the object they are called on: calling one on the wrong
type (for example `XMLNode.prototype.getName.call(xml)`) throws
`invalid parameter`. Methods that take an object of the extension
(`setAttributes`, `setBranch`, `setIndex`, `addDocument`) throw the same
error when given something else.

Indexes are numbers counted from `0` (strings are converted, fractions
are truncated). A negative, missing or non-numeric index is treated as
past the end: `getIndex` returns `undefined`, `setIndex` and
`removeIndex` do nothing.

Changes that would make a node its own descendant (`add`, `setIndex`,
`addDocument`, `setBranch`) throw `node cannot contain itself` and change
nothing.

## Constants

```javascript
XMLNodeType.None;                      // 0
XMLNodeType.Element;                   // 1
XMLNodeType.Content;                   // 2
XMLNodeType.CDATA;                     // 3
XMLNodeType.Comment;                   // 4
XMLNodeType.Declaration;               // 5
XMLNodeType.Bom;                       // 6
XMLNodeType.DocumentTypeDefinition;    // 7

XMLParserMode.Normal;                  // 0
XMLParserMode.Minified;                // 1
XMLParserMode.Indentation4Spaces;      // 2
XMLParserMode.IndentationTab;          // 3
```

Plain objects of numbers; see [Document model](document-model.md#nodes)
for the node types and [parser modes](document-model.md#whitespace-and-parser-modes)
for the modes.

## `XML.load(fileName[, mode])`

Parses the file and returns an `XMLDocument` holding the top level nodes,
or **`undefined`** when the file cannot be opened or is not well formed. It
does not throw and gives no error message.

```javascript
var xml = XML.load("config.xml", XMLParserMode.Minified);
if (Script.isUndefined(xml)) {
	throw new Error("config.xml missing or invalid");
};
```

`mode` defaults to `XMLParserMode.Normal`; an unknown mode is `Normal`
too.

## `XML.loadFromString(text[, mode])`

Same as `XML.load`, for XML text held in a string: returns an
`XMLDocument`, or **`undefined`** when the text is not well formed or
empty. `undefined` and `null` (also a missing argument) return
`undefined`; other values are converted with `toString`, so
`XML.loadFromString(5)` is a document holding the text `5`.

```javascript
var xml = XML.loadFromString("<config><port>8080</port></config>", XMLParserMode.Minified);
xml.find("port").getIndex(0).getBranch().getIndex(0).getName();   // "8080"
```

The text is read up to its first `\0` byte. Use it for XML received from
a process, a socket or an HTTP response, or embedded in a script.

## `XMLDocument`

```javascript
var xml = new XMLDocument();           // empty list
```

### `save(fileName[, mode])`

Writes the nodes of the list to `fileName` (created or truncated) and
returns `true`, or `false` when the file cannot be written. `mode`
defaults to `XMLParserMode.Normal`; an unknown mode is `Normal` too. Any list can be saved, also a
`getBranch()` view or a query result (that writes those nodes only, which
is usually not a well formed document).

### `saveToString([mode])`

Returns the text `save` would write, as a string. An empty list gives
`""`.

```javascript
var xml = XML.loadFromString("<r><a/><b>x</b></r>");
xml.saveToString();                                // <r><a /><b>x</b></r>
xml.saveToString(XMLParserMode.IndentationTab);    // "<r>\r\n\t<a />\r\n\t<b>x</b>\r\n</r>\r\n"
xml.get("r").getIndex(0).getBranch().saveToString();   // <a /><b>x</b>
```

### `length()`

Number of nodes in the list, of any type (text, comments, ... included).

### `getIndex(index)`

The `XMLNode` at `index` (0 based), or `undefined` past the end.

### `setIndex(index, node)`

Replaces the node at `index` with `node` (an `XMLNode`). Past the end
nothing happens. Throws `invalid parameter` when `node` is not an
`XMLNode`.

### `add(node)`

Appends `node` to the list and returns **the node** (so
`parent.getBranch().add(child)` returns `child`). Creates the node's child
list if it has none. Any other value is ignored and `undefined` is
returned.

### `removeIndex(index)`

Removes the node at `index` from the list. Past the end nothing happens.
On a `getBranch()` view this removes the child from the element; on a
query result it only removes it from the result.

### `addDocument(document)`

Appends every node of `document` (an `XMLDocument`) to this list. The
nodes are shared, not copied. `xml.addDocument(xml)` doubles the list.

### `get(name)`

New `XMLDocument` with the `Element` nodes of **this list** named `name`
(not their descendants). Empty when nothing matches.

### `find(name)`

New `XMLDocument` with the `Element` nodes named `name` in this list and
all their descendants, in document order. Empty when nothing matches.

### `findWithAttributeValue(name, attribute, value)`

As `find`, keeping only the elements that have an attribute named
`attribute` whose value is exactly `value`. Values are compared raw (an
attribute written `a&amp;b` matches `"a&amp;b"`, not `"a&b"`).

## `XMLNode`

```javascript
var node = new XMLNode();              // type None, name ""
```

| Method | Description |
|--------|-------------|
| `getType()` | the type as a number, compare with `XMLNodeType.*` |
| `setType(type)` | set the type, a number from `0` to `7` (`XMLNodeType.*`); anything else throws `invalid parameter` |
| `getName()` | tag name for elements, the text for the other types |
| `setName(name)` | set the tag name / the text (converted with `toString`) |
| `getAttributes()` | the element's `XMLAttributes`, created and attached on first call |
| `setAttributes(attributes)` | make the node use `attributes` (an `XMLAttributes`, shared, not copied) |
| `getBranch()` | the element's children as an `XMLDocument` view, created and attached on first call |
| `setBranch(document)` | make the node's children the list of `document` (an `XMLDocument`, shared, not copied) |

`setBranch(document)` shares the list: later changes through `document`
change the node's children, also when `document` was empty. Pass
`new XMLDocument()` to give a node a new empty child list (removing all
children). It throws `node cannot contain itself` when `document`
contains the node or one of its ancestors.

## `XMLAttributes`

```javascript
var attributes = new XMLAttributes();  // empty
```

| Method | Description |
|--------|-------------|
| `length()` | number of attributes |
| `getIndex(index)` | the `XMLAttribute` at `index`, `undefined` past the end |
| `setIndex(index, attribute)` | store `attribute` (an `XMLAttribute`, shared) at `index` |
| `removeIndex(index)` | remove the attribute at `index`; past the end nothing happens |
| `indexOf(name)` | index of the first attribute named `name`, or `-1` |
| `find(name)` | the first `XMLAttribute` named `name`, or `undefined` |
| `remove(name)` | remove **every** attribute named `name`, returns how many were removed |

- `setIndex(attributes.length(), attribute)` **appends**;
- `setIndex(k, attribute)` with `k < length()` replaces;
- `setIndex(k, attribute)` with `k > length()` does nothing (the list has
  no gaps);
- `removeIndex` and `remove` shift the following attributes down; the
  order of the others is kept.

Names are compared exactly (case sensitive, `x:id` is the name `x:id`).
Attribute names are not unique, in files or in this list: `setIndex`
never checks for an existing name, `find` / `indexOf` return the first
match, `remove` removes all of them.

```javascript
var attributes = node.getAttributes();
var id = attributes.find("id");                  // XMLAttribute or undefined
if (!Script.isUndefined(id)) {
	id.setValue("7");                            // changes the node
};
attributes.remove("obsolete");                   // 0, 1 or more removed
attributes.removeIndex(attributes.indexOf("x"));  // -1 when missing: nothing removed
```

## `XMLAttribute`

```javascript
var attribute = new XMLAttribute();    // name "", value ""
```

| Method | Description |
|--------|-------------|
| `getName()` / `setName(name)` | the attribute name |
| `getValue()` / `setValue(value)` | the raw value, without quotes, entities not decoded |

Values are written between double quotes. A value containing `"` is
written between single quotes when it has no `'`; escape `"` as `&quot;`
yourself otherwise.

## Text and entities

Nothing is decoded on read and nothing is encoded on write (see
[Document model](document-model.md#text-and-entities)). Escape text and
attribute values before putting them in the tree, unescape what you read:
use `xmlEscape` / `xmlUnescape` below. `CDATA` content needs neither, as
long as it does not contain `]]>`.

## Limitations

| Limitation | Effect | Work-around |
|------------|--------|-------------|
| No entity decoding / encoding | raw `&amp;` in values; unescaped `<` / `&` written as is | `xmlEscape` / `xmlUnescape` |
| `load` gives no error message | only `undefined` | check the file with another tool (`xmllint`) |
| No parent pointer, no "remove this node" | | remove by index from the parent's `getBranch()` |
| `getIndex` / `length` walk the list | quadratic loops on very long lists | fine for usual file sizes |
| No namespace processing, DTD or schema validation | names are plain strings | |

## Recipes

Functions must be defined before they are called: Quantum Script has no
hoisting. `a || b` returns a boolean in Quantum Script, so defaults are
written with `?:`.

### Load or fail

```javascript
Script.requireExtension("XML");

function xmlLoad(fileName) {
	var xml = XML.load(fileName, XMLParserMode.Minified);
	if (Script.isUndefined(xml)) {
		throw new Error("unable to load " + fileName);
	};
	return xml;
};
```

### Create a node

```javascript
function xmlNode(type, name) {
	var node = new XMLNode();
	node.setType(type);
	node.setName(name);
	return node;
};
```

### First child element by name

```javascript
// undefined if node has no child element named name
function childElement(node, name) {
	return node.getBranch().get(name).getIndex(0);
};
```

### Text of an element

```javascript
// Content and CDATA children joined, raw
function getText(node) {
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

// replace the children of node with one text node, text is raw
function setText(node, text) {
	var children = new XMLDocument();
	children.add(xmlNode(XMLNodeType.Content, text));
	node.setBranch(children);
};
```

### Read and write attributes

```javascript
// value of the first attribute named name, or defaultValue
function getAttribute(node, name, defaultValue) {
	var attribute = node.getAttributes().find(name);
	return Script.isUndefined(attribute) ? defaultValue : attribute.getValue();
};

// set the first attribute named name, or append it; value is raw
function setAttribute(node, name, value) {
	var attributes = node.getAttributes();
	var attribute = attributes.find(name);
	if (Script.isUndefined(attribute)) {
		attribute = new XMLAttribute();
		attribute.setName(name);
		attributes.setIndex(attributes.length(), attribute);
	};
	attribute.setValue(value);
};

// remove every attribute named name: node.getAttributes().remove(name)
```

### Remove a child

```javascript
// remove every child element named name from node
function removeChildren(node, name) {
	var children = node.getBranch();
	for (var k = children.length() - 1; k >= 0; --k) {
		var child = children.getIndex(k);
		if ((child.getType() == XMLNodeType.Element) && (child.getName() == name)) {
			children.removeIndex(k);
		};
	};
};
```

### Escape and unescape

```javascript
// text -> XML text or attribute value
function xmlEscape(text) {
	return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\"", "&quot;");
};

// the five predefined entities -> characters (numeric references are kept)
function xmlUnescape(text) {
	return text.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"").replace("&apos;", "'").replace("&amp;", "&");
};
```

(`String.prototype.replace` replaces every occurrence in Quantum Script.
`&amp;` is escaped first and unescaped last so `&amp;lt;` round trips.)

### Walk every element

```javascript
// call fn(node, depth) for every element under list, in document order
function walkElements(list, fn, depth) {
	for (var k = 0; k < list.length(); ++k) {
		var node = list.getIndex(k);
		if (node.getType() == XMLNodeType.Element) {
			fn(node, depth);
			walkElements(node.getBranch(), fn, depth + 1);
		};
	};
};

walkElements(xml, function(node, depth) {
	var indent = "";
	for (var k = 0; k < depth; ++k) {
		indent += "\t";
	};
	Console.writeLn(indent + node.getName());
}, 0);
```

### Read settings into an object

```xml
<settings>
	<add key="port" value="8080" />
	<add key="host" value="localhost" />
</settings>
```

```javascript
var settings = {};
var items = xmlLoad("settings.xml").find("add");
for (var k = 0; k < items.length(); ++k) {
	var item = items.getIndex(k);
	settings[getAttribute(item, "key", "")] = xmlUnescape(getAttribute(item, "value", ""));
};
// settings.port == "8080"
```

### Patch a value in place

```javascript
var xml = xmlLoad("app.config");
var port = xml.findWithAttributeValue("add", "key", "port").getIndex(0);
if (Script.isUndefined(port)) {
	throw new Error("port not found");
};
setAttribute(port, "value", "9090");
xml.save("app.config", XMLParserMode.IndentationTab);
```

### Parse text, produce text

```javascript
// XML text from another source
var reply = XML.loadFromString(text, XMLParserMode.Minified);
if (Script.isUndefined(reply)) {
	throw new Error("invalid XML reply");
};

// XML text for another destination, without a temporary file
var body = reply.saveToString(XMLParserMode.Minified);

// deep copy of a document (nodes are not shared with the original)
var copy = XML.loadFromString(original.saveToString());
```

### Copy part of a document into another

```javascript
var source = xmlLoad("a.xml");
var target = xmlLoad("b.xml");
var targetRoot = target.get("root").getIndex(0);
targetRoot.getBranch().addDocument(source.find("item"));   // nodes shared, not copied
target.save("b.xml", XMLParserMode.IndentationTab);
```

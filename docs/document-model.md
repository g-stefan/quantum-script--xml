# Document model

The script objects are handles to the C++ objects of the `file-xml`
library. Understanding three ideas — **a document is a list**, **handles
share**, **text is raw** — explains every method.

## Four object types

| Script type | Wraps (`XYO::FileXML`) | What it is |
|-------------|------------------------|------------|
| `XMLDocument` | `Document` | a **list of nodes**: a parsed file, the children of an element, or a query result |
| `XMLNode` | `Node` | one node: `type`, `name`, `attributes`, `branch` (children) |
| `XMLAttributes` | `Node::Attributes` | the ordered attribute list of an element |
| `XMLAttribute` | `Attribute` | one `name` / `value` pair |

`typeof(x)` returns the type name (`"XMLDocument"`, `"XMLNode"`, ...) and
`"" + x` gives the same text. All four are always *truthy*.

## Nodes

Every node has a `type` (a number, see `XMLNodeType`) and a `name`. What
`name` holds depends on the type:

| `XMLNodeType` | Value | Source | `getName()` |
|---------------|-------|--------|-------------|
| `None` | 0 | a `new XMLNode()` before `setType` | - (skipped by `save`) |
| `Element` | 1 | `<x:item id="1">...</x:item>`, `<br/>` | tag name: `x:item` |
| `Content` | 2 | text between markup | the raw text: `a &amp; b` |
| `CDATA` | 3 | `<![CDATA[ a < b ]]>` | text between `<![CDATA[` and `]]>`: ` a < b ` |
| `Comment` | 4 | `<!-- note -->` | text between `<!--` and `-->`: ` note ` |
| `Declaration` | 5 | `<?xml version="1.0"?>`, any `<?...?>` | text between `<?` and `?>`: `xml version="1.0"` |
| `Bom` | 6 | UTF-8 byte order mark at the start of the file | the 3 BOM bytes |
| `DocumentTypeDefinition` | 7 | `<!DOCTYPE html>` | text after `<!DOCTYPE `: `html` |

Only elements have attributes and children. **The text of an element is in
its children**, not in its name: `<name>Waffles</name>` is an `Element`
named `name` whose branch holds one `Content` node named `Waffles`.

## A document is a list

An `XMLDocument` is a list of nodes, nothing more. The same type is used
for three things:

1. **A loaded or built document** — the top level nodes: BOM, declaration,
   `DOCTYPE`, comments, the root element and (in `Normal` mode) the line
   breaks between them.

   ```
   XML.load("menu.xml", XMLParserMode.Minified)
     [0] Declaration  xml version="1.0" encoding="UTF-8"
     [1] Comment       menu
     [2] Element      menu
   ```

2. **A view of an element's children** — `node.getBranch()`. The view
   *shares* the element's list: `add`, `setIndex`, `removeIndex` on it
   change the element.

3. **A query result** — `get`, `find`, `findWithAttributeValue` return a
   **new list** that holds the matching nodes. The nodes are shared with the
   tree, the list is not: `add` / `setIndex` / `removeIndex` on a result
   change only the result.

| Operation on a list | Loaded document / `getBranch()` view | Query result |
|---------------------|--------------------------------------|--------------|
| change a node from it (`setName`, attributes, children) | changes the tree | changes the tree |
| `add`, `setIndex`, `removeIndex`, `addDocument` | changes the tree | changes only the result |

## Navigating

`get(name)` looks **only at the list it is called on**; it does not
descend. To go one level down, take the element's children with
`getBranch()`:

| Expression | Result |
|------------|--------|
| `xml.get("menu")` | the top level `menu` elements |
| `xml.get("menu").getIndex(0).getBranch().get("food")` | the `food` children of the first `menu` |
| `xml.get("menu").get("food")` | **empty**: the second `get` looks at the `menu` elements themselves |
| `xml.get("menu").find("food")` | `food` elements anywhere inside the top level `menu`s |
| `xml.find("food")` | every `food` element of the document, in document order |
| `xml.findWithAttributeValue("food", "id", "2")` | every `food` element with an attribute `id="2"` |

Names and values are compared as strings, exactly (case sensitive, no
namespace processing: `x:item` is the name `x:item`). `get` and `find` only
return `Element` nodes; `length()` and `getIndex()` count every node.

A query that matches nothing returns an **empty `XMLDocument`**
(`length() == 0`), never `undefined`. `getIndex(k)` past the end, or with
a negative or non-numeric index, returns `undefined`.

`length()` and `getIndex(k)` walk the list from its head: a loop over
`getIndex(k)` is quadratic in the list length. This is fine for files of
a few thousand nodes; for very large lists see the C++ library.

## Handles share

Every `XMLNode`, `XMLAttributes` and `XMLAttribute` you get from a
document is a handle to the object inside the tree, not a copy:

```javascript
var food = xml.find("food").getIndex(0);
food.setName("meal");                          // the tree now has <meal ...>
food.getAttributes().getIndex(0).setValue("9"); // attribute changed in the tree
```

The tree keeps its nodes alive; a handle also keeps its node alive after
it was removed from the tree, so a removed node can be added somewhere
else.

Adding the same node to two lists makes it appear twice in the output.
A node cannot become its own descendant: `add`, `setIndex`,
`addDocument` and `setBranch` throw `node cannot contain itself` instead
of creating a cycle (`node.getBranch().add(node)`, adding an ancestor
below one of its descendants, ...), and leave the tree unchanged.

## New objects

`new XMLDocument()`, `new XMLNode()`, `new XMLAttributes()` and
`new XMLAttribute()` take no arguments and return empty objects:

| Object | Starts as |
|--------|-----------|
| `new XMLDocument()` | empty list, `length()` is `0` |
| `new XMLNode()` | `getType()` is `XMLNodeType.None`, `getName()` is `""`, no attributes, no children |
| `new XMLAttributes()` | empty list |
| `new XMLAttribute()` | `getName()` and `getValue()` are `""` |

A new node must get a type before it is saved: `None` nodes are skipped.
`getAttributes()` and `getBranch()` create the node's attribute list and
child list the first time they are called, and the returned objects are
attached to the node.

## Whitespace and parser modes

`XMLParserMode` is used both for reading (`XML.load`) and for writing
(`save`):

| Mode | Value | Reading | Writing |
|------|-------|---------|---------|
| `Normal` | 0 | keep all text, including the line breaks and indentation between elements | as the tree is, no layout added |
| `Minified` | 1 | trim text, drop text that is only whitespace | everything on one line, text trimmed |
| `Indentation4Spaces` | 2 | as `Normal` | one node per line, 4 spaces per level, CRLF |
| `IndentationTab` | 3 | as `Normal` | one node per line, one tab per level, CRLF |

With `Normal` reading, `<r>\n  <a/>\n</r>` gives `r` three children:
`Content "\n  "`, `Element a`, `Content "\n"`. Read with `Minified` when
you want the data, then write with one of the indentation modes; a file
read with `Normal` and written with an indentation mode keeps its old
whitespace text nodes **and** gets the new indentation.

An omitted or unknown mode (not a number from `0` to `3`) is `Normal`;
strings are converted to numbers, so `"1"` is `Minified`.

## Text and entities

The parser does **not decode** entities and the writer does **not encode**
them. Text nodes and attribute values hold the raw characters found
between the markup:

| In the file | `getName()` / `getValue()` |
|-------------|----------------------------|
| `<a>x &amp; y</a>` | `x &amp; y` |
| `<a v="&lt;1&gt;"/>` | `&lt;1&gt;` |
| `<![CDATA[x & y]]>` | `x & y` (CDATA needs no decoding) |

When you put text into the tree, **escape it yourself** (`&` → `&amp;`,
`<` → `&lt;`, `>` → `&gt;`, `"` → `&quot;` in attribute values); otherwise
`save` writes `<e>1 < 2</e>`, which is not well formed and `XML.load`
cannot read back. See the `xmlEscape` / `xmlUnescape` helpers in the
[Script API recipes](script-api.md#recipes).

Strings are byte strings (UTF-8 by convention): the parser does not convert
encodings. A UTF-8 BOM is kept as a `Bom` node and written back.

## Output details

- Elements without children are written as `<a />`, also when they were
  read as `<a></a>`.
- Attributes keep their order; duplicate names are kept as they are. A
  value containing `"` (and no `'`) is written between single quotes.
- Indentation modes write CRLF line ends; `Normal` and `Minified` write no
  line ends of their own.
- Saving an empty document writes an empty file.

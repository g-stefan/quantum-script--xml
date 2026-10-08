# Quantum Script Extension XML

Quantum Script extension
- Reading XML files or text into a tree of nodes:
`XML.load` / `XML.loadFromString` return an `XMLDocument`, or `undefined`
when the input is missing or not well formed.
- Querying and changing the tree:
`get`, `find` and `findWithAttributeValue` select elements, nodes and
attributes are live handles that change the document in place.
- Building new documents and writing them, to a file or a string, with
or without indentation: `new XMLDocument()`, `new XMLNode()`, `add`,
`save`, `saveToString`.
- For configuration and project files, manifests and data exchange formats.

```javascript
Script.requireExtension("XML");

XMLNodeType.None;
XMLNodeType.Element;
XMLNodeType.Content;
XMLNodeType.CDATA;
XMLNodeType.Comment;
XMLNodeType.Declaration;
XMLNodeType.Bom;
XMLNodeType.DocumentTypeDefinition;
XMLParserMode.Normal;
XMLParserMode.Minified;
XMLParserMode.Indentation4Spaces;
XMLParserMode.IndentationTab;
XML;
XML.load(fileName,parserMode);
XML.loadFromString(text,parserMode);
XMLAttribute.prototype.getName();
XMLAttribute.prototype.setName(value);
XMLAttribute.prototype.getValue();
XMLAttribute.prototype.setValue(value);
XMLAttributes.prototype.getIndex(index);
XMLAttributes.prototype.setIndex(index,value);
XMLAttributes.prototype.length();
XMLAttributes.prototype.removeIndex(index);
XMLAttributes.prototype.indexOf(name);
XMLAttributes.prototype.find(name);
XMLAttributes.prototype.remove(name);
XMLNode.prototype.getType();
XMLNode.prototype.setType(value);
XMLNode.prototype.getName();
XMLNode.prototype.setName(value);
XMLNode.prototype.getAttributes();
XMLNode.prototype.setAttributes(value);
XMLNode.prototype.getBranch();
XMLNode.prototype.setBranch(value);
XMLDocument.prototype.save(fileName,parserMode);
XMLDocument.prototype.saveToString(parserMode);
XMLDocument.prototype.length();
XMLDocument.prototype.getIndex(idx);
XMLDocument.prototype.setIndex(idx,value);
XMLDocument.prototype.add(node);
XMLDocument.prototype.get(name);
XMLDocument.prototype.find(name);
XMLDocument.prototype.findWithAttributeValue(name,attribute,value);
XMLDocument.prototype.addDocument(document);
XMLDocument.prototype.removeIndex(index);
```

Built on `quantum-script` and `file-xml`, part of the XYO C++ SDK; also
available in magnet.

Text and attribute values are raw: entities are not decoded on read and
not encoded on write; see [text and entities](docs/document-model.md#text-and-entities).

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, read, change and write a file, hosts, register in a C++ host
- [Document model](docs/document-model.md) - documents as lists, views, sharing, whitespace, entities
- [Script API](docs/script-api.md) - every constant, object and method, limitations, recipes
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, build defines, how it works
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--xml](.claude/skills/quantum-script--xml/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.

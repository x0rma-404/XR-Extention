# XR Programming Language

<p align="center">
  <b>A small experimental programming language written in C++</b><br>
  with dedicated Visual Studio Code support.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/language-C%2B%2B17-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/editor-VS%20Code-007ACC?style=for-the-badge">
  <img src="https://img.shields.io/badge/files-.xr-orange?style=for-the-badge">
  <img src="https://img.shields.io/badge/license-MIT-green?style=for-the-badge">
</p>

---

## About

**XR** is a small experimental programming language built from scratch in **C++**.

Instead of directly following the syntax of languages such as Python, Java, or C++, XR uses its own syntax and concepts.

The project consists of two main parts:

* **XR Interpreter** — executes `.xr` source files.
* **XR VS Code Extension** — provides a complete development environment for XR.

The goal of XR is to experiment with language design, interpreters, syntax, developer tooling, and programming-language concepts while keeping the language simple and distinctive.

---

## Features

### Language

* C++17 interpreter
* `.xr` source files
* Variables
* Conditional statements
* Loops
* Lists
* Ranges
* Indexing
* Slicing
* Arithmetic operators
* Comparison operators
* Logical operators
* String operations
* Built-in functions
* Comments

### Visual Studio Code

* XR language recognition
* Syntax highlighting
* Autocomplete / IntelliSense
* Variable suggestions
* Built-in function suggestions
* Code snippets
* Error diagnostics
* Problems panel integration
* `XR: Run File` command
* Integrated interpreter execution

---

# XR Syntax

## Variables

Variables are declared using the `deyer` keyword.

```xr
deyer -> ad;
deyer -> yas;
```

Values are assigned using the `->` operator.

```xr
'Xorma' -> ad;
20 -> yas;
```

Variables can then be used directly:

```xr
> ad;
> yas;
```

Output:

```text
Xorma
20
```

---

## Printing

The `>` operator is used to print a value.

```xr
> 'Salam XR!';
```

Variables can also be printed:

```xr
deyer -> ad;

'Xorma' -> ad;

> ad;
```

Output:

```text
Xorma
```

Expressions can be printed as well:

```xr
> 'Salam ' ~ ad;
```

---

# Conditions

XR uses `?` to define a conditional statement.

```xr
? ad = 'Xorma' {
    > 'Dogru';
} : {
    > 'Yanlis';
}
```

The `:` block represents the alternative branch.

### Example

```xr
deyer -> yas;

20 -> yas;

? yas >= 18 {
    > 'Yetkin';
} : {
    > 'Yetkin deyil';
}
```

---

# Loops

XR uses `@` for loops.

A condition can be used directly:

```xr
@ x < 5 {
    > x;
}
```

XR can also iterate over a list:

```xr
@ x : #[1, 2, 3] {
    > x;
}
```

Output:

```text
1
2
3
```

---

# Lists

Lists use the `#[ ]` syntax.

```xr
#[1, 2, 3, 4, 5]
```

A list can be assigned to a variable:

```xr
deyer -> nums;

#[1, 2, 3, 4, 5] -> nums;
```

Lists can contain different values depending on the interpreter's supported types.

---

# Indexing

List elements are accessed using `.`.

```xr
deyer -> nums;

#[10, 20, 30] -> nums;

> nums.0;
> nums.1;
> nums.2;
```

Output:

```text
10
20
30
```

### Variable Index

Indexes can also be stored in variables:

```xr
nums.i
```

### Expression Index

Expressions can be used as indexes:

```xr
nums.(i + 1)
```

---

# Slicing

XR supports selecting a section of a list using ranges.

```xr
nums.(1..3)
```

The general syntax is:

```text
start..end
```

For example:

```xr
deyer -> nums;

#[10, 20, 30, 40, 50] -> nums;

> nums.(1..3);
```

---

# Ranges

Ranges are represented using `..`.

```xr
1..5
```

Ranges can be used with loops:

```xr
@ x : 1..5 {
    > x;
}
```

Output:

```text
1
2
3
4
5
```

---

# Operators

XR provides operators for arithmetic, comparison, logical operations, concatenation, power, and assignment.

## Arithmetic

| Operator | Operation      |
| -------- | -------------- |
| `+`      | Addition       |
| `-`      | Subtraction    |
| `*`      | Multiplication |
| `/`      | Division       |
| `%`      | Modulo         |

Example:

```xr
5 + 3
10 - 2
4 * 5
20 / 4
10 % 3
```

---

## Power

The `^` operator is used for power / repetition.

```xr
2 ^ 3
```

---

## Comparison

| Operator | Meaning               |
| -------- | --------------------- |
| `=`      | Equal                 |
| `<>`     | Not equal             |
| `<`      | Less than             |
| `>`      | Greater than          |
| `<=`     | Less than or equal    |
| `>=`     | Greater than or equal |

Example:

```xr
10 > 5
10 = 10
5 <> 3
```

---

## Logical Operators

| Operator | Meaning |
| -------- | ------- |
| `&`      | AND     |
| `\|`     | OR      |
| `!`      | NOT     |

Example:

```xr
x > 5 & x < 10
```

---

## Concatenation

The `~` operator combines values.

```xr
'Salam ' ~ 'XR'
```

Output:

```text
Salam XR
```

Variables can also be combined:

```xr
deyer -> ad;

'Xorma' -> ad;

> 'Salam ' ~ ad;
```

---

## Assignment

The `->` operator assigns a value to a variable.

```xr
5 -> x;
```

For example:

```xr
deyer -> x;

10 -> x;
```

---

# Built-in Functions

XR currently provides the following built-in functions:

| Function   | Purpose                        |
| ---------- | ------------------------------ |
| `uz()`     | Returns the length of a value  |
| `qat()`    | Combines two values            |
| `cixar()`  | Performs subtraction           |
| `metn()`   | Converts a value to text       |
| `eded()`   | Converts a value to a number   |
| `sirala()` | Sorts a list                   |
| `boyuk()`  | Returns the larger value       |
| `kicik()`  | Returns the smaller value      |
| `bol()`    | Performs division              |
| `yig()`    | Returns the sum of list values |
| `var()`    | Checks whether a value exists  |

---

## `uz()`

Returns the length of a value.

```xr
> uz('XR');
```

---

## `qat()`

Combines two values.

```xr
> qat('Hello', 'XR');
```

---

## `cixar()`

Performs subtraction.

```xr
> cixar(10, 3);
```

---

## `metn()`

Converts a value to text.

```xr
> metn(123);
```

---

## `eded()`

Converts a value to a number.

```xr
> eded('123');
```

---

## `sirala()`

Sorts a list.

```xr
> sirala(#[5, 2, 8, 1]);
```

---

## `boyuk()`

Returns the larger value.

```xr
> boyuk(10, 20);
```

---

## `kicik()`

Returns the smaller value.

```xr
> kicik(10, 20);
```

---

## `bol()`

Performs division.

```xr
> bol(20, 4);
```

---

## `yig()`

Returns the sum of values in a list.

```xr
> yig(#[1, 2, 3, 4]);
```

---

## `var()`

Checks whether a value exists.

```xr
> var(x);
```

---

# Comments

Comments begin with `//`.

```xr
// This is an XR comment

deyer -> ad;
```

Comments are ignored by the interpreter.

---

# Strings

XR uses single quotes for strings.

```xr
'Salam XR!'
```

Strings can be combined using `~`.

```xr
'Hello' ~ ' XR'
```

Output:

```text
Hello XR
```

---

# Complete Example

```xr
// XR example

deyer -> ad;
deyer -> nums;

'Xorma' -> ad;
#[1, 2, 3, 4, 5] -> nums;

> 'Salam ' ~ ad;

? ad = 'Xorma' {
    > 'Dogru ad';
} : {
    > 'Yanlis ad';
}

@ x : nums {
    > x;
}
```

Output:

```text
Salam Xorma
Dogru ad
1
2
3
4
5
```

---

# Visual Studio Code Extension

XR comes with a dedicated **Visual Studio Code extension** designed to make `.xr` development easier.

The extension provides:

* `.xr` language recognition
* Syntax highlighting
* Autocomplete
* Variable suggestions
* Built-in function documentation
* Code snippets
* Error diagnostics
* Problems panel integration
* XR interpreter execution

---

# Syntax Highlighting

XR source files are automatically recognized as `.xr` files.

The syntax definition is located at:

```text
vscode-extension/syntaxes/xr.tmLanguage.json
```

It provides highlighting for:

* Keywords
* Variables
* Strings
* Numbers
* Operators
* Comments
* Built-in functions
* Control structures

---

# Autocomplete

The extension provides autocomplete for:

* XR keywords
* Built-in functions
* Operators
* Snippets
* Declared variables

For example:

```xr
deyer -> ad;
deyer -> yas;

'Xorma' -> ad;
20 -> yas;
```

After declaring a variable, it can appear in autocomplete suggestions.

Built-in functions can also display documentation:

```text
uz(value)

Dəyərin uzunluğunu qaytarır
```

---

# Snippets

XR provides snippets for frequently used structures.

### Conditional

Typing:

```text
if
```

can generate:

```xr
? sert {
    
} : {
    
}
```

### Loop

Typing:

```text
loop
```

can generate:

```xr
@ sert {
    
}
```

### Foreach

Typing:

```text
foreach
```

can generate:

```xr
@ x : siyahi {
    
}
```

---

# Error Diagnostics

Interpreter errors can be displayed directly inside Visual Studio Code.

For example:

```xr
> ad2;
```

If `ad2` has not been declared, the interpreter can report:

```text
Xeta: teyin olunmamis deyisen: ad2
```

The extension can forward interpreter errors to the **Problems** panel, making errors easier to locate while developing.

---

# Running the Interpreter

The XR interpreter is written in **C++17**.

### Compile

```bash
g++ -std=c++17 xr.cpp -o xr.exe
```

### Run

```bash
xr.exe hello.xr
```

For example:

```bash
xr.exe examples/hello.xr
```

---

# Running XR in Visual Studio Code

Open the VS Code extension project.

Press:

```text
F5
```

This launches the **Extension Development Host**.

Create or open an `.xr` file.

Then open the Command Palette:

```text
Ctrl + Shift + P
```

Search for:

```text
XR: Run File
```

Press **Enter**.

The extension will execute the current XR file using the XR interpreter.

---

# Project Structure

```text
XR/
│
├── interpreter/
│   ├── xr.cpp
│   └── xr.exe
│
├── vscode-extension/
│   ├── package.json
│   ├── extension.js
│   ├── language-configuration.json
│   ├── xr.exe
│   │
│   ├── snippets/
│   │   └── xr.json
│   │
│   └── syntaxes/
│       └── xr.tmLanguage.json
│
├── examples/
│   ├── hello.xr
│   ├── variables.xr
│   ├── conditions.xr
│   └── loops.xr
│
└── README.md
```

---

# Examples

Example programs can be found in:

```text
examples/
```

Current examples include:

```text
hello.xr
variables.xr
conditions.xr
loops.xr
```

These examples demonstrate the basic features of the language.

---

# Development

### Interpreter

The XR interpreter is implemented in:

```text
interpreter/xr.cpp
```

### VS Code Extension

The extension logic is implemented in:

```text
vscode-extension/extension.js
```

### Language Definition

Syntax highlighting is defined in:

```text
vscode-extension/syntaxes/xr.tmLanguage.json
```

### Snippets

Code snippets are located at:

```text
vscode-extension/snippets/xr.json
```

---

# Project Status

XR is currently an **experimental programming language project**.

The current version includes:

* A working C++ interpreter
* A custom programming-language syntax
* `.xr` source files
* Lists and ranges
* Conditions and loops
* Built-in functions
* VS Code syntax highlighting
* Autocomplete
* Snippets
* Diagnostics
* File execution through VS Code

The language and development tools are still evolving, and syntax or features may change in future versions.

---

# Roadmap

Potential future improvements include:

* [ ] User-defined functions
* [ ] More data types
* [ ] Better type handling
* [ ] Improved error messages
* [ ] More advanced IntelliSense
* [ ] Debugging support
* [ ] Package / module system
* [ ] Standard library
* [ ] Better runtime error locations
* [ ] XR language server
* [ ] Cross-platform interpreter builds
* [ ] VS Code Marketplace release

---

# Why XR?

XR is not intended to replace established programming languages.

It is a project for exploring how a programming language works from the inside:

```text
Source Code
     │
     ▼
   Lexer
     │
     ▼
   Parser
     │
     ▼
 Interpreter
     │
     ▼
   Output
```

At the same time, the VS Code extension demonstrates how a custom language can be integrated into a modern development environment.

---

# Author

**Xorma**

GitHub:

[x0rma-404](https://github.com/x0rma-404?utm_source=chatgpt.com)

---

# License

This project is licensed under the **MIT License**.

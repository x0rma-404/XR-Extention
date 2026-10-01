# XR Programming Language

**XR** is a small experimental programming language written in **C++**, with dedicated **Visual Studio Code** support.

XR is designed with its own syntax rather than directly following Python, Java, or other existing programming languages.

The project contains a C++ interpreter and a VS Code extension that provides syntax highlighting, autocomplete, snippets, diagnostics, and file execution.

---

## Features

### Language

* C++ interpreter
* `.xr` source files
* Variables
* Conditions
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

### Visual Studio Code

* XR language recognition
* Syntax highlighting
* Autocomplete / IntelliSense
* Variable autocomplete
* Code snippets
* Error diagnostics
* Problems panel integration
* `XR: Run File` command

---

# XR Syntax

## Variables

Variables are declared using `deyer`.

```xr
deyer -> ad;
deyer -> yas;
```

A value can then be assigned using `->`.

```xr
'Xorma' -> ad;
20 -> yas;
```

Output:

```xr
> ad;
> yas;
```

---

## Printing

The `>` operator prints a value.

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

---

# Conditions

XR uses `?` for conditional statements.

```xr
? ad = 'Xorma' {
    > 'Dogru';
} : {
    > 'Yanlis';
}
```

The `:` block represents the alternative branch.

---

# Loops

XR uses `@` for loops.

A condition can be used with a loop:

```xr
@ x < 5 {
    > x;
}
```

XR also supports iterating over a list:

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

---

# Indexing

List elements can be accessed using `.`.

```xr
deyer -> nums;

#[10, 20, 30] -> nums;

> nums.0;
> nums.1;
> nums.2;
```

XR also supports variable indexes:

```xr
nums.i
```

and expressions:

```xr
nums.(i + 1)
```

---

# Slicing

A part of a list can be selected using a range.

```xr
nums.(1..3)
```

The range syntax uses:

```text
start..end
```

---

# Ranges

XR supports ranges using `..`.

```xr
1..5
```

Ranges can be used in loops:

```xr
@ x : 1..5 {
    > x;
}
```

---

# Operators

XR supports arithmetic, comparison, logical, concatenation, power, and assignment operators.

## Arithmetic

```text
+    Addition
-    Subtraction
*    Multiplication
/    Division
%    Modulo
```

Example:

```xr
5 + 3
10 - 2
4 * 5
20 / 4
10 % 3
```

---

## Power / Repetition

```text
^
```

Example:

```xr
2 ^ 3
```

---

## Comparison

```text
=     Equal
<>    Not equal
<     Less than
>     Greater than
<=    Less than or equal
>=    Greater than or equal
```

---

## Logical Operators

```text
&     AND
|     OR
!     NOT
```

---

## Concatenation

The `~` operator is used for combining values.

```xr
'Salam ' ~ 'XR'
```

---

## Assignment

The `->` operator assigns a value.

```xr
5 -> x;
```

---

# Built-in Functions

XR currently includes several built-in functions.

```text
uz
qat
cixar
metn
eded
sirala
boyuk
kicik
bol
yig
var
```

## `uz`

Returns the length of a value.

```xr
uz('XR')
```

---

## `qat`

Combines two values.

```xr
qat('Hello', 'XR')
```

---

## `cixar`

Performs subtraction.

```xr
cixar(10, 3)
```

---

## `metn`

Converts a value to text.

```xr
metn(123)
```

---

## `eded`

Converts a value to a number.

```xr
eded('123')
```

---

## `sirala`

Sorts a list.

```xr
sirala(#[5, 2, 8, 1])
```

---

## `boyuk`

Returns the larger value.

```xr
boyuk(10, 20)
```

---

## `kicik`

Returns the smaller value.

```xr
kicik(10, 20)
```

---

## `bol`

Performs division.

```xr
bol(20, 4)
```

---

## `yig`

Returns the sum of values in a list.

```xr
yig(#[1, 2, 3, 4])
```

---

## `var`

Checks whether a value exists.

```xr
var(x)
```

---

# Comments

Comments begin with `//`.

```xr
// This is an XR comment

deyer -> ad;
```

---

# Strings

XR uses single quotes for strings.

```xr
'Salam XR!'
```

Example:

```xr
'Hello' ~ ' XR'
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

---

# Visual Studio Code Extension

XR includes a Visual Studio Code extension.

The extension provides:

* `.xr` language recognition
* Syntax highlighting
* Autocomplete
* Variable suggestions
* Built-in function documentation
* Code snippets
* Problems panel diagnostics
* XR interpreter execution

---

# Running the Interpreter

The interpreter is written in C++.

Compile it using:

```bash
g++ -std=c++17 xr.cpp -o xr.exe
```

Then run an XR program:

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

This opens the **Extension Development Host**.

Create or open an `.xr` file.

Then open the Command Palette:

```text
Ctrl + Shift + P
```

Search for:

```text
XR: Run File
```

Press Enter.

The XR interpreter will execute the current `.xr` file.

---

# Autocomplete

XR provides autocomplete for built-in functions, keywords, operators, snippets, and variables.

For example:

```xr
deyer -> ad;
deyer -> yas;

'Xorma' -> ad;
20 -> yas;
```

When writing another XR statement, the declared variables can appear in autocomplete.

Built-in functions also provide descriptions such as:

```text
uz(value)

Dəyərin uzunluğunu qaytarır
```

---

# Snippets

The extension includes snippets for common XR structures.

For example, typing:

```text
if
```

can generate:

```xr
? sert {
    
} : {
    
}
```

Typing:

```text
loop
```

can generate:

```xr
@ sert {
    
}
```

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

XR interpreter errors can appear in the Visual Studio Code Problems panel.

For example:

```xr
> ad2;
```

when `ad2` has not been declared can produce:

```text
Xeta: teyin olunmamis deyisen: ad2
```

The extension can display the error directly in the editor.

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

Example programs are located in:

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

---

# Development

The interpreter is implemented in:

```text
interpreter/xr.cpp
```

The Visual Studio Code extension is implemented in:

```text
vscode-extension/extension.js
```

The XR syntax definition is located at:

```text
vscode-extension/syntaxes/xr.tmLanguage.json
```

Snippets are located at:

```text
vscode-extension/snippets/xr.json
```

---

# Project Status

XR is currently an experimental programming language project.

The current version includes a working interpreter and Visual Studio Code development environment.

The language and tooling may change as the project develops.

---

# Author

**Xorma**

GitHub:

```text
https://github.com/x0rma-404
```

---

# License

MIT License

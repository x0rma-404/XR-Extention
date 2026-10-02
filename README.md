# ⚡ XR - Experimental Programming Language & VS Code Extension

![Contributors](https://img.shields.io/github/contributors/x0rma-404/XR-Extention?style=for-the-badge&color=blue)
![Forks](https://img.shields.io/github/forks/x0rma-404/XR-Extention?style=for-the-badge&color=magenta)
![Stars](https://img.shields.io/github/stars/x0rma-404/XR-Extention?style=for-the-badge&color=yellow)

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue?style=for-the-badge)
![Editor](https://img.shields.io/badge/editor-VS%20Code-007ACC?style=for-the-badge)
![Files](https://img.shields.io/badge/files-.xr-orange?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)

XR is a small **experimental programming language** built from scratch in **C++**, with its own syntax, its own interpreter, and a dedicated **Visual Studio Code extension** — kind of like a mini **language + interpreter + IDE playground**! ✨

Instead of copying the syntax of Python, Java, or C++, XR comes with its own operators, keywords, and concepts, so you can explore how a programming language really works from the inside. 🧪

---

## 📚 Table of Contents

* [🚀 Features](#-features)
* [🧠 Built With](#-built-with)
* [📦 Installation](#-installation)
* [🎨 VS Code Extension Setup](#-vs-code-extension-setup)
* [🛠 Usage](#-usage)
* [➕ Operators](#-operators)
* [🧰 Built-in Functions](#-built-in-functions)
* [🧪 Complete Example](#-complete-example)
* [🎨 VS Code Extension Features](#-vs-code-extension-features)
* [🗂 Project Structure](#-project-structure)
* [🔍 How It Works](#-how-it-works)
* [🩺 Troubleshooting](#-troubleshooting)
* [📈 Future Plans](#-future-plans)
* [📄 License](#-license)
* [🤝 Contributing](#-contributing)

---

## 🚀 Features

### 🧠 Language

* ⚙️ **C++17 Interpreter**: Executes `.xr` source files from the command line.
* 🧩 **Custom Syntax**: Unique operators like `->`, `?`, `@`, `#[ ]`, and `~`.
* 📦 **Variables** declared with the `deyer` keyword.
* 🔀 **Conditional statements** using `?` and `:`.
* 🔁 **Loops** using `@` — with conditions, lists, and ranges.
* 📋 **Lists** with the `#[ ]` syntax.
* 🎯 **Indexing & Slicing**: access single items, variable indexes, expression indexes, and sections of a list.
* 📏 **Ranges** using `..`.
* ➕ **Arithmetic, comparison, and logical operators**.
* 🔤 **String operations** with single-quoted strings and concatenation.
* 🧰 **Built-in functions** for length, sorting, sum, min/max, conversions, and more.
* 💬 **Comments** using `//`.

### 🎨 Visual Studio Code

* 🌈 **XR language recognition** for `.xr` files.
* 🖍 **Syntax highlighting** for keywords, variables, strings, numbers, operators, comments, built-ins, and control structures.
* 💡 **Autocomplete / IntelliSense** with keywords, built-ins, operators, snippets, and declared variables.
* 📖 **Built-in function documentation** shown while typing.
* ✂️ **Code snippets** for conditions, loops, and foreach loops.
* 🐞 **Error diagnostics** sent directly to the **Problems** panel.
* ▶️ **`XR: Run File`** command with integrated interpreter execution.

---

## 🧠 Built With

* **C++17** for the XR interpreter
* **JavaScript** for the VS Code extension logic
* **TextMate Grammar (JSON)** for syntax highlighting
* **VS Code Extension API** for autocomplete, diagnostics, and commands
* **JSON** for snippets and language configuration

---

## 📦 Installation

You only need a **C++17 compatible compiler** (like `g++`) to build the interpreter.

1. Clone the repository:

```bash
git clone https://github.com/x0rma-404/XR-Extention.git
cd XR-Extention
```

2. Compile the interpreter:

```bash
g++ -std=c++17 interpreter/xr.cpp -o xr.exe
```

3. Run your first XR program:

```bash
xr.exe examples/hello.xr
```

4. Or run any other example:

```bash
xr.exe examples/variables.xr
xr.exe examples/conditions.xr
xr.exe examples/loops.xr
```

---

## 🎨 VS Code Extension Setup

1. Open the `vscode-extension` folder in **Visual Studio Code**.
2. Press `F5` to launch the **Extension Development Host**.
3. Create or open a file with the `.xr` extension.
4. Open the Command Palette:

```text
Ctrl + Shift + P
```

5. Search for:

```text
XR: Run File
```

6. Press **Enter** — the extension will execute the current XR file using the XR interpreter. 🚀

> [!NOTE]
> The extension uses the `xr.exe` file located inside the `vscode-extension` folder. Make sure it is compiled and up to date.

---

## 🛠 Usage

### 📌 Variables

Variables are declared using the `deyer` keyword and assigned using the `->` operator.

```xr
deyer -> ad;
deyer -> yas;

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

### 🖨 Printing

The `>` operator prints a value.

```xr
> 'Salam XR!';
```

Variables and expressions can be printed too:

```xr
deyer -> ad;
'Xorma' -> ad;

> ad;
> 'Salam ' ~ ad;
```

Output:

```text
Xorma
Salam Xorma
```

### ❓ Conditions

XR uses `?` for conditions, and the `:` block is the alternative branch.

```xr
? ad = 'Xorma' {
    > 'Dogru';
} : {
    > 'Yanlis';
}
```

Another example:

```xr
deyer -> yas;
20 -> yas;

? yas >= 18 {
    > 'Yetkin';
} : {
    > 'Yetkin deyil';
}
```

### 🔁 Loops

XR uses `@` for loops.

A condition can be used directly:

```xr
@ x < 5 {
    > x;
}
```

Iterating over a list:

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

Iterating over a range:

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

### 📋 Lists

Lists use the `#[ ]` syntax and can be assigned to variables.

```xr
deyer -> nums;

#[1, 2, 3, 4, 5] -> nums;
```

### 🎯 Indexing

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

Indexes can be stored in variables:

```xr
nums.i
```

Expressions can also be used as indexes:

```xr
nums.(i + 1)
```

### ✂️ Slicing

Select a section of a list using ranges:

```text
start..end
```

```xr
deyer -> nums;
#[10, 20, 30, 40, 50] -> nums;

> nums.(1..3);
```

### 📏 Ranges

Ranges are written using `..` and work great with loops.

```xr
1..5
```

### 🔤 Strings

XR uses single quotes for strings, and `~` combines them.

```xr
'Salam XR!'
'Hello' ~ ' XR'
```

Output:

```text
Hello XR
```

### 💬 Comments

Comments begin with `//` and are ignored by the interpreter.

```xr
// This is an XR comment

deyer -> ad;
```

---

## ➕ Operators

XR provides operators for arithmetic, comparison, logical operations, concatenation, power, and assignment.

### ➕ Arithmetic

| Operator | Operation |
| -------- | --------- |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `%` | Modulo |

```xr
5 + 3
10 - 2
4 * 5
20 / 4
10 % 3
```

### ⚡ Power

The `^` operator is used for power / repetition.

```xr
2 ^ 3
```

### ⚖️ Comparison

| Operator | Meaning |
| -------- | ------- |
| `=` | Equal |
| `<>` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |

```xr
10 > 5
10 = 10
5 <> 3
```

### 🧠 Logical

| Operator | Meaning |
| -------- | ------- |
| `&` | AND |
| `\|` | OR |
| `!` | NOT |

```xr
x > 5 & x < 10
```

### 🔗 Concatenation

The `~` operator combines values.

```xr
'Salam ' ~ 'XR'
```

Output:

```text
Salam XR
```

### 📝 Assignment

The `->` operator assigns a value to a variable.

```xr
deyer -> x;

10 -> x;
```

---

## 🧰 Built-in Functions

XR currently provides the following built-in functions:

| Function | Purpose |
| -------- | ------- |
| `uz()` | Returns the length of a value |
| `qat()` | Combines two values |
| `cixar()` | Performs subtraction |
| `metn()` | Converts a value to text |
| `eded()` | Converts a value to a number |
| `sirala()` | Sorts a list |
| `boyuk()` | Returns the larger value |
| `kicik()` | Returns the smaller value |
| `bol()` | Performs division |
| `yig()` | Returns the sum of list values |
| `var()` | Checks whether a value exists |

### 📏 `uz()`

Returns the length of a value.

```xr
> uz('XR');
```

### 🔗 `qat()`

Combines two values.

```xr
> qat('Hello', 'XR');
```

### ➖ `cixar()`

Performs subtraction.

```xr
> cixar(10, 3);
```

### 🔤 `metn()`

Converts a value to text.

```xr
> metn(123);
```

### 🔢 `eded()`

Converts a value to a number.

```xr
> eded('123');
```

### 📊 `sirala()`

Sorts a list.

```xr
> sirala(#[5, 2, 8, 1]);
```

### ⬆️ `boyuk()`

Returns the larger value.

```xr
> boyuk(10, 20);
```

### ⬇️ `kicik()`

Returns the smaller value.

```xr
> kicik(10, 20);
```

### ➗ `bol()`

Performs division.

```xr
> bol(20, 4);
```

### ➕ `yig()`

Returns the sum of values in a list.

```xr
> yig(#[1, 2, 3, 4]);
```

### 🔍 `var()`

Checks whether a value exists.

```xr
> var(x);
```

---

## 🧪 Complete Example

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

## 🎨 VS Code Extension Features

XR comes with a dedicated **Visual Studio Code extension** designed to make `.xr` development easier and more fun. 🎉

### 🖍 Syntax Highlighting

`.xr` files are automatically recognized. The syntax definition is located at:

```text
vscode-extension/syntaxes/xr.tmLanguage.json
```

It provides highlighting for:

* 🔑 Keywords
* 📦 Variables
* 🔤 Strings
* 🔢 Numbers
* ➕ Operators
* 💬 Comments
* 🧰 Built-in functions
* 🔀 Control structures

### 💡 Autocomplete

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

After declaring a variable, it appears in autocomplete suggestions. Built-in functions also display documentation:

```text
uz(value)

Dəyərin uzunluğunu qaytarır
```

### ✂️ Snippets

| Trigger | Generates |
| ------- | --------- |
| `if` | Conditional block |
| `loop` | Condition loop |
| `foreach` | List loop |

**Conditional** (`if`):

```xr
? sert {
    
} : {
    
}
```

**Loop** (`loop`):

```xr
@ sert {
    
}
```

**Foreach** (`foreach`):

```xr
@ x : siyahi {
    
}
```

### 🐞 Error Diagnostics

Interpreter errors can be displayed directly inside Visual Studio Code.

```xr
> ad2;
```

If `ad2` has not been declared, the interpreter reports:

```text
Xeta: teyin olunmamis deyisen: ad2
```

The extension forwards interpreter errors to the **Problems** panel, making errors easier to locate while developing.

### ▶️ Run File

Use the Command Palette (`Ctrl + Shift + P`) and run `XR: Run File` to execute the current XR file instantly using the integrated interpreter.

---

## 🗂 Project Structure

```text
XR-Extention/
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

### 📂 Where is what?

* **Interpreter** → `interpreter/xr.cpp`
* **VS Code extension logic** → `vscode-extension/extension.js`
* **Syntax highlighting** → `vscode-extension/syntaxes/xr.tmLanguage.json`
* **Snippets** → `vscode-extension/snippets/xr.json`
* **Examples** → `examples/`

---

## 🔍 How It Works

XR isn't meant to replace established languages — it's a project for exploring how a language works from the inside:

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

* 🔤 **Lexer** breaks the source code into tokens.
* 🌳 **Parser** turns tokens into a structured program.
* ⚙️ **Interpreter** executes the program and produces output.

At the same time, the VS Code extension shows how a custom language can be integrated into a modern development environment.

---

## 🩺 Troubleshooting

* **`g++` is not recognized** → Install a C++ compiler (for example MinGW-w64 on Windows) and make sure it is added to your `PATH`.
* **`XR: Run File` does nothing** → Make sure `xr.exe` exists inside the `vscode-extension` folder and was compiled from the latest `xr.cpp`.
* **`Xeta: teyin olunmamis deyisen`** → You used a variable that was never declared. Declare it first with `deyer -> name;`.
* **Extension commands are missing** → Launch the extension with `F5` from the `vscode-extension` project so the Extension Development Host starts.

---

## 📊 Project Status

XR is currently an **experimental programming language project**. The current version includes:

* ✅ A working C++ interpreter
* ✅ A custom programming-language syntax
* ✅ `.xr` source files
* ✅ Lists and ranges
* ✅ Conditions and loops
* ✅ Built-in functions
* ✅ VS Code syntax highlighting
* ✅ Autocomplete
* ✅ Snippets
* ✅ Diagnostics
* ✅ File execution through VS Code

> [!WARNING]
> The language and development tools are still evolving, so syntax or features may change in future versions.

---

## 📈 Future Plans

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

## 📄 License

This project is licensed under the **MIT License**.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!  
Feel free to check the [issues page](https://github.com/x0rma-404/XR-Extention/issues).

1. 🍴 Fork the repository
2. 🌿 Create your feature branch (`git checkout -b feature/amazing-feature`)
3. 💾 Commit your changes (`git commit -m "Add amazing feature"`)
4. 📤 Push to the branch (`git push origin feature/amazing-feature`)
5. 🔁 Open a Pull Request

### ✨ Contributors

A huge thanks to these amazing people who have contributed to XR:

<a href="https://github.com/x0rma-404/XR-Extention/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=x0rma-404/XR-Extention" />
</a>

---

## 👤 Author

**Xorma**

GitHub: [x0rma-404](https://github.com/x0rma-404)

---

## 🙌 Thanks

Thank you for exploring XR! Feel free to ⭐ the repo to show support.

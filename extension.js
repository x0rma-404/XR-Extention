const vscode = require('vscode');
const path = require('path');
const cp = require('child_process');


/* =========================================================
   DIAGNOSTICS
   ========================================================= */

const diagnostics =
    vscode.languages.createDiagnosticCollection('xr');


function addDiagnostic(document, message) {

    const text = message.trim();

    if (!text) {
        return;
    }

    const diagnosticsList = [];

    const match =
        text.match(/Xeta:\s*(.+?)(?:\r?\n|$)/);

    if (!match) {
        return;
    }

    const errorMessage = match[1].trim();

    let line = 0;
    let startChar = 0;
    let endChar = 1;


    // Undefined variable
    const undefinedMatch =
        errorMessage.match(
            /teyin olunmamis deyisen:\s*(\S+)/
        );


    if (undefinedMatch) {

        const variable = undefinedMatch[1];

        const found =
            findWord(document, variable);

        if (found) {

            line = found.line;
            startChar = found.start;
            endChar = found.end;

        }
    }


    const range =
        new vscode.Range(
            new vscode.Position(
                line,
                startChar
            ),
            new vscode.Position(
                line,
                endChar
            )
        );


    const diagnostic =
        new vscode.Diagnostic(
            range,
            `XR: ${errorMessage}`,
            vscode.DiagnosticSeverity.Error
        );


    diagnostic.source = 'XR';

    diagnosticsList.push(diagnostic);

    diagnostics.set(
        document.uri,
        diagnosticsList
    );
}


/* =========================================================
   FIND WORD
   ========================================================= */

function findWord(document, word) {

    for (
        let line = 0;
        line < document.lineCount;
        line++
    ) {

        const text =
            document.lineAt(line).text;


        const regex =
            new RegExp(
                `\\b${escapeRegex(word)}\\b`
            );


        const match =
            regex.exec(text);


        if (match) {

            return {
                line: line,
                start: match.index,
                end: match.index + word.length
            };

        }
    }

    return null;
}


/* =========================================================
   ESCAPE REGEX
   ========================================================= */

function escapeRegex(text) {

    return text.replace(
        /[.*+?^${}()|[\]\\]/g,
        '\\$&'
    );

}


/* =========================================================
   ACTIVATE
   ========================================================= */

function activate(context) {


    /* =====================================================
       XR RUN FILE
       ===================================================== */

    const runCommand =
        vscode.commands.registerCommand(
            'xr.runFile',
            function () {

                const editor =
                    vscode.window.activeTextEditor;


                if (!editor) {

                    vscode.window.showErrorMessage(
                        'XR: Aktiv fayl tapilmadi.'
                    );

                    return;
                }


                const document =
                    editor.document;


                if (document.languageId !== 'xr') {

                    vscode.window.showErrorMessage(
                        'XR: Bu fayl XR fayli deyil.'
                    );

                    return;
                }


                if (document.isDirty) {

                    document.save();

                }


                const filePath =
                    document.fileName;


                const xrPath =
                    path.join(
                        __dirname,
                        'xr.exe'
                    );


                const terminal =
                    vscode.window.createTerminal(
                        'XR'
                    );


                terminal.show();


                cp.execFile(
                    xrPath,
                    [filePath],
                    {
                        windowsHide: true
                    },
                    function (error, stdout, stderr) {

                        if (stdout) {

                            terminal.sendText(
                                `echo ${JSON.stringify(stdout)}`
                            );

                        }


                        if (stderr) {

                            terminal.sendText(
                                `echo ${JSON.stringify(stderr)}`
                            );


                            addDiagnostic(
                                document,
                                stderr
                            );

                        }


                        if (error && !stderr) {

                            terminal.sendText(
                                `echo ${JSON.stringify(error.message)}`
                            );

                        }

                    }
                );

            }
        );


    context.subscriptions.push(
        runCommand
    );


    /* =====================================================
       DIAGNOSTICS
       ===================================================== */

    context.subscriptions.push(
        diagnostics
    );


    /* =====================================================
       CLEAR DIAGNOSTICS WHEN FILE CHANGES
       ===================================================== */

    context.subscriptions.push(

        vscode.workspace.onDidChangeTextDocument(
            function (event) {

                diagnostics.delete(
                    event.document.uri
                );

            }
        )

    );


    /* =====================================================
       AUTOCOMPLETE
       ===================================================== */

    const completionProvider =

        vscode.languages.registerCompletionItemProvider(
            'xr',
            {

                provideCompletionItems(document) {

                    const items = [];


                    /* =====================================
                       BUILT-IN FUNCTIONS
                       ===================================== */

                    const functions = [

                        [
                            'uz',
                            'uz(value)',
                            'Dəyərin uzunluğunu qaytarır'
                        ],

                        [
                            'qat',
                            'qat(a, b)',
                            'İki dəyəri birləşdirir'
                        ],

                        [
                            'cixar',
                            'cixar(a, b)',
                            'Dəyərdən çıxma əməliyyatı'
                        ],

                        [
                            'metn',
                            'metn(value)',
                            'Dəyəri mətnə çevirir'
                        ],

                        [
                            'eded',
                            'eded(value)',
                            'Dəyəri ədədə çevirir'
                        ],

                        [
                            'sirala',
                            'sirala(list)',
                            'Siyahını sıralayır'
                        ],

                        [
                            'boyuk',
                            'boyuk(a, b)',
                            'Böyük olan dəyəri qaytarır'
                        ],

                        [
                            'kicik',
                            'kicik(a, b)',
                            'Kiçik olan dəyəri qaytarır'
                        ],

                        [
                            'bol',
                            'bol(a, b)',
                            'Bölmə əməliyyatı'
                        ],

                        [
                            'yig',
                            'yig(list)',
                            'Siyahının cəmini qaytarır'
                        ],

                        [
                            'var',
                            'var(value)',
                            'Dəyərin mövcudluğunu yoxlayır'
                        ]

                    ];


                    for (
                        const [
                            name,
                            signature,
                            description
                        ]
                        of functions
                    ) {

                        const item =
                            new vscode.CompletionItem(
                                name,
                                vscode.CompletionItemKind.Function
                            );


                        item.detail =
                            signature;


                        item.documentation =
                            new vscode.MarkdownString(
                                description
                            );


                        items.push(item);

                    }


                    /* =====================================
                       KEYWORDS
                       ===================================== */

                    const keywords = [

                        [
                            'deyer',
                            'Dəyişən elan edir'
                        ],

                        [
                            '?',
                            'Şərt bloku'
                        ],

                        [
                            '@',
                            'Dövr bloku'
                        ],

                        [
                            '->',
                            'Dəyər mənimsətmə operatoru'
                        ]

                    ];


                    for (
                        const [
                            name,
                            description
                        ]
                        of keywords
                    ) {

                        const item =
                            new vscode.CompletionItem(
                                name,
                                vscode.CompletionItemKind.Keyword
                            );


                        item.documentation =
                            new vscode.MarkdownString(
                                description
                            );


                        items.push(item);

                    }


                    /* =====================================
                       OPERATORS
                       ===================================== */

                    const operators = [

                        ['=', 'Bərabərlik'],

                        ['<>', 'Fərqlilik'],

                        ['&', 'VƏ'],

                        ['|', 'VƏ YA'],

                        ['!', 'DEYİL'],

                        ['+', 'Toplama'],

                        ['-', 'Çıxma'],

                        ['*', 'Vurma'],

                        ['/', 'Bölmə'],

                        ['%', 'Qalıq'],

                        ['^', 'Qüvvət / təkrar'],

                        ['~', 'Birləşdirmə'],

                        ['..', 'Aralıq']

                    ];


                    for (
                        const [
                            operator,
                            description
                        ]
                        of operators
                    ) {

                        const item =
                            new vscode.CompletionItem(
                                operator,
                                vscode.CompletionItemKind.Operator
                            );


                        item.documentation =
                            new vscode.MarkdownString(
                                description
                            );


                        items.push(item);

                    }


                    /* =====================================
                       XR VARIABLES
                       ===================================== */

                    const text =
                        document.getText();


                    const variableRegex =
                        /\bdeyer\s*->\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*;/g;


                    let match;


                    while (
                        (match = variableRegex.exec(text)) !== null
                    ) {

                        const variableName =
                            match[1];


                        const item =
                            new vscode.CompletionItem(
                                variableName,
                                vscode.CompletionItemKind.Variable
                            );


                        item.detail =
                            'XR variable';


                        item.documentation =
                            new vscode.MarkdownString(
                                `XR dəyişəni: **${variableName}**`
                            );


                        items.push(item);

                    }


                    return items;

                }

            }
        );


    context.subscriptions.push(
        completionProvider
    );


    /* =====================================================
       SNIPPETS
       ===================================================== */

    const snippetProvider =

        vscode.languages.registerCompletionItemProvider(
            'xr',
            {

                provideCompletionItems() {

                    const snippets = [];


                    /* =====================================
                       IF / ELSE
                       ===================================== */

                    const ifSnippet =
                        new vscode.CompletionItem(
                            'if',
                            vscode.CompletionItemKind.Snippet
                        );


                    ifSnippet.insertText =
                        new vscode.SnippetString(
                            '? ${1:sert} {\n' +
                            '    ${2}\n' +
                            '} : {\n' +
                            '    ${3}\n' +
                            '}'
                        );


                    ifSnippet.detail =
                        'XR if / else';


                    snippets.push(
                        ifSnippet
                    );


                    /* =====================================
                       IF
                       ===================================== */

                    const ifOnlySnippet =
                        new vscode.CompletionItem(
                            'ifonly',
                            vscode.CompletionItemKind.Snippet
                        );


                    ifOnlySnippet.insertText =
                        new vscode.SnippetString(
                            '? ${1:sert} {\n' +
                            '    ${2}\n' +
                            '}'
                        );


                    ifOnlySnippet.detail =
                        'XR if';


                    snippets.push(
                        ifOnlySnippet
                    );


                    /* =====================================
                       LOOP
                       ===================================== */

                    const loopSnippet =
                        new vscode.CompletionItem(
                            'loop',
                            vscode.CompletionItemKind.Snippet
                        );


                    loopSnippet.insertText =
                        new vscode.SnippetString(
                            '@ ${1:sert} {\n' +
                            '    ${2}\n' +
                            '}'
                        );


                    loopSnippet.detail =
                        'XR loop';


                    snippets.push(
                        loopSnippet
                    );


                    /* =====================================
                       FOREACH
                       ===================================== */

                    const foreachSnippet =
                        new vscode.CompletionItem(
                            'foreach',
                            vscode.CompletionItemKind.Snippet
                        );


                    foreachSnippet.insertText =
                        new vscode.SnippetString(
                            '@ ${1:x} : ${2:siyahi} {\n' +
                            '    ${3}\n' +
                            '}'
                        );


                    foreachSnippet.detail =
                        'XR foreach';


                    snippets.push(
                        foreachSnippet
                    );


                    /* =====================================
                       VARIABLE
                       ===================================== */

                    const variableSnippet =
                        new vscode.CompletionItem(
                            'deyer',
                            vscode.CompletionItemKind.Snippet
                        );


                    variableSnippet.insertText =
                        new vscode.SnippetString(
                            'deyer -> ${1:ad};'
                        );


                    variableSnippet.detail =
                        'XR variable declaration';


                    snippets.push(
                        variableSnippet
                    );


                    /* =====================================
                       PRINT
                       ===================================== */

                    const printSnippet =
                        new vscode.CompletionItem(
                            'print',
                            vscode.CompletionItemKind.Snippet
                        );


                    printSnippet.insertText =
                        new vscode.SnippetString(
                            '> ${1:deyer};'
                        );


                    printSnippet.detail =
                        'XR print';


                    snippets.push(
                        printSnippet
                    );


                    /* =====================================
                       LIST
                       ===================================== */

                    const listSnippet =
                        new vscode.CompletionItem(
                            'list',
                            vscode.CompletionItemKind.Snippet
                        );


                    listSnippet.insertText =
                        new vscode.SnippetString(
                            '#[${1:1, 2, 3}]'
                        );


                    listSnippet.detail =
                        'XR list';


                    snippets.push(
                        listSnippet
                    );


                    /* =====================================
                       RANGE
                       ===================================== */

                    const rangeSnippet =
                        new vscode.CompletionItem(
                            'range',
                            vscode.CompletionItemKind.Snippet
                        );


                    rangeSnippet.insertText =
                        new vscode.SnippetString(
                            '${1:1}..${2:5}'
                        );


                    rangeSnippet.detail =
                        'XR range';


                    snippets.push(
                        rangeSnippet
                    );


                    return snippets;

                }

            }
        );


    context.subscriptions.push(
        snippetProvider
    );

}


/* =========================================================
   DEACTIVATE
   ========================================================= */

function deactivate() {

    diagnostics.clear();
    diagnostics.dispose();

}


module.exports = {
    activate,
    deactivate
};
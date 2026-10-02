#include "interpreter.hpp"
#include "error.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    Interpreter vm;

    if (argc == 1) {
        vm.repl();
        return 0;
    }

    std::string a1 = argv[1];

    if (a1 == "-v" || a1 == "--version") {
        std::cout << "XR Interpreter v0.0.1 (OOP Edition)\n";
        return 0;
    }

    if (a1 == "-h" || a1 == "--help") {
        std::cout << "Istifade:\n"
                     "  xr <fayl.xr>     XR faylini icra et\n"
                     "  xr -e \"kod;\"    Kodu birbasa icra et\n"
                     "  xr               REPL rejimine gec\n"
                     "  xr -v            Versiyani goster\n"
                     "  xr -h            Komek mesajini goster\n";
        return 0;
    }

    if (a1 == "-e" && argc >= 3) {
        try {
            vm.run(argv[2]);
            return 0;
        } catch (const XRError& e) {
            std::cerr << "Xeta: " << e.what() << "\n";
            return 1;
        } catch (const std::exception& e) {
            std::cerr << "Xeta: " << e.what() << "\n";
            return 1;
        }
    }

    return vm.runFile(a1) ? 0 : 1;
}

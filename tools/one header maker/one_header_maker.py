import os
import re
import argparse
import sys

def process_file(file_path, base_dir, processed_files, system_includes, file_content_list):
    """
    Rekurencyjnie przetwarza plik, zbierając systemowe includes i zawartość po #pragma once.
    """
    if file_path in processed_files:
        return
    
    processed_files.add(file_path)

    reading_content = False  # Flaga, która określa, czy kopiować zawartość

    with open(file_path, 'r', encoding='utf-8') as infile:
        content_without_directives = []
        for line in infile:
            line_stripped = line.strip()
            
            if not reading_content:
                if line_stripped.startswith('#pragma once'):
                    reading_content = True
                continue
            
            # Dyrektywy #include
            if line_stripped.startswith('#include'):
                # Sprawdzanie, czy to nagłówek systemowy
                match_system = re.match(r'#include\s*<(.+)>', line_stripped)
                if match_system:
                    system_includes.add(line_stripped)
                # Sprawdzanie, czy to nagłówek projektowy
                else:
                    match_project = re.match(r'#include\s*"(.*)"', line_stripped)
                    if match_project:
                        included_file = match_project.group(1)
                        # Tworzenie pełnej ścieżki do dołączonego pliku
                        included_path = os.path.normpath(os.path.join(os.path.dirname(file_path), included_file))
                        if os.path.exists(included_path):
                            # Rekurencyjne przetwarzanie
                            process_file(included_path, base_dir, processed_files, system_includes, file_content_list)
                        else:
                            # Jeśli nie ma pliku w projekcie, dodaj dyrektywę
                            content_without_directives.append(line)
            # Ignorowanie #pragma once
            elif line_stripped.startswith('#pragma once'):
                continue
            # Zapisywanie pozostałej zawartości
            else:
                content_without_directives.append(line)
    
    file_content_list.append((os.path.relpath(file_path, base_dir), "".join(content_without_directives)))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Link c++ header files into one")
    parser.add_argument('input_file', type=str, help='Input c++ header file')
    parser.add_argument('output_file', type=str, help='Output file name')
    parser.add_argument('-i', '--initial-text', type=str, default='', help='Optional text added at the begining of the file')

    args = parser.parse_args()

    # Sprawdzenie, czy plik wejściowy istnieje
    if not os.path.exists(args.input_file):
        print(f"Błąd: Plik wejściowy '{args.input_file}' nie istnieje.")
        sys.exit(1)

    base_directory = os.path.dirname(os.path.abspath(args.input_file))
    
    processed_files = set()
    system_includes = set()
    file_content_list = []

    try:
        process_file(args.input_file, base_directory, processed_files, system_includes, file_content_list)

        # Zapisz do pliku wyjściowego
        with open(args.output_file, 'w', encoding='utf-8') as outfile:
            if args.initial_text:
                outfile.write(args.initial_text + "\n\n")

            outfile.write("#pragma once\n\n")
            
            # Dodaj unikalne nagłówki systemowe
            for include_line in sorted(list(system_includes)):
                outfile.write(include_line + "\n")
            
            outfile.write("\n")
            
            # Dodaj zawartość scalonych plików
            for rel_path, content in file_content_list:
                outfile.write(f'#pragma region {rel_path}\n')
                outfile.write(content)
                outfile.write(f'\n#pragma endregion // {rel_path}\n\n')

        print(f'\nSukces! Plik "{args.input_file}" został scalony do "{args.output_file}".')
    except Exception as e:
        print(f'Wystąpił nieoczekiwany błąd: {e}')
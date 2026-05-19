import os
import shutil
import argparse
from pathlib import Path

def create_pcre2cpp_package(source_root, output_dir, version):
    source_path = Path(source_root)
    output_path = Path(output_dir)

    zip_name = f"pcre2cpp_v{version}"
    final_zip_path = output_path / zip_name

    print(f"Creating archive: {zip_name}.zip...")
    shutil.make_archive(
        base_name=str(final_zip_path),
        format='zip',
        root_dir=str(source_path)
    )

    print(f"Success! File saved: {final_zip_path}.zip")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Pakowanie zawartości folderów mstd do ZIP.")
    parser.add_argument("source", help="Ścieżka do folderu nadrzędnego")
    parser.add_argument("output", help="Ścieżka do folderu zapisu")
    parser.add_argument("version", help="Numer wersji (np. 1.0.2)")

    args = parser.parse_args()

    create_pcre2cpp_package(args.source, args.output, args.version)
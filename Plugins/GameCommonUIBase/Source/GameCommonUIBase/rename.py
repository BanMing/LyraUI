import os

def rename_files_recursive(directory):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.startswith('UIB'):
                old_path = os.path.join(root, filename)
                new_filename = 'GameUI' + filename[len('UIB'):]
                new_path = os.path.join(root, new_filename)
                try:
                    os.rename(old_path, new_path)
                    print(f"Renamed: {old_path} -> {new_path}")
                except Exception as e:
                    print(f"Failed to rename {old_path}: {e}")

if __name__ == "__main__":
    current_dir = os.getcwd()
    rename_files_recursive(current_dir)
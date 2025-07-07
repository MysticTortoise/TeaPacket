import sys, os, shutil

this_script_path = os.path.dirname(os.path.realpath(__file__))
platform = sys.argv[2]
assets_path = sys.argv[3]
build_path = sys.argv[4]

def asset_parse_and_copy(src_path, dest_path):
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    shutil.copy(src_path, dest_path)
    print(src_path)

def asset_check():
    for subdir, dirs, files in os.walk(assets_path):
        for file in files:
            # Get Directory path
            file_dir = str(subdir).replace(assets_path, "")
            file_dir.replace("\\", "/")
            file_dir = file_dir[1:] + "/"
            # SRC and DEST File path creation
            src_file = os.path.join(subdir, file)
            dest_file = build_path + "/assets/" + file_dir + file
            # Copy if the dest file flat out doesn't exist
            if not os.path.exists(dest_file):
                asset_parse_and_copy(src_file, dest_file)
                continue
            # Check modify times
            src_mtime = os.path.getmtime(src_file)
            dest_mtime = os.path.getmtime(dest_file)
            if src_mtime > dest_mtime:
                asset_parse_and_copy(src_file, dest_file)
                continue
            # File hasn't changed.




def build():
    asset_check()

if __name__ == "__main__":
    build()
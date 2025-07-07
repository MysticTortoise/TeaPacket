import sys, os, shutil, importlib.util, pathlib

this_script_path = os.path.dirname(os.path.realpath(__file__))
platform = sys.argv[2]
assets_path = sys.argv[3]
build_path = sys.argv[4]


def asset_parse_and_copy(src_path, dest_path):
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    _, ext = os.path.splitext(src_path)
    ext = ext[1:]
    def check_script(script):
        script = script + "/prebuild/asset_parse/" + ext + ".py"
        if os.path.exists(script):
            spec = importlib.util.spec_from_file_location(ext + ".py", script)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            module.parse_and_copy(src_path, dest_path)
            return True
        return False
    if check_script(platform):
        return
    if check_script(str(pathlib.Path(__file__).parents[1])):
        return
    shutil.copy(src_path, dest_path)
    print(src_path)

def asset_check():
    print("Checking asset registry...")
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

def check_external_tools():
    print("Checking external tools...")
    for subdir, dirs, files in os.walk(platform + "/prebuild/external_tools"):
        for file in files:
            if file[-3:] != ".py":
                continue

            spec = importlib.util.spec_from_file_location(str(file), os.path.join(subdir,file))
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            print("Checking " + file[:-3] + "...")
            if not module.check():
                print("Grabbing " + file[:-3] + "...")
                module.grab()
        break


def build():
    check_external_tools()
    asset_check()

if __name__ == "__main__":
    build()
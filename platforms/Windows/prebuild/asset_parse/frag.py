import importlib, os.path

def parse_and_copy(src_path, dest_path):
    spec = importlib.util.spec_from_file_location("shader.py", os.path.dirname(__file__) + "/shader.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.parse_and_copy_shader(src_path, dest_path)

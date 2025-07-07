import os
import subprocess, pathlib

def parse_and_copy_shader(src_path, dest_path):
    work_dir = str(pathlib.Path(__file__).parents[1]) + "/external_tools/shader_transpiler/"


    subprocess.run("glslangValidator.exe -H -V -o shader.spv \"" + src_path + "\"", cwd=work_dir, shell=True, check=True)
    subprocess.run("spirv-cross.exe --hlsl --shader-model 50 shader.spv --output \"" + dest_path + "\"", cwd=work_dir, shell=True, check=True)
    os.remove(work_dir + "/shader.spv")
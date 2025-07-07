import os, subprocess, shutil

dir = os.path.dirname(__file__) + "/shader_transpiler/"

def check():
    if not os.path.exists(dir + "glslangValidator.exe"):
        return False
    if not os.path.exists(dir + "spirv-cross.exe"):
        return False
    return True

def grab():
    if not os.path.isdir(dir):
        os.mkdir(dir)
    if not os.path.exists(dir + "glslangValidator.exe"):
        # Compile and get glslangValidator
        if not os.path.isdir(dir + "glslang"):
            print("Cloning glslang", flush=True)
            subprocess.run("git clone https://github.com/KhronosGroup/glslang.git", shell=True, cwd=dir)
        print("Updating glslang sources", flush=True)
        subprocess.run("python " + dir + "glslang/update_glslang_sources.py", shell=True, cwd=dir + "glslang")
        print("CMAKE Configuring", flush=True)
        subprocess.run("cmake -B " + dir + "glslangbuild -DCMAKE_BUILD_TYPE=Release", shell=True, cwd =dir + "glslang")
        print("CMAKE Build", flush=True)
        subprocess.run("cmake --build . --config Release", cwd=dir + "glslangbuild", shell=True)
        #shutil.copy(dir + "glslangbuild/StandAlone/Release/glslang.exe", dir)
        shutil.copy(dir + "glslangbuild/StandAlone/Release/glslangValidator.exe", dir)
        if os.path.exists(dir + "glslangValidator.exe"):
            shutil.rmtree(dir + "glslang/", ignore_errors=True)
            shutil.rmtree(dir + "glslangbuild/", ignore_errors=True)
    if not os.path.exists(dir + "spirv_cross.exe"):
        # Compile and get spirv_cross
        if not os.path.isdir(dir + "SPIRV-Cross"):
            print("Cloning SPIRV-Cross")
            subprocess.run("git clone https://github.com/KhronosGroup/SPIRV-Cross.git", shell=True, cwd=dir)
        print("CMAKE running SPIRV-Cross")
        subprocess.run("cmake -B " + dir + "spirv-cross-build -DCMAKE_BUILD_TYPE=Release", shell=True, cwd=dir + "SPIRV-Cross")
        subprocess.run("cmake --build . --config Release", cwd=dir + "spirv-cross-build", shell=True)
        shutil.copy(dir + "spirv-cross-build/Release/spirv-cross.exe", dir)
        if os.path.exists(dir + "spirv-cross.exe"):
            shutil.rmtree(dir + "SPIRV-Cross/", ignore_errors=True)
            shutil.rmtree(dir + "spirv-cross-build/", ignore_errors=True)



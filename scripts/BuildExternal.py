import subprocess
import os
import shutil

msbuild_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

def run_cmake(source_dir, build_dir):
    # Create the build directory if it doesn't exist
    os.makedirs(build_dir, exist_ok=True)

    # Run CMake to generate the Visual Studio solution
    cmake_command = [
        'cmake',
        '-G', 'Visual Studio 17 2022',
        '-S', source_dir,
        '-B', build_dir
    ]
    subprocess.run(cmake_command, check=True)

def build_solution(library, build_dir, library_dir, configuration):
    # Use MSBuild to build the solution
    msbuild_command = [
        msbuild_path,
        '/m',          # Build with multiple processes
        f'/p:Configuration={configuration}',  # Set the configuration
        '/p:Platform=x64',           # Set the platform to x64
        f'{build_dir}/{library}.sln'  # Specify the solution file
    ]

    subprocess.run(msbuild_command, check=True)
    try:
        shutil.copytree(build_dir+r'\src\\'+configuration, library_dir+'/'+configuration, dirs_exist_ok=True)
    except:
        shutil.copytree(build_dir+'/'+library+'/'+configuration, library_dir+'/'+configuration, dirs_exist_ok=True)

def build_and_compile(library):
    source_directory = r'../Gale/external/'+library
    build_directory = r'../Gale/external/'+library+'/build'
    library_directory = r'../Gale/vendor/'+library

    run_cmake(source_directory, build_directory)

    # Build for both Release and Debug configurations
    for config in ['Debug', 'Release']:
        # Build the solution
        build_solution(library, build_directory, library_directory, config)

    return

if __name__ == "__main__":
    build_and_compile('glfw')
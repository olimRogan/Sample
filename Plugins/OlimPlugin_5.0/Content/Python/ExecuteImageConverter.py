import unreal
import os

projectDir = unreal.SystemLibrary.get_project_directory()
exePath = os.path.join(projectDir, 'Config', 'ConvertPNGToJPG.exe')

if os.path.isfile(exePath):
    print(exePath)
    os.system(exePath)
else:
    print('File is invalid')



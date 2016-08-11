import os
import sys
#files in Pyengine2 folder
root = os.environ["PYENGINE_WORKSPACE_DIR"] # this is a path to the workspace directory. Pyengine2 is in that directory

def cpyLuaAndLuahRecursive(srcDir, targetDir, platform):
    names = os.listdir(srcDir)
    for name in names:
        fullSrcPath = os.path.join(srcDir, name)
        if name != '.svn' and os.path.isdir(fullSrcPath):
            #recursively continue
            cpyLuaAndLuahRecursive(fullSrcPath, os.path.join(targetDir, name), platform)
        else:
            ext = os.path.splitext(name)[1]
            if ext == '.lua' or ext == '.luah':
                #copy the file
                fullTargetPath = os.path.join(targetDir, name)
                print "Copying: %s to %s" % (fullSrcPath, fullTargetPath)
                if platform == 'IOS':
                    os.system('mkdir -p "%s"' % os.path.split(fullTargetPath)[0])
                    os.system('cp -f "%s" "%s"' % (fullSrcPath, fullTargetPath))
                else:
                    os.system('XbCp /d /Y /t "%s" "%s"' % (fullSrcPath, fullTargetPath))

def main():
    conf = {}
    execfile(os.path.join(os.environ["PYENGINE_WORKSPACE_DIR"], 'GlobalConfig', 'Dirs.py'), conf)
    
    platform = "XBox360" #default target platform (other option might be IOS)
    
    if len(sys.argv) > 2:
        platform = sys.argv[2]
    
    print 'Deployment platform:', platform
    if platform == 'IOS':
        if (len(sys.argv) < 4):
            print 'PYENGINE: TOOLS: ERROR: not target path provided fro IOS option. Dont know where to deploy files.'
        
        targetWorkspace = sys.argv[3]
    else:
        targetWorkspace = conf['X360WorkspacePath']
    
    #parse whole workspace instead of just Engine folder because other solutions might have lua files
    #todo: go trhough game soltuions only
    conf = {}
    execfile(os.path.join(os.environ["PYENGINE_WORKSPACE_DIR"], 'GlobalConfig', 'Dirs.py'), conf)
    processes = conf['LaunchOptions'].keys() + ["PrimeEngine"]
    for process in processes:
        cpyLuaAndLuahRecursive(os.path.join(root, "Code", process), os.path.join(targetWorkspace, "Code", process), platform)
if __name__ == "__main__":
    main()
    
